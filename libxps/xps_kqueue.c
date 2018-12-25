//
//  xps_kqueue.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_event.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include "xps_core.h"
#include "xps_logger.h"

#define XPS_EVENT_MAXN  32

typedef struct xps_kqueue {
    int                 fd;
    int                 nevents;
    int                 nchanges;
    int                 max_events;
    int                 max_changes;
    struct kevent       events[XPS_EVENT_MAXN];
    struct kevent       changes[XPS_EVENT_MAXN];
    xps_core_t          *core;
    xps_event_actions_t actions;
} xps_kqueue_t;

XPS_INLINE int xps_kqueue_worker(void *ctx) {
    xps_kqueue_t *kq = (xps_kqueue_t *)ctx;
    int nevents = kevent(kq->fd, kq->changes, kq->nchanges, kq->events, kq->max_events, NULL);
    if (nevents < 0) {
        log_error("poll kevent failed: %s", strerror(errno));
        return XPS_ERROR;
    }
    kq->nchanges = 0;
    xps_core_t *core = kq->core;
    for (int i = 0; i < nevents; i++) {
        struct kevent *e = kq->events + i;
        switch (e->filter) {
            case EVFILT_READ:
            case EVFILT_WRITE:
                if (e->udata != NULL) {
                    xps_event_t *ev = (xps_event_t *)(e->udata);
                    if (ev->handler != NULL) {
                        ev->available = (unsigned)e->data;
                        if (e->fflags & EV_EOF) ev->eof = 1;
                        ev->handler(ev, core);
                    }
                }
                break;
            case EVFILT_TIMER:
                if (e->udata != NULL) {
                    xps_event_timer_t *timer = (xps_event_timer_t *)(e->udata);
                    if (timer->handler != NULL && timer->handler(timer) == XPS_DONE) {
                        return XPS_DONE;
                    }
                }
                break;
            case EVFILT_USER:
                if (e->udata != NULL) {
                    xps_event_notify_t *notify = (xps_event_notify_t *)(e->udata);
                    if (notify->handler != NULL && notify->handler(notify) == XPS_DONE) {
                        return XPS_DONE;
                    }
                }
                break;
        }
    }
    return XPS_OK;
}

XPS_INLINE void xps_kqueue_push_event(xps_kqueue_t *kq, xps_event_t *ev, unsigned flags) {
    if (kq->nchanges >= kq->max_changes) {
        kevent(kq->fd, kq->changes, kq->nchanges, NULL, 0, NULL);
        kq->nchanges = 0;
    }
    struct kevent *e = kq->changes + kq->nchanges++;
    e->ident    = ev->fd;
    e->filter   = ev->event;
    e->flags    = flags;
    e->data     = 0;
    e->udata    = ev;
}

XPS_INLINE void xps_kqueue_add_event(xps_event_actions_t *acts, xps_event_t *ev, int fd, unsigned flags) {
    ev->fd          = fd;
    ev->event       = (flags&XPS_EVFLG_READ ? EVFILT_READ : EVFILT_WRITE);
    ev->available   = 0;
    ev->eof         = 0;
    ev->closed      = 0;
    xps_kqueue_push_event((xps_kqueue_t *)acts->ctx, ev, EV_ADD|(flags&XPS_EVFLG_ENABLE ? EV_ENABLE : EV_DISABLE));
}

XPS_INLINE void xps_kqueue_del_event(xps_event_actions_t *acts, xps_event_t *ev) {
    if (ev->closed == 0) {
        xps_kqueue_push_event((xps_kqueue_t *)acts->ctx, ev, EV_DELETE);
        ev->available   = 0;
        ev->eof         = 1;
        ev->closed      = 1;
    }
}

// Notify
XPS_INLINE void xps_kqueue_notify_send(xps_event_notify_t *notify) {
    struct kevent e;
    EV_SET(&e, (uintptr_t)notify, EVFILT_USER, EV_ENABLE, NOTE_TRIGGER, 0, notify);
    kevent((int)notify->data, &e, 1, NULL, 0, NULL);
}

XPS_INLINE void xps_kqueue_notify_reset(xps_event_notify_t *notify) {
    struct kevent e;
    EV_SET(&e, (uintptr_t)notify, EVFILT_USER, EV_CLEAR, 0, 0, notify);
    kevent((int)notify->data, &e, 1, NULL, 0, NULL);
}

XPS_INLINE void xps_kqueue_notify_del(xps_event_notify_t *notify) {
    struct kevent e;
    EV_SET(&e, (uintptr_t)notify, EVFILT_USER, EV_DELETE, 0, 0, notify);
    kevent((int)notify->data, &e, 1, NULL, 0, NULL);
}

XPS_INLINE xps_event_notify_t *xps_kqueue_add_notify(xps_event_actions_t *acts, xps_event_notify_handler_pt handler) {
    xps_kqueue_t *kq = (xps_kqueue_t *)acts->ctx;
    if (kq->core != NULL) {
        xps_event_notify_t *notify = xps_palloc(kq->core->pool, sizeof(xps_event_notify_t));
        if (notify != NULL) {
            notify->data    = kq->fd;
            notify->handler = handler;
            notify->send    = xps_kqueue_notify_send;
            notify->reset   = xps_kqueue_notify_reset;
            notify->close   = xps_kqueue_notify_del;

            struct kevent e;
            EV_SET(&e, (uintptr_t)notify, EVFILT_USER, EV_ADD|EV_CLEAR, 0, 0, notify);
            kevent(kq->fd, &e, 1, NULL, 0, NULL);
            return notify;
        }
    }
    return NULL;
}

// Timer
XPS_INLINE void xps_event_timer_del(xps_event_timer_t *timer) {
    if (timer->data != 0) {
        struct kevent e;
        EV_SET(&e, (uintptr_t)timer, EVFILT_TIMER, EV_DELETE, 0, 0, 0);
        kevent((int)timer->data, &e, 1, NULL, 0, NULL);
    }
}

XPS_INLINE xps_event_timer_t *xps_kqueue_add_timer(xps_event_actions_t *acts, xps_event_time_handler_pt handler, unsigned interval, void *ptr) {
    xps_kqueue_t *kq = (xps_kqueue_t *)acts->ctx;
    if (kq->core != NULL) {
        xps_event_timer_t *t = xps_palloc(kq->core->pool, sizeof(xps_event_timer_t));
        if (t != NULL) {
            t->data     = kq->fd;
            t->ctx      = ptr;
            t->handler  = handler;
            t->del      = xps_event_timer_del;
            struct kevent e;
            EV_SET(&e, (uintptr_t)t, EVFILT_TIMER, EV_ADD, 0, interval, t);
            kevent(kq->fd, &e, 1, NULL, 0, NULL);
            return t;
        }
    }
    return NULL;
}

XPS_INLINE void xps_kqueue_cleanup(void *ptr) {
    xps_kqueue_t *kq = ptr;
    if (kq->core != NULL) {
        if (kq->core->evacts == &kq->actions) {
            kq->core->evacts = NULL;
        }
        kq->core = NULL;
    }
    if (kq->fd != INVALID_SOCKET) {
        close(kq->fd);
        kq->fd = INVALID_SOCKET;
    }
}

XPS_INLINE int xps_kqueue_load(xps_core_t *core) {
    int fd = kqueue();
    if (fd == INVALID_SOCKET) {
        log_error("load kqueue failed: %s", strerror(errno));
    } else {
        xps_pool_cleanup_t *c = xps_pool_cleanup_add(core->pool, sizeof(xps_kqueue_t));
        if (c != NULL) {
            c->handler = xps_kqueue_cleanup;
            xps_kqueue_t *kq = c->data;
            bzero(kq, sizeof(*kq));
            kq->fd                  = fd;
            kq->nevents             = 0;
            kq->nchanges            = 0;
            kq->max_events          = xps_countof(kq->events);
            kq->max_changes         = xps_countof(kq->changes);
            kq->actions.ctx         = kq;
            kq->actions.add         = xps_kqueue_add_event;
            kq->actions.del         = xps_kqueue_del_event;
            kq->actions.add_notify  = xps_kqueue_add_notify;
            kq->actions.add_timer   = xps_kqueue_add_timer;
            kq->core                = core;
            core->evacts            = &kq->actions;
            if (xps_core_add_worker(core, xps_kqueue_worker, kq) == XPS_OK) {
                return XPS_OK;
            }
        }
        close(fd);
    }
    return XPS_ERROR;
}

XPS_MODULE_EVENT_DECL(kqueue)
    .load = xps_kqueue_load,
XPS_MODULE_END()

//
//  xps_kqueue.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_kqueue.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include "xps_core.h"
#include "xps_logger.h"

#define XPS_EVENT_MAXN  32
#define EV_ID_EXIT      1

struct xps_kqueue {
    int                 fd;
    int                 nevents;
    int                 nchanges;
    int                 max_events;
    int                 max_changes;
    struct kevent       events[XPS_EVENT_MAXN];
    struct kevent       changes[XPS_EVENT_MAXN];
    xps_core_t          *core;
    xps_event_manager_t manager;
};

XPS_INLINE int xps_kqueue_worker(void *ctx) {
    xps_kqueue_t *kq = (xps_kqueue_t *)ctx;
    int nevents = kevent(kq->fd, kq->changes, kq->nchanges, kq->events, kq->max_events, NULL);
    if (nevents < 0) {
        log_error("poll kevent failed: %s", strerror(errno));
        return XPS_ERROR;
    }
    kq->nchanges = 0;
    for (int i = 0; i < nevents; i++) {
        struct kevent *e = kq->events + i;
        switch (e->filter) {
            case EVFILT_TIMER:
                break;
            case EVFILT_USER:
                if (e->ident == EV_ID_EXIT) {
                    log_debug("recv stop event");
                    return XPS_DONE;
                }
                break;
        }
    }
    return XPS_OK;
}

XPS_INLINE void xps_kqueue_sign(xps_event_manager_t *manager, int sign) {
    xps_kqueue_t *kq = xps_structof(manager, xps_kqueue_t, manager);
    if (kq->fd != INVALID_SOCKET) {
        struct kevent ev;
        if (sign == XPS_EVENT_SIGN_INIT) {
            EV_SET(&ev, EV_ID_EXIT, EVFILT_USER, EV_ENABLE|EV_CLEAR, 0, 0, NULL);
            kevent(kq->fd, &ev, 1, NULL, 0, NULL);
        } else if (sign == XPS_EVENT_SIGN_EXIT) {
            EV_SET(&ev, EV_ID_EXIT, EVFILT_USER, EV_ENABLE, NOTE_TRIGGER, 0, NULL);
            kevent(kq->fd, &ev, 1, NULL, 0, NULL);
        }
    }
}

XPS_INLINE void xps_kqueue_cleanup(void *ptr) {
    xps_kqueue_t *kq = ptr;
    if (kq->core != NULL) {
        if (kq->core->event_manager == &kq->manager) {
            kq->core->event_manager = NULL;
        }
        kq->core = NULL;
    }
    if (kq->fd != INVALID_SOCKET) {
        struct kevent evs[1];
        EV_SET(&evs[0], EV_ID_EXIT, EVFILT_USER, EV_DELETE, 0, 0, NULL);
        kevent(kq->fd, evs, xps_countof(evs), NULL, 0, NULL);
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
            kq->fd              = fd;
            kq->nevents         = 0;
            kq->nchanges        = 0;
            kq->max_events      = xps_countof(kq->events);
            kq->max_changes     = xps_countof(kq->changes);
            kq->core            = core;
            kq->manager.sign    = xps_kqueue_sign;
            core->event_manager = &kq->manager;

            struct kevent evs[1];
            EV_SET(&evs[0], EV_ID_EXIT, EVFILT_USER, EV_ADD|EV_DISABLE, 0, 0, NULL);
            kevent(kq->fd, evs, xps_countof(evs), NULL, 0, NULL);
            
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

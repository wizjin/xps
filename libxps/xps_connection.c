//
//  xps_connection.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_connection.h"
#include "xps_core.h"
#include "xps_inet.h"
#include "xps_event.h"

XPS_INLINE void xps_connection_closer(xps_connection_t *c) {
    if (c->reader_ev) {
        c->inet->core->evacts->del(c->inet->core->evacts, &c->reader);
        c->reader_ev = 0;
    }
    if (c->writer_ev) {
        c->inet->core->evacts->del(c->inet->core->evacts, &c->writer);
        c->writer_ev = 0;
    }
    if (c->ident != INVALID_SOCKET) {
        // flush event cache before close socket.
        c->inet->core->evacts->flush(c->inet->core->evacts);
        xps_tcp_close((int)c->ident);
        c->ident = INVALID_SOCKET;
    }
}

XPS_INLINE xps_connection_t *xps_connection_alloc(xps_inet_t *inet) {
    xps_connection_t *c = NULL;
    if (!xps_queue_empty(&inet->freed)) {
        xps_queue_t *q = xps_queue_head(&inet->freed);
        xps_queue_remove(q);
        c = xps_structof(q, xps_connection_t, queue);
    } else if (inet->alloced < inet->maxn) {
        c = xps_palloc(inet->core->pool, sizeof(xps_connection_t));
        if (c != NULL) {
            inet->alloced++;
            bzero(c, sizeof(*c));
            c->inet = inet;
        }
    }
    return c;
}

XPS_API xps_connection_t *xps_connection_create(xps_inet_t *inet, int fd) {
    xps_connection_t *c = xps_connection_alloc(inet);
    if (c != NULL) {
        // Reset connetcion
        xps_tcp_enable_async(fd);
        xps_queue_push_tail(&inet->used, &c->queue);
        c->closer       = xps_connection_closer;
        c->ident        = fd;
        c->closed       = 0;
        c->destroyed    = 0;
        if (c->data == NULL) c->data = xps_buffer_alloc(inet->core->cache);
        if (c->data != NULL) {
            log_debug("connect alloc: %p", c);
            return c;
        }
        xps_connection_close(c);
    }
    return NULL;
}

XPS_API void xps_connection_close(xps_connection_t *c) {
    if (c->closed == 0) {
        // only mark, not real close.
        c->closed = 1;

        xps_queue_remove(&c->queue);
        xps_queue_push_tail(&c->inet->closed, &c->queue);
    }
}

XPS_API void xps_connection_add_reader(xps_connection_t *c, unsigned flags, xps_event_handler_pt handler) {
    xps_event_actions_t *evacts = c->inet->core->evacts;
    xps_connection_set_reader(c, handler);
    if (c->reader_ev) {
        evacts->set(evacts, &c->reader, flags);
    } else {
        evacts->add(evacts, &c->reader, (int)c->ident, XPS_EVFLG_READ|flags);
        c->reader_ev = 1;
    }
}

XPS_API void xps_connection_add_writer(xps_connection_t *c, unsigned flags, xps_event_handler_pt handler) {
    xps_event_actions_t *evacts = c->inet->core->evacts;
    xps_connection_set_writer(c, handler);
    if (c->writer_ev) {
        evacts->set(evacts, &c->writer, flags);
    } else {
        evacts->add(evacts, &c->writer, (int)c->ident, XPS_EVFLG_WRITE|flags);
        c->writer_ev = 1;
    }
}

XPS_INLINE void xps_connection_free(xps_connection_t *c) {
    if (c->closed == 0) {
        xps_connection_close(c);
    }
    if (c->destroyed == 0) {
        c->destroyed = 1;

        // real close connection.
        c->closer(c);
        if (c->data != NULL) {
            xps_buffer_free(c->inet->core->cache, c->data);
            c->data = NULL;
        }

        xps_queue_remove(&c->queue);
        xps_queue_push_tail(&c->inet->freed, &c->queue);
        
        log_debug("connect free: %p", c);
    }
}

XPS_INLINE int xps_inet_worker(void *ctx) {
    xps_inet_t *inet = (xps_inet_t *)ctx;
    while (!xps_queue_empty(&inet->closed)) {
        xps_connection_free(xps_structof(xps_queue_head(&inet->closed), xps_connection_t, queue));
    }
    return XPS_OK;
}

XPS_INLINE void xps_inet_cleanup(void *ptr) {
    xps_inet_t *inet = (xps_inet_t *)ptr;
    if (inet->core != NULL) {
        if (!xps_queue_empty(&inet->used)) log_warn("used connect leak");
        if (!xps_queue_empty(&inet->closed)) log_warn("closed connect leak");
        while (!xps_queue_empty(&inet->used)) {
            xps_connection_close(xps_structof(xps_queue_head(&inet->used), xps_connection_t, queue));
        }
        xps_inet_worker(ptr);
        inet->core = NULL;
    }
    // TODO: free all connections.
}

XPS_INLINE int xps_inet_load(xps_core_t *core) {
    xps_pool_cleanup_t *c = xps_pool_cleanup_add(core->pool, sizeof(xps_inet_t));
    if (c != NULL) {
        c->handler          = xps_inet_cleanup;
        xps_inet_t *inet    = c->data;
        inet->core          = core;
        inet->alloced       = 0;
        inet->maxn          = XPS_MAX_CONNECTION_COUNT;
        core->inet          = inet;

        xps_queue_init(&inet->used);
        xps_queue_init(&inet->closed);
        xps_queue_init(&inet->freed);
        if (xps_core_add_worker(core, xps_inet_worker, inet) == XPS_OK) {
            return XPS_OK;
        }
    }
    return XPS_ERROR;
}

XPS_MODULE_SYS_DECL(inet)
    .load = xps_inet_load,
XPS_MODULE_END()

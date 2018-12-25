//
//  xps_connection.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_connection.h"
#include "xps_core.h"
#include "xps_inet.h"

struct xps_inet {
    xps_queue_t used;
    xps_queue_t closed;
    xps_queue_t freed;
    xps_core_t  *core;
    size_t      alloced;
    size_t      maxn;
};

XPS_API xps_connection_t *xps_connection_alloc(xps_inet_t *inet, int fd) {
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
    if (c != NULL) {
        // Reset connetcion
        xps_tcp_enable_async(fd);
        xps_queue_push_tail(&inet->used, &c->queue);
        c->fd           = fd;
        c->closed       = 0;
        c->destroyed    = 0;
        log_debug("connect alloc: %p", c);
    }
    return c;
}

XPS_API void xps_connection_close(xps_connection_t *c) {
    if (c->closed == 0) {
        c->closed = 1;
        if (c->read_ev) {
            c->inet->core->evacts->del(c->inet->core->evacts, &c->read);
            c->read_ev = 0;
        }
        if (c->write_ev) {
            c->inet->core->evacts->del(c->inet->core->evacts, &c->write);
            c->write_ev = 0;
        }
        if (c->fd != INVALID_SOCKET) {
            xps_tcp_close(c->fd);
            c->fd = INVALID_SOCKET;
        }
        xps_queue_remove(&c->queue);
        xps_queue_push_tail(&c->inet->closed, &c->queue);
    }
}

XPS_INLINE void xps_connection_free(xps_connection_t *c) {
    if (c->closed == 0) {
        xps_connection_close(c);
    }
    if (c->destroyed == 0) {
        c->destroyed = 1;

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
            xps_connection_close(xps_structof(xps_queue_head(&inet->closed), xps_connection_t, queue));
        }
        xps_inet_worker(ptr);
        inet->core = NULL;
    }
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

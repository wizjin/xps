//
//  xps_input_tcp.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_input_tcp.h"
#include <stdlib.h>
#include <unistd.h>
#include "xps_core.h"

XPS_INLINE void xps_input_tcp_close(xps_input_t *input, xps_core_t *core) {
    xps_input_tcp_t *l = xps_structof(input, xps_input_tcp_t, input);
    if (l->fd != INVALID_SOCKET) {
        core->evacts->del(core->evacts, &l->event);
        close(l->fd);
        l->fd = INVALID_SOCKET;
    }
}

XPS_INLINE void xps_input_tcp_event_handler(xps_event_t *ev, xps_core_t *core) {
    xps_input_tcp_t *l = xps_structof(ev, xps_input_tcp_t, event);
    if (ev->eof) {
        log_error("listen failed, force close: %p", l);
        xps_input_tcp_close(&l->input, core);
    } else {
        int n = ev->available;
        for (int i = 0; i < n; i++) {
            struct sockaddr_in addr;
            bzero(&addr, sizeof(addr));
            socklen_t socklen = sizeof(addr);
            int fd = accept(l->fd, (struct sockaddr *)&addr, &socklen);
            if (fd == INVALID_SOCKET) {
                log_warn("accept new connect failed: %s", strerror(errno));
                break;
            }
            xps_connection_t *conn = l->accept(core, fd);
            if (conn == NULL) {
                log_warn("too many connect");
                close(fd);
                break;
            }
            log_debug("accept new connect: %d", fd);
        }
    }
}

XPS_INLINE int xps_input_tcp_open(xps_input_t *input, xps_core_t *core) {
    xps_input_tcp_t *l = xps_structof(input, xps_input_tcp_t, input);
    if (l->fd == INVALID_SOCKET) {
        int fd = socket(l->addr.sin_family, SOCK_STREAM, 0);
        if (fd == INVALID_SOCKET) {
            log_error("create listen socket failed: %s", strerror(errno));
        } else {
            if (xps_tcp_bind(fd, (struct sockaddr *)&l->addr, l->addr.sin_len) != 0) {
                log_error("bind socket failed: %s", strerror(errno));
            } else if (listen(fd, XPS_MAX_ACCEPT_COUNT) != 0) {
                log_error("listen socket failed: %s", strerror(errno));
            } else {
                xps_tcp_fastopen_enable(fd);
                xps_tcp_disable_block(fd);
                l->fd               = fd;
                l->event.handler = xps_input_tcp_event_handler;
                core->evacts->add(core->evacts, &l->event, fd, XPS_EVFLG_READ|XPS_EVFLG_ENABLE);
                return XPS_OK;
            }
            close(fd);
        }
    }
    return XPS_ERROR;
}

XPS_API xps_input_t *xps_input_tcp_alloc(xps_core_t *core, xps_input_module_t *input, const char *option) {
    xps_input_tcp_t *l = xps_palloc(core->pool, sizeof(xps_input_tcp_t));
    if (l != NULL) {
        l->input.open           = xps_input_tcp_open;
        l->input.close          = xps_input_tcp_close;
        l->accept               = ((xps_input_tcp_module_t *)input)->accept;
        l->fd                   = INVALID_SOCKET;
        l->addr.sin_len         = sizeof(l->addr);
        l->addr.sin_family      = AF_INET;
        l->addr.sin_port        = htons(atoi(option));  // TODO: use xps_atoi
        l->addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        return &l->input;
    }
    return NULL;
}

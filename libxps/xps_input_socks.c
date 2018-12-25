//
//  xps_input_socks.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_input_tcp.h"
#include "xps_core.h"
#include "xps_router.h"
#include "xps_connection.h"

#define XPS_INPUT_SOCKS_VERSION             0x05
#define XPS_INPUT_SOCKS_METHOD              0x00
#define XPS_INPUT_SOCKS_CMD_CONNECT         0x01
#define XPS_INPUT_SOCKS_STATUS_INITED       1
#define XPS_INPUT_SOCKS_STATUS_CONNECT      2
#define XPS_INPUT_SOCKS_STATUS_CLOSE        3
#define XPS_INPUT_SOCKS_STATUS_RESPONSE     4
#define XPS_INPUT_SOCKS_STATUS_TRANSPORT    5

XPS_INLINE void xps_input_socks_reader(xps_event_t *ev) {
    xps_connection_t *c = xps_structof(ev, xps_connection_t, reader);
    xps_buffer_t *pdata = c->data;
    if (ev->available) {
        size_t len = read((int)c->ident, c->data->data, c->data->maxn);
        switch (c->status) {
            case XPS_INPUT_SOCKS_STATUS_INITED:
                if (len > 2 && pdata->data[0] == XPS_INPUT_SOCKS_VERSION) {
                    for (int i = 0; i < pdata->data[1]; i++) {
                        if (pdata->data[2+i] == XPS_INPUT_SOCKS_METHOD) {
                            pdata->data[0]  = XPS_INPUT_SOCKS_VERSION;
                            pdata->data[1]  = XPS_INPUT_SOCKS_METHOD;
                            pdata->count    = 2;
                            c->status       = XPS_INPUT_SOCKS_STATUS_CONNECT;
                            xps_connection_disable_reader(c);
                            xps_connection_enable_writer(c);
                            return;
                        }
                    }
                }
                break;
            case XPS_INPUT_SOCKS_STATUS_CONNECT:
                if (len > 6 && pdata->data[0] == XPS_INPUT_SOCKS_VERSION) {
                    if (pdata->data[1] == XPS_INPUT_SOCKS_CMD_CONNECT) {
                        int port = 0;
                        int atype = pdata->data[3];
                        char *hostname = (char *)pdata->data;
                        switch (atype) {
                            case XPS_ADDR_TYPE_IP4:
                                port = (pdata->data[8] << 8) + pdata->data[9];
                                xps_router_actions_t *router = c->inet->core->router;
                                c->endpoint = router->connect_byip(router, *(in_addr_t *)(pdata->data + 4), port);
                                break;
                            case XPS_ADDR_TYPE_HOSTNAME:
                                if (pdata->data[4] > 0) {
                                    int len = pdata->data[4];
                                    port = (pdata->data[5 + len] << 8) + pdata->data[6 + len];
                                    xps_str_t host = { .len = len, .data = hostname + 5 };
                                    xps_router_actions_t *router = c->inet->core->router;
                                    c->endpoint = router->connect_byhost(router, &host, port);
                                }
                                break;
                        }
                        if (c->endpoint != NULL) {
                            static const uint8_t reply[] = { XPS_INPUT_SOCKS_VERSION, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                            pdata->count = sizeof(reply);
                            memcpy(pdata->data, reply, pdata->count);
                            c->endpoint->endpoint = c;
                            c->status = XPS_INPUT_SOCKS_STATUS_RESPONSE;
                            xps_connection_disable_reader(c);
                            xps_connection_enable_writer(c);
                            return;
                        }
                    }
                }
                break;
        }
    }
    xps_connection_close(c);
}

XPS_INLINE void xps_input_socks_writer(xps_event_t *ev) {
    xps_connection_t *c = xps_structof(ev, xps_connection_t, writer);
    if (ev->available) {
        if (write((int)c->ident, c->data->data, c->data->count) == c->data->count) {
            if (c->status != XPS_INPUT_SOCKS_STATUS_RESPONSE) {
                xps_connection_enable_reader(c);
                xps_connection_disable_writer(c);
            } else {
                c->status = XPS_INPUT_SOCKS_STATUS_TRANSPORT;
                //c->reader = xps_connection_transport_reader;
                //c->writer = xps_connection_transport_writer;
                xps_connection_enable_reader(c);
                xps_connection_disable_writer(c);
                //c->endpoint->send(c->endpoint, 0);
            }
            return;
        }
    }
    xps_connection_close(c);
}

XPS_INLINE xps_connection_t *xps_input_socks_accept(xps_core_t *core, int fd) {
    xps_connection_t *c = xps_connection_create(core->inet, fd);
    if (c != NULL) {
        c->status = XPS_INPUT_SOCKS_STATUS_INITED;
        xps_connection_add_reader(c, XPS_EVFLG_ENABLE, xps_input_socks_reader);
        xps_connection_add_writer(c, XPS_EVFLG_DISABLE, xps_input_socks_writer);
        return c;
    }
    return NULL;
}

XPS_MODULE_INPUT_TCP_DECL(socks)
    .accept = xps_input_socks_accept,
XPS_MODULE_END()

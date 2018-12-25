//
//  xps_input_tcp.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_INPUT_TCP_H__
#define __XPS_INPUT_TCP_H__

#include "xps_config.h"
#include "xps_inet.h"
#include "xps_input.h"
#include "xps_event.h"

typedef struct xps_connection       xps_connection_t;
typedef struct xps_input_tcp        xps_input_tcp_t;
typedef struct xps_input_tcp_module xps_input_tcp_module_t;
typedef xps_connection_t *(*xps_input_tcp_accept_pt)(xps_core_t *core, int fd);

struct xps_input_tcp {
    xps_input_t             input;
    xps_event_t             event;
    int                     fd;
    struct sockaddr_in      addr;
    xps_input_tcp_accept_pt accept;
};

struct xps_input_tcp_module {
    XPS_MODULE_INPUT
    xps_input_tcp_accept_pt accept;
};

#define XPS_MODULE_INPUT_TCP_DECL(_name)    \
    XPS_MODULE_DECL(_name, xps_input_tcp_module_t, XPS_MODULE_TYPE_INPUT)   \
    .alloc = xps_input_tcp_alloc,

XPS_API xps_input_t *xps_input_tcp_alloc(xps_core_t *core, xps_input_module_t *input, const char *option);


#endif /* __XPS_INPUT_TCP_H__ */

//
//  xps_input_http.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_input_tcp.h"
#include "xps_core.h"
#include "xps_connection.h"

XPS_INLINE xps_connection_t *xps_input_http_accept(xps_core_t *core, int fd) {
    xps_connection_t *c = xps_connection_create(core->inet, fd);
    if (c != NULL) {
        xps_connection_close(c);
    }
    return NULL;
}

XPS_MODULE_INPUT_TCP_DECL(http)
    .accept = xps_input_http_accept,
XPS_MODULE_END()

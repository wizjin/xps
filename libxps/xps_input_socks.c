//
//  xps_input_socks.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_input_tcp.h"
#include "xps_core.h"

XPS_INLINE xps_connection_t *xps_input_socks_accept(xps_core_t *core, int fd) {
    return NULL;
}

XPS_MODULE_INPUT_TCP_DECL(socks)
    .accept = xps_input_socks_accept,
XPS_MODULE_END()

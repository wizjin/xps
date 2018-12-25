//
//  xps_input_socks.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_input.h"
#include "xps_core.h"
#include "xps_palloc.h"

XPS_INLINE int xps_input_socks_open(xps_input_t *input, xps_core_t *core) {
    return XPS_OK;
}

XPS_INLINE void xps_input_socks_close(xps_input_t *input) {
}

XPS_INLINE xps_input_t *xps_input_socks_alloc(xps_core_t *core, xps_input_module_t *input, const char *option) {
    xps_input_t *in = xps_palloc(core->pool, sizeof(xps_input_t));
    if (in != NULL) {
        in->open = xps_input_socks_open;
        in->close = xps_input_socks_close;
        return in;
    }
    return NULL;
}

XPS_MODULE_INPUT_DECL(socks)
    .alloc = xps_input_socks_alloc,
XPS_MODULE_END()

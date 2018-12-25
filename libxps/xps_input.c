//
//  xps_input.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_input.h"
#include "xps_core.h"

XPS_API int xps_input_module_active(xps_core_t *core, const char *name, const char *option) {
    xps_input_module_t *m = (xps_input_module_t *)xps_modules_find(core, name);
    if (m != NULL && m->type == XPS_MODULE_TYPE_INPUT) {
        xps_input_t *input = m->alloc(core, m, option);
        if (input != NULL) {
            xps_chain_push(&core->inputs, &input->link);
            input->closed = 1;
            return XPS_OK;
        }
    }
    return XPS_ERROR;
}

XPS_API int xps_input_modules_open(xps_core_t *core) {
    int res = XPS_OK;
    for (xps_chain_node_t *p = xps_chain_head(&core->inputs); p != NULL; p = xps_chain_next(p)) {
        xps_input_t *input = xps_structof(p, xps_input_t, link);
        if (input->closed == 1 && input->open != NULL) {
            res = input->open(input, core);
            if (res != XPS_OK) break;
        }
        input->closed = 0;
    }
    return res;
}

XPS_API void xps_input_modules_close(xps_core_t *core) {
    for (xps_chain_node_t *p = xps_chain_head(&core->inputs); p != NULL; p = xps_chain_next(p)) {
        xps_input_t *input = xps_structof(p, xps_input_t, link);
        if (input->close != NULL) input->close(input);
        input->closed = 1;
    }
}

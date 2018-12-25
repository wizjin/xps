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
            return XPS_OK;
        }
    }
    return XPS_ERROR;
}

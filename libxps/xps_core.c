//
//  xps_core.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_core.h"
#include "xps_logger.h"
#include "xps_sys.h"
#include "xps_malloc.h"

XPS_API xps_core_t *xps_core_create(void) {
    if (xps_sys_init() == XPS_OK) {
        xps_core_t *core = xps_calloc(sizeof(xps_core_t));
        if (core != NULL) {
            log_info("xps core created.");
            core->inited = 1;
            return core;
        }
        xps_sys_fini();
    }
    return NULL;
}

XPS_API void xps_core_destory(xps_core_t *core) {
    if (core != NULL) {
        if (core->inited) {
            core->inited = 0;
            log_info("xps core destroyed.");
        }
        xps_free(core);
    }
    xps_sys_fini();
}

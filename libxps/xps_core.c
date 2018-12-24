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
#include "xps_module.h"

XPS_API xps_core_t *xps_core_create(void) {
    if (xps_sys_init() == XPS_OK) {
        // TODO: malloc core from pool.
        xps_core_t *core = xps_calloc(sizeof(xps_core_t));
        if (core != NULL) {
            core->pool = xps_pool_create(XPS_POOL_DEFAULT_SIZE);
            if (core->pool != NULL) {
                if (xps_module_load(core, NULL) == 0) {
                    core->inited = 1;
                    log_info("xps core created.");
                    log_info("memory used: %.2lfKB", xps_get_used_memory()/1024.0);
                    return core;
                }
            }
            
        }
        xps_core_destory(core);
    }
    return NULL;
}

XPS_API void xps_core_destory(xps_core_t *core) {
    if (core != NULL) {
        xps_core_stop(core);
        if (core->inited) {
            core->inited = 0;
            log_info("xps core destroyed.");
        }
        if (core->pool != NULL) {
            xps_pool_destroy(core->pool);
            core->pool = NULL;
        }
        xps_free(core);
    }
    xps_sys_fini();
}

XPS_API int xps_core_start(xps_core_t *core) {
    if (core != NULL && core->started == 0) {
        core->started = 1;
        return XPS_OK;
    }
    return XPS_ERROR;
}

XPS_API void xps_core_stop(xps_core_t *core) {
    if (core != NULL && core->started == 1) {
        core->started = 0;
    }
}

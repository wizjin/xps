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
#include "xps_event.h"

#define XPS_MODULE_LIST             \
    XPS_MODULE_IMPORT(kqueue)       \

#define XPS_MODULE_IMPORT(_name)    XPS_EXTERN xps_module_t *XPS_MODULE_NAME(_name);
XPS_MODULE_LIST
#undef XPS_MODULE_IMPORT
#define XPS_MODULE_IMPORT(_name)    XPS_MODULE_NAME(_name),

XPS_API xps_core_t *xps_core_create(void) {
    if (xps_sys_init() == XPS_OK) {
        // TODO: malloc core from pool.
        xps_core_t *core = xps_calloc(sizeof(xps_core_t));
        if (core != NULL) {
            core->pool = xps_pool_create(XPS_POOL_DEFAULT_SIZE);
            if (core->pool != NULL) {
                if (xps_module_load(core, XPS_MODULE_LIST NULL) == XPS_OK) {
                    core->inited = 1;
                    log_info("xps core created.");
                    log_info("pool free: %.2lfKB", xps_pool_get_free_size(core->pool)/1024.0);
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

XPS_INLINE void *xps_core_worker(void *ptr) {
    log_trace("xps core worker enter");
    xps_core_t *core = (xps_core_t *)ptr;
    if (core != NULL) {
        int running = 1;
        while (running) {
            int res = xps_action_call(&core->workers);
            if (res != XPS_OK) {
                if (res != XPS_DONE) {
                    log_error("xps core worker failed: %d", res);
                }
                running = 0;
                break;
            }
        }
    }
    log_trace("xps core worker leave");
    return NULL;
}

XPS_API int xps_core_start(xps_core_t *core) {
    if (core != NULL && core->started == 0 && core->worker == NULL) {
        core->started = 1;
        xps_event_manager_sign(core->event_manager, XPS_EVENT_SIGN_INIT);
        if (pthread_create(&core->worker, NULL, xps_core_worker, core) != 0) {
            log_error("pthread_create failed: %s", strerror(errno));
        } else {
            return XPS_OK;
        }
        xps_core_stop(core);
    }
    return XPS_ERROR;
}

XPS_API void xps_core_stop(xps_core_t *core) {
    if (core != NULL && core->started == 1) {
        core->started = 0;
        if (core->worker != NULL) {
            xps_event_manager_sign(core->event_manager, XPS_EVENT_SIGN_EXIT);
            pthread_join(core->worker, NULL);
            core->worker = NULL;
        }
    }
}


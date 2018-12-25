//
//  xps_module.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_module.h"
#include <stdarg.h>
#include "xps_logger.h"
#include "xps_core.h"

typedef struct {
    xps_module_t    *module;
    xps_queue_t     queue;
} xps_module_node_t;

XPS_API void xps_modules_init(xps_modules_t *modules) {
    xps_queue_init(&modules->queue);
}

XPS_API int xps_modules_load(xps_core_t *core, ...) {
    int res = XPS_OK;
    va_list ap;
    va_start(ap, core);
    xps_module_t *m;
    while ((m = va_arg(ap, xps_module_t *)) != NULL) {
        if (m->loaded == 0) {
            if (m->load != NULL && (res = m->load(core)) != XPS_OK) {
                log_error("load %s module failed: %d", m->name.data, res);
                break;
            }
            m->loaded = 1;
            xps_module_node_t *node = xps_palloc(core->pool, sizeof(xps_module_node_t));
            if (node == NULL) {
                log_error("store %s module failed: %d", m->name.data, res);
                break;
            }
            
            xps_queue_push_tail(&core->modules.queue, &node->queue);
            node->module = m;
            log_info("load %s module success.", m->name.data);
        }
    }
    va_end(ap);
    return res;
}

XPS_API xps_module_t *xps_modules_find(xps_core_t *core, const char *name) {
    xps_module_t *module = NULL;
    for (xps_queue_t *n = xps_queue_head(&core->modules.queue); n != xps_queue_sentinel(&core->modules.queue); n = xps_queue_next(n)) {
        xps_module_node_t *m = xps_queue_data(n, xps_module_node_t, queue);
        if (strcmp(name, m->module->name.data) == 0) {
            return m->module;
        }
    }
    return module;
}

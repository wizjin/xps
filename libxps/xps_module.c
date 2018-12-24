//
//  xps_module.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_module.h"
#include <stdarg.h>
#include "xps_logger.h"

XPS_API int xps_module_load(xps_core_t *core, ...) {
    int res = XPS_OK;
    va_list ap;
    va_start(ap, core);
    xps_module_t *m;
    while ((m = va_arg(ap, xps_module_t *)) != NULL) {
        if (m->loaded != 0) {
            if (m->load != NULL && (res = m->load(core)) != XPS_OK) {
                log_error("load %s module failed: %d", m->name.data, res);
                break;
            }
            m->loaded = 1;
            log_info("load %s module success.", m->name.data);
        }
    }
    va_end(ap);
    return res;
}

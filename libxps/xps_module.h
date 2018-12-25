//
//  xps_module.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_MODULE_H__
#define __XPS_MODULE_H__

#include "xps_config.h"
#include "xps_string.h"
#include "xps_queue.h"

typedef struct xps_core     xps_core_t;
typedef struct xps_module   xps_module_t;
typedef struct xps_modules  xps_modules_t;

#define XPS_MODULE_COMMON           \
    xps_str_t   name;               \
    unsigned    type:8;             \
    unsigned    loaded:1;           \
    int (*load)(xps_core_t *core);

struct xps_module {
    XPS_MODULE_COMMON
};

struct xps_modules {
    xps_queue_t         queue;
};

XPS_API void xps_modules_init(xps_modules_t *modules);
XPS_API int xps_modules_load(xps_core_t *core, ...);
XPS_API xps_module_t *xps_modules_find(xps_core_t *core, const char *name);

#define XPS_MODULE_NAME(_name)              xps_##_name##_module
#define XPS_MODULE_END()                    };
#define XPS_MODULE_COMMON_INIT(_name)       .name = xps_string(#_name), .loaded = 0
#define XPS_MODULE_DECL(_name, _class, _type)   \
    XPS_EXTERN_DEFINE xps_module_t *XPS_MODULE_NAME(_name) = (xps_module_t *)&(_class){ \
    XPS_MODULE_COMMON_INIT(_name),              \
    .type = (_type),

// Module Types
#define XPS_MODULE_TYPE_SYS                 1
#define XPS_MODULE_TYPE_EVENT               2
#define XPS_MODULE_TYPE_INPUT               3

#define XPS_MODULE_SYS_DECL(_name)          XPS_MODULE_DECL(_name, xps_module_t, XPS_MODULE_TYPE_SYS)


#endif /* __XPS_MODULE_H__ */

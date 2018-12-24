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

typedef struct xps_core     xps_core_t;
typedef struct xps_module   xps_module_t;
typedef int (*xps_module_load_pt)(xps_core_t *core);

struct xps_module {
    xps_str_t           name;
    unsigned            type:8;
    unsigned            loaded:1;
    xps_module_load_pt  load;
};

XPS_API int xps_module_load(xps_core_t *core, ...);



#endif /* __XPS_MODULE_H__ */

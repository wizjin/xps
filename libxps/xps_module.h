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

typedef struct xps_module xps_module_t;

struct xps_module {
    xps_str_t   name;
    unsigned    type:8;
    unsigned    loaded:1;
};


#endif /* __XPS_MODULE_H__ */

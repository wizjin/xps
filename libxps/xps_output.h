//
//  xps_output.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_OUTPUT_H__
#define __XPS_OUTPUT_H__

#include "xps_config.h"
#include "xps_module.h"
#include "xps_inet.h"

typedef struct xps_connection       xps_connection_t;
typedef struct xps_output_module    xps_output_module_t;
typedef xps_connection_t *(*xps_output_alloc_pt)(xps_core_t *core, xps_inet_addr_t *addr);

struct xps_output_module {
    XPS_MODULE_COMMON
    xps_output_alloc_pt  alloc;
};

#define XPS_MODULE_OUTPUT_DECL(_name)   XPS_MODULE_DECL(_name, xps_output_module_t, XPS_MODULE_TYPE_OUTPUT)


#endif /* __XPS_OUTPUT_H__ */

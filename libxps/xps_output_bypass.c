//
//  xps_output_bypass.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_output.h"
#include "xps_core.h"
#include "xps_connection.h"

XPS_API xps_connection_t *xps_output_bypass_alloc(xps_core_t *core, xps_inet_addr_t *addr) {
    // TODO: bypass connection.
    return NULL;
}

XPS_MODULE_OUTPUT_DECL(bypass)
    .alloc  = xps_output_bypass_alloc,
XPS_MODULE_END()

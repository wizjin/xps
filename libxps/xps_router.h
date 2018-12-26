//
//  xps_router.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_ROUTER_H__
#define __XPS_ROUTER_H__

#include "xps_config.h"
#include "xps_inet.h"
#include "xps_string.h"

typedef struct xps_connection       xps_connection_t;
typedef struct xps_router_actions   xps_router_actions_t;

struct xps_router_actions {
    xps_connection_t *(*connect)(xps_router_actions_t *acts, xps_inet_addr_t *addr);
};


#endif /* __XPS_ROUTER_H__ */

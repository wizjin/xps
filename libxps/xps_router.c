//
//  xps_router.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_router.h"
#include "xps_module.h"
#include "xps_core.h"
#include "xps_output.h"

typedef struct {
    xps_router_actions_t    actions;
    xps_core_t              *core;
    xps_output_alloc_pt     bypass;
} xps_router_t;

XPS_INLINE xps_connection_t *xps_router_connect_byip(xps_router_actions_t *acts, in_addr_t addr, uint16_t port) {
    xps_router_t *router = xps_structof(acts, xps_router_t, actions);
    xps_inet_addr_t a;
    bzero(&a, sizeof(a));
    a.type  = XPS_ADDR_TYPE_IP4;
    a.port  = htons(port);
    a.addr  = addr;
    return router->bypass(router->core, &a);
}

XPS_INLINE xps_connection_t *xps_router_connect_byhost(xps_router_actions_t *acts, xps_str_t *hostname, uint16_t port) {
    xps_router_t *router = xps_structof(acts, xps_router_t, actions);
    xps_inet_addr_t a;
    bzero(&a, sizeof(a));
    a.type  = XPS_ADDR_TYPE_HOSTNAME;
    a.port  = htons(port);
    a.host  = hostname;
    return router->bypass(router->core, &a);
}

XPS_INLINE void xps_router_cleanup(void *ptr) {
}

XPS_INLINE int xps_router_load(xps_core_t *core) {
    xps_pool_cleanup_t *c = xps_pool_cleanup_add(core->pool, sizeof(xps_router_t));
    if (c != NULL) {
        c->handler = xps_router_cleanup;
        xps_router_t *router = c->data;
        router->actions.connect_byip    = xps_router_connect_byip;
        router->actions.connect_byhost  = xps_router_connect_byhost;
        router->core    = core;
        core->router    = &router->actions;

        xps_output_module_t *m  = (xps_output_module_t *)xps_modules_find(core, "bypass");
        if (m != NULL) {
            router->bypass  = m->alloc;
            return XPS_OK;
        }
    }
    return XPS_ERROR;
}

XPS_MODULE_SYS_DECL(router)
    .load = xps_router_load,
XPS_MODULE_END()

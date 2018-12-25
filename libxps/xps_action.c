//
//  xps_action.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_action.h"

XPS_API int xps_action_call(xps_chain_t *act) {
    int res = XPS_OK;
    for (xps_chain_node_t *p = xps_chain_head(act); p != NULL; p = xps_chain_next(p)) {
        xps_action_node_t *action = xps_structof(p, xps_action_node_t, link);
        res = action->action(action->ctx);
        if (res != XPS_OK) break;
    }
    return res;
}

XPS_API int xps_action_push(xps_chain_t *act, xps_pool_t *pool, xps_action_pt action, void *ctx) {
    xps_action_node_t * p = xps_palloc(pool, sizeof(xps_action_node_t));
    if (p != NULL) {
        xps_chain_push(act, &p->link);
        p->action   = action;
        p->ctx      = ctx;
        return XPS_OK;
    }
    return XPS_ERROR;
}

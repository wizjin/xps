//
//  xps_action.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_action.h"

XPS_API int xps_action_call(xps_action_t *act) {
    int res = XPS_OK;
    for (xps_action_node_t *p = act->head; p != NULL; p = p->next) {
        res = p->action(p->ctx);
        if (res != XPS_OK) break;
    }
    return res;
}

XPS_API int xps_action_push(xps_action_t *act, xps_pool_t *pool, xps_action_pt action, void *ctx) {
    xps_action_node_t * p = xps_palloc(pool, sizeof(xps_action_node_t));
    if (p != NULL) {
        p->next     = NULL;
        p->action   = action;
        p->ctx      = ctx;
        if (act->head == NULL) {
            act->head = p;
        }
        if (act->tail != NULL) {
            act->tail->next = p;
        }
        act->tail = p;
        return XPS_OK;
    }
    return XPS_ERROR;
}

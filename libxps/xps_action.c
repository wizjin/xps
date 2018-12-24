//
//  xps_action.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_action.h"

XPS_API int xps_action_call(xps_action_t *sentinel) {
    int res = XPS_OK;
    for (xps_action_t *p = sentinel->next; p != NULL; p = p->next) {
        res = p->action(p->ctx);
        if (res != XPS_OK) break;
    }
    return res;
}

XPS_API int xps_action_push(xps_action_t *sentinel, xps_pool_t *pool, xps_action_pt action, void *ctx) {
    xps_action_t * p = xps_palloc(pool, sizeof(xps_action_t));
    if (p != NULL) {
        p->next         = sentinel->next;
        sentinel->next  = p;
        p->action       = action;
        p->ctx          = ctx;
        return XPS_OK;
    }
    return XPS_ERROR;
}

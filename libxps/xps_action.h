//
//  xps_action.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_ACTION_H__
#define __XPS_ACTION_H__

#include "xps_config.h"
#include "xps_palloc.h"

typedef struct xps_action xps_action_t;
typedef int (*xps_action_pt)(void *ctx);

struct xps_action {
    xps_action_t    *next;
    xps_action_pt   action;
    void            *ctx;
};


XPS_API int xps_action_call(xps_action_t *head);
XPS_API int xps_action_push(xps_action_t *head, xps_pool_t *pool, xps_action_pt action, void *ctx);


#endif /* __XPS_ACTION_H__ */

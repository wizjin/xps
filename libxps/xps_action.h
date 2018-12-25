//
//  xps_action.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_ACTION_H__
#define __XPS_ACTION_H__

#include "xps_config.h"
#include "xps_chain.h"
#include "xps_palloc.h"

typedef struct xps_action_node  xps_action_node_t;
typedef int (*xps_action_pt)(void *ctx);

struct xps_action_node {
    xps_chain_node_t    link;
    xps_action_pt       action;
    void                *ctx;
};

XPS_API int xps_action_call(xps_chain_t *act);
XPS_API int xps_action_push(xps_chain_t *act, xps_pool_t *pool, xps_action_pt action, void *ctx);


#endif /* __XPS_ACTION_H__ */

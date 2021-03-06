//
//  xps_core.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_CORE_H__
#define __XPS_CORE_H__

#include "xps_config.h"
#include <pthread.h>
#include "xps_logger.h"
#include "xps_palloc.h"
#include "xps_action.h"
#include "xps_event.h"

typedef struct xps_core             xps_core_t;
typedef struct xps_inet             xps_inet_t;
typedef struct xps_cache            xps_cache_t;
typedef struct xps_modules          xps_modules_t;
typedef struct xps_router_actions   xps_router_actions_t;

struct xps_core {
    xps_pool_t              *pool;
    xps_inet_t              *inet;
    xps_cache_t             *cache;
    xps_event_actions_t     *evacts;
    xps_router_actions_t    *router;
    xps_event_notify_t      *notify;
    xps_chain_t             workers;
    xps_chain_t             inputs;
    xps_modules_t           modules;
    pthread_t               worker;

    unsigned    inited:1;
    unsigned    started:1;
};

XPS_API xps_core_t *xps_core_create(void);
XPS_API void xps_core_destory(xps_core_t *core);
XPS_API int xps_core_start(xps_core_t *core);
XPS_API void xps_core_stop(xps_core_t *core);

#define xps_core_add_worker(_core, _worker, _ctx)   \
    xps_action_push(&(_core)->workers, (_core)->pool, _worker, _ctx)


#endif /* __XPS_CORE_H__ */

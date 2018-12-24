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
#include "xps_action.h"

typedef struct xps_core             xps_core_t;
typedef struct xps_event_manager    xps_event_manager_t;

struct xps_core {
    xps_pool_t          *pool;
    xps_event_manager_t *event_manager;
    xps_queue_t         modules;
    xps_action_t        workers;
    pthread_t           worker;

    unsigned            inited:1;
    unsigned            started:1;
};

XPS_API xps_core_t *xps_core_create(void);
XPS_API void xps_core_destory(xps_core_t *core);
XPS_API int xps_core_start(xps_core_t *core);
XPS_API void xps_core_stop(xps_core_t *core);

#define xps_core_add_worker(_core, _worker, _ctx)   xps_action_push(&(_core)->workers, (_core)->pool, _worker, _ctx)


#endif /* __XPS_CORE_H__ */

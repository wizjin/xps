//
//  xps_core.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_CORE_H__
#define __XPS_CORE_H__

#include "xps_config.h"
#include "xps_palloc.h"

typedef struct xps_core xps_core_t;

struct xps_core {
    xps_pool_t  *pool;
    xps_queue_t modules;
    unsigned    inited:1;
    unsigned    started:1;
};

XPS_API xps_core_t *xps_core_create(void);
XPS_API void xps_core_destory(xps_core_t *core);
XPS_API int xps_core_start(xps_core_t *core);
XPS_API void xps_core_stop(xps_core_t *core);


#endif /* __XPS_CORE_H__ */

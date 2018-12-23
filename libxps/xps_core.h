//
//  xps_core.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_CORE_H__
#define __XPS_CORE_H__

#include "xps_config.h"
#include "xps_queue.h"

typedef struct xps_core xps_core_t;

struct xps_core {
    unsigned    inited;
    xps_queue_t modules;
};

XPS_API xps_core_t *xps_core_create(void);
XPS_API void xps_core_destory(xps_core_t *core);


#endif /* __XPS_CORE_H__ */

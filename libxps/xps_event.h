//
//  xps_event.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_EVENT_H__
#define __XPS_EVENT_H__

#include "xps_config.h"
#include "xps_module.h"

#define XPS_EVENT_SIGN_INIT     0
#define XPS_EVENT_SIGN_EXIT     1

typedef struct xps_event_manager xps_event_manager_t;

struct xps_event_manager {
    void (*sign)(xps_event_manager_t *manager, int sign);
};

XPS_API void xps_event_manager_sign(xps_event_manager_t *mgr, int sign);

#define XPS_MODULE_EVENT_DECL(_name)    XPS_MODULE_DECL(_name, xps_module_t, XPS_MODULE_TYPE_EVENT)


#endif /* __XPS_EVENT_H__ */

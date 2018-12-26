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

#define XPS_EVFLG_READ              0x01
#define XPS_EVFLG_WRITE             0x02
#define XPS_EVFLG_ENABLE            0x04
#define XPS_EVFLG_DISABLE           0x08

typedef struct xps_event            xps_event_t;
typedef struct xps_event_timer      xps_event_timer_t;
typedef struct xps_event_notify     xps_event_notify_t;
typedef struct xps_event_actions    xps_event_actions_t;
typedef void (*xps_event_handler_pt)(xps_event_t *event);
typedef int (*xps_event_time_handler_pt)(xps_event_timer_t *timer);
typedef int (*xps_event_notify_handler_pt)(xps_event_notify_t *notify);

struct xps_event {
    xps_event_handler_pt    handler;
    int                     fd;
    uint16_t                event;
    unsigned                available;
    unsigned                eof:1;
    unsigned                closed:1;
};

struct xps_event_timer {
    uintptr_t   data;
    void        *ctx;
    xps_event_time_handler_pt   handler;
    void (*del)(xps_event_timer_t *timer);
};

struct xps_event_notify {
    uintptr_t                   data;
    xps_event_notify_handler_pt handler;
    void (*send)(xps_event_notify_t *notify);
    void (*reset)(xps_event_notify_t *notify);
    void (*close)(xps_event_notify_t *notify);
};

struct xps_event_actions {
    void *ctx;
    void (*add)(xps_event_actions_t *acts, xps_event_t *ev, int fd, unsigned flags);
    void (*set)(xps_event_actions_t *acts, xps_event_t *ev, unsigned flags);
    void (*del)(xps_event_actions_t *acts, xps_event_t *ev);
    void (*flush)(xps_event_actions_t *acts);
    xps_event_timer_t *(*add_timer)(xps_event_actions_t *acts, xps_event_time_handler_pt handler, unsigned interval, void *ptr);
    xps_event_notify_t *(*add_notify)(xps_event_actions_t *acts, xps_event_notify_handler_pt handler);
};

#define XPS_MODULE_EVENT_DECL(_name)    XPS_MODULE_DECL(_name, xps_module_t, XPS_MODULE_TYPE_EVENT)


#endif /* __XPS_EVENT_H__ */

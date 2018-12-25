//
//  xps_connection.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_CONNECTION_H__
#define __XPS_CONNECTION_H__

#include "xps_config.h"
#include "xps_event.h"

typedef struct xps_inet         xps_inet_t;
typedef struct xps_connection   xps_connection_t;

struct xps_connection {
    xps_event_t read;
    xps_event_t write;
    xps_queue_t queue;
    xps_inet_t  *inet;
    int         fd;

    unsigned    destroyed:1;
    unsigned    closed:1;
    unsigned    read_ev:1;
    unsigned    write_ev:1;
};

XPS_API xps_connection_t *xps_connection_alloc(xps_inet_t *inet, int fd);
XPS_API void xps_connection_close(xps_connection_t *c);


#endif /* __XPS_CONNECTION_H__ */

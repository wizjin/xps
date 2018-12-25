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
#include "xps_buffer.h"

typedef struct xps_inet         xps_inet_t;
typedef struct xps_connection   xps_connection_t;
typedef void (*xps_connection_colser_pt)(xps_connection_t *c);

struct xps_inet {
    xps_queue_t used;
    xps_queue_t closed;
    xps_queue_t freed;
    xps_core_t  *core;
    size_t      alloced;
    size_t      maxn;
};

struct xps_connection {
    xps_queue_t                 queue;
    xps_inet_t                  *inet;
    xps_buffer_t                *data;
    xps_connection_t            *endpoint;
    xps_connection_colser_pt    closer;
    
    xps_event_t reader;
    xps_event_t writer;
    uintptr_t   ident;

    unsigned    status:8;
    unsigned    destroyed:1;
    unsigned    closed:1;
    unsigned    reader_ev:1;
    unsigned    writer_ev:1;
};

XPS_API xps_connection_t *xps_connection_create(xps_inet_t *inet, int fd);
XPS_API void xps_connection_close(xps_connection_t *c);
XPS_API void xps_connection_add_reader(xps_connection_t *c, unsigned flags, xps_event_handler_pt handler);
XPS_API void xps_connection_add_writer(xps_connection_t *c, unsigned flags, xps_event_handler_pt handler);

#define xps_connection_set_event(c, e, f)   (c)->inet->core->evacts->set((c)->inet->core->evacts, (e), (f))
#define xps_connection_set_reader(c, r)     (c)->reader.handler = (r)
#define xps_connection_set_writer(c, w)     (c)->writer.handler = (w)
#define xps_connection_enable_reader(c)     xps_connection_set_event((c), &(c)->reader, XPS_EVFLG_ENABLE)
#define xps_connection_disable_reader(c)    xps_connection_set_event((c), &(c)->reader, XPS_EVFLG_DISABLE)
#define xps_connection_enable_writer(c)     xps_connection_set_event((c), &(c)->writer, XPS_EVFLG_ENABLE)
#define xps_connection_disable_writer(c)    xps_connection_set_event((c), &(c)->writer, XPS_EVFLG_DISABLE)


#endif /* __XPS_CONNECTION_H__ */

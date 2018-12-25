//
//  xps_queue.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_QUEUE_H__
#define __XPS_QUEUE_H__

#include "xps_config.h"

typedef struct xps_queue xps_queue_t;

struct xps_queue {
    xps_queue_t *prev;
    xps_queue_t *next;
};

#define xps_queue_init(h)                           \
    (h)->prev = h;                                  \
    (h)->next = h

#define xps_queue_empty(h)                          \
    ((h) == (h)->prev)

#define xps_queue_sentinel(h)                       \
    (h)

#define xps_queue_head(h)                           \
    (h)->next

#define xps_queue_tail(h)                           \
    (h)->prev

#define xps_queue_push_head(h, e)                   \
    (e)->next = (h)->next;                          \
    (e)->next->prev = e;                            \
    (e)->prev = h;                                  \
    (h)->next = e

#define xps_queue_insert_after   xps_queue_push_head

#define xps_queue_push_tail(h, e)                   \
    (e)->prev = (h)->prev;                          \
    (e)->prev->next = e;                            \
    (e)->next = h;                                  \
    (h)->prev = e

#define xps_queue_next(q)                           \
    (q)->next

#define xps_queue_prev(q)                           \
    (q)->prev

#define xps_queue_remove(x)                         \
    (x)->next->prev = (x)->prev;                    \
    (x)->prev->next = (x)->next

XPS_API void xps_queue_sort(xps_queue_t *queue, int (*cmp)(const xps_queue_t *, const xps_queue_t *));


#endif /* __XPS_QUEUE_H__ */

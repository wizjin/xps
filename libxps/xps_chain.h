//
//  xps_chain.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_CHAIN_H__
#define __XPS_CHAIN_H__

#include "xps_config.h"

typedef struct xps_chain        xps_chain_t;
typedef struct xps_chain_node   xps_chain_node_t;

struct xps_chain_node {
    xps_chain_node_t    *next;
};

struct xps_chain {
    xps_chain_node_t    sentinel;
    xps_chain_node_t    *tail;
};

#define xps_chain_init(c)       \
    (c)->sentinel.next = NULL;  \
    (c)->tail = &(c)->sentinel

#define xps_chain_push(c, l)    \
    (l)->next = NULL;           \
    (c)->tail->next = (l);      \
    (c)->tail = (l)

#define xps_chain_head(c)       \
    (c)->sentinel.next

#define xps_chain_next(l)       \
    (l)->next


#endif /* __XPS_CHAIN_H__ */

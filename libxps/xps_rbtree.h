//
//  xps_rbtree.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_RBTREE_H__
#define __XPS_RBTREE_H__

#include "xps_config.h"

typedef unsigned xps_rbtree_key_t;
typedef struct xps_rbtree       xps_rbtree_t;
typedef struct xps_rbtree_node  xps_rbtree_node_t;
typedef void (*xps_rbtree_insert_pt) (xps_rbtree_node_t *root, xps_rbtree_node_t *node, xps_rbtree_node_t *sentinel);

struct xps_rbtree_node {
    xps_rbtree_key_t    key;
    xps_rbtree_node_t   *left;
    xps_rbtree_node_t   *right;
    xps_rbtree_node_t   *parent;
    uint8_t             color;
    uint8_t             data;
};

struct xps_rbtree {
    xps_rbtree_node_t       *root;
    xps_rbtree_node_t       *sentinel;
    xps_rbtree_insert_pt    insert;
};

#define xps_rbtree_init(t, s, i)            \
            xps_rbtree_sentinel_init(s);    \
            (t)->root = s;                  \
            (t)->sentinel = s;              \
            (t)->insert = i

#define xps_rbt_red(node)                   ((node)->color = 1)
#define xps_rbt_black(node)                 ((node)->color = 0)
#define xps_rbt_is_red(node)                ((node)->color)
#define xps_rbt_is_black(node)              (!xps_rbt_is_red(node))
#define xps_rbt_copy_color(n1, n2)          (n1->color = n2->color)
#define xps_rbtree_sentinel_init(node)      xps_rbt_black(node)

XPS_API void xps_rbtree_insert(xps_rbtree_t *tree, xps_rbtree_node_t *node);
XPS_API void xps_rbtree_delete(xps_rbtree_t *tree, xps_rbtree_node_t *node);
XPS_API xps_rbtree_node_t *xps_rbtree_min(xps_rbtree_node_t *node, xps_rbtree_node_t *sentinel);
XPS_API void xps_rbtree_insert_value(xps_rbtree_node_t *root, xps_rbtree_node_t *node, xps_rbtree_node_t *sentinel);


#endif /* __XPS_RBTREE_H__ */

//
//  xps_rbtree.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_rbtree.h"

XPS_INLINE void xps_rbtree_left_rotate(xps_rbtree_node_t **root, xps_rbtree_node_t *sentinel, xps_rbtree_node_t *node) {
    xps_rbtree_node_t   *temp = node->right;
    node->right = temp->left;
    if (temp->left != sentinel) {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }
    temp->left = node;
    node->parent = temp;
}

XPS_INLINE void xps_rbtree_right_rotate(xps_rbtree_node_t **root, xps_rbtree_node_t *sentinel, xps_rbtree_node_t *node) {
    xps_rbtree_node_t   *temp = node->left;
    node->left = temp->right;
    if (temp->right != sentinel) {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->right) {
        node->parent->right = temp;
    } else {
        node->parent->left = temp;
    }
    temp->right = node;
    node->parent = temp;
}

XPS_API void xps_rbtree_insert(xps_rbtree_t *tree, xps_rbtree_node_t *node) {
    /* a binary tree insert */
    xps_rbtree_node_t **root = &tree->root;
    xps_rbtree_node_t *sentinel = tree->sentinel;
    if (*root == sentinel) {
        node->parent    = NULL;
        node->left      = sentinel;
        node->right     = sentinel;
        xps_rbt_black(node);
        *root = node;
        return;
    }
    tree->insert(*root, node, sentinel);
    
    /* re-balance tree */
    xps_rbtree_node_t *temp;
    while (node != *root && xps_rbt_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;
            
            if (xps_rbt_is_red(temp)) {
                xps_rbt_black(node->parent);
                xps_rbt_black(temp);
                xps_rbt_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    xps_rbtree_left_rotate(root, sentinel, node);
                }
                xps_rbt_black(node->parent);
                xps_rbt_red(node->parent->parent);
                xps_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }
        } else {
            temp = node->parent->parent->left;
            if (xps_rbt_is_red(temp)) {
                xps_rbt_black(node->parent);
                xps_rbt_black(temp);
                xps_rbt_red(node->parent->parent);
                node = node->parent->parent;
                
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    xps_rbtree_right_rotate(root, sentinel, node);
                }
                
                xps_rbt_black(node->parent);
                xps_rbt_red(node->parent->parent);
                xps_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }
    xps_rbt_black(*root);
}

XPS_API void xps_rbtree_delete(xps_rbtree_t *tree, xps_rbtree_node_t *node) {
    /* a binary tree delete */
    xps_rbtree_node_t   *subst, *temp;
    xps_rbtree_node_t   **root = &tree->root;
    xps_rbtree_node_t   *sentinel = tree->sentinel;
    if (node->left == sentinel) {
        temp = node->right;
        subst = node;
    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;
    } else {
        subst = xps_rbtree_min(node->right, sentinel);
        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }
    if (subst == *root) {
        *root = temp;
        xps_rbt_black(temp);
        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;
        return;
    }
    int red = xps_rbt_is_red(subst);
    if (subst == subst->parent->left) {
        subst->parent->left = temp;
    } else {
        subst->parent->right = temp;
    }
    if (subst == node) {
        temp->parent = subst->parent;
    } else {
        if (subst->parent == node) {
            temp->parent = subst;
        } else {
            temp->parent = subst->parent;
        }
        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        xps_rbt_copy_color(subst, node);
        if (node == *root) {
            *root = subst;
        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }
        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }
        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }
    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;
    if (red) {
        return;
    }
    /* a delete fixup */
    xps_rbtree_node_t *w;
    while (temp != *root && xps_rbt_is_black(temp)) {
        if (temp == temp->parent->left) {
            w = temp->parent->right;
            if (xps_rbt_is_red(w)) {
                xps_rbt_black(w);
                xps_rbt_red(temp->parent);
                xps_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }
            if (xps_rbt_is_black(w->left) && xps_rbt_is_black(w->right)) {
                xps_rbt_red(w);
                temp = temp->parent;
            } else {
                if (xps_rbt_is_black(w->right)) {
                    xps_rbt_black(w->left);
                    xps_rbt_red(w);
                    xps_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }
                xps_rbt_copy_color(w, temp->parent);
                xps_rbt_black(temp->parent);
                xps_rbt_black(w->right);
                xps_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        } else {
            w = temp->parent->left;
            if (xps_rbt_is_red(w)) {
                xps_rbt_black(w);
                xps_rbt_red(temp->parent);
                xps_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }
            if (xps_rbt_is_black(w->left) && xps_rbt_is_black(w->right)) {
                xps_rbt_red(w);
                temp = temp->parent;
            } else {
                if (xps_rbt_is_black(w->left)) {
                    xps_rbt_black(w->right);
                    xps_rbt_red(w);
                    xps_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }
                xps_rbt_copy_color(w, temp->parent);
                xps_rbt_black(temp->parent);
                xps_rbt_black(w->left);
                xps_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }
    xps_rbt_black(temp);
}

XPS_API xps_rbtree_node_t *xps_rbtree_min(xps_rbtree_node_t *node, xps_rbtree_node_t *sentinel) {
    while (node->left != sentinel) {
        node = node->left;
    }
    return node;
}

XPS_API void xps_rbtree_insert_value(xps_rbtree_node_t *root, xps_rbtree_node_t *node, xps_rbtree_node_t *sentinel) {
    xps_rbtree_node_t   **p;
    xps_rbtree_node_t   *temp = root;
    for (;;) {
        p = (node->key < temp->key) ? &temp->left : &temp->right;
        if (*p == sentinel) {
            break;
        }
        temp = *p;
    }
    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    xps_rbt_red(node);
}

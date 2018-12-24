//
//  xps_palloc.c
//  XPS
//
//  Created by WizJin on 2018/12/23.
//

#include "xps_palloc.h"
#include "xps_logger.h"
#include "xps_malloc.h"

#define xps_align(d, a)             (((d) + (a - 1)) & ~(a - 1))
#define xps_align_ptr(p, a)         (uint8_t *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))
#define XPS_POOL_ALIGNMENT          16
#define XPS_MAX_ALLOC_FROM_POOL     (xps_pagesize - 1)

XPS_INLINE void *xps_palloc_block(xps_pool_t *pool, size_t size) {
    size_t psize = (size_t) (pool->d.end - (uint8_t *)pool);
    uint8_t *m = xps_memalign(XPS_POOL_ALIGNMENT, psize);
    if (m != NULL) {
        xps_pool_t *new = (xps_pool_t *)m;
        new->d.end      = m + psize;
        new->d.next     = NULL;
        new->d.failed   = 0;
        m               += sizeof(xps_pool_data_t);
        m               = xps_align_ptr(m, XPS_ALIGNMENT);
        new->d.last     = m + size;
        xps_pool_t *p;
        for (p = pool->current; p->d.next; p = p->d.next) {
            if (p->d.failed++ > 4) {
                pool->current = p->d.next;
            }
        }
        p->d.next = new;
    }
    return m;
}

XPS_INLINE void *xps_palloc_small(xps_pool_t *pool, size_t size) {
    xps_pool_t *p = pool->current;
    do {
        uint8_t *m = xps_align_ptr(p->d.last, XPS_ALIGNMENT);
        if ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;
            return m;
        }
        p = p->d.next;
    } while (p != NULL);
    return xps_palloc_block(pool, size);
}

XPS_INLINE void *xps_palloc_large(xps_pool_t *pool, size_t size) {
    void *p = xps_memalign(XPS_POOL_ALIGNMENT, size);
    if (p != NULL) {
        xps_pool_large_t  *large;
        int n = 0;
        for (large = pool->large; large != NULL; large = large->next) {
            if (large->alloc == NULL) {
                large->alloc = p;
                return p;
            }
            if (n++ > 3) break;
        }
        large = xps_palloc_small(pool, sizeof(xps_pool_large_t));
        if (large == NULL) {
            xps_free(p);
            return NULL;
        }
        large->alloc    = p;
        large->next     = pool->large;
        pool->large     = large;
    }
    return p;
}

static xps_queue_t  xps_pool_used_queue;

XPS_PRIVATE void xps_pool_init(void) {
    xps_queue_init(&xps_pool_used_queue);
}

XPS_PRIVATE void xps_pool_fini(void) {
    int i = 0;
    while (!xps_queue_empty(&xps_pool_used_queue)) {
        xps_queue_t *q = xps_queue_head(&xps_pool_used_queue);
        xps_pool_destroy(xps_queue_data(q, xps_pool_t, queue));
        i++;
    }
    if (i > 0) log_warn("find leaked pool: %d", i);
}

XPS_API xps_pool_t *xps_pool_create(size_t size) {
    xps_pool_t *p = xps_memalign(XPS_POOL_ALIGNMENT, size);
    if (p != NULL) {
        p->d.last   = (uint8_t *)p + sizeof(xps_pool_t);
        p->d.end    = (uint8_t *)p + size;
        p->d.next   = NULL;
        p->d.failed = 0;
        size        -= sizeof(xps_pool_t);
        p->max      = xps_min(size, XPS_MAX_ALLOC_FROM_POOL);
        p->current  = p;
        p->large    = NULL;
        p->cleanup  = NULL;
        xps_queue_push_head(&xps_pool_used_queue, &p->queue);
    }
    return p;
}

XPS_API void xps_pool_destroy(xps_pool_t *pool) {
    xps_queue_remove(&pool->queue);
    for (xps_pool_cleanup_t *c = pool->cleanup; c != NULL; c = c->next) {
        if (c->handler) c->handler(c->data);
    }
    for (xps_pool_large_t *l = pool->large; l != NULL; l = l->next) {
        if (l->alloc) xps_free(l->alloc);
    }
    do {
        xps_pool_t *next = pool->d.next;
        xps_free(pool);
        pool = next;
    } while (pool != NULL);
}

XPS_API void xps_reset_pool(xps_pool_t *pool) {
    for (xps_pool_large_t *l = pool->large; l != NULL; l = l->next) {
        if (l->alloc) {
            xps_free(l->alloc);
        }
    }
    for (xps_pool_t *p = pool; p; p = p->d.next) {
        p->d.last   = (uint8_t *) p + sizeof(xps_pool_t);
        p->d.failed = 0;
    }
    pool->current   = pool;
    pool->large     = NULL;
}

XPS_API void *xps_palloc(xps_pool_t *pool, size_t size) {
    if (size <= pool->max) {
        return xps_palloc_small(pool, size);
    }
    return xps_palloc_large(pool, size);
}

XPS_API void *xps_pcalloc(xps_pool_t *pool, size_t size) {
    void *p = xps_palloc(pool, size);
    if (p != NULL) bzero(p, size);
    return p;
}

XPS_API int xps_pfree(xps_pool_t *pool, void *p) {
    for (xps_pool_large_t *l = pool->large; l != NULL; l = l->next) {
        if (p == l->alloc) {
            xps_free(l->alloc);
            l->alloc = NULL;
            return XPS_OK;
        }
    }
    return XPS_DECLINED;
}

XPS_API xps_pool_cleanup_t *xps_pool_cleanup_add(xps_pool_t *p, size_t size) {
    xps_pool_cleanup_t *c = xps_palloc(p, sizeof(xps_pool_cleanup_t));
    if (c != NULL) {
        if (size <= 0) c->data = NULL;
        else {
            c->data = xps_palloc(p, size);
            if (c->data == NULL) return NULL;
        }
        c->handler  = NULL;
        c->next     = p->cleanup;
        p->cleanup  = c;
    }
    return c;
}

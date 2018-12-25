//
//  xps_buffer.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_buffer.h"
#include "xps_core.h"
#include "xps_malloc.h"

typedef struct xps_cache xps_cache_t;

struct xps_cache {
    xps_buffer_t    *freed;
    xps_core_t      *core;
    size_t          alloced;
    size_t          maxn;
};

XPS_API xps_buffer_t *xps_buffer_alloc(xps_cache_t *cache) {
    xps_buffer_t *buffer = NULL;
    if (cache->freed != NULL) {
        buffer = cache->freed;
        cache->freed    = cache->freed->next;
        buffer->next    = NULL;
    } else if (cache->alloced < cache->maxn) {
        cache->alloced++;
        int page = (int)xps_pagesize;
        buffer = xps_palloc(cache->core->pool, page);
        if (buffer != NULL) {
            bzero(buffer, page);
            buffer->maxn = page - sizeof(*buffer);
        }
    }
    return buffer;
}

XPS_API void xps_buffer_free(xps_cache_t *cache, xps_buffer_t *buffer) {
    buffer->next    = cache->freed;
    cache->freed    = buffer;
    buffer->offset  = 0;
    buffer->count   = 0;
}

XPS_INLINE void xps_cache_cleanup(void *ptr) {
    // TODO: free all buffer.
}

XPS_INLINE int xps_cache_load(xps_core_t *core) {
    xps_pool_cleanup_t *c = xps_pool_cleanup_add(core->pool, sizeof(xps_cache_t));
    if (c != NULL) {
        c->handler          = xps_cache_cleanup;
        xps_cache_t *cache  = c->data;
        core->cache         = cache;
        cache->core         = core;
        cache->alloced      = 0;
        cache->maxn         = XPS_MAX_BUFFER_COUNT;
        cache->freed        = NULL;
        return XPS_OK;
    }
    return XPS_ERROR;
}

XPS_MODULE_EVENT_DECL(cache)
    .load = xps_cache_load,
XPS_MODULE_END()

//
//  xps_malloc.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_malloc.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <malloc/malloc.h>
#include "xps_logger.h"

XPS_EXTERN_DEFINE size_t xps_pagesize;
static atomic_size_t xps_used_memory = 0;

XPS_PRIVATE void xps_memory_reset(void) {
    atomic_store_explicit(&xps_used_memory, 0, memory_order_relaxed);
}

XPS_PRIVATE size_t xps_get_used_memory(void) {
    return xps_used_memory;
}

XPS_API void *xps_memalign(size_t alignment, size_t size) {
    // Note: sometime memory maybe overflow.
    if (atomic_load_explicit(&xps_used_memory, memory_order_relaxed) < XPS_MAX_MALLOC_MEMORY) {
        void *p;
        int err = posix_memalign(&p, alignment, size);
        if (err != 0) {
            log_error("memalign %zu %zu failed: %s", alignment, size, strerror(err));
        } else {
            atomic_fetch_add_explicit(&xps_used_memory, malloc_size(p), memory_order_relaxed);
            return p;
        }
    }
    return NULL;
}

XPS_API void *xps_calloc(size_t size) {
    void *p = xps_malloc(size);
    if (p != NULL) bzero(p, size);
    return p;
}

XPS_API void xps_free(void *p) {
    atomic_fetch_sub_explicit(&xps_used_memory, malloc_size(p), memory_order_relaxed);
    free(p);
}

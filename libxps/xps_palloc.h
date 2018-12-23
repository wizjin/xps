//
//  xps_palloc.h
//  XPS
//
//  Created by WizJin on 2018/12/23.
//

#ifndef __XPS_PALLOC_H__
#define __XPS_PALLOC_H__

#include "xps_config.h"
#include "xps_queue.h"

typedef struct xps_pool         xps_pool_t;
typedef struct xps_pool_large   xps_pool_large_t;
typedef struct xps_pool_cleanup xps_pool_cleanup_t;
typedef void (*xps_pool_cleanup_pt)(void *data);

typedef struct {
    uint8_t     *last;
    uint8_t     *end;
    xps_pool_t  *next;
    uint        failed;
} xps_pool_data_t;

struct xps_pool_large {
    xps_pool_large_t    *next;
    void                *alloc;
};

struct xps_pool_cleanup {
    xps_pool_cleanup_pt handler;
    void                *data;
    xps_pool_cleanup_t  *next;
};

struct xps_pool {
    xps_pool_data_t     d;
    size_t              max;
    xps_pool_t          *current;
    xps_pool_large_t    *large;
    xps_pool_cleanup_t  *cleanup;
    xps_queue_t         queue;
};

XPS_PRIVATE void xps_pool_init(void);
XPS_PRIVATE void xps_pool_fini(void);
XPS_API xps_pool_t *xps_pool_create(size_t size);
XPS_API void xps_pool_destroy(xps_pool_t *pool);
XPS_API void xps_pool_reset(xps_pool_t *pool);
XPS_API void *xps_palloc(xps_pool_t *pool, size_t size);
XPS_API void *xps_pcalloc(xps_pool_t *pool, size_t size);
XPS_API int xps_pfree(xps_pool_t *pool, void *p);
XPS_API xps_pool_cleanup_t *xps_pool_cleanup_add(xps_pool_t *p, size_t size);


#endif /* __XPS_PALLOC_H__ */

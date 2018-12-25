//
//  xps_buffer.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_BUFFER_H__
#define __XPS_BUFFER_H__

#include "xps_config.h"

typedef struct xps_cache    xps_cache_t;
typedef struct xps_buffer   xps_buffer_t;

struct xps_buffer {
    xps_buffer_t    *next;
    size_t          offset;
    size_t          count;
    size_t          maxn;
    uint8_t         data[];
};

XPS_API xps_buffer_t *xps_buffer_alloc(xps_cache_t *cache);
XPS_API void xps_buffer_free(xps_cache_t *cache, xps_buffer_t *buffer);


#endif /* __XPS_BUFFER_H__ */

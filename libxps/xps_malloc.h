//
//  xps_malloc.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_MALLOC_H__
#define __XPS_MALLOC_H__

#include "xps_config.h"
#include <strings.h>

XPS_EXTERN size_t xps_pagesize;

XPS_API void xps_memory_reset(void);
XPS_API size_t xps_get_used_memory(void);
XPS_API void *xps_memalign(size_t alignment, size_t size);
XPS_API void *xps_calloc(size_t size);
XPS_API void xps_free(void *p);

#define xps_malloc(_size)   xps_memalign(XPS_MEMORY_ALIGNMENT, (_size))


#endif /* __XPS_MALLOC_H__ */

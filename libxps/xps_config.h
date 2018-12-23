//
//  xps_config.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_CONFIG_H__
#define __XPS_CONFIG_H__

// System Macro
#define XPS_API
#define XPS_PRIVATE
#define XPS_EXTERN                      extern
#define XPS_EXTERN_DEFINE
#define XPS_INLINE                      inline static

// Const Macro
#define INVALID_SOCKET                  -1
#define XPS_OK                          0
#define XPS_ERROR                       -1
#define XPS_DECLINED                    -2

#define XPS_ALIGNMENT                   sizeof(unsigned long)  // must geater 4

#ifndef XPS_MAX_MALLOC_MEMORY
#   define XPS_MAX_MALLOC_MEMORY        (8 * 1024 * 1024)
#endif

#ifndef XPS_DEFAULT_POOL_SIZE
#   define XPS_DEFAULT_POOL_SIZE        (16 * 1024)
#endif

// Misc Macro
#define xps_countof(x)                  (sizeof(x)/sizeof(x[0]))
#define xps_structof(x, type, member)   (type *) ((uint8_t *)(x) - offsetof(type, member))
#define xps_min(a, b)                   ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define xps_max(a, b)                   ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

// Type Define
#include <stdint.h>
#include <stddef.h>


#endif /* __XPS_CONFIG_H__ */

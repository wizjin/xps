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
#define XPS_DONE                        1
#define XPS_OK                          0
#define XPS_ERROR                       -1
#define XPS_DECLINED                    -2

#define XPS_ALIGNMENT                   sizeof(unsigned long)

#ifndef XPS_USED_MEMORY_MAX
#   define XPS_USED_MEMORY_MAX          (8 * 1024 * 1024)
#endif

#ifndef XPS_POOL_DEFAULT_SIZE
#   define XPS_POOL_DEFAULT_SIZE        (16 * 1024)
#endif

#ifndef XPS_MAX_CONNECTION_COUNT
#   define XPS_MAX_CONNECTION_COUNT     256
#endif

#ifndef XPS_MAX_ACCEPT_COUNT
#   define XPS_MAX_ACCEPT_COUNT         4
#endif

#ifndef XPS_MAX_BUFFER_COUNT
#   define XPS_MAX_BUFFER_COUNT         XPS_MAX_CONNECTION_COUNT
#endif

#ifndef XPS_TCP_FASTOPEN
#   define XPS_TCP_FASTOPEN             1
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

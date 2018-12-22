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
#define XPS_EXTERN                  extern
#define XPS_EXTERN_DEFINE
#define XPS_INLINE                  inline static

// Const Macro
#define INVALID_SOCKET              -1
#define XPS_OK                      0
#define XPS_ERROR                   -1

#ifndef XPS_MAX_MALLOC_MEMORY
#   define XPS_MAX_MALLOC_MEMORY    (8 * 1024 * 1024)
#endif

#ifndef XPS_MEMORY_ALIGNMENT
#   define XPS_MEMORY_ALIGNMENT     16  // must geater 8
#endif

// Misc Macro
#define countof(x)                  (sizeof(x)/sizeof(x[0]))


// Type Define
#include <stdint.h>


#endif /* __XPS_CONFIG_H__ */

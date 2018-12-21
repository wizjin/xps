//
//  libxps.h
//  XPS
//
//  Created by WizJin on 2018/12/20.
//  Copyright © 2018 wizjin. All rights reserved.
//

#ifndef __LIBXPS_H__
#define __LIBXPS_H__

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

#ifdef FOUNDATION_EXPORT
#   define LIBXPS_EXPORT    FOUNDATION_EXPORT
#else
#if defined(__cplusplus)
#   define LIBXPS_EXPORT    extern "C"
#else
#   define LIBXPS_EXPORT    extern
#endif
#endif

LIBXPS_EXPORT const char *xps_version(void);


#endif /* __LIBXPS_H__ */

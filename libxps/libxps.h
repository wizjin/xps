//
//  libxps.h
//  XPS
//
//  Created by WizJin on 2018/12/20.
//  Copyright © 2018 wizjin. All rights reserved.
//

#ifndef __LIBXPS_H__
#define __LIBXPS_H__

#if defined(__cplusplus)
#   define LIBXPS_EXPORT   extern "C"
#else
#   define LIBXPS_EXPORT   extern
#endif

LIBXPS_EXPORT const char *xps_version(void);


#endif /* __LIBXPS_H__ */

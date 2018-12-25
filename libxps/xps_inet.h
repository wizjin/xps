//
//  xps_inet.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_INET_H__
#define __XPS_INET_H__

#include "xps_config.h"
#include <netinet/in.h>

XPS_API void xps_tcp_disable_block(int fd);
XPS_API void xps_tcp_fastopen_enable(int fd);
XPS_API int xps_tcp_bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
XPS_API int xps_tcp_connect(int fd, const struct sockaddr *addr, socklen_t addrlen);


#endif /* __XPS_INET_H__ */

//
//  xps_inet.h
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#ifndef __XPS_INET_H__
#define __XPS_INET_H__

#include "xps_config.h"
#include <unistd.h>
#include <netinet/in.h>

XPS_API void xps_tcp_enable_async(int fd);
XPS_API void xps_tcp_enable_fastopen(int fd);
XPS_API int xps_tcp_bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
XPS_API int xps_tcp_connect(int fd, const struct sockaddr *addr, socklen_t addrlen);

#define xps_tcp_listen(fd)  listen((fd), XPS_MAX_ACCEPT_COUNT)
#define xps_tcp_accept      accept
#define xps_tcp_close       close


#endif /* __XPS_INET_H__ */

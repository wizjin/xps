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
#include "xps_string.h"

#define XPS_ADDR_TYPE_IP4                   1
#define XPS_ADDR_TYPE_HOSTNAME              3

typedef struct xps_inet_addr xps_inet_addr_t;

struct xps_inet_addr {
    unsigned    type;
    xps_str_t   *host;
    in_addr_t   addr;
    in_port_t   port;
};

XPS_API void xps_tcp_enable_async(int fd);
XPS_API void xps_tcp_enable_fastopen(int fd);
XPS_API int xps_tcp_bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
XPS_API int xps_tcp_connect(int fd, const struct sockaddr *addr, socklen_t addrlen);

#define xps_tcp_listen(fd)  listen((fd), XPS_MAX_ACCEPT_COUNT)
#define xps_tcp_accept      accept
#define xps_tcp_close       close


#endif /* __XPS_INET_H__ */

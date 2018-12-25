//
//  xps_inet.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_inet.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include "xps_logger.h"

XPS_API void xps_tcp_disable_block(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK);
}

static int xps_tcp_fastopen_is_enable = XPS_TCP_FASTOPEN;

XPS_API void xps_tcp_fastopen_enable(int fd) {
    if (xps_tcp_fastopen_is_enable) {
        int opt = 1;
        if (setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &opt, sizeof(opt)) != 0) {
            if (errno == EPROTONOSUPPORT || errno == ENOPROTOOPT) {
                log_warn("tcp fast open is not supported on this platform");
                xps_tcp_fastopen_is_enable = 0;
            } else {
                log_error("tcp fast open enable failed: %s", strerror(errno));
            }
        }
    }
}

XPS_API int xps_tcp_bind(int fd, const struct sockaddr *addr, socklen_t addrlen) {
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
        log_warn("enable socket reuseaddr failed: %s", strerror(errno));
    }
    if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on)) != 0){
        log_warn("enable socket no sigpipe failed: %s", strerror(errno));
    }
    return bind(fd, (struct sockaddr *)addr, addrlen);
}

XPS_API int xps_tcp_connect(int fd, const struct sockaddr *addr, socklen_t addrlen) {
    int timeout = 5; // connect timeout
    setsockopt(fd, IPPROTO_TCP, TCP_CONNECTIONTIMEOUT, &timeout, sizeof(timeout));
#if HMS_TCP_FASTOPEN > 0
    sa_endpoints_t endpoints;
    endpoints.sae_srcif = 0;
    endpoints.sae_srcaddr = NULL;
    endpoints.sae_srcaddrlen = 0;
    endpoints.sae_dstaddr = (struct sockaddr *)addr;
    endpoints.sae_dstaddrlen = addrlen;
    return connectx(fd, &endpoints, SAE_ASSOCID_ANY, CONNECT_RESUME_ON_READ_WRITE|CONNECT_DATA_IDEMPOTENT, NULL, 0, NULL, NULL);
#else
    return connect(fd, addr, addrlen);
#endif
}

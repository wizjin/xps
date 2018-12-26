//
//  xps_output_bypass.c
//  XPS
//
//  Created by WizJin on 2018/12/25.
//

#include "xps_output.h"
#include "xps_core.h"
#include "xps_connection.h"

XPS_API xps_connection_t *xps_output_bypass_alloc(xps_core_t *core, xps_inet_addr_t *addr) {
    //if (addr->type != XPS_ADDR_TYPE_IP4) addr->addr = xps_dns_lookup_address(core, addr->hostname);
    if (addr->addr != INADDR_NONE) {
        struct sockaddr_in srv_addr;
        bzero(&srv_addr, sizeof(srv_addr));
        srv_addr.sin_len            = sizeof(srv_addr);
        srv_addr.sin_family         = AF_INET;
        srv_addr.sin_port           = addr->port;
        srv_addr.sin_addr.s_addr    = addr->addr;
        log_debug("ip address: %s", inet_ntoa(srv_addr.sin_addr));
        int fd = socket(srv_addr.sin_family, SOCK_STREAM, 0);
        if (fd != INVALID_SOCKET) {
            xps_connection_t *c = xps_connection_create(core->inet, fd);
            if (c != NULL) {
                int ret =xps_tcp_connect((int)c->ident, (struct sockaddr *)&srv_addr, srv_addr.sin_len);
                if (ret != 0 && errno != EINPROGRESS) {
                    log_error("connect failed: %s", strerror(errno));
                } else {
                    //c->reader = xps_connection_transport_reader;
                    //c->writer = xps_output_bypass_writer;
                    return c;
                }
                xps_connection_close(c);
                return NULL;
            }
            close(fd);
        }
    }
    return NULL;
}

XPS_MODULE_OUTPUT_DECL(bypass)
    .alloc  = xps_output_bypass_alloc,
XPS_MODULE_END()

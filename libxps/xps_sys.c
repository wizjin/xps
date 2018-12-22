//
//  xps_sys.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_sys.h"
#include <unistd.h>
#include <signal.h>
#include "xps_logger.h"
#include "xps_malloc.h"

static int xps_sys_init_count = 0;

XPS_API int xps_sys_init(void) {
    if (xps_sys_init_count++ <= 0) {
        // init sign.
        struct sigaction sa;
        bzero(&sa, sizeof(sa));
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGPIPE, &sa, NULL) != 0) {
            log_warn("sigaction SIGPIPE failed.");
        }
        // init malloc system.
        xps_pagesize = getpagesize();
        xps_memory_reset();
    }
    return 0;
}

XPS_API void xps_sys_fini(void) {
    if (--xps_sys_init_count <= 0) {
        size_t used = xps_get_used_memory();
        if (used > 0) {
            log_warn("find leaked memory: %zu", used);
        }
    }
}

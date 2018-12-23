//
//  xps_sys.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_sys.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "xps_logger.h"
#include "xps_malloc.h"
#include "xps_palloc.h"

XPS_EXTERN_DEFINE FILE* xps_logger_file = NULL;
static unsigned xps_sys_init_count = 0;

XPS_PRIVATE int xps_sys_init(void) {
    if (xps_sys_init_count++ <= 0) {
        // init logger
        if (xps_logger_file == NULL) {
            xps_logger_file = stderr;
        }
        // init sign.
        struct sigaction sa;
        bzero(&sa, sizeof(sa));
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGPIPE, &sa, NULL) != 0) {
            log_warn("sigaction SIGPIPE failed.");
        }
        // init random seed.
        srand((unsigned)time(NULL));
        // init malloc system.
        xps_pagesize = getpagesize();
        xps_memory_reset();
        xps_pool_init();
    }
    return XPS_OK;
}

XPS_PRIVATE void xps_sys_fini(void) {
    if (--xps_sys_init_count <= 0) {
        xps_pool_fini();
        size_t used = xps_get_used_memory();
        if (used > 0) {
            log_warn("find leaked memory: %zu", used);
        }
    }
}

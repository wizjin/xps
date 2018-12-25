//
//  main.c
//  XPS Demo
//
//  Created by WizJin on 2018/12/22.
//

#include <stdio.h>
#include <unistd.h>
#include "libxps.h"
#include "xps_logger.h"
#include "xps_core.h"
#include "xps_input.h"

int main(int argc, const char * argv[]) {
    printf("start xps version: %s\n", xps_version());
    xps_core_t *core = xps_core_create();
    if (core != NULL) {
        if (xps_input_module_active(core, "socks", "9002") != XPS_OK) {
            log_error("active socks failed.");
        } else if (xps_core_start(core) == XPS_OK) {
            log_info("press any key to exit.");
            getc(stdin);
            xps_core_stop(core);
        }
        xps_core_destory(core);
    }
    return 0;
}

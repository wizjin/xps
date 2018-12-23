//
//  main.c
//  XPS Demo
//
//  Created by WizJin on 2018/12/22.
//

#include <stdio.h>
#include "libxps.h"
#include "xps_core.h"

int main(int argc, const char * argv[]) {
    printf("start xps version: %s\n", xps_version());
    xps_core_t *core = xps_core_create();
    if (core != NULL) {
        xps_core_destory(core);
    }
    return 0;
}

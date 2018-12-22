//
//  xps_string.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_string.h"

XPS_API int xps_atoi(const char *line, size_t n) {
    int value;
    if (n > 0) {
        for (value = 0; n--; line++) {
            if (*line >= '0' && *line <= '9') {
                value = value * 10 + (*line - '0');
                continue;
            }
            return XPS_ERROR;
        }
        return value;
    }
    return XPS_ERROR;
}

XPS_API const char *xps_strlchr(const char *p, const char *last, char c) {
    while (p < last) {
        if (*p == c) {
            return p;
        }
        p++;
    }
    return NULL;
}

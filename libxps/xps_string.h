//
//  xps_string.h
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#ifndef __XPS_STRING_H__
#define __XPS_STRING_H__

#include "xps_config.h"

typedef struct {
    size_t  len;
    char    *data;
} xps_str_t;

#define xps_string(str)     { sizeof(str) - 1, (char *)str }
#define xps_string_null     { 0, NULL }

XPS_API int xps_atoi(const char *line, size_t n);

XPS_INLINE const char *xps_strlchr(const char *p, const char *last, char c) {
    while (p < last) {
        if (*p == c) {
            return p;
        }
        p++;
    }
    return NULL;
}


#endif /* __XPS_STRING_H__ */

//
//  xps_input.h
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#ifndef __XPS_INPUT_H__
#define __XPS_INPUT_H__

#include "xps_config.h"
#include "xps_chain.h"
#include "xps_module.h"

typedef struct xps_input        xps_input_t;
typedef struct xps_input_module xps_input_module_t;
typedef xps_input_t *(*xps_input_alloc_pt)(xps_core_t *core, xps_input_module_t *input, const char *option);

struct xps_input {
    xps_chain_node_t    link;
    int (*open)(xps_input_t *input, xps_core_t *core);
    void (*close)(xps_input_t *input);
    unsigned            closed:1;
};

#define XPS_MODULE_INPUT    \
    XPS_MODULE_COMMON       \
    xps_input_alloc_pt  alloc;

struct xps_input_module {
    XPS_MODULE_INPUT
};

#define XPS_MODULE_INPUT_DECL(_name)    XPS_MODULE_DECL(_name, xps_input_module_t, XPS_MODULE_TYPE_INPUT)

XPS_API int xps_input_module_active(xps_core_t *core, const char *name, const char *option);
XPS_API int xps_input_modules_open(xps_core_t *core);
XPS_API void xps_input_modules_close(xps_core_t *core);


#endif /* __XPS_INPUT_H__ */

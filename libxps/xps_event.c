//
//  xps_event.c
//  XPS
//
//  Created by WizJin on 2018/12/24.
//

#include "xps_event.h"

XPS_API void xps_event_manager_sign(xps_event_manager_t *manager, int sign) {
    if (manager != NULL && manager->sign != NULL) {
        manager->sign(manager, sign);
    }
}

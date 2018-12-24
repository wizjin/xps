//
//  xps_queue.c
//  XPS
//
//  Created by WizJin on 2018/12/22.
//

#include "xps_queue.h"

XPS_API void xps_queue_sort(xps_queue_t *queue, int (*cmp)(const xps_queue_t *, const xps_queue_t *)) {
    xps_queue_t *q = xps_queue_head(queue);
    if (q != xps_queue_tail(queue)) {
        xps_queue_t *prev, *next;
        for (q = xps_queue_next(q); q != xps_queue_sentinel(queue); q = next) {
            
            prev = xps_queue_prev(q);
            next = xps_queue_next(q);
            
            xps_queue_remove(q);
            
            do {
                if (cmp(prev, q) <= 0) break;
                prev = xps_queue_prev(prev);
                
            } while (prev != xps_queue_sentinel(queue));
            
            xps_queue_insert_after(prev, q);
        }
    }
}

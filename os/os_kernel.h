#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdint.h>
#include <stdbool.h>

void os_kernel_launch(uint32_t quanta);

void os_kernel_init(void);

bool os_kernel_add_thread(void(*task));
                              
//void os_thread_yield(void);
                           
extern void os_scheduler_launch(void);                         

#endif //_OS_KERNEL_H

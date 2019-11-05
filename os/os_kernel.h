#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct tcb 
{
    int32_t*    stack_pt;
    struct tcb* next_pt;
} tcb_t;


void os_kernel_launch(uint32_t quanta);

void os_kernel_init(void);

bool os_kernel_add_threads(void(*task0) (void),
                           void(*tas1) (void),
                           void(*task2) (void));
                              
//void os_thread_yield(void);
                           
extern void os_scheduler_launch(void);                         

#endif //_OS_KERNEL_H

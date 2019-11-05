#include "os_kernel.h"
#include "internals/internals.h"
#include "shared/device.h"  // Device header
#include "core_cm3.h"       // Cortex m3 core header
#include <stddef.h>


#define MAX_NUM_OF_THREADS          (64)
#define STACK_SIZE                  (18)
#define NUM_OF_REGISTERS_TO_STORE   (16)

#define STACK_POINTER_INDEX         ((STACK_SIZE) - (NUM_OF_REGISTERS_TO_STORE))
#define XPSR_INDEX                  (NUM_OF_REGISTERS_TO_STORE - 1) //xPSR
#define PC_INDEX                    (NUM_OF_REGISTERS_TO_STORE - 2) //PC

typedef struct tcb 
{
    int32_t*    stack_pt;
    struct tcb* next_pt;
} tcb_t;

int32_t TCB_STACK[MAX_NUM_OF_THREADS][STACK_SIZE];

tcb_t tcbs[MAX_NUM_OF_THREADS];

int8_t last_thread_id = -1;
tcb_t* current_pt;

static void os_kernel_stack_init(int thread_id)
{
    tcbs[thread_id].stack_pt = &TCB_STACK[thread_id][STACK_POINTER_INDEX];
    tcbs[thread_id].stack_pt[XPSR_INDEX] = XPSR_RESET_VALUE;
}


bool os_kernel_add_thread(void(*task))
{
    disable_irq();
    
    tcb_t* new_thread = &tcbs[last_thread_id + 1];
    if(last_thread_id >= 0)
    {
        tcbs[last_thread_id].next_pt = new_thread;
    }
    new_thread->next_pt = &tcbs[0];
    
    os_kernel_stack_init(last_thread_id + 1);
    new_thread->stack_pt[PC_INDEX] = (int32_t)task;
    ++last_thread_id;
        
    if(current_pt == NULL)
    {
        current_pt = new_thread;
    }
    
    enable_irq();
    return true;
}

void os_kernel_init()
{
    //By disabling and enabling of the interruptions we can provide something like "atomic operations" 
    disable_irq();
    init_internals();
    enable_irq();
}

void os_kernel_launch(uint32_t quanta)
{
    disable_scheduler_irq();
    reset_scheduler_counter();
    set_scheduler_quanta(quanta);
    enable_scheduler_irq();
    os_scheduler_launch();
}

void os_scheduler_swap()
{
    current_pt = current_pt->next_pt;
}

/* //FIXME: Fix error while scheduling
inline void os_thread_yield() 
{
    request_scheduler_irq();
}
*/


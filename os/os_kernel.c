#include "os_kernel.h"
#include "internals/internals.h"
#include "shared/device.h"  // Device header
#include "core_cm3.h"       // Coretex m3 core header

#define NUM_OF_THREADS              (3)
#define STACK_SIZE                  (18)
#define NUM_OF_REGISTERS_TO_STORE   (16)

#define STACK_POINTER_INDEX         ((STACK_SIZE) - (NUM_OF_REGISTERS_TO_STORE))
#define XPSR_INDEX                  (NUM_OF_REGISTERS_TO_STORE - 1) //xPSR
#define PC_INDEX                    (NUM_OF_REGISTERS_TO_STORE - 2) //PC


int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

tcb_t tcbs[NUM_OF_THREADS];
tcb_t *current_pt;

static void os_kernel_stack_init(int thread_id)
{
    tcbs[thread_id].stack_pt = &TCB_STACK[thread_id][STACK_POINTER_INDEX];
    tcbs[thread_id].stack_pt[XPSR_INDEX] = XPSR_RESET_VALUE;
}

bool os_kernel_add_threads(void(*task0) (void), void(*task1) (void), void(*task2) (void))
{
    //By disabling and enabling of the interruptions we can provide something like "atomic operations" 
    disable_irq(); 
    
    tcbs[0].next_pt = &tcbs[1];
    tcbs[1].next_pt = &tcbs[2];
    tcbs[2].next_pt = &tcbs[0];
    
    os_kernel_stack_init(0);
    tcbs[0].stack_pt[PC_INDEX] = (int32_t)task0; //PC
    os_kernel_stack_init(1);
    tcbs[1].stack_pt[PC_INDEX] = (int32_t)task1; //PC
    os_kernel_stack_init(2);
    tcbs[2].stack_pt[PC_INDEX] = (int32_t)task2; //PC

    current_pt = &tcbs[0];
    
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

void os_thread_yield() 
{
    request_scheduler_irq();
}


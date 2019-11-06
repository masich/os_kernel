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

/*
Thread states:

Start -> Ready <=> Running ->Terminated
           \         /
             Waiting 
*/

typedef enum 
{
    START = 0,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} state_t;


typedef struct tcb 
{
    int32_t*    stack_pt;
    int16_t     id;
    state_t     state;
    struct tcb* next_pt;
} tcb_t;


static int32_t TCB_STACK[MAX_NUM_OF_THREADS][STACK_SIZE];
static tcb_t tcbs[MAX_NUM_OF_THREADS];
static int8_t last_thread_id = -1;
static uint8_t threads_num = 0;

tcb_t* current_pt;


static void os_kernel_stack_init(tcb_t* thread, int32_t pc_address)
{
    thread->stack_pt = &TCB_STACK[thread->id][STACK_POINTER_INDEX];
    thread->stack_pt[XPSR_INDEX] = XPSR_RESET_VALUE;
    thread->stack_pt[PC_INDEX] = pc_address;
}


bool os_kernel_add_thread(void(*task))
{
    disable_irq();
    
    uint8_t new_thread_id = last_thread_id + 1;
    tcb_t* new_thread = &tcbs[new_thread_id];
    new_thread->id = new_thread_id;
    new_thread->state = START;
    new_thread->next_pt = &tcbs[0];
    
    if(last_thread_id >= 0)
    {
        tcbs[last_thread_id].next_pt = new_thread;
    }
    
    os_kernel_stack_init(new_thread, (int32_t)task);
    last_thread_id = new_thread_id;
    threads_num = new_thread_id + 1;
    
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

// TODO: Do this staff in the os_scheduler_launch()
static void os_kernel_prepare_threads()
{
    for(int8_t thread_index = threads_num - 1; 0 <= thread_index; --thread_index)
    {
        if (tcbs[thread_index].state == START)
        {
            tcbs[thread_index].state = READY; 
            current_pt = &tcbs[thread_index];
        }
    }
    current_pt->state = RUNNING;
}

void os_kernel_launch(uint32_t quanta)
{
    disable_scheduler_irq();
    reset_scheduler_counter();
    set_scheduler_quanta(quanta);
    os_kernel_prepare_threads();        
    enable_scheduler_irq();
    os_scheduler_launch();
}

void os_scheduler_swap()
{
    if (current_pt->state == RUNNING)
    {
        current_pt->state = READY;
    }

    uint8_t temp_threads_num = threads_num;
    do {
        current_pt = current_pt->next_pt;
        if (current_pt->state == READY)
        {
            current_pt->state = RUNNING;
            return;
        }
    } while(--temp_threads_num);
    
    // TODO: Instead of NULL it is suitable to use something like IDLE threads
    current_pt = NULL;
}

/* //FIXME: Fix error while scheduling
inline void os_thread_yield() 
{
    request_scheduler_irq();
}
*/


#include "os/os_kernel.h"
#include <stdbool.h>

#define QUANTA (10)


uint32_t count0, count1, count2;

void task0()
{
    while(true)
    {
        count0++;
    }
}

void task1() 
{
	while(true)
    {
        count1++;
        os_thread_yield();
    }
}
void task2() 
{
    while(true)
    {
        count2++;
        os_thread_yield();
    }
}


int main()
{
    os_kernel_init();
    os_kernel_add_threads(&task0, &task1, &task2);
    os_kernel_launch(QUANTA);
}

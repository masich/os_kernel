#include "os/os_kernel.h"
#include "os/internals/bsp.h"

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
    }
}
void task2() 
{
    while(true)
    {
        count2++;
    }
}

int main()
{
    os_kernel_init();
    os_kernel_add_thread(&task0);
    os_kernel_add_thread(&task1);
    os_kernel_add_thread(&task2);
    os_kernel_launch(QUANTA);
}

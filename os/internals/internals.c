#include "internals.h"
#include "../shared/device.h" // Device header
#include "core_cm3.h"         // Cortex m3 core header

#define BUS_FREQ                    (16000000)       								//16 MHz

#define SYS_PRI_N3                  (*(volatile uint32_t *)(0xE000ED20)) //System Handler Priority Registers, SysTick priority register address (Cortex M3)
#define RESET_SYSTICK_PRIORITY      (0x00FFFFF)
#define ICSR                        (*(volatile uint32_t *)(0xE000ED04)) //Interrupt control and state register (same for all Cortex M)

static uint32_t MILLIS_PRESCALER;

void init_internals()
{
    MILLIS_PRESCALER = BUS_FREQ / 1000;
}

void enable_irq()
{
    __enable_irq();
}

void disable_irq()
{
    __disable_irq();
}

void request_scheduler_irq()
{
    ICSR |= 1 << 26; //The SysTick interrupt is requested by writing 1 to bit[26] of the ICSR
}

void disable_scheduler_irq()
{
    SysTick->CTRL = 0; //Disable SysTick interruptions
}

void reset_scheduler_counter()
{
    SysTick->VAL = 0;  //Reset a SysTick value 
}

void set_scheduler_quanta(uint32_t quanta)
{
    //Count down from quanta (SysTick will be called every (quanta * MILLIS_PRESCALER) tick
    SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1; //And -1 because all counts begins from 0
}

void enable_scheduler_irq()
{
    //Enable SysTick, use processor clock, and counting down to zero pends the SysTick handler (See SysTick control and status register)
    SYS_PRI_N3 = (SYS_PRI_N3 & RESET_SYSTICK_PRIORITY) | 0xE0000000; //Set the SysTick interruption priority 
    SysTick->CTRL = 0x00000007;   
}

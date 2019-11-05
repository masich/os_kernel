#include "../shared/device.h"
#include "bsp.h"
#include <stdbool.h>

#define LED_HIGH (1U << 13)
#define LED_BIT (1U << 21)
#define GPIOC_CLOCK (1U << 4)


static bool is_gpio_initialized = false;

void led_on(void) 
{
    if(!is_gpio_initialized) 
    {
        gpio_init();
    }
	GPIOC->ODR &= ~LED_HIGH;
}

void led_off(void)
{
    if(!is_gpio_initialized)
    {
        gpio_init();
    }
	GPIOC->ODR |= LED_HIGH;
}

void toggle_led(void)
{
    
}

void gpio_init(void)
{
	RCC->APB2ENR |= GPIOC_CLOCK; 				//Set up the GPIOC clock on the APB2 bus (APB2 enable register)
	GPIOC->CRH |= LED_BIT;						//Set the PIN's mode (low speed 2MHz, GP output)
	led_off(); 									//Only because my led is blinking by default
    is_gpio_initialized = true;
}

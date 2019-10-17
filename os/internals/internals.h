#ifndef _INTERNALS_H
#define _INTERNALS_H

#include <stdint.h>

#define XPSR_RESET_VALUE    (0x01000000)    //bit[24] is 1 because of Thumb mode


void init_internals(void);

void disable_irq(void);

void enable_irq(void);

void request_scheduler_irq(void);

void disable_scheduler_irq(void);

void reset_scheduler_counter(void);

void set_scheduler_quanta(uint32_t quanta);

void enable_scheduler_irq(void);

#endif //_INTENALS_H

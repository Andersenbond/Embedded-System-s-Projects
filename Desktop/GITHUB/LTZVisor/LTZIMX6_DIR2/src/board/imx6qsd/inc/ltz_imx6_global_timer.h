#ifndef __IMX6_GLOBAL_TIMER_H__
#define __IMX6_GLOBAL_TIMER_H__
#include "types.h"
// Base address for the ARM GLOBAL TIMER.
#define REGS_ARM_GLOBAL_TIMER_BASE 			(0x00a00000)
// Global Timer Control Register (RW)
#define HW_ARM_GLOBAL_TIMER_CONTROL_ADDR	(REGS_ARM_GLOBAL_TIMER_BASE + 0x208)
//Global Timer Counter Register 1
#define HW_ARM_GLOBAL_TIMER_COUNTER1_ADDR   (REGS_ARM_GLOBAL_TIMER_BASE + 0x200 + 1*0x4)
//Global Timer Counter Register 2
#define HW_ARM_GLOBAL_TIMER_COUNTER2_ADDR   (REGS_ARM_GLOBAL_TIMER_BASE + 0x200 + 2*0x4)

#define HW_ARM_GLOBAL_TIMER_CONTROL   		  (*(volatile uint32_t*) HW_ARM_GLOBAL_TIMER_CONTROL_ADDR )
#define HW_ARMGLOBALTIMER_COUNTER1        	(*(volatile uint32_t*) HW_ARM_GLOBAL_TIMER_COUNTER1_ADDR)
#define HW_ARMGLOBALTIMER_COUNTER2        	(*(volatile uint32_t*) HW_ARM_GLOBAL_TIMER_COUNTER2_ADDR)
//Value to divide the clock in order to get a frequency of 1MHz
#define MHZ_DEVIDER  						(1000000)

void ltz_system_time_init(void);
void ltz_init_global_timer(void);
uint64_t ltz_get_microseconds(void);

#endif

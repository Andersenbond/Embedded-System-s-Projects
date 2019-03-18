#ifndef __IMX6_EPIT_H__
#define __IMX6_EPIT_H__
#include "types.h"
// Base address for EPIT instance number 1.
#define REGS_EPIT1_BASE 		(0x020d0000)
// Base address for EPIT inEPIT Status register instance number 2.
#define REGS_EPIT2_BASE 	(0x020d4000)
//EPIT Control register (RW)

#define REGS_EPIT_BASE(x) ((x) == 1 ? REGS_EPIT1_BASE : (x) == 2 ? REGS_EPIT2_BASE : 0x00d00000)
#define HW_EPIT_CR_ADDR(x)     (REGS_EPIT_BASE(x) + 0x0)
//EPIT Control register (RW)
#define HW_EPIT_SR_ADDR(x)     (REGS_EPIT_BASE(x) + 0x4)
//EPIT Load register (RW)
#define HW_EPIT_LR_ADDR(x)     (REGS_EPIT_BASE(x) + 0x8)
//Pointer to the registers
#define HW_EPIT_CR(x)          (*(volatile uint32_t *) HW_EPIT_CR_ADDR(x))
#define HW_EPIT_LR(x)          (*(volatile uint32_t *) HW_EPIT_LR_ADDR(x))
#define HW_EPIT_SR(x)          (*(volatile uint32_t *) HW_EPIT_SR_ADDR(x))



void ltz_epit_enable(int);
void ltz_epit_disable(int);
void ltz_epit_reload(int, uint32_t reload_value);
void ltz_epit_init(int, uint32_t load_value);
void ltz_epit_delay_us( int, uint32_t micro_seconds);
uint32_t ltz_epit_get_compare_event(int instance);
#endif

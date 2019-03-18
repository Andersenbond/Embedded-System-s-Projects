#ifndef __IMX6_CCM_H__
#define __IMX6_CCM_H__
#include <types.h>
#define REGS_CCM_BASE 			(0x020c4000) // Base address for CCM Control Register

//CCM Serial Clock Divider Register 1
#define HW_CCM_CSCDR1_ADDR      (REGS_CCM_BASE + 0x24)
//CCM Clock Gating Register 1
#define HW_CCM_CCGR1_ADDR      	(REGS_CCM_BASE + 0x6c)
//CCM Clock Gating Register 5
#define HW_CCM_CCGR5_ADDR       (REGS_CCM_BASE + 0x7C)
//CCM Bus Clock Multiplexer Register (RW)
#define HW_CCM_CBCMR_ADDR       (REGS_CCM_BASE + 0x18)
//CCM Bus Clock Divider Register (RW)
#define HW_CCM_CBCDR_ADDR       (REGS_CCM_BASE + 0x14)

extern const uint32_t PLL1_CLOCK;
extern const uint32_t PLL2_CLOCK[];
extern const uint32_t PLL3_CLOCK[];
extern const uint32_t PLL4_CLOCK;
extern const uint32_t PLL5_CLOCK;

#define HW_CCM_CSCDR1           (*(volatile uint32_t*)HW_CCM_CSCDR1_ADDR)
#define HW_CCM_CCGR1            (*(volatile uint32_t*) HW_CCM_CCGR1_ADDR)
#define HW_CCM_CCGR5            (*(volatile uint32_t*) HW_CCM_CCGR5_ADDR)
#define HW_CCM_CBCMR            (*(volatile uint32_t*) HW_CCM_CBCMR_ADDR)
#define HW_CCM_CBCDR            (*(volatile uint32_t*) HW_CCM_CBCDR_ADDR)

#endif

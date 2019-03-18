#ifndef __IMX6_AIPSTZ_H__
#define __IMX6_AIPSTZ_H__
#include "types.h"

#define REGS_AIPSTZ1_BASE (0x02000000) //!< Base address for AIPSTZ instance number 1.
#define REGS_AIPSTZ2_BASE (0x02100000) //!< Base address for AIPSTZ instance number 2.
#define REGS_AIPSTZ_BASE(x) ( (x) == 1 ? REGS_AIPSTZ1_BASE : (x) == 2 ? REGS_AIPSTZ2_BASE : 0x00d00000)


#define HW_AIPSTZ_MPR_ADDR(x)        (REGS_AIPSTZ_BASE(x) + 0x00)
#define HW_AIPSTZ_OPACR0_ADDR(x)     (REGS_AIPSTZ_BASE(x) + 0x40)
#define HW_AIPSTZ_OPACR1_ADDR(x)     (REGS_AIPSTZ_BASE(x) + 0x44)
#define HW_AIPSTZ_OPACR2_ADDR(x)     (REGS_AIPSTZ_BASE(x) + 0x48)
#define HW_AIPSTZ_OPACR3_ADDR(x)     (REGS_AIPSTZ_BASE(x) + 0x4c)
#define HW_AIPSTZ_OPACR4_ADDR(x)     (REGS_AIPSTZ_BASE(x) + 0x50)


#define HW_AIPSTZ_MPR(x)             (*(volatile uint32_t *) HW_AIPSTZ_MPR_ADDR(x))
#define HW_AIPSTZ_OPACR0(x)          (*(volatile uint32_t *) HW_AIPSTZ_OPACR0_ADDR(x))
#define HW_AIPSTZ_OPACR1(x)          (*(volatile uint32_t *) HW_AIPSTZ_OPACR1_ADDR(x))
#define HW_AIPSTZ_OPACR2(x)          (*(volatile uint32_t *) HW_AIPSTZ_OPACR2_ADDR(x))
#define HW_AIPSTZ_OPACR3(x)          (*(volatile uint32_t *) HW_AIPSTZ_OPACR3_ADDR(x))
#define HW_AIPSTZ_OPACR4(x)          (*(volatile uint32_t *) HW_AIPSTZ_OPACR4_ADDR(x))


void init_aipstz(void);

#endif

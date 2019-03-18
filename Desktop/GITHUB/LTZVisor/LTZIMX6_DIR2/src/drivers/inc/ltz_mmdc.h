
#ifndef _LTZVIROR_MMDC_H_
#define _LTZVIROR_MMDC_H_
#include <types.h>
#include <printk.h>

#define REGS_MMDC1_BASE (0x021b0000) //!< Base address for MMDC instance number 1.
#define REGS_MMDC2_BASE (0x021b4000) //!< Base address for MMDC instance number 2.

#define REGS_MMDC_BASE(x) ( (x) == 1 ? REGS_MMDC1_BASE : (x) == 2 ? REGS_MMDC2_BASE : 0x00d00000)


#define HW_MMDC_MDCTL_ADDR(x)       (REGS_MMDC_BASE(x) + 0x0)
#define HW_MMDC_MDMISC_ADDR(x)      (REGS_MMDC_BASE(x) + 0x18)
#define HW_MMDC_MDASP_ADDR(x)       (REGS_MMDC_BASE(x) + 0x40)

#define HW_MMDC_MDCTL(x)            (*(volatile uint32_t *) HW_MMDC_MDCTL_ADDR(x))
#define HW_MMDC_MDMISC(x)           (*(volatile uint32_t *) HW_MMDC_MDMISC_ADDR(x))
#define HW_MMDC_MDASP(x)            (*(volatile uint32_t *) HW_MMDC_MDASP_ADDR(x))

#endif

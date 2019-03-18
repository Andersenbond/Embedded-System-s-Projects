#ifndef _TZASC_CONFIG_H_
#define _TZASC_CONFIG_H_
#include "types.h"

#define REGS_CCM_BASE            (0x020c4000) //!< Base address for CCM.
#define HW_CCM_CCGR2_ADDR        (REGS_CCM_BASE + 0x70)
#define HW_CCM_CCGR2             (*(volatile uint32_t *) HW_CCM_CCGR2_ADDR)



#define TZASC_CLOCK_EN           (0x03C00000)

#define REGS_IOMUXC_BASE         (0x020e0000) //!< Base address for IOMUXC.
#define HW_IOMUXC_GPR3_ADDR      (REGS_IOMUXC_BASE + 0x0C)
#define HW_IOMUXC_GPR9_ADDR      (REGS_IOMUXC_BASE + 0x24)

#define HW_IOMUXC_GPR3           (*(volatile uint32_t *) HW_IOMUXC_GPR3_ADDR)
#define HW_IOMUXC_GPR9           (*(volatile uint32_t *) HW_IOMUXC_GPR9_ADDR)



#define AIPS_TZ2_BASE_ADDR	  	0x02100000
#define AIPS2_OFF_BASE_ADDR     (0x02100000 + 0x80000)
#define IP2APB_TZASC1_BASE_ADDR (AIPS2_OFF_BASE_ADDR+0x50000)
#define IP2APB_TZASC2_BASE_ADDR (AIPS2_OFF_BASE_ADDR+0x54000)

#define HW_TZASC_BASE_ADDR(x) ( (x) == 1 ? IP2APB_TZASC1_BASE_ADDR : (x) == 2 ? IP2APB_TZASC2_BASE_ADDR : 0x00d00000)


#define HW_TZASC_CTRL_ADDR(x)             (HW_TZASC_BASE_ADDR(x) + 0x00)
#define HW_TZASC_INT_STATUS_ADDR(x)       (HW_TZASC_BASE_ADDR(x) + 0x10)
#define HW_TZASC_INT_CLEAR_ADDR(x)        (HW_TZASC_BASE_ADDR(x) + 0x14)
#define HW_TZASC_FAIL_HIGH_ADDR(x)        (HW_TZASC_BASE_ADDR(x) + 0x24)
#define HW_TZASC_FAIL_LOW_ADDR(x)         (HW_TZASC_BASE_ADDR(x) + 0x20)
#define HW_TZASC_FAIL_ID_ADDR(x)          (HW_TZASC_BASE_ADDR(x) + 0x2C)
#define HW_TZASC_FAIL_CONTR_ADDR(x)       (HW_TZASC_BASE_ADDR(x) + 0x28)
#define HW_TZASC_SPEC_CONTR_ADDR(x)       (HW_TZASC_BASE_ADDR(x) + 0x30)

#define HW_TZASC_REG_SET_LOW0_ADDR(x)    (HW_TZASC_BASE_ADDR(x) + 0x100)
#define HW_TZASC_REG_SET_LOW1_ADDR(x)    (HW_TZASC_BASE_ADDR(x) + 0x110)
#define HW_TZASC_REG_SET_LOW2_ADDR(x)    (HW_TZASC_BASE_ADDR(x) + 0x120)
#define HW_TZASC_REG_SET_LOW3_ADDR(x)    (HW_TZASC_BASE_ADDR(x) + 0x130)
#define HW_TZASC_REG_SET_LOW4_ADDR(x)    (HW_TZASC_BASE_ADDR(x) + 0x140)


#define HW_TZASC_REG_SET_HIGH0_ADDR(x)   (HW_TZASC_BASE_ADDR(x) + 0x104)
#define HW_TZASC_REG_SET_HIGH1_ADDR(x)   (HW_TZASC_BASE_ADDR(x) + 0x114)
#define HW_TZASC_REG_SET_HIGH2_ADDR(x)   (HW_TZASC_BASE_ADDR(x) + 0x124)
#define HW_TZASC_REG_SET_HIGH3_ADDR(x)   (HW_TZASC_BASE_ADDR(x) + 0x134)
#define HW_TZASC_REG_SET_HIGH4_ADDR(x)   (HW_TZASC_BASE_ADDR(x) + 0x144)


#define HW_TZASC_REG0_ATTRIB_ADDR(x)     (HW_TZASC_BASE_ADDR(x) + 0x108)
#define HW_TZASC_REG1_ATTRIB_ADDR(x)     (HW_TZASC_BASE_ADDR(x) + 0x118)


#define HW_TZASC_CTRL(x)             (*(volatile uint32_t *) HW_TZASC_CTRL_ADDR(x))
#define HW_TZASC_INT_STATUS(x)       (*(volatile uint32_t *) HW_TZASC_INT_STATUS_ADDR(x))
#define HW_TZASC_INT_CLEAR(x)        (*(volatile uint32_t *) HW_TZASC_INT_CLEAR_ADDR(x))
#define HW_TZASC_FAIL_HIGH(x)        (*(volatile uint32_t *) HW_TZASC_FAIL_HIGH_ADDR(x))
#define HW_TZASC_FAIL_LOW(x)         (*(volatile uint32_t *) HW_TZASC_FAIL_LOW_ADDR(x))
#define HW_TZASC_FAIL_ID(x)          (*(volatile uint32_t *) HW_TZASC_FAIL_ID_ADDR(x))
#define HW_TZASC_FAIL_CONTR(x)       (*(volatile uint32_t *) HW_TZASC_FAIL_CONTR_ADDR(x))
#define HW_TZASC_SPEC_CONTR(x)       (*(volatile uint32_t *) HW_TZASC_SPEC_CONTR_ADDR(x))

#define HW_TZASC_REG_SET_HIGH1(x)    (*(volatile uint32_t *) HW_TZASC_REG_SET_HIGH1_ADDR(x))
#define HW_TZASC_REG_SET_LOW1(x)    (*(volatile uint32_t *) HW_TZASC_REG_SET_LOW1_ADDR(x))



#define HW_TZASC_REG0_ATTRIB(x)      (*(volatile uint32_t *) HW_TZASC_REG0_ATTRIB_ADDR(x))
#define HW_TZASC_REG1_ATTRIB(x)      (*(volatile uint32_t *) HW_TZASC_REG1_ATTRIB_ADDR(x))

#define TZASC_NO_OF_REG              (0x00000001)
#define TZASC_ADDR_WIDTH             (0b011111<<8)
#define TZASC_CONF_REG               ( TZASC_NO_OF_REG | TZASC_ADDR_WIDTH)
#define TZASC_SPEC_OFF               (0x00000003)
#define TZASC_REG0_ATTRIB            (0xF0000000) // ALL ACCESS
#define TZASC_REG1_ATTRIB_EN         (0x00000001) // SECURE ACCESS
#define TZASC_REG1_ATTRIB_SIZE       /*(0b011101<<1)*/(0b0011001<<1) // SECURE ACCESS
#define TZASC_REG1_ATTRIB_SP         (0xC0000000) // SECURE ACCESS
#define TZASC_REG1_ATTRIB            (TZASC_REG1_ATTRIB_EN |TZASC_REG1_ATTRIB_SIZE | TZASC_REG1_ATTRIB_SP) // ALL ACCESS


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define REGS_MMDC1_BASE (0x021b0000) //!< Base address for MMDC instance number 1.
#define REGS_MMDC2_BASE (0x021b4000) //!< Base address for MMDC instance number 2.

#define REGS_MMDC_BASE(x) ( (x) == 1 ? REGS_MMDC1_BASE : (x) == 2 ? REGS_MMDC2_BASE : 0x00d00000)

#define HW_MMDC_MDCTL_ADDR(x)      (REGS_MMDC_BASE(x) + 0x0)
#define HW_MMDC_MDPDC_ADDR(x)      (REGS_MMDC_BASE(x) + 0x4)
#define HW_MMDC_MDMISC_ADDR(x)     (REGS_MMDC_BASE(x) + 0x18)
#define HW_MMDC_MDSCR_ADDR(x)      (REGS_MMDC_BASE(x) + 0x1c)


#define HW_MMDC_MDCTL(x)           (*(volatile uint32_t *) HW_MMDC_MDCTL_ADDR(x))
#define HW_MMDC_MDPDC(x)           (*(volatile uint32_t *) HW_MMDC_MDPDC_ADDR(x))
#define HW_MMDC_MDMISC(x)          (*(volatile uint32_t *) HW_MMDC_MDMISC_ADDR(x))
#define HW_MMDC_MDSCR(x)           (*(volatile uint32_t *) HW_MMDC_MDSCR_ADDR(x))



#endif

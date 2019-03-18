
#ifndef __MEMORY_SECURITY_CONF_LTZVISOR_H
#define __MEMORY_SECURITY_CONF_LTZVISOR_H
#include "types.h"


#define REGS_IOMUXC_BASE (0x020e0000) //!< Base address for IOMUXC.
#define HW_IOMUXC_GPR10_ADDR      (REGS_IOMUXC_BASE + 0x28)
#define HW_IOMUXC_GPR10           (*(volatile uint32_t *) HW_IOMUXC_GPR10_ADDR)

/*
Secure OCRAM start address, in order to change the OCRAM start address change
 this define value, the values range from OCRAM_START_ADDR to OCRAM_END_ADDR,
 the when choosing the secure OCRAM start address one must consider that the
 granularity is of 4KB, in other words the secure OCRAM size must be divisible
 4KB ((OCRAM_END_ADDR - SECURE_OCRAM_START_ADDR) % 4KB) = 0
*/
#define SECURE_OCRAM_START_ADDR          (0x00934000)

#define OCRAM_START_ADDR                 (0x00900000)
#define OCRAM_END_ADDR                   (0x0093FFFF)

#define OCRAM_FREE_AREA_START_ADDR       (0x00907000)
#define OCRAM_SECURE_START_ADDR          (0x00934000)
#define OCRAM_SECURE_END_ADDR            (0x00938000)
#define OCRAM_MMU_TABLE_START_ADDD       (OCRAM_SECURE_END_ADDR)
#define OCRAM_MMU_TABLE_END_ADDD         (0x0093E000)
#define OCRAM_STACK_START_ADD            (OCRAM_MMU_TABLE_END_ADDD)
#define OCRAM_RAM_VECTOR_START_ADDR      (0x0093FFB8)
#define LOCK_OCRAM_TZ_EN                 (0b00010000)
#define LOCK_OCRAM_TZ_ADDR               (0b00110100)// ((SECURE_OCRAM_START_ADDR - OCRAM_START_ADDR)/4*1024) //

#define CONFIG_OCRAM_SECURITY()      (HW_IOMUXC_GPR10 = (((int)LOCK_OCRAM_TZ_ADDR<<5) | LOCK_OCRAM_TZ_EN))





#endif

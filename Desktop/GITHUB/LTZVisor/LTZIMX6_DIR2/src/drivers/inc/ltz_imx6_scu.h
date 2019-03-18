
#ifndef __LTZ_SCU_H__
#define __LTZ_SCU_H__

#include <types.h>

#define ARM_PERIPHBASE			    	             0x00A00000
#define SCU_BASE_ADDR                        ARM_PERIPHBASE
#define SCU_CONTROL_BASE_ADDR 		         (SCU_BASE_ADDR + 0x00)  //  SCU Control Register
#define SCU_CONFIG_BASE_ADDR 		           (SCU_BASE_ADDR + 0x04)  //  SCU Configuration Register
#define SCU_CPU_PWR_STAT_BASE_ADDR 	       (SCU_BASE_ADDR + 0x08)  //  SCU CPU Power Register
#define SCU_INVAL_SECURE_BASE_ADDR 	       (SCU_BASE_ADDR + 0x0C)  //  SCU Invalidate All Registers in Secure State Register
#define SCU_FILTERSTART_BASE_ADDR 	       (SCU_BASE_ADDR + 0x40)  //  Filtering Start Address Register
#define SCU_FILTEREND_BASE_ADDR 	         (SCU_BASE_ADDR + 0x44)  //  Filtering End Address Register
#define SCU_ACCESS_CONTROL_SAC_BASE_ADDR   (SCU_BASE_ADDR + 0x50)  //  SCU Access Control (SAC) Register
#define SCU_NON_SEC_ACCESS_CTR_BASE_ADDR   (SCU_BASE_ADDR + 0x54)  //  SCU Non-secure Access Control (SNSAC) Register

#define LTZ_LINUX_SCU_ACC_CTR_SAC_VAL      (0x0000000f)
#define LTZ_LINUX_SCU_NON_SEC_CTR_VAL      (0xfff)



#define SCU_ACCESS_CONTROL_SAC           (*(volatile uint32_t *) SCU_ACCESS_CONTROL_SAC_BASE_ADDR)
#define SCU_ACCESS_CONTROL_SAC_WR(v)     (SCU_ACCESS_CONTROL_SAC = (v))

#define SCU_NON_SEC_ACCESS_CTR           (*(volatile uint32_t *) SCU_NON_SEC_ACCESS_CTR_BASE_ADDR)
#define SCU_NON_SEC_ACCESS_CTR_WR(v)     (SCU_NON_SEC_ACCESS_CTR = (v))


#endif

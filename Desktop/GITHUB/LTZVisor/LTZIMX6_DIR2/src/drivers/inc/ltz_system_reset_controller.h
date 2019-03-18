
#ifndef __IMX6_SYSTEM_RESET_CONTROLLER_H__
#define __IMX6_SYSTEM_RESET_CONTROLLER_H__

/* SRC Base address*/
#define REGS_SRC_BASE         (0x020d8000)

/* SRC Control Register */
#define HW_SRC_SCR_ADDR       (REGS_SRC_BASE + 0x00)

/* SRC General Purpose Register Base address */
#define HW_SRC_GPR1_ADDR      (REGS_SRC_BASE + 0x20)
#define HW_SRC_GPR2_ADDR      (REGS_SRC_BASE + 0x24)
#define HW_SRC_GPR3_ADDR      (REGS_SRC_BASE + 0x28)
#define HW_SRC_GPR4_ADDR      (REGS_SRC_BASE + 0x2C)
#define HW_SRC_GPR5_ADDR      (REGS_SRC_BASE + 0x30)
#define HW_SRC_GPR6_ADDR      (REGS_SRC_BASE + 0x34)
#define HW_SRC_GPR7_ADDR      (REGS_SRC_BASE + 0x38)
#define HW_SRC_GPR8_ADDR      (REGS_SRC_BASE + 0x3C)
#define HW_SRC_GPR9_ADDR      (REGS_SRC_BASE + 0x40)
#define HW_SRC_GPR10_ADDR     (REGS_SRC_BASE + 0x44)

/* SRC Control Pointer */
#define HW_SRC_SCR            (*(volatile uint32_t *) HW_SRC_SCR_ADDR )

/* SRC General Purpose Register Pointers */
#define HW_SRC_GPR1           (*(volatile uint32_t *) HW_SRC_GPR1_ADDR)
#define HW_SRC_GPR2           (*(volatile uint32_t *) HW_SRC_GPR2_ADDR)
#define HW_SRC_GPR3           (*(volatile uint32_t *) HW_SRC_GPR3_ADDR)
#define HW_SRC_GPR4           (*(volatile uint32_t *) HW_SRC_GPR4_ADDR)
#define HW_SRC_GPR5           (*(volatile uint32_t *) HW_SRC_GPR5_ADDR)
#define HW_SRC_GPR6           (*(volatile uint32_t *) HW_SRC_GPR6_ADDR)
#define HW_SRC_GPR7           (*(volatile uint32_t *) HW_SRC_GPR7_ADDR)
#define HW_SRC_GPR8           (*(volatile uint32_t *) HW_SRC_GPR8_ADDR)



#define SRC_CORE1_ENABLE      (0x00400000)  //!< Bit mask for SRC_CORE1_ENABLE.
#define SRC_CORE2_ENABLE      (0x00800000)  //!< Bit mask for SRC_CORE2_ENABLE.
#define SRC_CORE3_ENABLE      (0x01000000)  //!< Bit mask for SRC_CORE3_ENABLE.


#endif

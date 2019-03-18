#ifndef _LTZVIROR_CSU_H_
#define _LTZVIROR_CSU_H_
#include <types.h>
/*
 * i.MX6DQ CSU
 *
 * CSU Registers
 *
 * Registers defined in this header file:
 * - HW_CSU_CSLn - Config security level register
 * - HW_CSU_HP0 - HP0 register
 * - HW_CSU_HP1 - HP1 register
 * - HW_CSU_SA - Secure access register
 * - HW_CSU_HPCONTROL0 - HPCONTROL0 register
 * - HW_CSU_HPCONTROL1 - HPCONTROL1 register
 *
 * - hw_csu_t - Struct containing all module registers.
 */

//! @name Module base addresses
#define DEFAULT_CSL_VAL   (0x00ff00ff)
#define DEFAULT_CSL_SA    (0x55555554)


#define REGS_CSU_BASE   (0x021c0000) //!< Base address for CSU.

#define HW_CSU_CSLn_COUNT     (40)
#define HW_CSU_CSLn_ADDR(n)   (REGS_CSU_BASE + 0x0 + (0x4 * (n)))

#define HW_CSU_CSLn(n)        (*(volatile uint32_t *) HW_CSU_CSLn_ADDR(n))
#define HW_CSU_CSLn_RD(n)     (HW_CSU_CSLn(n))
#define HW_CSU_CSLn_WR(n, v)  (HW_CSU_CSLn(n) = (v))
#define HW_CSU_CSLn_SET(n, v) (HW_CSU_CSLn_WR(n, HW_CSU_CSLn_RD(n) |  (v)))
#define HW_CSU_CSLn_CLR(n, v) (HW_CSU_CSLn_WR(n, HW_CSU_CSLn_RD(n) & ~(v)))
#define HW_CSU_CSLn_TOG(n, v) (HW_CSU_CSLn_WR(n, HW_CSU_CSLn_RD(n) ^  (v)))



#define HW_CSU_SA_ADDR      (REGS_CSU_BASE + 0x218)

#define HW_CSU_SA           (*(volatile uint32_t *) HW_CSU_SA_ADDR)
#define HW_CSU_SA_RD()      (HW_CSU_SA)
#define HW_CSU_SA_WR(v)     (HW_CSU_SA = (v))
#define HW_CSU_SA_SET(v)    (HW_CSU_SA_WR(HW_CSU_SA_RD() |  (v)))
#define HW_CSU_SA_CLR(v)    (HW_CSU_SA_WR(HW_CSU_SA_RD() & ~(v)))
#define HW_CSU_SA_TOG(v)    (HW_CSU_SA_WR(HW_CSU_SA_RD() ^  (v)))



#endif

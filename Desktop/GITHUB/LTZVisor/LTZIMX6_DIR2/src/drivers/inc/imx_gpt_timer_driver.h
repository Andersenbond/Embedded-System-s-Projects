#ifndef __IMX6_GPT_H__
#define __IMX6_GPT_H__
#include <types.h>

#define REGS_GPT_BASE (0x02098000) //!< Base address for GPT.


#define HW_GPT_CR_ADDR      (REGS_GPT_BASE + 0x0)
#define HW_GPT_PR_ADDR      (REGS_GPT_BASE + 0x4)
#define HW_GPT_SR_ADDR      (REGS_GPT_BASE + 0x8)
#define HW_GPT_IR_ADDR      (REGS_GPT_BASE + 0xc)
#define HW_GPT_OCR1_ADDR    (REGS_GPT_BASE + 0x10)
#define HW_GPT_OCR2_ADDR    (REGS_GPT_BASE + 0x14)
#define HW_GPT_OCR2_ADDR    (REGS_GPT_BASE + 0x14)
#define HW_GPT_OCR3_ADDR    (REGS_GPT_BASE + 0x18)
#define HW_GPT_ICR1_ADDR    (REGS_GPT_BASE + 0x1c)
#define HW_GPT_ICR2_ADDR    (REGS_GPT_BASE + 0x20)
#define HW_GPT_CNT_ADDR     (REGS_GPT_BASE + 0x24)


#define HW_GPT_CR           (*(volatile uint32_t *) HW_GPT_CR_ADDR)
#define HW_GPT_PR           (*(volatile uint32_t *) HW_GPT_PR_ADDR)
#define HW_GPT_SR           (*(volatile uint32_t *) HW_GPT_SR_ADDR)
#define HW_GPT_IR           (*(volatile uint32_t *) HW_GPT_IR_ADDR)
#define HW_GPT_CNT          (*(volatile uint32_t *) HW_GPT_CNT_ADDR)
#define HW_GPT_OCR1         (*(volatile uint32_t *) HW_GPT_OCR1_ADDR)
#define HW_GPT_OCR2         (*(volatile uint32_t *) HW_GPT_OCR2_ADDR)
#define HW_GPT_OCR3         (*(volatile uint32_t *) HW_GPT_OCR3_ADDR)
#define HW_GPT_ICR1         (*(volatile uint32_t *) HW_GPT_ICR1_ADDR)
#define HW_GPT_ICR2         (*(volatile uint32_t *) HW_GPT_ICR2_ADDR)


#define HW_GPT_CR_RD()      (HW_GPT_CR)
#define HW_GPT_PR_RD()      (HW_GPT_PR)
#define HW_GPT_SR_RD()      (HW_GPT_SR)
#define HW_GPT_IR_RD()      (HW_GPT_IR)
#define HW_GPT_CNT_RD()     (HW_GPT_CNT)
#define HW_GPT_OCR1_RD()    (HW_GPT_OCR1)
#define HW_GPT_OCR2_RD()    (HW_GPT_OCR2)
#define HW_GPT_OCR3_RD()    (HW_GPT_OCR3)
#define HW_GPT_ICR1_RD()    (HW_GPT_ICR1)
#define HW_GPT_ICR2_RD()    (HW_GPT_ICR2)

#endif

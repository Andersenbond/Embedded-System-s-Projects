

/*
 * Copyright (c) 2012, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 #define BOARD_SMART_DEVICE
 #include <types.h>
 #include <ltz_regsiomuxc.h>
 #include "regsusdhc.h"

 typedef enum {
 	GPIO_NONE  = 0,
 	GPIO_PORT1 = 1,
  GPIO_PORT2 = 2,
  GPIO_PORT3 = 3,
  GPIO_PORT4 = 4,
  GPIO_PORT5 = 5,
  GPIO_PORT6 = 6,
  GPIO_PORT7 = 7,
 } GPIO_PORT;

 #define HW_GPIO_INSTANCE_COUNT (7) //!< Number of instances of the GPIO module.
 #define INVALID_PARAMETER (-1)

 #define GPIO_GDIR_OUTPUT            1   //!< GPIO pin is output
 #define GPIO_GDIR_INPUT             0   //!< GPIO pin is output

 #define GPIO_LOW_LEVEL              0   //!< GPIO pin is low
 #define GPIO_HIGH_LEVEL             1   //!< GPIO pin is high
 #define GPIO_ICR_LOW_LEVEL          0   //!< Interrupt is low-level
 #define GPIO_ICR_HIGH_LEVEL         1   //!< Interrupt is high-level
 #define GPIO_ICR_RISE_EDGE          2   //!< Interrupt is rising edge
 #define GPIO_ICR_FALL_EDGE          3   //!< Interrupt is falling edge
 #define GPIO_IMR_MASKED             0   //!< Interrupt is masked
 #define GPIO_IMR_UNMASKED           1   //!< Interrupt is unmasked
 #define GPIO_ISR_NOT_ASSERTED       0   //!< Interrupt is not asserted
 #define GPIO_ISR_ASSERTED           1   //!< Interrupt is asserted
 #define GPIO_EDGE_SEL_DISABLE       0   //!< Edge select is disabled
 #define GPIO_EDGE_SEL_ENABLE        1   //!< Edge select is enabled

 #define HW_GPIO1 (1) //!< Instance number for GPIO1.
 #define HW_GPIO2 (2) //!< Instance number for GPIO2.
 #define HW_GPIO3 (3) //!< Instance number for GPIO3.
 #define HW_GPIO4 (4) //!< Instance number for GPIO4.
 #define HW_GPIO5 (5) //!< Instance number for GPIO5.
 #define HW_GPIO6 (6) //!< Instance number for GPIO6.
 #define HW_GPIO7 (7) //!< Instance number for GPIO7.

 #define REGS_GPIO1_BASE (0x0209c000) //!< Base address for GPIO instance number 1.
 #define REGS_GPIO2_BASE (0x020a0000) //!< Base address for GPIO instance number 2.
 #define REGS_GPIO3_BASE (0x020a4000) //!< Base address for GPIO instance number 3.
 #define REGS_GPIO4_BASE (0x020a8000) //!< Base address for GPIO instance number 4.
 #define REGS_GPIO5_BASE (0x020ac000) //!< Base address for GPIO instance number 5.
 #define REGS_GPIO6_BASE (0x020b0000) //!< Base address for GPIO instance number 6.
 #define REGS_GPIO7_BASE (0x020b4000) //!< Base address for GPIO instance number 7.
 #define REGS_GPIO_BASE(x)       ( (x) == HW_GPIO1 ? REGS_GPIO1_BASE : (x) == HW_GPIO2 ? REGS_GPIO2_BASE : (x) == HW_GPIO3 ? REGS_GPIO3_BASE : (x) == HW_GPIO4 ? REGS_GPIO4_BASE : (x) == HW_GPIO5 ? REGS_GPIO5_BASE : (x) == HW_GPIO6 ? REGS_GPIO6_BASE : (x) == HW_GPIO7 ? REGS_GPIO7_BASE : 0x00d00000)

 #define HW_GPIO_DR_ADDR(x)      (REGS_GPIO_BASE(x) + 0x0)
 #define HW_GPIO_GDIR_ADDR(x)    (REGS_GPIO_BASE(x) + 0x4)
 #define HW_GPIO_DR(x)           (*(volatile uint32_t *) HW_GPIO_DR_ADDR(x))
 #define HW_GPIO_GDIR(x)         (*(volatile uint32_t *) HW_GPIO_GDIR_ADDR(x))


 #define HW_GPIO_DR_ADDR(x)      (REGS_GPIO_BASE(x) + 0x0)
 #define HW_GPIO_DR_RD(x)        (HW_GPIO_DR(x))
 #define HW_GPIO_DR_WR(x, v)     (HW_GPIO_DR(x) = (v))
 #define HW_GPIO_DR_SET(x, v)    (HW_GPIO_DR_WR(x, HW_GPIO_DR_RD(x) |  (v)))
 #define HW_GPIO_DR_CLR(x, v)    (HW_GPIO_DR_WR(x, HW_GPIO_DR_RD(x) & ~(v)))
 #define HW_GPIO_DR_TOG(x, v)    (HW_GPIO_DR_WR(x, HW_GPIO_DR_RD(x) ^  (v)))




 #define REGS_IOMUXC_BASE (0x020e0000) //!< Base address for IOMUXC.
 #define HW_IOMUXC_SW_MUX_CTL_PAD_GPIO02_ADDR      (REGS_IOMUXC_BASE + 0x234)
 #define HW_IOMUXC_SW_MUX_CTL_PAD_GPIO02           (*(volatile uint32_t *) HW_IOMUXC_SW_MUX_CTL_PAD_GPIO02_ADDR)

 #define BV_IOMUXC_SW_MUX_CTL_PAD_GPIO02_MUX_MODE__ALT5 (0x5) //!< Select signal GPIO1_IO02.
 #define HW_IOMUXC_SW_MUX_CTL_PAD_GPIO02  (0x5);          // GPIO1[2]

 uint32_t gpio_inst = HW_GPIO1;
 uint32_t bit = 2;



 bool gpio_set_direction(int32_t port, int32_t pin, int32_t dir){
     uint32_t oldVal = 0, newVal = 0;
     oldVal = HW_GPIO_GDIR(port);
     if (dir == GPIO_GDIR_INPUT)
         newVal = oldVal & (~(1 << pin));
     else
         newVal = oldVal | (1 << pin);
     HW_GPIO_GDIR(port) = newVal ;
     return true;
 }




 bool  gpio_set_level(int32_t port, int32_t pin, uint32_t level){

     uint32_t mask = 1 << pin;
     int32_t dir = HW_GPIO_GDIR(port) & mask ? GPIO_GDIR_OUTPUT : GPIO_GDIR_INPUT;
     if (dir != GPIO_GDIR_OUTPUT){
       //  printk("GPIO_PORT%d_%d is not configured to be output!\n", port, pin);
         return false;
     }
     uint32_t value = HW_GPIO_DR(port);   // read current value
     if (level == GPIO_LOW_LEVEL)            // fix it up
     	value &= ~mask;
     else if ( level == GPIO_HIGH_LEVEL)
     	value |= mask;
     HW_GPIO_DR(port) = value;             // write new value
     return true; //SUCCESS;
 }



 int32_t gpio_get_level(int32_t port, int32_t pin)
 {
     if ((port > HW_GPIO_INSTANCE_COUNT) || (port < 1))
     {
         printk("Invalid GPIO Instance GPIO_PORT%d parameter. GPIO_PORT1~GPIO_PORT%d is allowed.\n",
         		port, HW_GPIO_INSTANCE_COUNT);
         return INVALID_PARAMETER;
     }

     if ((pin > 31) || (pin < 0))
     {
         printk("Invalid GPIO Pin %d parameter. Pin 0~31 is allowed.\n", pin);
         return INVALID_PARAMETER;
     }

     uint32_t mask = 1 << pin;

     return HW_GPIO_DR_RD(port) & mask ? GPIO_HIGH_LEVEL : GPIO_LOW_LEVEL;
 }



////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/*!
  * Set up the GPIO for USDHC
  */
void usdhc_gpio_config(uint32_t instance)
{
    switch (instance) {
    case HW_USDHC1:
        break;

    case HW_USDHC2:
        break;

    case HW_USDHC3:
        break;
    case HW_USDHC4:
        break;

    default:
        break;
    }
}

bool usdhc_card_detected(uint32_t instance)
{
    bool ret_val = false;

#if defined(BOARD_SABRE_AI)
    if (instance == HW_USDHC1) {
        // SDb_CD_B
        // Bit HIGH == pin LOW == Card Detected.
        if (BG_USDHC_PRES_STATE_CDPL(HW_USDHC_PRES_STATE_RD(instance))) {
            ret_val = true;
        }
    } else if (instance == HW_USDHC3) {
        // SDa_CD_B using GPIO6_15
        // Bit LOW == pin LOW ==  Card Detected.
        ret_val = gpio_get_level(GPIO_PORT6, 15) == GPIO_LOW_LEVEL;
    }
#elif defined(BOARD_SMART_DEVICE)
    if (instance == HW_USDHC2) {
        // SD2_CD_B using GPIO2_02
        // Bit LOW == pin LOW ==  Card Detected.
        ret_val = gpio_get_level(GPIO_PORT2, 2) == GPIO_LOW_LEVEL;
    } else if (instance == HW_USDHC3) {
        // SD3_CD_B using GPIO2_00
        // Bit LOW == pin LOW ==  Card Detected.
        ret_val = gpio_get_level(GPIO_PORT2, 0) == GPIO_LOW_LEVEL;
    }
#else
    if (BG_USDHC_PRES_STATE_CDPL(HW_USDHC_PRES_STATE_RD(instance))) {
        // Bit HIGH == pin LOW ==  Card Detected.
        ret_val = true;
    }
#endif

    return ret_val;
}

bool usdhc_write_protected(uint32_t instance)
{
    bool ret_val = true;

#if defined(BOARD_SABRE_AI)
    if (instance == HW_USDHC1) {
        // SDb_WP (CSI0_DATA_EN_GPIO5_20)
        // Bit HIGH == pin HIGH == Write Protected.
        ret_val = gpio_get_level(GPIO_PORT5, 20) == GPIO_HIGH_LEVEL;
    } else if (instance == HW_USDHC3) {
        // SDa_WP (SD2_DAT2_GPIO1_13)
        // Bit HIGH == pin HIGH == Write Protected.
        ret_val = gpio_get_level(GPIO_PORT1, 13) == GPIO_HIGH_LEVEL;
    }
#elif defined(BOARD_SMART_DEVICE)
    if (instance == HW_USDHC2) {
        // SD2_WP using GPIO2_03
        // Bit HIGH == pin HIGH == Write Protected.
        ret_val = gpio_get_level(GPIO_PORT2, 3) == GPIO_HIGH_LEVEL;
    } else if (instance == HW_USDHC3) {
        // SD3_WP using GPIO2_01
        // Bit HIGH == pin HIGH == Write Protected.
        ret_val = gpio_get_level(GPIO_PORT2, 1) == GPIO_HIGH_LEVEL;
    }
#else
    if (BG_USDHC_PRES_STATE_WPSPL(HW_USDHC_PRES_STATE_RD(instance))) {
        // Bit HIGH == pin LOW == NOT Write Protected (Write Enabled).
        ret_val = false;
    }
#endif

    return ret_val;
}

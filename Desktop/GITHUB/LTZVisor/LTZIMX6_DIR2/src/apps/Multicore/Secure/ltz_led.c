
#include <types.h>
#include <printk.h>
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


void ltz_led (bool status){
    gpio_set_direction(gpio_inst, bit, GPIO_GDIR_OUTPUT);
    //printk("Is the USER LED blinking?\n");
    if (status)
        gpio_set_level(gpio_inst, bit, GPIO_HIGH_LEVEL);
    else
        gpio_set_level(gpio_inst, bit, GPIO_LOW_LEVEL);
}

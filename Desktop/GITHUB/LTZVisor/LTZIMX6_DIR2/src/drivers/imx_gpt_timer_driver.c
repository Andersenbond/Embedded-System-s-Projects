#include <imx_gpt_timer_driver.h>
#include <imx6_ccm.h>
#include <gic.h>
#include <interrupt.h>
#include <printk.h>

void gpt_counter_enable(){
    // ensure to start the counter in a proper state by clearing possible remaining events
    HW_GPT_SR |= 0x3F;
    // enable the interrupts or clear the register for polling
    HW_GPT_IR |= 0x00000007;

    // finally, enable the counter
    HW_GPT_CR |= 1;
}

gpt_counter_print(void){
  printk ("%x\n",HW_GPT_CNT_RD());

}

void gpt_counter_disable(void){
    // disable the counter
    HW_GPT_CR  &= (~0x00000001);
    HW_GPT_SR |= 0x3F;
}

void gpt_counter_reset(void){
  gpt_counter_disable();
  printk ("%x\n",HW_GPT_CNT_RD());
  gpt_counter_enable();
}


void init_gpt_timer(void){
  uint32_t control_reg_tmp = 0;

  // enable the source clocks to the GPT port
   HW_CCM_CCGR1 |= 0x00300000;// | 0x00c00000;

   // start with a known state by disabling and reseting the module
   HW_GPT_CR |= 0x00008000;

   // wait for the reset to complete
   while ((HW_GPT_CR & 0x00008000) != 0);

   control_reg_tmp |= 0x40;

   // the prescaler can be changed at any time, and
   // this affects the output clock immediately
   HW_GPT_PR |= 0;

   // set the counter free running mode
   control_reg_tmp |= 0x00000200;

   // specify from where the counter starts to count when enabled
   // this code makes it start from 0
   control_reg_tmp |= 0x00000002;

   //compare event 1 active
   control_reg_tmp  &= (~0x00700000);

   // finally write the control register
   HW_GPT_CR = control_reg_tmp;

   HW_GPT_OCR1 = 0x0f000000;
}

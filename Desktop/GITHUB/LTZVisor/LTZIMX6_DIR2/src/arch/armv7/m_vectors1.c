#include <types.h>
#include <gic.h>
#include <interrupt.h>
#include <ltzvisor_multicore_spinlock.h>
#include <ltzvisor.h>
extern void smc_handler();
extern irq_hdlr_t g_interrupt_handlers[IMX_INTERRUPT_COUNT];
extern volatile uint32_t g_vectNum[4];

__attribute__ ((interrupt("ABT")))
void MON_ABT_HDLR(int val){
 printk("MON_ABT handler %x\n",val );
   while(1);
}

__attribute__ ((interrupt("PREF")))
void MON_PREFECH_HDLR(int val){
  printk("MON_PREFETCH handler\n");
  while(1);
}

__attribute__ ((interrupt("SMC")))
void MON_SMC_HDLR(int val){
  printk("SMC handler\n");
  while(1);
}

__attribute__ ((interrupt("RESV")))
void MON_RESERVED_HDLR(int val){
  printk("MON_RESERVED handler\n");
  while(1);
}


FreeRTOSnsCleanFIQ(void){
  ltz_epit_get_compare_event(1);
}

//! @brief Handles IRQ exceptions.
__attribute__ ((interrupt("IRQ")))
void MON_FIQ_HDLR(void){
    // vectNum = RESERVED[31:13] | CPUID[12:10] | INTERRUPT_ID[9:0]
    // send ack and get ID source
    uint32_t vectNum = gic_read_irq_ack();
    printk("MON FIQ handler: ID: %d\n",vectNum);

    // Check that INT_ID isn't 1023 or 1022 (spurious interrupt)
    if (vectNum & 0x0200  )
    {
        gic_write_end_of_irq(vectNum);  // send end of irq
    }
    else
    {
        // copy the local value to the global image of CPUID
        unsigned cpu = (vectNum >> 10) & 0x7;
        unsigned irq = vectNum & 0x1FF;

        // Store the current interrupt number.
        g_vectNum[cpu] = irq;

        // Call the service routine stored in the handlers array. If there isn't
        // one for this IRQ, then call the default handler.
        irq_hdlr_t isr = g_interrupt_handlers[irq];
        if (isr)
        {
            if(vectNum == 88) FreeRTOSnsCleanFIQ();
            else isr();
        }
        else
        {
            default_interrupt_routine();
        }

        // Clear current interrupt number.
        g_vectNum[cpu] = 0;

        // Signal the end of the irq.
        gic_write_end_of_irq(vectNum);
    }
}




//! @brief Handles IRQ exceptions.
__attribute__ ((interrupt("IRQ")))
void MON_IRQ_HDLR(void){
    // vectNum = RESERVED[31:13] | CPUID[12:10] | INTERRUPT_ID[9:0]
    // send ack and get ID source
    uint32_t vectNum = gic_read_irq_ack();
    printk("IRQ handler: ID: %d\n",vectNum);

    // Check that INT_ID isn't 1023 or 1022 (spurious interrupt)
    if (vectNum & 0x0200)
    {
        gic_write_end_of_irq(vectNum);  // send end of irq
    }
    else
    {
        // copy the local value to the global image of CPUID
        unsigned cpu = (vectNum >> 10) & 0x7;
        unsigned irq = vectNum & 0x1FF;

        // Store the current interrupt number.
        g_vectNum[cpu] = irq;

        // Call the service routine stored in the handlers array. If there isn't
        // one for this IRQ, then call the default handler.
        irq_hdlr_t isr = g_interrupt_handlers[irq];
        if (isr)
        {
            isr();
        }
        else
        {
            default_interrupt_routine();
        }

        // Clear current interrupt number.
        g_vectNum[cpu] = 0;

        // Signal the end of the irq.
        gic_write_end_of_irq(vectNum);
        gic_send_sgi(3, 0, kGicSgiFilter_UseTargetList);
    }
}



uint32_t cfiq_interrupt_handler(void){
  volatile static uint32_t counter = 2000;
  uint32_t vectNum = gic_read_irq_ack(), ret = 0;
  if (vectNum & 0x0200){
    gic_write_end_of_irq(vectNum);  // send end of irq
  }
  else{
    unsigned cpu = (vectNum >> 10) & 0x7;
    unsigned irq = vectNum & 0x1FF;
    g_vectNum[cpu] = irq;
    irq_hdlr_t isr = g_interrupt_handlers[irq];
    if (isr){
      if(counter > 0){
          FreeRTOSnsCleanFIQ();
          counter--;
          ret = 88;
      }
      else {
        //isr();
          FreeRTOSnsCleanFIQ();
          counter = 2000;
          ret = 89;
      }
    }
    else{
      default_interrupt_routine();
    }
    g_vectNum[cpu] = 0;
    gic_write_end_of_irq(vectNum);
  }
  return ret;
}




__attribute__ ((interrupt("UND")))
void MON_UNDEFINED_HDLR(int val){
  printk("MON_UNDEFINED handler\n");
  while(1);
}


void show_regs (int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k){
  ltzvisor_spin_lock(&printk_spinlock, TIMEOUT_FOREVER);
  printk("CODE HERE: r0: %x |r1: %x |r2: %x|r3: %x|r4: %x|r5: %x|r6: %x|r7: %x|r8: %x|r9: %x|r10: %x\n",a,b,c,d,e,f,g,h,i,j,k);
  ltzvisor_spin_unlock(&printk_spinlock);
}

void im_here(void){
  static int i = 0;
  printk("CODE HERE: %d\n", i++);
}




void multicore_fiq_interrupt_handler(void){
    uint32_t vectNum = gic_read_irq_ack();
    gic_write_end_of_irq(vectNum);
    ltzvisor_spin_lock(&printk_spinlock, TIMEOUT_FOREVER);
    printk("NS_Guest 1 - FIQ: %d  on CORE_%d\n",vectNum, cpu_get_current());
    ltzvisor_spin_unlock(&printk_spinlock);
}



void configure_epit_ns(void){
    int irq_id = 89;
    ltzvisor_spin_lock(&printk_spinlock, TIMEOUT_FOREVER);
    int cpu_id = cpu_get_current();

    gic_init();
    arm_set_interrupt_state(true);
    enable_secure_interrupt(irq_id, cpu_id, 0);

    printk("EPIT Configured on CORE%d\n",cpu_id);
    ltzvisor_spin_unlock(&printk_spinlock);

 }

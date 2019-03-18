#include <ltzvisor_hw.h>
#include <ltz_imx6_scu.h>
/**
 * LTZVisor hardware initialization
 *
 * @param
 *
 * @retval Return TRUE if success or False if not
 */
 #define get_ptr_val(x)  (*((volatile unsigned long *)(x)))
#define set_ptr_val(x,y)  ((*((volatile unsigned long *)(x))) = (y))

 void write__(uint32_t ptr){
 	int i = 0, j = 0;
 	printk("Writing... \n");
 	for(i = 0; i<100; i++){
    set_ptr_val(ptr,0xfacada10);
  			ptr+=4;
 		}
    printk("End writing \n");
 }

 void imx6_init_csu(void){
 int i = 0;
 HW_CSU_SA_WR(DEFAULT_CSL_SA);
 for(i=0; i<HW_CSU_CSLn_COUNT; i++)
   HW_CSU_CSLn_WR(i,DEFAULT_CSL_VAL);
 }



 void read__(uint32_t ptr){
 	int i = 0, j = 0;
 	printk("------------------------------------------------------------------- \n");
 	for(i = 0; i<10; i++){
 		printk("LINE%x:",i);
 		for(j = 0; j<10; j++){
 			printk(" |%d-> %x ",j ,get_ptr_val(ptr));
 			ptr+=4;
 		}
 		printk("\n");
 	}
 	printk("------------------------------------------------------------------- \n");
 }

 #define __REG(x) (*((volatile unsigned long *)(x)))


 // Central Security Unit
 void ltzvisor_csu_config(void){
 	unsigned long reg;
 	/* For Peripheral access policy, This is may be similar with TZPC function in my opinion
 	   it can be decide device secure or non-secure on the AXI-APB bus dynamically
 	   now setting all peripheral can be access in nonNS_GUEST-secure
 	   but it will change for secure access restrict, example the internal ram access permit
  */
  for(reg = 0x021C0000; reg < 0x021C00A0; reg = reg + 4){
   	__REG(reg) = 0x00ff00ff;
    //if(reg == (0x021C0000 + 26*4))
    //    __REG(reg) = 0x00ff00ff;
  }
 	// for OCRAM secure status setting
 	// start address it 0, it means all internal ram will be use as secure ram
 	__REG(0x020e0028) = 0xFFFF;//0x00003810;
  //CONFIG_OCRAM_SECURITY();
 }

 void ltzvisor_ddr_tzasc_config(void){
    HW_TZASC_CTRL(1)        = 0x1F01; //TZASC_CONF_REG;
    HW_TZASC_SPEC_CONTR(1)  = TZASC_SPEC_OFF;
    HW_TZASC_REG_SET_LOW1(1) = 0x0;
    HW_TZASC_REG_SET_HIGH1(1) = 0x0;
    HW_TZASC_REG0_ATTRIB(1) = TZASC_REG0_ATTRIB;
    HW_TZASC_REG1_ATTRIB(1) = TZASC_REG0_ATTRIB;

    HW_TZASC_CTRL(2)        = 0x1F01 ;//TZASC_CONF_REG;
    HW_TZASC_SPEC_CONTR(2)  = TZASC_SPEC_OFF;
    HW_TZASC_REG_SET_LOW1(2) = 0x0;
    HW_TZASC_REG_SET_HIGH1(2) = 0x0;
    HW_TZASC_REG0_ATTRIB(2) = TZASC_REG0_ATTRIB;
    HW_TZASC_REG1_ATTRIB(2) = TZASC_REG0_ATTRIB; //TZASC_REG1_ATTRIB;
}


void tick_timer_interrupt_routine(void){
    ltz_epit_get_compare_event(2);
}

void epit_kickoff(void){
   int irq_id = 89;
   printk("EPIT is programmed to generate an interrupt every 5s.\n");
   ltz_epit_init(2,2000000);
   register_interrupt_routine(irq_id, tick_timer_interrupt_routine);
   enable_secure_interrupt(irq_id, 0, 0);
   ltz_epit_enable(2);
}


uint32_t ltzvisor_hw_init(void){
	uint32_t ret = TRUE;
  iomux_config();
  gic_init();
  arm_set_interrupt_state(true);
	ltz_uart_init(115200);
	ltzvisor_csu_config();
	//ltz_system_time_init();
  ltzvisor_spin_init(&printk_spinlock);
  ltzvisor_spin_init(&active_OS_spinlock);
  /*
   Configure TZASC
   */
   ltzvisor_ddr_tzasc_config();
   ltz_imx6_scu_config();
   //imx6_init_csu();
	return ret;
}

uint32_t get_next_OS(void){
    CURRENT_OS_RUNNING  = ++NEXT_OS_RUNNING;
    if (CURRENT_OS_RUNNING >= MAX_OS_RUNNING)
      CURRENT_OS_RUNNING = 0x0;
    return CURRENT_OS_RUNNING;
}

uint32_t ltz_hw_disable(void){
  gic_enable(false);
  arm_set_interrupt_state(false);
}

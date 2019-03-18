#include <printk.h>
#include <interrupt.h>
#include <ltzvisor_hw.h>
#include <imx6_uart_driver.h>
#include <ltz_imx6_epit_driver.h>
#include <ltz_imx6_global_timer.h>
#include <ltzvisor_hw.h>
#include <ltz_led.h>

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#define YIELD(){ \
	asm volatile("ldr r0, =0xffffff1");\
	asm volatile("smc #0");\
}

#define YIELD_10(){ \
	asm volatile("ldr r0, =-31");\
	asm volatile("ldr r1, =0x10000000");\
	asm volatile("smc #0");\
}
#define YIELD_11(){ \
	asm volatile("ldr r0, =-30");\
	asm volatile("ldr r1, =0x10000000");\
	asm volatile("ldr r2, =0xfaca9999");\
	asm volatile("smc #0");\
}

#define YIELD_2(){ \
	asm volatile("ldr r0, =0x80000001");\
	asm volatile("smc #0");\
}
#define YIELD_3(){ \
	asm volatile("ldr r0, =0x80000002");\
	asm volatile("smc #0");\
}
//////////////////////////////////////////////////////////


void vClearTickInterrupt( void ){
	ltz_epit_get_compare_event(1);
}
/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.  This file contains a function
 * that is suitable for use on the Zynq SoC.
 */

void vConfigureTickInterrupt( void ) {
	ltz_epit_init(1,1000);
	int irq_id = 88;
	printk("Baremetal Timer Tick programmed2.\n");
 	register_interrupt_routine(irq_id, vClearTickInterrupt);
	enable_secure_interrupt(irq_id, 0, 0);
 	//ltz_epit_enable(1);
}

void SecureLED(void){
		static uint64_t i = 0;
		if ((i++%2) == 0){
			ltz_led(true);
		}
		else {
				ltz_led(false);
		}
}

#define reg32_read(addr)      *((volatile uint32_t *)(addr))
#define reg32_write(addr,val) *((volatile uint32_t *)(addr)) = (val)

void main(void){
 	printk("main\n");
	//reg32_write(0x10000000, 0xfacada10);
	//printk("read val: %x\n", read_smc());
//	write_smc(0x10000000, 0x9876543);
 //	printk("read val: %x\n", read_smc());
	/*while (1) ;
	printk("YIELD_1\n");
	YIELD_10();
	printk("YIELD_2\n");
	YIELD_11();
	printk("YIELD_3\n");
	YIELD_3();
 */

	vConfigureTickInterrupt();
	int i,j;
	printk("cleanup_before_linux_select\n");
	cleanup_before_linux_select();
	//printk("armv7_init_nonsec\n");
  //armv7_init_nonsec();
	//printk("_do_nonsec_entry\n");
	//_do_nonsec_entry();
	//printk("Reset_MVBAR\n");
	//Reset_MVBAR();
	//asm volatile("BKPT");
		printk("while SecureLED\n");
	while(1){
		SecureLED();
		YIELD();
	}

	ltz_hw_disable();
	printk("**************************************************\n");
	printk("***************** LTZVISOR Done ******************\n");
			while (1);
}


#include <ltzvisor_hw.h>
#include <ltzvisor_multicore_spinlock.h>
#include <ltz_led.h>
///////////////////////////////////////////////////////////
 int extern cpu_get_NS();
void extern ltz_led (bool status);
///////////////////////////////////////////////////////////



void SecureLED(int i){
		if ((i%2) == 0){
			printk("  -> Secure OS LED ON\n");
			ltz_led(true);
		}
		else {
				printk("  -> Secure OS LED OFF\n");
				ltz_led(false);
		}
}


void main(void){
	int i = 0, j = 0, k;
  printk("ARM TrustZone on i.MX6 Board.\n");
  printk("Registering NS Guest.\n");

	while(1){
		printk("++++++++++++++++++++++++++++++++++++++\n");
		printk("     Secure OS Running - Count: %d\n", i++);
		SecureLED(i);
		printk("    Switching to Non-Secure \n");
		printk("++++++++++++++++++++++++++++++++++++++\n");
		for (k = 0 ; k < 10000000; k++);
  }
	ltzvisor_hw_disable();
	printk("**************************************************\n");
	printk("********************Test Done*********************\n");
	printk("**************************************************\n");
	while(1);
	return;
}

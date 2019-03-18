#include "../inc/types.h"
#include "../inc/printk.h"
#include "../inc/ltzvisor_hw.h"
#include "../inc/ltzvisor_multicore_spinlock.h"


__attribute__ ((__section__(".ns_guest1")))
void ns_guest1_main(void){
   int i = 0, j = 0;
    while(1){
       printk("NS_Guest 1 Running \n";
      for(i = 0; i < 1000000; i++);
   }
}


__attribute__ ((__section__(".ns_guest2")))
void ns_guest2_main(void){
  int i = 0, j = 0;
  while(1){
     ltzvisor_spin_lock(&printk_spinlock, TIMEOUT_FOREVER);
      printk("NS_Guest 2 Running: %d\n", j++);
      for(i = 0; i < 1000000; i++);
      ltzvisor_spin_unlock(&printk_spinlock);


   }
}

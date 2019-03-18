#include "inc/ltzvisor_multicore_spinlock.h"

void ltzvisor_spin_init(spinlock_t * lock){
  lock->owner = LTZ_SPIN_FREE;
 }

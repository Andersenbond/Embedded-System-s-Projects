#ifndef __LTZVISOR_SPINLOCK_MULTICORE_H__
#define __LTZVISOR_SPINLOCK_MULTICORE_H__
#include "types.h"

#define  LTZ_SPIN_FREE     0xff
#define  TIMEOUT_FOREVER  0xffffffff
#define  TIMEOUT_NO_LOCK  0x00000000


typedef struct _spinlock {
		/* Lock count.*/
    uint32_t owner;
		/* Padding to make the lock consume a full cache line (32 bytes)*/
    uint32_t _cacheLineFiller[7];
} spinlock_t __attribute__ ((aligned (32)));


void ltzvisor_spin_unlock(spinlock_t * lock);

int ltzvisor_spin_lock(spinlock_t * lock, uint32_t timeout);

void ltzvisor_spin_init(spinlock_t * lock);



/*Used to access the printk function */
extern spinlock_t printk_spinlock;

#endif

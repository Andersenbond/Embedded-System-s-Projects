#include "inc/ltz_imx6_global_timer.h"
#include "inc/ltz_imx6_epit_driver.h"
#include "inc/imx6_ccm.h"

// The ARM private peripheral clock is half the CPU clock.
#define PERIPHERAL_CLOCK 					    (PLL1_CLOCK/2)
#define GLOBAL_TIMER_PRESCALER 				((PERIPHERAL_CLOCK/MHZ_DEVIDER)-1)

uint32_t microsecondTimerMultiple;

void ltz_init_global_timer(void){
     uint32_t prescaler = GLOBAL_TIMER_PRESCALER;

    // Divide down the prescaler until it fits into 8 bits. We add up the number of ticks it takes to equal a microsecond interval.
    microsecondTimerMultiple = 1;
    while (prescaler > 0xff){
        prescaler /= 2;
        microsecondTimerMultiple += 1;
    }

    // Disable the timer clearing the Enable bit.
	HW_ARM_GLOBAL_TIMER_CONTROL &= (~(0x00000001));

    // Clear counter.
	HW_ARMGLOBALTIMER_COUNTER1 = 0x00000000;
	HW_ARMGLOBALTIMER_COUNTER2 = 0x00000000;

    // Set prescaler and clear other flags.
	HW_ARM_GLOBAL_TIMER_CONTROL |= ((prescaler << 8) & 0x0000ff00);

    // Now turn on the timer.
	HW_ARM_GLOBAL_TIMER_CONTROL |= 0x00000001;
}


/**
* The function return the number of microseconds passed after the Global Timer have been started.
* The maximum number combining both registers is 9,223,372,036,854,775,807 microseconds that is
* equivalent to 292471,20867753599305 years.
*/
uint64_t ltz_get_microseconds(void){
	uint32_t last_top = HW_ARMGLOBALTIMER_COUNTER2;
	uint32_t bottom;
	uint32_t actual_top;

	while(1){
		bottom 		= HW_ARMGLOBALTIMER_COUNTER1;
		actual_top 	= HW_ARMGLOBALTIMER_COUNTER2;
		if(actual_top == last_top)
			return (((uint64_t)(actual_top << 32)) | ((uint64_t)bottom))/microsecondTimerMultiple;
		last_top = actual_top;
	}
}


void ltz_system_time_init(void){
    // Init microsecond tick counter.
    ltz_init_global_timer();
    ltz_epit_init(1,1000);
}

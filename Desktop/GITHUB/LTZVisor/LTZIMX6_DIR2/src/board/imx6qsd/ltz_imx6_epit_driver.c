#include "inc/ltz_imx6_epit_driver.h"
#include "inc/imx6_ccm.h"

#define PRE_PERIPH_CLK_MULT_SEL ((HW_CCM_CBCMR & 0x00600000) >> 21)
/* Values:
 * - 00 - derive clock from PLL2 main 528MHz clock
 * - 01 - derive clock from 396MHz PLL2 PFD (default)
 * - 10 - derive clock from 352M PFD
 * - 11 - derive clock from 198MHz clock (divided 396MHz PLL2 PFD)
*/

//AHB Default value - 132MHZ -> Reference Manual - pag 808
//#define AHB_PODF_DIV 			(((HW_CCM_CBCDR & 0x00001c00)>>10)+1)
//IPG Default value - 66MHZ  -> Reference Manual - pag 808
//#define IPG_PODF_DIV 			(((HW_CCM_CBCDR & 0x00000300)>>8)+1)
//PLL2 - System PLL (functional frequency 528 MHz) - pag 810
//Although PLL2 does have a DIV_SELECT register field, it is intended that this PLL will only be run at the default frequency of 528 MHz.
//#define CLOCK_SOURCE			(PLL2_CLOCK[PRE_PERIPH_CLK_MULT_SEL])

//#define EPIT_MAIN_CLOCK 		(CLOCK_SOURCE / AHB_PODF_DIV / IPG_PODF_DIV)

#define PRE_PERIPH_CLK_MULT_SEL ((HW_CCM_CBCMR & 0x00600000) >> 21)
/* Values:
 * - 00 - derive clock from PLL2 main 528MHz clock
 * - 01 - derive clock from 396MHz PLL2 PFD (default)
 * - 10 - derive clock from 352M PFD
 * - 11 - derive clock from 198MHz clock (divided 396MHz PLL2 PFD)
*/

//AHB Default value - 132MHZ -> Reference Manual - pag 808
#define AHB_PODF_DIV 			(((HW_CCM_CBCDR & 0x00001c00)>> 10)+1)
//IPG Default value - 66MHZ  -> Reference Manual - pag 808
#define IPG_PODF_DIV 			(((HW_CCM_CBCDR & 0x00000300)>> 0x8)+1)
//PLL2 - System PLL (functional frequency 528 MHz) - pag 810
//Although PLL2 does have a DIV_SELECT register field, it is intended that this PLL will only be run at the default frequency of 528 MHz.
#define EPIT_CLOCK_SOURCE		(PLL2_CLOCK[PRE_PERIPH_CLK_MULT_SEL])
#define EPIT_MAIN_CLOCK 		(EPIT_CLOCK_SOURCE / AHB_PODF_DIV / IPG_PODF_DIV)

//Defines to configure the EPIT DRIVER
#define CLOCK_SOURCE   		(0x01)
/* Clock source:
 * - 00 - Clock is off
 * - 01 - Peripheral clock
 * - 10 - High-frequency reference clock
 * - 11 - Low-frequency reference clock
 */
 // The desired frequency for the IPIT for the timer is 1MHz (1us per tick)
 #define CLOCK_PRESCALER  	(EPIT_MAIN_CLOCK/1000000)
 /* Prescaler values:
 * - 0x000 - Divide by 1
 * - 0x001 - Divide by 2...
 * - 0xFFF - Divide by 4096
 */
 #define RELOAD_MODE		(0x01)
 /* Reload mode:
 * 0 - When reaches zero it rolls over to 0xFFFF_FFFF (free-running mode)
 * 1 - When reaches zero it reloads from the modulus register (set-and-forget mode)
 */
 #define LOW_POWER_MODE		(0x03)
 /* Low Power mode:
  * - 0 - EPIT is disabled in wait mode and stop mode
  * - 1 - EPIT is enabled in wait mode and disabled stop mode
  * - 2 - EPIT is disabled in wait mode and enabled stop mode
  * - 3 - EPIT is enabled in wait mode and stop mode
  */
#define LOAD_VALUE 			(1000)
#define EPIT_IRQ_MODE		(0x01)
/* IRQ mode:
 *	0 - EPIT Compare interrupt disabled
 *  1 - EPIT Compare interrupt enabled
 */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



void ltz_epit_enable(int instance ){
	// Write one to the register to clear the bit in case a Compare event occurred before starting the EPIT timer
	HW_EPIT_SR(instance) |= 0x00000001;
	// set the mode when the output compare event occur: IRQ or polling
	if(EPIT_IRQ_MODE == 0)
		HW_EPIT_CR(instance) &= (~0x00000004); //disable interrupt
	if(EPIT_IRQ_MODE == 1)
		HW_EPIT_CR(instance) |= ( 0x00000004); //enable interrupt
	// Enable the counter
	HW_EPIT_CR(instance) |= 0x00000001;
}

void ltz_epit_disable(int instance){
	//Set to the max value so that it ensures that the counter couldn't reach 0 when it is disabled.
	HW_EPIT_LR(instance) = 0xFFFFFFFF;
	// disable epit the counter
	HW_EPIT_CR(instance) &= (~0x00000001);
	// Disable  the compare interrupt
	HW_EPIT_CR(instance) &= (~0x00000004);
	// Write one to the register to clear the bit in case a Compare event occurred before disabling the EPIT timer
	HW_EPIT_SR(instance) |= 0x00000001;
}

void ltz_epit_reload(int instance, uint32_t reload_value){
	// Write the new load value to the load register
    HW_EPIT_LR(instance) = reload_value;
}

void ltz_epit_init(int instance, uint32_t load_value){
	uint32_t temp_CR = 0;
	// enable the source clocks to the EPIT port
	HW_CCM_CCGR1 |= 0x00003000;
    // Reset the EPIT module
    HW_EPIT_CR(instance) 	 |= 0x00010000;
    // wait for the reset to complete
    while ((HW_EPIT_CR(instance) & 0x00010000) != 0);

    //Set the source clock for the counter
    temp_CR |= ((CLOCK_SOURCE<<24) & 0x03000000);

    // set the counter clock prescaler value according to the value defined in the header file
    temp_CR |= (((CLOCK_PRESCALER)<<(4)) & 0x0000fff0);

    // set the reload mode
	switch(RELOAD_MODE){
		case 0: temp_CR &= (~0x00000008); break; //free-running mode
		case 1: temp_CR |= ( 0x00000008); break; //set-and-forget mode
    }
    // set power mode
	switch(LOW_POWER_MODE){
		case 0: {
			temp_CR &= (~0x00080000); //disabled in wait mode
			temp_CR &= (~0x00200000); //disabled in stop mode
		}
		break;
		case 1: {
			temp_CR |= ( 0x00080000); // enabled in wait mode
			temp_CR &= (~0x00200000); //disabled in stop mode
		}
		break;
		case 2: {
			temp_CR &= (~0x00080000); //disabled in wait mode
			temp_CR |= ( 0x00200000); // enabled in stop mode
		}
		break;
		case 3: {
			temp_CR |= (0x00080000); //enabled in wait mode
			temp_CR |= (0x00200000); //enabled in stop mode
		}
		break;
		default: ;
    }
	temp_CR |= 0x00020000; // EPIT counter overwrite enabled (IOVW)
	temp_CR |= 0x00000002; // Counter starts count from load value
	//write the setting to the register
	HW_EPIT_CR(instance) = temp_CR;
    // Write the load register
    HW_EPIT_LR(instance) = load_value;
}


uint32_t ltz_epit_get_compare_event(int instance){
    uint32_t status_register;
    // get the status
    status_register = HW_EPIT_SR(instance);

    // clear it if the bit is set
    if (status_register & 0x00000001){
        HW_EPIT_SR(instance) |= 0x00000001;
    }
    // return the read value before the bit was cleared
    return status_register & 0x00000001;
}


void ltz_epit_delay_us(int instance, uint32_t micro_seconds){
    if (micro_seconds == 0) {
        return;
    }
	//Since the IOVW bit in the HW_EPIT_CR was set when initializing the Epit module, writing on the HW_EPIT_LR register results in immediate overwriting of counter value.
	ltz_epit_reload(instance, micro_seconds);
    /* enable the counter first */
	ltz_epit_enable(instance);
    /* wait for the compare event */
    while (!ltz_epit_get_compare_event(instance)) ;
    /* disable the counter to save power */
    ltz_epit_disable(instance);
}

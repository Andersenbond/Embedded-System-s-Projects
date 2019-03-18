/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <interrupt.h>
void vClearTickInterrupt( void ){
	FreeRTOS_Tick_Handler();
	ltz_epit_get_compare_event(1);
}
/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.  This file contains a function
 * that is suitable for use on the Zynq SoC.
 */

void vConfigureTickInterrupt( void ) {
	extern void FreeRTOS_Tick_Handler( void );
	const uint8_t ucRisingEdge = 3;
	ltz_epit_init(1,1000);
	int irq_id = 88;
	printk("FreeRTOS Timer Tick programmed2.\n");
 	register_interrupt_routine(irq_id, vClearTickInterrupt);
	enable_secure_interrupt(irq_id, 0, portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT);
 	ltz_epit_enable(1);
	enable_interrupt(3, 0, portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT); // non-secure SGI
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/* This is the callback function which is called by the FreeRTOS Cortex-A port
layer in response to an interrupt.  If the function is called
vApplicationFPUSafeIRQHandler() then it is called after the floating point
registers have been saved.  If the function is called vApplicationIRQHandler()
then it will be called without first having saved the FPU registers.  See
http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html for
more information */
void vApplicationFPUSafeIRQHandler( uint32_t ulICCIAR )
{
 uint32_t ulInterruptID;

	/* Re-enable interrupts. */
	__asm ( "cpsie i" );

	/* The ID of the interrupt is obtained by bitwise anding the ICCIAR value
	with 0x3FF. */
	ulInterruptID = ulICCIAR & 0x3FFUL;
	if( ulInterruptID < 164 )
	{
		/* Call the function installed in the array of installed handler functions. */
		unsigned cpu = (ulInterruptID >> 10) & 0x7;
		unsigned irq = ulInterruptID & 0x1FF;

		// Store the current interrupt number.
	  //g_vectNum[cpu] = irq;
	}
}

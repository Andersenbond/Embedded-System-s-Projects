/*
 * Copyright (c) 2011-2012, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file timer.c
 * @brief Basic timer functions
 *
 * @ingroup diag_timer
 */

#include "types.h"
#include "epit.h"
#include <imx6_ccm.h>
#include <interrupt.h>
#include "regsepit.h"

#define  g_system_timer_port  HW_EPIT1

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void hal_delay_us(uint32_t usecs)
{
  system_time_init();
    uint32_t instance = g_system_timer_port;
    if (usecs == 0) {
        return;
    }

    /* enable the counter first */
    epit_counter_enable(instance, usecs, POLLING_MODE);

    /* wait for the compare event */
    while (!epit_get_compare_event(instance)) ;

    /* disable the counter to save power */
    epit_counter_disable(instance);
}

void system_time_init(void)
{
    uint32_t freq;
    // Init microsecond tick counter.
  //  time_init_global_timer();

    /* EPIT1 is used for the delay function */
    /* Initialize the EPIT timer used for system time functions */
    /* typical IPG_CLK is in MHz, so divide it to get a reference
       clock of 1MHz => 1us per count */
    freq = EPIT_MAIN_CLOCK;
    epit_init(g_system_timer_port, CLKSRC_IPG_CLK, freq / 1000000,
              SET_AND_FORGET, 1000, WAIT_MODE_EN | STOP_MODE_EN);
}



////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

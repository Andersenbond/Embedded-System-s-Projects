/*
 * LTZVisor, a Lightweight TrustZone-assisted Hypervisor
 *
 * Copyright (c) TZVisor Project (www.tzvisor.org), 2017-
 *
 * Authors:
 *  Sandro Pinto <sandro@tzvisor.org>
 *  Jorge Pereira <jorgepereira89@gmail.com>
 *
 * This file is part of LTZVisor.
 *
 * LTZVisor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, with a special
 * exception described below.
 *
 * Linking this code statically or dynamically with other modules
 * is making a combined work based on this code. Thus, the terms
 * and conditions of the GNU General Public License V2 cover the
 * whole combination.
 *
 * As a special exception, the copyright holders of LTZVisor give
 * you permission to link LTZVisor with independent modules to
 * produce a statically linked executable, regardless of the license
 * terms of these independent modules, and to copy and distribute
 * the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and
 * conditions of the license of that module. An independent module
 * is a module which is not derived from or based on LTZVisor.
 *
 * LTZVisor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 * [ltzvisor_nsguest_config.c]
 *
 * This file contains the LTZVisor NS Guest configuration.
 *
 * (#) $id: ltzvisor_nsguest_config.c 10-10-2015 s_pinto & j_pereira $
 * (#) $id: ltzvisor_nsguest_config.c 18-09-2017 s_pinto (modified)$
*/

#include <ltzvisor_nsguest_config.h>
#include <ltz_system_reset_controller.h>
#include <ltzvisor.h>


/** Info from ltzvisor_nsguest.S */
extern void _start_secondary(void);
extern uint32_t nsguest1_bin_start, nsguest1_bin_end, ns_guest1_text;
extern uint32_t nsguest2_bin_start, nsguest2_bin_end, ns_guest2_text;

/** Config structure according to NS Guest */
struct nsguest_conf_entry Core_nsguests_config[] ={
	{
		.gce_name = "IMX6-Baremetal_1",
		.gce_id = 0,
		/* No ram  disk needed */
		.gce_trd_init = 0,
		/* Binary image size */
		.gce_bin_start = (uint32_t) &nsguest1_bin_start,
		.gce_bin_end = (uint32_t) &nsguest1_bin_end,
		/* Load address */
		.gce_bin_load = (uint32_t)&nsguest1_bin_start , //nsguest_execution_address, ///0x00100000,
	}
	,
	{
		.gce_name = "IMX6-Baremetal_2",
		.gce_id = 1,
		/* No ram  disk needed */
		.gce_trd_init = 0,
		/* Binary image size */
		.gce_bin_start = (uint32_t) &nsguest2_bin_start,
		.gce_bin_end = (uint32_t) &nsguest2_bin_end,
		/* Load address */
		.gce_bin_load = (uint32_t) &nsguest2_bin_start,// (uint32_t)&nsguest2_bin_start, //nsguest_execution_address, ///0x00100000,
	}

};

/*uint32_t get_next_OS(void){
    CURRENT_OS_RUNNING = ((++NEXT_OS_RUNNING) % (MAX_OS_RUNNING));
}*/

//extern tzmachine NS_Guests;
void ltzvisor_wake_core(int core){
    if (core > 3 || core < 1){
      printk("ERROR: Starting Secondary Core \n");
      return;
    }
  //  printk ("Vector Address: %x", vector);
   switch (core){
      case 1:{
         HW_SRC_GPR3 = ((uint32_t)&_start_secondary);
         HW_SRC_GPR4 = ((uint32_t)NS_Guests);
         HW_SRC_SCR |= SRC_CORE1_ENABLE;
         break;
     }
     case 2:{
         HW_SRC_GPR5 = ((uint32_t) & _start_secondary);
         HW_SRC_GPR6 = ((uint32_t) NS_Guests);

         HW_SRC_SCR |= SRC_CORE2_ENABLE;
         break;
     }
     case 3:{
         HW_SRC_GPR7 = ((uint32_t) & _start_secondary);
         HW_SRC_GPR8 = ((uint32_t) NS_Guests);
         HW_SRC_SCR |= SRC_CORE3_ENABLE;
         break;
    }
  }
}


/*void ltzvisor_stop_ns_guest_multicore(int core){
  if (core > 3 || core < 1){
    printk("ERROR: Stoping Secondary Core \n");
    return;
  }
  switch (core){
    case 1:{
       HW_SRC_SCR &= (~SRC_CORE1_ENABLE);
       break;
   }
   case 2:{
       HW_SRC_SCR &= (~SRC_CORE2_ENABLE);
       break;
   }
   case 3:{
       HW_SRC_SCR &= (~SRC_CORE3_ENABLE);
       break;
  }
}*/

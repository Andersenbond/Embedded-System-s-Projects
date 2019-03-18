/*
 * LTZVisor, a Lightweight TrustZone-assisted Hypervisor
 *
 * Copyright (c) TZVisor Project (www.tzvisor.org), 2017-
 *
 * Authors:
 *  Sandro Pinto <sandro@tzvisor.org>
 *  Ailton Lopes <ronnytarrafal@gmail.com>
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
 * [cpu_stack.c]
 *
 * This file contains stack allocation for all processor modes.
 *
 * (#) $id: cpu_stack.c 10-05-2015 s_pinto & j_pereira $
 * (#) $id: cpu_stack.c 19-09-2017 s_pinto (modified) $
*/

#include <cpu_stack.h>


/******************************************************************************/
/******************************************************************************/
/*                        CORE 0 Stack pointers definition                    */
/******************************************************************************/
/******************************************************************************/

/** IRQ stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _irq_stack[STACK_SIZE/4];

/** FIQ stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _fiq_stack[STACK_SIZE/4];

/** ABORT stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _abort_stack[STACK_SIZE/4];

/** UNDEF stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _undefined_stack[STACK_SIZE/4];

/** SYS stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _user_stack[(STACK_SIZE)/4];

/** MON stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _monitor_stack[STACK_SIZE/4];

/** SVC stack allocation (4-bitSTACK_SIZE alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _supervisor_stack[STACK_SIZE/4];


/******************************************************************************/
/******************************************************************************/
/*                   CORE 1 | 2 | 3 Stack pointers definition                 */
/******************************************************************************/
/******************************************************************************/

/** IRQ stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _irq_stack_secondary[STACK_SIZE_SECONDARY/4];

/** FIQ stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _fiq_stack_secondary[STACK_SIZE_SECONDARY/4];

/** ABORT stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _abort_stack_secondary[STACK_SIZE_SECONDARY/4];

/** UNDEF stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _undefined_stack_secondary[STACK_SIZE_SECONDARY/4];

/** SYS stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _user_stack_secondary[STACK_SIZE_SECONDARY/4];

/** MON stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _monitor_stack_secondary[STACK_SIZE_SECONDARY/4];

/** SVC stack allocation (4-bit alligned) */
__attribute__ ((aligned (4))) __attribute__ ((section (".bss")))
uint32_t _supervisor_stack_secondary[STACK_SIZE_SECONDARY/4];

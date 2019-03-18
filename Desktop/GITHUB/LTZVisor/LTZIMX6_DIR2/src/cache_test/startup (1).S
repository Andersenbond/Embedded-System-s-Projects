#include <cpu_defines.h>
@ ------------------------------------------------------------------------------
@ ---------- last 5 bit of the Current Program Status Register (CPSR) ----------
@                             Mode field:
@
@ b10000 = User mode       || b10001 = FIQ mode    || b10010 = IRQ mode
@ b10011 = Supervisor mode || b10111 = Abort mode  || b11011 = Undefined mode
@                          || b11111 = System mode ||
@ ----------------------------- User Mode --------------------------------------
.equ  Mode_USR, 0x10
@ ------------------------- Fast Interrupt Mode --------------------------------
.equ  Mode_FIQ, 0x11
@ --------------------------- Interrupt Mode -----------------------------------
.equ  Mode_IRQ, 0x12
@ ------------------------ -- Supervisor Mode ----------------------------------
.equ  Mode_SVC, 0x13
@ ----------------------------- Abort Mode -------------------------------------
.equ  Mode_ABT, 0x17
@ --------------------------- Undefined Mode -----------------------------------
.equ  Mode_UND, 0x1B
@ ----------------------------- System Mode ------------------------------------
.equ  Mode_SYS, 0x1F
@ ----------------------------- Monitor Mode -----------------------------------
.equ  Mode_MON, 0x16
@ ------------------------------------------------------------------------------

@------------- 6th and 7th bit of Program Status Register (CPSR) ---------------
@ ----------------------- IRQ interrupts disabled ------------------------------
.equ  I_Bit,    0x80
@ ----------------------- FIQ interrupts disabled ------------------------------
.equ  F_Bit,    0x40
@ ------------------------------------------------------------------------------


@ ------------------------------ CONSTANTS -------------------------------------
 .equ  LTZ_SPIN_FREE,        0xff

@-------------------------------------------------------------------------------

.code 32
.global _start
.global vectors
.global m_vectors
.global configure_epit_ns
.global main
.global GET_NS_WORLD_CONTEXT
.global WORLD_CONTEXT_SAVE
.global WORLD_CONTEXT_RESTORE

@-------------------------------------------------------------------------------
@ CORE 0 entry point, checks core if 0 continue the system reset if not b .
@-------------------------------------------------------------------------------
.align 4
_start:
  mrc   p15, 0, r0, c0, c0, 5
  and   r0, r0, #3
  cmp  r0, #0
	beq   ltz_reset_handler
  b .

@-------------------------------------------------------------------------------
@ ----------------------------- Reset Handler ----------------------------------
.global ltz_reset_handler
.align 4
ltz_reset_handler:
@ Configure CPU

  /* Set SCTLR */
  mrc	p15, 0, r1, c1, c0, 0			@ Read SCTLR
  bic	r1, r1, #0x10000000			@ Clear TEX bit
  bic	r1, r1, #0x00002000			@ Clear Vectors bit
  mcr	p15, 0, r1, c1, c0, 0			@ Write SCTLR
  /* Set NSACR */
  mrc	p15, 0, r1, c1, c1, 2			@ Read NSACR
  ldr	r2, =NSACR_REG_VAL
  orr	r1, r1, r2				@ Mask r1 with r2
  mcr	p15, 0, r1, c1, c1, 2			@ Write NSACR

  /* Set Auxiliary register reset value */
  mrc	p15, 0, r0, c1, c0, 1			@ Read ACTLR
  ldr  	r0, =0x00				@ Clear registers
  mcr	p15, 0, r0, c1, c0, 1			@ Write ACTLR


@ ------------------------------------------------------------------------------
@     Core 0 (Default) Reset Handler, the handler prepare the core before
@     jumping to the ltzvisor_main, it's set the Stack for the different
@     modes, installed the VBAR and MVBAR, configured the P15 registers,
@     managed the virtual memory and initialized the .bss section.
@ ------------------------------------------------------------------------------
  @                     Set the stack for the Core 0 modes
  @ ------------------------------- FIQ mode -----------------------------------
	msr	cpsr_c,#(Mode_FIQ | I_Bit | F_Bit)
	ldr	r1, =_fiq_stack
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE

  @ ----------------------------- Abort mode -----------------------------------
	msr	cpsr_c,#(Mode_ABT | I_Bit | F_Bit)
	ldr	r1,=_abort_stack
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE

  @ --------------------------- Undefined mode ---------------------------------
	msr	cpsr_c,#(Mode_UND | I_Bit | F_Bit)
	ldr	r1,=_undefined_stack
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE

  @ ---------------------------- System mode -----------------------------------
	msr	cpsr_c,#(Mode_SYS | I_Bit | F_Bit)
	ldr	r1,=_user_stack
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE

  @ --------------------------- Monitor Mode -----------------------------------
	msr	cpsr_c,#(Mode_MON | I_Bit | F_Bit)
	ldr	r1,=_monitor_stack
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE

  @ ----------------------------------------------------------------------------
  @                Set up the VBAR and MVBAR Vector tables
  @ ----------------------------------------------------------------------------
	@----------------------- Set vector table (VBAR) -----------------------------
	ldr	r0, =vectors
	mcr	p15, 0, r0, c12, c0, 0	     @ Write VBA

  @---------------------- Set vector table (MVBAR) -----------------------------
  ldr	r0, =ltz_monitor
	mcr	p15, 0, r0, c12, c0, 1	    @ Write MVBAR

  @--------------- Set the SCR FIQ bit Routing FIQ to Monitor ------------------
	mrc p15, 0, r0, c1, c1, 0				@ Read Secure Configuration Register data
	orr r0 , r0, #SCR_EA_BIT	@ Route aborts to Monitor
	//orr	r0, r0, #SCR_FIQ_BIT        @ Route FIQ to Monitor
	mcr p15, 0, r0, c1, c1, 0

  @----------------------- Configure P15 registers -----------------------------
  @bl configure_coprocessor_registers

  @---------------------- Configure Virtual Memory -----------------------------
  @bl configure_virtual_memory
	mrc 	p15, 0, r0, c1, c0, 0		@ Read SCTLR register
	bic   r0, r0, #(0x1<<12) 			@ Instruction caching disabled (I Bit)
	bic   r0, r0, #(0x1<<11) 			@ Program flow prediction disabled (Z Bit)
	bic   r0, r0, #(0x1<<2 ) 			@ Data caching disabled (C Bit)
	bic   r0, r0, #(0x1) 			@ MMU disabled (M Bit)
	mcr 	p15, 0, r0, c1, c0, 0		@ Write SCTLR register

	bl mmu_tlbs_invalidate
	
	bl iCache_invalidate
	bl dCache_invalidate

	mov   r0, #0
	mcr   p15, 0, r0, c7, c5, 6   @ Invalidate entire branch predictor array

	ldr r0, =s_main_page_table
	@mcr    p15, 0, r0, c2, c0, 0	@ Write TTBR0
	bl mmu_ttbr0_set

	ldr r0, =0x55555555
	@mcr    p15, 0, r0, c3, c0, 0	@ Write DACR
	bl mmu_domainAccess_set

	@mrc     p15, 0, r1, c1, c0, 0		@ Read SCTLR
	@orr     r1, r1, #(0x1)
	@mcr     p15, 0, r1, c1, c0, 0  		@ Write SCTLR
	bl mmu_enable

  @--------------------- Initialize the .bss section ---------------------------
  bl _initialize_bss_

  @---------------------- Branch to ltzvisor_main ------------------------------
	bl ltzvisor_main
  @---------------------In case ltzvisor_main return ---------------------------
  @                     Shouldn't reach this point
	b .
@ ------------------------------------------------------------------------------
@ ------------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

/**
 * SCR Non-Secure bit set (macro)
 *
 * @param
 * @retval
 */
.global scr_set_ns_bit
.func scr_set_ns_bit
scr_set_ns_bit:
  push 	{r0}
  mrc     p15, 0, r0, c1, c1, 0	 @ Read SCR
  orr	r0, r0, #SCR_NS_BIT	       @ Enable NS bit
  orr	r0, r0, #SCR_FIQ_BIT	     @ Enable the route of FIQ in Monitor
  mcr     p15, 0, r0, c1, c1, 0	 @ Write SCR
  pop 	{r0}
.endfunc

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
@ ------------------------------------------------------------------------------
@ ----------------------- GET Non-Secure Stack Pointer -------------------------
@ ------------------------------------------------------------------------------
/**
 * Get NS guest context's pointer (macro)
 *
 * @param
 * @retval	NS_guest context pointer
 */
 .global GET_NS_WORLD_CONTEXT
.macro GET_NS_WORLD_CONTEXT
  mrc p15, 0, r0, c13, c0, 4		@ Read PL1 only thread ID reg
  add r0, r0, #0               	@ Add offset
.endm
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
@ ------------------------------------------------------------------------------
@ ------------------------ World Context Restore Macro -------------------------
@ ------------------------------------------------------------------------------
/**
 * World context restore (macro)
 * @param	r0 = world context pointer
 * @retval
 *
 */
 .global WORLD_CONTEXT_RESTORE
.macro WORLD_CONTEXT_RESTORE
  @ -------------------- Restore registers (r0-r12, lr) ------------------------
	mov	  r4, r0
	ldmia r4!, {r0 - r3}
	push 	{r0, r1}
	mov 	r0, r4
	ldmia	r0!, {r4 - r12}
  @ --------------------- Restore Modes Banked Registers -----------------------
  @ -------------------------- Restore Monitor Mode ----------------------------
	ldmia	r0!, {r1, lr}
	msr	  spsr_cxsf, r1
  @ ------------------------- Restore Supervisor Mode --------------------------
	cps 	#Mode_SVC
	ldmia r0!, {r1, r13, lr}
	msr 	spsr_cxsf, r1
  @ -------------------------- Restore System Mode -----------------------------
	cps 	#Mode_SYS
	ldmia  r0!, {r1,r13, lr}
	msr 	spsr_cxsf, r1
  @ --------------------------- Restore Abort Mode -----------------------------
	cps 	#Mode_ABT
	ldmia	r0!, {r1, r13, lr}
	msr	  spsr_cxsf, r1
  @ ------------------------- Restore Undefined Mode ---------------------------
	cps 	#Mode_UND
	ldmia	r0!, {r1, r13, lr}
	msr	  spsr_cxsf, r1
  @ ---------------------------- Restore IRQ Mode ------------------------------
	cps 	#Mode_IRQ
	ldmia	r0!, {r1, r13, lr}
	msr	  spsr_cxsf, r1
  @ ------------------------ Get Back to Monitor Mode --------------------------
	cps 	#Mode_MON
	pop	  {r0, r1}
.endm
@ ------------------------------------------------------------------------------
@ --------------------------------- END Macro ----------------------------------
@ ------------------true------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
.global ltzvisor_start_core_guest
.global multicore_cp15_restore
.global _start_secondary
.func   _start_secondary
_start_secondary:
@ ------------------------------------------------------------------------------
@      Core 1,2 and 3 Reset Handler, the handler prepare the core before
@     jumping to the ltzvisor_main, it's set the Stack for the different
@     modes, installed the VBAR and MVBAR, configured the P15 registers,
@       managed the virtual memory and initialized the .bss section.
@ ------------------------------------------------------------------------------
  mov r2, r0

  @                Set the stack for the Core 1,2 and 3 modes
  @ ------------------------------- FIQ mode -----------------------------------
	msr	cpsr_c,#(Mode_FIQ | I_Bit | F_Bit)
	ldr	r1, =_fiq_stack_secondary
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE_SECONDARY

  @ ----------------------------- Abort mode -----------------------------------
	msr	cpsr_c,#(Mode_ABT | I_Bit | F_Bit)
	ldr	r1,=_abort_stack_secondary
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE_SECONDARY

  @ --------------------------- Undefined mode ---------------------------------
	msr	cpsr_c,#(Mode_UND | I_Bit | F_Bit)
	ldr	r1,=_undefined_stack_secondary
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE_SECONDARY

  @ ---------------------------- System mode -----------------------------------
	msr	cpsr_c,#(Mode_SVC | I_Bit | F_Bit)
	ldr	r1,=_user_stack_secondary
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE_SECONDARY

  @ --------------------------- Monitor Mode -----------------------------------
	msr	cpsr_c,#(Mode_MON | I_Bit | F_Bit)
	ldr	r1,=_monitor_stack_secondary
	add	r1, r1, r0, lsl #STACK_SIZE_SHIFT
	add	sp, r1, #STACK_SIZE_SECONDARY

  @ ----------------------------------------------------------------------------
  @ ----------------------------------------------------------------------------
  @                Set up the VBAR and MVBAR Vector tables
  @ ----------------------------------------------------------------------------
	@----------------------- Set vector table (VBAR) -----------------------------
	ldr	r0, =vectors_secondary
	mcr	p15, 0, r0, c12, c0, 0	    @ Write VBA

  @---------------------- Set vector table (MVBAR) -----------------------------
  ldr	r0, =ns_monitor
	mcr	p15, 0, r0, c12, c0, 1	    @ Write MVBAR

  @----------------------- Configure P15 registers -----------------------------
  bl configure_coprocessor_registers

  @---------------------- Configure Virtual Memory -----------------------------
	@bl configure_virtual_memory
  @---------------------Configure SCU for Secondary COREs-----------------------
  //bl scu_enable_maintenance_broadcast
  //bl scu_join_smp
  //bl scu_enable
  //msr   spsr_cxsf, #Mode_SVC | SCR_FIQ_BIT


  mcr p15, 0, r2, c13, c0, 4      @ Save Context Struct
  GET_NS_WORLD_CONTEXT
  //ldr r3,  =SHIFT_CP15
  //add r0, r3
  bl . @multicore_cp15_restore
  bl configure_epit_ns
  GET_NS_WORLD_CONTEXT
  @WORLD_CONTEXT_RESTORE


  @--------------- Set the SCR NS bit puting the CORE in NS mode ---------------
  mrc p15, 0, r0, c1, c1, 0			  @ Read Secure Configuration Register data
  orr	r0, r0, #SCR_FIQ_BIT        @ Route FIQ to Monitor
  mcr p15, 0, r0, c1, c1, 0


  @ --------------- Branch to the funtion received as argument -----------------


  bl . @ltzvisor_start_core_guest


  @------------------------- In case ns main return ----------------------------
  @                        Shouldn't reach this point
	b .
@ ------------------------------------------------------------------------------
.endfunc
@ ------------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


.global configure_virtual_memory
.func   configure_virtual_memory
configure_virtual_memory:
@------------------------ Disable MMU & L1 Caches ------------------------------
@ ------------------------------------------------------------------------------
	push {r0}
	mrc 	p15, 0, r0, c1, c0, 0		@ Read SCTLR register
	bic   r0, r0, #(0x1<<12) 			@ Instruction caching disabled (I Bit)
	bic   r0, r0, #(0x1<<11) 			@ Program flow prediction disabled (Z Bit)
	bic   r0, r0, #(0x1<<2 ) 			@ Data caching disabled (C Bit)
	bic   r0, r0, #(0x1<<0 ) 			@ MMU disabled (M Bit)
	mcr 	p15, 0, r0, c1, c0, 0		@ Write SCTLR register

	mov    r0, #0
	mcr    p15, 0, r0, c8, c7, 0	@ TLBIALL
	
	bl iCache_invalidate
	bl dCache_invalidate

	mov   r0, #0
	mcr   p15, 0, r0, c7, c5, 6   @ Invalidate entire branch predictor array

	@ldr r0, =s_main_page_table
	@mcr    p15, 0, r0, c2, c0, 0	@ Write TTBR0
	@bl mmu_ttbr0_set

	@ldr r0, =0x55555555
	@mcr    p15, 0, r0, c3, c0, 0	@ Write DACR
	@bl mmu_domainAccess_set

	@mrc     p15, 0, r1, c1, c0, 0		@ Read SCTLR
	@orr     r1, r1, #(0x1)
	@mcr     p15, 0, r1, c1, c0, 0  		@ Write SCTLR
	@bl mmu_enable
@
@--------------------- Invalidate Instruction cache ----------------------------
@

 	mov 	r0, #0
	mcr 	p15, 0, r0, c7, c5, 0 	@ Instruction Cache Invalidate All
	mov 	r0, #0
	mcr 	p15, 0, r0, c8, c3, 0		@ Invalidate entire unified TLB Inner Shareable
	pop   {r0}
	bx    lr
@ ------------------------------------------------------------------------------
@ ------------------------------- End Function ---------------------------------
  .endfunc
@ ------------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


.global configure_coprocessor_registers
.func   configure_coprocessor_registers
configure_coprocessor_registers:
@ ------------------------------------------------------------------------------
@ ---------- Sets the Non-secure access permission for coprocessors ------------
	push  {r0}
	MRC   p15, 0, r0, c1, c1, 2  @ Read NSACR data
	bic   r0, r0, #(0x1<<18)     @ Non-secure state can modify the SMP bit
	bic 	r0, r0, #(0x1<<11)		 @ Non-secure state can access the coprocessor 11
	bic 	r0, r0, #(0x1<<10)     @ Non-secure state can access the coprocessor 10
	MCR   p15, 0, r0, c1, c1, 2  @ Write NSACR data

@ ---------------------------- Set Vector Table --------------------------------
@	ldr	  r0, =vectors
@	mcr	  p15, 0, r0, c12, c0, 0 @ Write to VBAR

@-------------------------- Set Monitor Vector Table ---------------------------
@ ldr	  r0, =ltz_monitor
@	mcr	  p15, 0, r0, c12, c0, 1   @ Write to MVBAR
@	pop   {r0}
	bx    lr
@-------------------------------------------------------------------------------
@ ------------------------------- End Function ---------------------------------
	.endfunc
@ ------------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


.global _initialize_bss_
.func   _initialize_bss_
_initialize_bss_:
@ ------------------------------------------------------------------------------
	push  {r1-r3}
@ ----------------------- bss initialization (zeros)----------------------------
	ldr	 r1, =__bss_start
	ldr	 r2, =__bss_end
	mov	 r0, #0
2:
	cmp	 r1, r2
	bgt	 1f
	str	 r0,	[r1], #4
	b	   2b
1:
	pop  {r1-r3}
	bx   lr
@ ------------------------------------------------------------------------------
@ ------------------------------- End Function ---------------------------------
.endfunc
@ ------------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


 @ int spin_lock(spinlock_t * lock, uint32_t timeout)
  .global ltzvisor_spin_lock
  .func   ltzvisor_spin_lock
@ ------------------------------------------------------------------------------
ltzvisor_spin_lock:
  @ ---------------- check if the spin is currently unlocked -------------------
  ldrex   r1, [r0]
  cmp     r1, #LTZ_SPIN_FREE

  @ ---------------------- wait for an event signal ----------------------------
  wfene
  bne     ltzvisor_spin_lock
  @ ----------------------------------------------------------------------------
  @ ---------------- Read CPU ID and attempt to grab lock ----------------------
  @  Check if the write was successful and if the write failed, start over.
  @ ----------------------------------------------------------------------------
  mrc     p15, 0, r1, c0, c0, 5   @ read CPU ID
  and     r1, r1, #3
  strex   r2, r1, [r0]
  cmp     r2, #0
  bne     ltzvisor_spin_lock

  @ ---------- Ensure that accesses to shared resource have completed ----------
  @                           and return to caller
  @ ----------------------------------------------------------------------------
  dmb
  bx      lr
  @ ----------------------------- End Function ---------------------------------
  @ ----------------------------------------------------------------------------
  .endfunc
  @ ----------------------------------------------------------------------------

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


  @ void spin_unlock(spinlock_t * lock)
  .global ltzvisor_spin_unlock
  .func ltzvsior_spin_unlock
ltzvisor_spin_unlock:
  @ ----------------------------------------------------------------------------
  @   Check if the CPU unlocking the lock made it, if yes continue to unlock
  @                       if not return without success
  @ ----------------------------------------------------------------------------
  mrc     p15, 0, r1, c0, c0, 5
  and     r1, r1, #3
  ldr     r2, [r0]
  cmp     r1, r2
  movne   r0, #1
  bxne    lr
  @ ----------------------------------------------------------------------------
  @ DMB ensures that all explicit memory accesses that appear in program order
  @ before the DMB instruction are observed before any explicit memory accesses
  @        that appear in program order after the DMB instruction.
  @ ----------------------------------------------------------------------------
  dmb
  @ ----------- Unlock the spinlock by writing to the lock field ---------------
  @
  mov     r1, #LTZ_SPIN_FREE
  str     r1, [r0]
  @ ---------------------- Data Synchronization Barrier -------------------------
  @   acts as a special kind of memory barrier. No instruction in program order
  @      after this instruction executes until this instruction completes.
  @ -----------------------------------------------------------------------------
  dsb
  @ ---------- Send event to wake up other cores waiting on Spinlock------------
  @                   and return to the caller with success
  @ ----------------------------------------------------------------------------
  sev
  mov     r0, #0
  bx      lr
  @ ----------------------------------------------------------------------------
  @ ----------------------------- End Function ---------------------------------
  @ ----------------------------------------------------------------------------
  .endfunc
  @ ----------------------------------------------------------------------------



  .global cpu_get_NS
  @ int cpu_get_current(void)@
  @ get current CPU ID
  .func cpu_get_NS
cpu_get_NS:
    mrc p15, 0, r0, c1, c1, 0				@ Read Secure Configuration Register data
    BX	  lr
  .endfunc    @cpu_get_current()@



  .global call_linux
  call_linux:
    ldr r0, =0x30000000
    mov lr, r0
    bx  lr

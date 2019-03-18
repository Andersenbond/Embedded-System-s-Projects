#include <non_secure_support.h>
#include <armv7.h>
.equ Mode_MON, 0x16 @ Monitor Mode


	.align	5
/* the vector table for secure state and HYP mode */
_monitor_vectors:
	.word 0	/* reset */
	.word 0 /* undef */
	adr pc, _secure_monitor
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

.macro is_cpu_virt_capable	tmp
	mrc	p15, 0, \tmp, c0, c1, 1		@ read ID_PFR1
	and	\tmp, \tmp, #CPUID_ARM_VIRT_MASK	@ mask virtualization bits
	cmp	\tmp, #(1 << CPUID_ARM_VIRT_SHIFT)
.endm

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

.macro get_gicd_addr	addr
	get_cbar_addr	\addr
	add	\addr, \addr, #GIC_DIST_OFFSET	@ GIC dist i/f offset
.endm

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

.macro get_cbar_addr	addr
	mrc	p15, 4, \addr, c15, c0, 0	@ read CBAR
	bfc	\addr, #0, #15			@ clear reserved bits
.endm

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------


.macro get_gicc_addr	addr, tmp
	get_cbar_addr	\addr
	is_cpu_virt_capable \tmp
	movne	\tmp, #GIC_CPU_OFFSET_A9	@ GIC CPU offset for A9
	moveq	\tmp, #GIC_CPU_OFFSET_A15	@ GIC CPU offset for A15/A7
	add	\addr, \addr, \tmp
.endm

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

.global _nonsec_init
.func _nonsec_init
_nonsec_init:
 

 	mrc	p15, 0, r0, c1, c1, 2
	movw	r1, #0x3fff
	movt	r1, #0x0004
	orr	r0, r0, r1
	mcr	p15, 0, r0, c1, c1, 2		@ NSACR = all copros to non-sec


 	bx	lr

.endfunc


@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

.global _do_nonsec_entry
.func _do_nonsec_entry
_do_nonsec_entry:
	mov	r0, #0
	mov r1, #0
	mov	r2, #0
  //smc #0
	bl _secure_monitor
  bx lr
 .endfunc

@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------
@@-----------------------------------------------------------------------------

_secure_monitor:

	mov	ip, lr

	mrc	p15, 0, r5, c1, c1, 0		@ read SCR
	bic	r5, r5, #0x4a			@ clear IRQ, EA, nET bits
	orr	r5, r5, #0x31			@ enable NS, AW, FW bits

	@ FIQ preserved for secure mode
	mov	r6, #SVC_MODE			@ default mode is SVC


	mov	lr, ip
	mov	ip, #( I_BIT | A_BIT)	 @ Set A, I and F
	tst	lr, #1				         @ Check for Thumb PC
	orrne	ip, ip, #T_BIT	     @ Set T if Thumb
	orr	ip, ip, r6			       @ Slot target mode in
	msr	spsr_cxfs, ip			     @ Set full SPSR

	movs	pc, lr				       @ ERET to non-secure

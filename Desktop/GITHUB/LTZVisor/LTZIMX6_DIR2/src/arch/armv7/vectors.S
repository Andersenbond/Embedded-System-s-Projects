    .text
    .align 3
    .global _start
    .global IRQ_HDLR
    .global FIQ_HDLR
    .global ABT_HDLR
    .global SWI_HDLR
    .global im_here
    .global PREFECH_HDLR
    .global RESERVED_HDLR
    .global UNDEFINED_HDLR


    reset_exception:
        .word     _start
    undefined_exception:
        .word     UNDEFINED_HDLR
    swi_exception:
        .word     SWI_HDLR
    prefetch_abort_exception:
        .word     PREFECH_HDLR
    data_abort_exception:
        .word     ABT_HDLR
    reserved_exception:
        .word     RESERVED_HDLR
    irq_exception:
        .word     IRQ_HDLR
    fiq_exception:
        .word     FIQ_HDLR


    // chosen because Branch instruction only has a range of 32 MB relative to the pc
    .align 5
    .global vectors
vectors:
    /*ldr pc,*/ b reset_exception
    ldr pc, undefined_exception
    ldr pc, swi_exception
    ldr pc, prefetch_abort_exception
    ldr pc, data_abort_exception
    ldr pc, reserved_exception /* reserved vector */
    ldr pc, irq_exception
    ldr pc, fiq_exception
    .word 0                     // extra word in the RAM vectors



    .ns_reset_exception:
        .word     _start
    .ns_undefined_exception:
        .word     UNDEFINED_HDLR
    .ns_swi_exception:
        .word     SWI_HDLR
    .ns_prefetch_abort_exception:
        .word     PREFECH_HDLR
    .ns_data_abort_exception:
        .word     ABT_HDLR
    .ns_reserved_exception:
        .word     RESERVED_HDLR
    .ns_irq_exception:
        .word     IRQ_HDLR
    .ns_fiq_exception:
        .word     FIQ_HDLR


@.ns_guest_vector
    .global ns_vectors
    ns_vectors:
        ldr     pc, .ns_reset_exception
        ldr     pc, .ns_undefined_exception
        ldr     pc, .ns_swi_exception
        ldr     pc, .ns_prefetch_abort_exception
        ldr     pc, .ns_data_abort_exception
        ldr     pc, .ns_reserved_exception /* reserved vector */
        ldr     pc, .ns_fiq_exception
        ldr     pc, .ns_irq_exception
        .word 0                     // extra word in the RAM vectors







@-------------------------------------------------------------------------------
@-------------------------------------------------------------------------------
@-------------------------------------------------------------------------------
@-------------------------------------------------------------------------------
.global vectors_secondary
vectors_secondary:
    ldr     pc, .reset_exception_s
    ldr     pc, .undefined_exception_s
    ldr     pc, .swi_exception_s
    ldr     pc, .prefetch_abort_exception_s
    ldr     pc, .data_abort_exception_s
    ldr     pc, .reserved_exception_s /* reserved vector */
    ldr     pc, .irq_exception_s
    ldr     pc, .fiq_exception_s
    .word 0                     // extra word in the RAM vectors

.reset_exception_s:
    .word     _start_secondary
.undefined_exception_s:
    .word     UNDEFINED_HDLR
.swi_exception_s:
    .word     SWI_HDLR
.prefetch_abort_exception_s:
    .word     PREFECH_HDLR
.data_abort_exception_s:
    .word     ABT_HDLR
.reserved_exception_s:
    .word     RESERVED_HDLR
.irq_exception_s:
    .word     IRQ_HDLR
.fiq_exception_s:
    .word     FIQ_HDLR

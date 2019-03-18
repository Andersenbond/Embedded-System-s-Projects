.text
    .global _start
    .global MON_IRQ_HDLR
    .global MON_FIQ_HDLR
    .global MON_ABT_HDLR
    .global MON_SMC_HDLR
    .global MON_PREFECH_HDLR
    .global MON_RESERVED_HDLR
    .global MON_UNDEFINED_HDLR

    .align 8
    .global m_vectors
m_vectors:
    ldr     pc, .m_reset_exception
    ldr     pc, .m_undefined_exception
    ldr     pc, .m_smc_exception
    ldr     pc, .m_prefetch_abort_exception
    ldr     pc, .m_data_abort_exception
    ldr     pc, .m_reserved_exception /* reserved vector */
    ldr     pc, .m_irq_exception
    ldr     pc, .m_fiq_exception
    .word 0                     // extra word in the RAM vectors

.m_reset_exception:
    .word     _start
.m_undefined_exception:
    .word     MON_UNDEFINED_HDLR
.m_smc_exception:
    .word     MON_SMC_HDLR
.m_prefetch_abort_exception:
    .word     MON_PREFECH_HDLR
.m_data_abort_exception:
    .word     MON_ABT_HDLR
.m_reserved_exception:
    .word     MON_RESERVED_HDLR
.m_irq_exception:
    .word     MON_IRQ_HDLR
.m_fiq_exception:
    .word     MON_FIQ_HDLR


__reserved_exception:
  b __reserved_exception

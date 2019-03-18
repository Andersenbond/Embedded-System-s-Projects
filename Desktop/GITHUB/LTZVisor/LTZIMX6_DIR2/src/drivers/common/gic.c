#include <gic.h>
#include <cortex_a9.h>

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static inline gicd_t * gic_get_gicd(void);
static inline gicc_t * gic_get_gicc(void);
static inline uint32_t irq_get_register_offset(uint32_t irqID);
static inline uint32_t irq_get_bit_offset(uint32_t irqID);
static inline uint32_t irq_get_bit_mask(uint32_t irqID);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static inline gicd_t * gic_get_gicd(void){
    uint32_t base = get_arm_private_peripheral_base() + GIC_Distributor_Offset;
    return (gicd_t *)base;
}

static inline gicc_t * gic_get_gicc(void){
    uint32_t base = get_arm_private_peripheral_base() + GIC_CPUInterfalce_Offset;
    return (gicc_t *)base;
}

static inline uint32_t irq_get_register_offset(uint32_t irqID){
    return irqID / 32;
}

static inline uint32_t irq_get_bit_offset(uint32_t irqID){
    return irqID & 0x1f;
}

static inline uint32_t irq_get_bit_mask(uint32_t irqID){
    return 1 << irq_get_bit_offset(irqID);
}

void gic_enable(bool enableIt){
    gicd_t * gicd = gic_get_gicd();

    if (enableIt){
        // Enable both secure and non-secure.
        gicd->CTLR |= kBM_GICD_CTLR_EnableGrp0;
        gic_init_security_configall();
    }
    else{
        // Clear the enable bits.
        gicd->CTLR &= ~(kBM_GICD_CTLR_EnableGrp0);
    }
}

void gic_set_irq_security(uint32_t irqID, bool isSecure){
    gicd_t * gicd = gic_get_gicd();

    uint32_t reg = irq_get_register_offset(irqID);
    uint32_t mask = irq_get_bit_mask(irqID);

    uint32_t value = gicd->IGROUPRn[reg];
    if (!isSecure){
        value &= ~mask;
    }
    else{
        value |= mask;
    }
    gicd->IGROUPRn[reg] = value;
}

void gic_enable_irq(uint32_t irqID, bool isEnabled){
    gicd_t * gicd = gic_get_gicd();

    uint32_t reg = irq_get_register_offset(irqID);
    uint32_t mask = irq_get_bit_mask(irqID);

    // Select set-enable or clear-enable register based on enable flag.
    if (isEnabled){
        gicd->ISENABLERn[reg] = mask;
    }
    else{
        gicd->ICENABLERn[reg] = mask;
    }
}

void gic_set_irq_priority(uint32_t ID, uint32_t priority){
    gicd_t * gicd = gic_get_gicd();

    // Update the priority register. The priority registers are byte accessible, and the register
    // struct has the priority registers as a byte array, so we can just index directly by the
    // interrupt ID.
    gicd->IPRIORITYRn[ID] = priority & 0xff;
}

void gic_set_cpu_target(uint32_t irqID, unsigned cpuNumber, bool enableIt)
{
    // Make sure the CPU number is valid.
    //assert(cpuNumber <= 7);

    gicd_t * gicd = gic_get_gicd();
    uint8_t cpuMask = 1 << cpuNumber;

    // Like the priority registers, the target registers are byte accessible, and the register
    // struct has the them as a byte array, so we can just index directly by the
    // interrupt ID.
    if (enableIt){
        gicd->ITARGETSRn[irqID] |= (cpuMask & 0xff);
    }
    else{
        gicd->ITARGETSRn[irqID] &= ~(cpuMask & 0xff);
    }
}

void gic_send_sgi(uint32_t irqID, uint32_t target_list, uint32_t filter_list){
    gicd_t * gicd = gic_get_gicd();

    gicd->SGIR = (filter_list << kBP_GICD_SGIR_TargetListFilter)
                    | (target_list << kBP_GICD_SGIR_CPUTargetList)
                    | (irqID & 0xf);
}

void gic_cpu_enable(bool enableIt)
{
    gicc_t * gicc = gic_get_gicc();

    if (enableIt){
        gicc->CTLR |= kBM_GICC_CTLR_EnableS  |kBM_GICC_CTLR_EnableNS | kBM_GICC_CTLR_FIQEn ;
    }
    else{
        gicc->CTLR &= ~(kBM_GICC_CTLR_EnableS | kBM_GICC_CTLR_FIQEn );
    }
}

void gic_set_cpu_priority_mask(uint32_t priority){
    gicc_t * gicc = gic_get_gicc();
    gicc->PMR = priority & 0xff;
}

uint32_t gic_read_irq_ack(void){
    gicc_t * gicc = gic_get_gicc();
    return gicc->IAR;
}

void gic_write_end_of_irq(uint32_t irqID){
    gicc_t * gicc = gic_get_gicc();
    gicc->EOIR = irqID;
}

void gic_init(void){
    gicd_t * gicd = gic_get_gicd();

    // First disable the distributor.
    gic_enable(false);

    // Clear all pending interrupts.
    int i;
    for (i = 0; i < 32; ++i){
        gicd->ICPENDRn[i] = 0xffffffff;
    }

    // Set all interrupts to secure.
    for (i = 0; i < 8; ++i){
        gicd->IGROUPRn[i] = 0;
    }

    // Init the GIC CPU interface.
    gic_init_cpu();

    // Now enable the distributor.
    gic_enable(true);
}

void gic_init_cpu(void){
    // Init the GIC CPU interface.
    gic_set_cpu_priority_mask(0xff);

    // Enable preemption.
    gicc_t * gicc = gic_get_gicc();
    gicc->BPR = 0;

    // Enable signaling the CPU.
    gic_cpu_enable(true);
}

 void gic_init_security_configall(void){
    gicd_t * gicd = gic_get_gicd();
	  uint32_t num_regs;
	  /* Configure all global interrupts as NS Interrupts */
	  for(num_regs=1; num_regs < 8; num_regs++){
		gicd->IGROUPRn[num_regs] = 0xFFFFFFFF;
}

}
void Print_CPUInterface(void){
    gicc_t * gicc = gic_get_gicc();

    printk("CPU Interface Registers\n");
    printk("-----------------------------------------------------------\n");
    printk("CPU Interface Control Register: %x\n", gicc->CTLR);
    printk("Interrupt Priority Mask Register: %x\n", gicc->PMR);
    printk("Binary Point Register: %d\n", gicc->BPR);
    printk("End of Interrupt Register: %d\n", gicc->IAR);
    printk("Running Priority Register: %d\n", gicc->RPR);
    printk("Highest Priority Pending Interrupt Register: %d\n", gicc->HPPIR);
    printk("Aliased Binary Point Register: %d\n", gicc->ABPR);
    printk("CPU Interface Identification Register: %d\n", gicc->IIDR);
    printk("-----------------------------------------------------------\n");
}

void Print_CPUDestributor(void){
    gicd_t * gicc = gic_get_gicd();
    int i;
    printk("CPU Interface Registers\n");
    printk("-----------------------------------------------------------\n");
    printk("CPU Interface Control Register: %x\n", gicc->CTLR);
    printk("IIDR: %d\n", gicc->IIDR);
    for(i = 0; i < 32; i++)
      printk("ISENABLERn%d: %x\n",i, gicc->ISENABLERn[i]);
    for(i=0; i<8; i++)
    printk("IGROUPR%d: %x\n",i, gicc->IGROUPRn[i]);

    printk("-----------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

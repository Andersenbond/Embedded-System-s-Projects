#ifndef __LTZ_GIC_H__
#define __LTZ_GIC_H__
#include "types.h"
#include "printk.h"
#define 	GIC_Distributor_Offset  (0x1000)    //!< GIC distributor offset.
#define    	GIC_CPUInterfalce_Offset (0x100)     //!< GIC CPU interface offset.


//! @brief Options for sending a software generated interrupt.
//!
//! These options are used for the @a filter_list parameter of the gic_send_sgi()
//! function. They control how to select which CPUs that the interrupt is
//! sent to.
enum _gicd_sgi_filter
{
    //! Forward the interrupt to the CPU interfaces specified in the @a target_list parameter.
    kGicSgiFilter_UseTargetList = 0,

    //! Forward the interrupt to all CPU interfaces except that of the processor that requested
    //! the interrupt.
    kGicSgiFilter_AllOtherCPUs = 1,

    //! Forward the interrupt only to the CPU interface of the processor that requested the
    //! interrupt.
    kGicSgiFilter_OnlyThisCPU = 2
};


/*! @brief GIC distributor registers.
 * Uses the GICv2 register names, but does not include GICv2 registers. The
 * IPRIORITYRn and ITARGETSRn registers are byte accessible, so their types are
 * uint8_t instead of uint32_t to reflect this. These members are indexed directly
 * with the interrupt number.
 */

struct gic_distributor_registers{
    uint32_t CTLR;              //!< Distributor Control Register.
    uint32_t TYPER;             //!< Interrupt Controller Type Register.
    uint32_t IIDR;              //!< Distributor Implementer Identification Register.
    uint32_t _reserved0[29];
    uint32_t IGROUPRn[8];       //!< Interrupt Group Registers.
    uint32_t _reserved1[24];
    uint32_t ISENABLERn[32];    //!< Interrupt Set-Enable Registers.
    uint32_t ICENABLERn[32];    //!< Interrupt Clear-Enable Registers.
    uint32_t ISPENDRn[32];      //!< Interrupt Set-Pending Registers.
    uint32_t ICPENDRn[32];      //!< Interrupt Clear-Pending Registers.
    uint32_t ICDABRn[32];       //!< Active Bit Registers.
    uint32_t _reserved2[32];
    uint8_t IPRIORITYRn[255 * sizeof(uint32_t)];  //!< Interrupt Priority Registers. (Byte accessible)
    uint32_t _reserved3;
    uint8_t ITARGETSRn[255 * sizeof(uint32_t)];   //!< Interrupt Processor Targets Registers. (Byte accessible)
    uint32_t _reserved4;
    uint32_t ICFGRn[64];        //!< Interrupt Configuration Registers.
    uint32_t _reserved5[128];
    uint32_t SGIR;              //!< Software Generated Interrupt Register
};

//! @brief GIC CPU interface registers.
//!
//! Uses the GICv2 register names. Does not include GICv2 registers.

struct gic_cpu_interface_registers
{
    uint32_t CTLR;  //!< CPU Interface Control Register.
    uint32_t PMR;   //!< Interrupt Priority Mask Register.
    uint32_t BPR;   //!< Binary Point Register.
    uint32_t IAR;   //!< Interrupt Acknowledge Register.
    uint32_t EOIR;  //!< End of Interrupt Register.
    uint32_t RPR;   //!< Running Priority Register.
    uint32_t HPPIR; //!< Highest Priority Pending Interrupt Register.
    uint32_t ABPR;  //!< Aliased Binary Point Register. (only visible with a secure access)
    uint32_t _reserved[56];
    uint32_t IIDR;  //!< CPU Interface Identification Register.
};


//! @brief Bitfields constants for the GICD_SGIR register.
enum gicd_sgir_fields
{
    kBP_GICD_SGIR_TargetListFilter = 24,
    kBM_GICD_SGIR_TargetListFilter = (0x3 << kBP_GICD_SGIR_TargetListFilter),

    kBP_GICD_SGIR_CPUTargetList = 16,
    kBM_GICD_SGIR_CPUTargetList = (0xff << kBP_GICD_SGIR_CPUTargetList),

    kBP_GICD_SGIR_NSATT = 15,
    kBM_GICD_SGIR_NSATT = (1 << kBP_GICD_SGIR_NSATT),

    kBP_GICD_SGIR_SGIINTID = 0,
    kBM_GICD_SGIR_SGIINTID = 0xf
};


//! @brief Bitfields constants for the GICC_CTLR register.
enum _gicc_ctlr_fields
{
    kBP_GICC_CTLR_EnableS = 0,
    kBM_GICC_CTLR_EnableS = (1 << 0),

    kBP_GICC_CTLR_EnableNS = 1,
    kBM_GICC_CTLR_EnableNS = (1 << 1),

    kBP_GICC_CTLR_AckCtl = 2,
    kBM_GICC_CTLR_AckCtl = (1 << 2),

    kBP_GICC_CTLR_FIQEn = 3,
    kBM_GICC_CTLR_FIQEn = (1 << 3),

    kBP_GICC_CTLR_SBPR = 4,
    kBM_GICC_CTLR_SBPR = (1 << 4)
};


//! @brief Bitfields constants for the GICD_CTLR register.
enum _gicd_ctlr_fields
{
    kBM_GICD_CTLR_EnableGrp1 = (1 << 1),
    kBM_GICD_CTLR_EnableGrp0 = (1 << 0)
};


//! @brier Type for the GIC distributor registers.
typedef volatile struct gic_distributor_registers gicd_t;

//! @brier Type for the GIC CPU interface registers.
typedef volatile struct gic_cpu_interface_registers gicc_t;



//! @name Initialization
//@{
//! @brief Init interrupt handling.
//!
//! This function is intended to be called only by the primary CPU init code, so it will
//! only be called once during system bootup.
//!
//! Also inits the current CPU. You don't need to call gic_init_cpu() separately.
//!
//! @post The interrupt distributor and the current CPU interface are enabled. All interrupts
//!     that were pending are cleared, and all interrupts are made secure (group 0).
void gic_init(void);

//! @brief Init the current CPU's GIC interface.
//!
//! @post Enables the CPU interface and sets the priority mask to 255. Interrupt preemption
//!     is disabled by setting the Binary Point to a value of 7.
void gic_init_cpu(void);
//@}

//! @name GIC Interrupt Distributor Functions
//@{
//! @brief Enable or disable the GIC Distributor.
//!
//! Enables or disables the GIC distributor passing both secure (group 0) and non-secure
//! (group 1) interrupts to the CPU interfaces.
//!
//! @param enableIt Pass true to enable or false to disable.
void gic_enable(bool enableIt);

//! @brief Set the security mode for an interrupt.
//!
//! @param irqID The interrupt number.
//! @param isSecure Whether the interrupt is taken to secure mode.
void gic_set_irq_security(uint32_t irqID, bool isSecure);

//! @brief Enable or disable an interrupt.
//!
//! @param irqID The number of the interrupt to control.
//! @param isEnabled Pass true to enable or false to disable.
void gic_enable_irq(uint32_t irqID, bool isEnabled);

//! @brief Set whether a CPU will receive a particular interrupt.
//!
//! @param irqID The interrupt number.
//! @param cpuNumber The CPU number. The first CPU core is 0.
//! @param enableIt Whether to send the interrupt to the specified CPU. Pass true to enable
//!     or false to disable.
void gic_set_cpu_target(uint32_t irqID, unsigned cpuNumber, bool enableIt);

//! @brief Set an interrupt's priority.
//!
//! @param irq_id The interrupt number.
//! @param priority The priority for the interrupt. In the range of 0 through 0xff, with
//!     0 being the highest priority.
void gic_set_irq_priority(uint32_t irq_id, uint32_t priority);

//! @brief Send a software generated interrupt to a specific CPU.
//!
//! @param irq_id The interrupt number to send.
//! @param target_list Each bit indicates a CPU to which the interrupt will be forwarded.
//!     Bit 0 is CPU 0, bit 1 is CPU 1, and so on. If the value is 0, then the interrupt
//!     will not be forwarded to any CPUs. This parameter is only used if @a filter_list
//!     is set to #kGicSgiFilter_UseTargetList.
//! @param filter_list One of the enums of the #_gicd_sgi_filter enumeration. The selected
//!     option determines which CPUs the interrupt will be sent to. If the value
//!     is #kGicSgiFilter_UseTargetList, then the @a target_list parameter is used.
void gic_send_sgi(uint32_t irq_id, uint32_t target_list, uint32_t filter_list);
//@}

//! @name GIC CPU Interface Functions
//@{
//! @brief Enable or disable the interface to the GIC for the current CPU.
//!
//! @param enableIt Pass true to enable or false to disable.
void gic_cpu_enable(bool enableIt);

//! @brief Set the mask of which interrupt priorities the CPU will receive.
//!
//! @param priority The lowest priority that will be passed to the current CPU. Pass 0xff to
//!     allow all priority interrupts to signal the CPU.
void gic_set_cpu_priority_mask(uint32_t priority);

//! @brief Acknowledge starting of interrupt handling and get the interrupt number.
//!
//! Normally, this function is called at the beginning of the IRQ handler. It tells the GIC
//! that you are starting to handle an interupt, and returns the number of the interrupt you
//! need to handle. After the interrupt is handled, you should call gic_write_end_of_irq()
//! to signal that the interrupt is completely handled.
//!
//! In some cases, a spurious interrupt might happen. One possibility is if another CPU handles
//! the interrupt. When a spurious interrupt occurs, the end of the interrupt should be indicated
//! but nothing else.
//!
//! @return The number for the highest priority interrupt available for the calling CPU. If
//!     the return value is 1022 or 1023, a spurious interrupt has occurred.
uint32_t gic_read_irq_ack(void);

//! @brief Signal the end of handling an interrupt.
//!
//! @param irq_id The number of the interrupt for which handling has finished.
void gic_write_end_of_irq(uint32_t irq_id);

void Print_CPUInterface(void);

#endif

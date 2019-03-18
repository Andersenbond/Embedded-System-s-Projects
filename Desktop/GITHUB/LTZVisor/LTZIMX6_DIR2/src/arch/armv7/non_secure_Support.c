#include <gic.h>
#include <types.h>
#include <cortex_a9.h>

#define __iormb()	          dmb()
#define __iowmb()	          dmb()
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))

#define writel(v,c)	({ uint32_t __v = v; __iowmb(); __arch_putl(__v,c); __v; })
#define readl(c)	  ({ uint32_t __v = __arch_getl(c); __iormb(); __v; })

uint32_t get_gicd_base_address(){
    uint32_t base = get_arm_private_peripheral_base() + GIC_Distributor_Offset;
    return base;
}
static unsigned int read_id_pfr1(void){
	uint32_t reg;
	asm("mrc p15, 0, %0, c0, c1, 1\n" : "=r"(reg));
	return reg;
}





int armv7_init_nonsec(void)
{
	uint32_t reg;
	uint32_t itlinesnr, i;
	 uint32_t gic_dist_addr;

	/* check whether the CPU supports the security extensions */
	reg = read_id_pfr1();
	if ((reg & 0xF0) == 0) {
		printk("nonsec: Security extensions not implemented.\n");
		return -1;
	}

	/* the SCR register will be set directly in the monitor mode handler,
	 * according to the spec one should not tinker with it in secure state
	 * in SVC mode. Do not try to read it once in non-secure state,
	 * any access to it will trap.
	 */
 /*	gic_dist_addr = get_gicd_base_address();
	if (gic_dist_addr == -1)
		return -1;*/

	/* enable the GIC distributor */
	//writel(readl(gic_dist_addr + GICD_CTLR) | 0x03, gic_dist_addr + GICD_CTLR);

	/* TYPER[4:0] contains an encoded number of available interrupts */
	//itlinesnr = readl(gic_dist_addr + GICD_TYPER) & 0x1f;

	/* set all bits in the GIC group registers to one to allow access
	 * from non-secure state. The first 32 interrupts are private per
	 * CPU and will be set later when enabling the GIC for each core
	 */
	//for (i = 1; i <= itlinesnr; i++)
		//writel((unsigned)-1, gic_dist_addr + GICD_IGROUPRn + 4 * i);

	/*
	 * Relocate secure section before any cpu runs in secure ram.
	 * smp_kick_all_cpus may enable other cores and runs into secure
	 * ram, so need to relocate secure section before enabling other
	 * cores.
	 */
	//relocate_secure_section();

//#ifndef CONFIG_ARMV7_PSCI
	//smp_set_core_boot_addr((unsigned long)secure_ram_addr(_smp_pen), -1);
	//smp_kick_all_cpus();
//#endif

	/* call the non-sec switching code on this CPU also */
  _nonsec_init();
	//secure_ram_addr(_nonsec_init)();
	return 0;
}



#include <types.h>
#define L2_PL310_BASE 0x00A02000
#define PL310_AUX_CTRL_ASSOCIATIVITY_MASK	(1 << 16)

#define BM_SCTLR_I (1 << 12)   //!< Instruction cache enable
#define BM_SCTLR_C (1 << 2)    //!< Data cache enable

struct pl310_regs {
	uint32_t pl310_cache_id;
	uint32_t pl310_cache_type;
	uint32_t pad1[62];
	uint32_t pl310_ctrl;
	uint32_t pl310_aux_ctrl;
	uint32_t pl310_tag_latency_ctrl;
	uint32_t pl310_data_latency_ctrl;
	uint32_t pad2[60];
	uint32_t pl310_event_cnt_ctrl;
	uint32_t pl310_event_cnt1_cfg;
	uint32_t pl310_event_cnt0_cfg;
	uint32_t pl310_event_cnt1_val;
	uint32_t pl310_event_cnt0_val;
	uint32_t pl310_intr_mask;
	uint32_t pl310_masked_intr_stat;
	uint32_t pl310_raw_intr_stat;
	uint32_t pl310_intr_clear;
	uint32_t pad3[323];
	uint32_t pl310_cache_sync;
	uint32_t pad4[15];
	uint32_t pl310_inv_line_pa;
	uint32_t pad5[2];
	uint32_t pl310_inv_way;
	uint32_t pad6[12];
	uint32_t pl310_clean_line_pa;
	uint32_t pad7[1];
	uint32_t pl310_clean_line_idx;
	uint32_t pl310_clean_way;
	uint32_t pad8[12];
	uint32_t pl310_clean_inv_line_pa;
	uint32_t pad9[1];
	uint32_t pl310_clean_inv_line_idx;
	uint32_t pl310_clean_inv_way;
	uint32_t pad10[64];
	uint32_t pl310_lockdown_dbase;
	uint32_t pl310_lockdown_ibase;
	uint32_t pad11[190];
	uint32_t pl310_addr_filter_start;
	uint32_t pl310_addr_filter_end;
	uint32_t pad12[190];
	uint32_t pl310_test_operation;
	uint32_t pad13[3];
	uint32_t pl310_line_data;
	uint32_t pad14[7];
	uint32_t pl310_line_tag;
	uint32_t pad15[3];
	uint32_t pl310_debug_ctrl;
	uint32_t pad16[7];
	uint32_t pl310_prefetch_ctrl;
	uint32_t pad17[7];
	uint32_t pl310_power_ctrl;
};

extern void ltz_v7_flush_dcache_all(void);
extern void ltz_v7_invalidate_dcache_all(void);

 struct pl310_regs *const pl310 = (struct pl310_regs *)L2_PL310_BASE;

 void my_v7_outer_cache_disable(void){
	pl310->pl310_ctrl = 0;
}


//__________________________________________________________________________
//__________________________________________________________________________
//__________________________________________________________________________

#define ltz_isb() __asm__ __volatile__ ("" : : : "memory")
#define     isb() __asm__ __volatile__ ("" : : : "memory")
#define ltz_nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");
#define dmb()		__asm__ __volatile__ ("" : : : "memory")
#define __iowmb()	dmb()
#define __iormb()	dmb()
#define CP15DSB	asm volatile ("mcr     p15, 0, %0, c7, c10, 4" : : "r" (0))

#define dsb() CP15DSB

#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
#define ltz_writel(v,c)	({ uint32_t __v = v; __iowmb(); __arch_putl(__v,c); __v; })

#define ltz_readl(c)	({ uint32_t __v = __arch_getl(c); __iormb(); __v; })

//__________________________________________________________________________
//__________________________________________________________________________

#define CR_M	(1 << 0)	/* MMU enable				*/
#define CR_C	(1 << 2)	/* Dcache enable			*/
#define CR_I	(1 << 12)	/* Icache enable			*/
#define CONFIG_SYS_CACHELINE_SIZE 	32
#define ARMV7_DCACHE_INVAL_RANGE	1
#define ARMV7_DCACHE_CLEAN_INVAL_RANGE	2

/* CCSIDR */
#define CCSIDR_LINE_SIZE_OFFSET		0
#define CCSIDR_LINE_SIZE_MASK		0x7
#define CCSIDR_ASSOCIATIVITY_OFFSET	3
#define CCSIDR_ASSOCIATIVITY_MASK	(0x3FF << 3)
#define CCSIDR_NUM_SETS_OFFSET		13
#define CCSIDR_NUM_SETS_MASK		(0x7FFF << 13)

//__________________________________________________________________________
//__________________________________________________________________________

static void ltz_cp_delay (void){
	volatile int i;
	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < 100; i++)
		ltz_nop();
	asm volatile("" : : : "memory");
}

//__________________________________________________________________________
//__________________________________________________________________________

static inline unsigned int ltz_get_cr(void){
	unsigned int val;
	asm("mrc p15, 0, %0, c1, c0, 0	@ get CR" : "=r" (val) : : "cc");
	return val;
}

static inline void ltz_set_cr(unsigned int val){
	asm volatile("mcr p15, 0, %0, c1, c0, 0	@ set CR" : : "r" (val) : "cc");
	ltz_isb();
}




//__________________________________________________________________________

int ltz_check_cache_range(unsigned long start, unsigned long stop){
	int ok = 1;
	if (start & (CONFIG_SYS_CACHELINE_SIZE - 1))
		ok = 0;
	if (stop & (CONFIG_SYS_CACHELINE_SIZE - 1))
		ok = 0;
	if (!ok)
		printk("CACHE: Misaligned operation at range [%08lx, %08lx]\n",start, stop);

	return ok;
}
//__________________________________________________________________________

static uint32_t ltz_get_ccsidr(void){
	uint32_t ccsidr;
	/* Read current CP15 Cache Size ID Register */
	asm volatile ("mrc p15, 1, %0, c0, c0, 0" : "=r" (ccsidr));
	return ccsidr;
}

//__________________________________________________________________________
static void ltz_v7_dcache_clean_inval_range(uint32_t start, uint32_t stop, uint32_t line_len){
	uint32_t mva;
	/* Align start to cache line boundary */
	start &= ~(line_len - 1);
	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCCIMVAC - Clean & Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c14, 1" : : "r" (mva));
	}
}

//__________________________________________________________________________
static void ltz_v7_dcache_inval_range(uint32_t start, uint32_t stop, uint32_t line_len){
	uint32_t mva;

	if (!ltz_check_cache_range(start, stop))
		return;

	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCIMVAC - Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (mva));
	}
}
//__________________________________________________________________________

//__________________________________________________________________________


static void ltz_v7_dcache_maint_range(uint32_t start, uint32_t stop, uint32_t range_op){
	uint32_t line_len, ccsidr;
	ccsidr = ltz_get_ccsidr();
	line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>CCSIDR_LINE_SIZE_OFFSET) + 2;
	/* Converting from words to bytes */
	line_len += 2;
	/* converting from log2(linelen) to linelen */
	line_len = 1 << line_len;

	switch (range_op) {
	case ARMV7_DCACHE_CLEAN_INVAL_RANGE:
		ltz_v7_dcache_clean_inval_range(start, stop, line_len);
		break;
	case ARMV7_DCACHE_INVAL_RANGE:
		ltz_v7_dcache_inval_range(start, stop, line_len);
		break;
	}

	/* DSB to make sure the operation is complete */
	dsb();
}


//__________________________________________________________________________
//__________________________________________________________________________
//__________________________________________________________________________

/* Flush(clean invalidate) memory from start to stop-1 */
void ltz_v7_outer_cache_flush_range(uint32_t start, uint32_t stop){
	//struct pl310_regs *const pl310 = (struct pl310_regs *)L2_PL310_BASE;

	/* PL310 currently supports only 32 bytes cache line */
	uint32_t pa, line_size = 32;

	/*
	 * Align to the beginning of cache-line - this ensures that
	 * the first 5 bits are 0 as required by PL310 TRM
	 */
	start &= ~(line_size - 1);

	for (pa = start; pa < stop; pa = pa + line_size)
		ltz_writel(pa, &pl310->pl310_clean_inv_line_pa);

	ltz_writel(0, &pl310->pl310_cache_sync);
}

//__________________________________________________________________________

void ltz_flush_dcache_range(unsigned long start, unsigned long stop){
	ltz_check_cache_range(start, stop);
	ltz_v7_dcache_maint_range(start, stop, ARMV7_DCACHE_CLEAN_INVAL_RANGE);
	ltz_v7_outer_cache_flush_range(start, stop);
}


//__________________________________________________________________________
static void pl310_background_op_all_ways(uint32_t *op_reg){
	uint32_t assoc_16, associativity, way_mask;
	assoc_16 = ltz_readl(&pl310->pl310_aux_ctrl) &
			PL310_AUX_CTRL_ASSOCIATIVITY_MASK;
	if (assoc_16)
		associativity = 16;
	else
		associativity = 8;

	way_mask = (1 << associativity) - 1;
	/* Invalidate all ways */
	ltz_writel(way_mask, op_reg);
	/* Wait for all ways to be invalidated */
	while (ltz_readl(op_reg) && way_mask)
		;
	ltz_writel(0, &pl310->pl310_cache_sync);
}

//__________________________________________________________________________


void ltz_v7_outer_cache_flush_all(void){
	pl310_background_op_all_ways(&pl310->pl310_clean_inv_way);
}


void ltz_v7_outer_cache_inval_all(void){
	pl310_background_op_all_ways(&pl310->pl310_inv_way);
}



void ltz_flush_dcache_all(void){
	ltz_v7_flush_dcache_all();
	ltz_v7_outer_cache_flush_all();
}



//__________________________________________________________________________



//__________________________________________________________________________


static void ltz_cache_disable(uint32_t cache_bit){
	uint32_t reg;

	reg = ltz_get_cr();
	ltz_cp_delay();

	if (cache_bit == CR_C) {
		/* if cache isn;t enabled no need to disable */
		if ((reg & CR_C) != CR_C)
			return;
		/* if disabling data cache, disable mmu too */
		cache_bit |= CR_M;
	}
	reg = ltz_get_cr();
	ltz_cp_delay();
	if (cache_bit == (CR_C | CR_M)) {
		ltz_flush_dcache_all();
		ltz_set_cr(reg& ~CR_C);
		ltz_flush_dcache_all();
	}
	ltz_set_cr(reg & ~cache_bit);
}

//--------------------------------------------------------------------------

void ltz_invalidate_dcache_all(void){
	ltz_v7_invalidate_dcache_all();

	ltz_v7_outer_cache_inval_all();
}

//__________________________________________________________________________


void ltz_invalidate_icache_all(void){
	/*
	 * Invalidate all instruction caches to PoU.
	 * Also flushes branch target cache.
	 */
	asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));

	/* Invalidate entire branch predictor array */
	asm volatile ("mcr p15, 0, %0, c7, c5, 6" : : "r" (0));

	/* Full system DSB - make sure that the invalidation is complete */
	dsb();

	/* ISB - make sure the instruction stream sees it */
	isb();
}
//__________________________________________________________________________

//__________________________________________________________________________

//__________________________________________________________________________

//__________________________________________________________________________

//__________________________________________________________________________



void cleanup_before_linux_select(){
	printk("Cleanup\n");
	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * we turn off caches etc ...
		/*
		* turn off D-cache
		* dcache_disable() in turn flushes the d-cache and disables MMU
		*/
		ltz_cache_disable(CR_C); //dcache_disable();
		my_v7_outer_cache_disable();
		ltz_invalidate_dcache_all(); //invalidate_dcache_all();
		ltz_cache_disable(CR_I); //icache_disable();
		ltz_invalidate_icache_all(); //invalidate_icache_all();

		/*
		* After D-cache is flushed and before it is disabled there may
		* be some new valid entries brought into the cache. We are
		* sure that these lines are not dirty and will not affect our
		* execution. (because unwinding the call-stack and setting a
		* bit in CP15 SCTRL is all we did during this. We have not
		* pushed anything on to the stack. Neither have we affected
		* any static data) So just invalidate the entire d-cache again
		* to avoid coherency problems for kernel
		*/

}




////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////



#include <ltz_mmdc.h>




void read_mmdc_regs(void){
    printk(" HW_MMDC_MDCTL(1): %x\n", HW_MMDC_MDCTL(1));
    printk(" HW_MMDC_MDASP(1): %x\n", HW_MMDC_MDASP(1));
    printk(" HW_MMDC_MDMISC(1): %x\n", HW_MMDC_MDMISC(1));
    
    printk(" HW_MMDC_MDCTL(2): %x\n", HW_MMDC_MDCTL(2));
    printk(" HW_MMDC_MDASP(2): %x\n", HW_MMDC_MDASP(2));
    printk(" HW_MMDC_MDMISC(2): %x\n", HW_MMDC_MDMISC(2));
}

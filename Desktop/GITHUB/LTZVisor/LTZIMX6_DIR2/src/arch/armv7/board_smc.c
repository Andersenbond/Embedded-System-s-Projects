#include <types.h>
#include <gic.h>
#include <interrupt.h>


#define L2X0_ENABLE	             0x80000000
#define L2X0_DISABLE	           0x80000001
#define L2X0_SET_DEBUG	         0x80000002
#define LTZVISOR_READ_SYSCALL		 (-30)
#define LTZVISOR_WRITE_SYSCALL	 (-31)
#define PRCESSOR_ID              (-34)
#define PROCESSOR_MPIDR          (-33)
#define MMU_READ_WRITE           (-32)
#define PRCESSOR_ID2             (-35)

inline uint32_t read32( volatile void * addr ){

 return *(uint32_t*)addr;
}

inline void write32( volatile void *addr, uint32_t data ){

 *(uint32_t*)addr = data;
}

#define reg32_read(addr)      *((volatile uint32_t *)(addr))
#define reg32_write(addr,val) *((volatile uint32_t *)(addr)) = (val)


static inline uint32_t READ_PROCESSSOR_ID(void){
	uint32_t midr;
	asm volatile ("mrc	p15, 0, %[midr], c0, c0, 0"
			: [midr] "=r" (midr)
	);
	return midr;
}

static inline uint32_t READ_PROCESSOR_MPIDR(void){
	uint32_t midr;
	asm volatile ("mrc	p15, 0, %[midr], c0, c0, 5"
			: [midr] "=r" (midr)
	);
	return midr;
}

static inline uint32_t MMU_READ_WRITE_F(uint32_t val){
  uint32_t midr;

  asm volatile ("mrc	p15, 0, %[val], c1, c0, 0"
			: [val] "=r" (val)
	);

  asm volatile ("mrc	p15, 0, %[midr], c0, c0, 0"
			: [midr] "=r" (midr)
	);

  return midr;
}


uint32_t board_smc(uint32_t r0, uint32_t r1,uint32_t r2, uint32_t r3){
    printk("Board smc: %x\n", r0);
    int i = 0;
    switch(r0)
    {
         case L2X0_ENABLE :
         {
            printk("L2 Enable\n");
            while(1);
         } break;
         case L2X0_DISABLE : {
            printk("L2 Disable\n");
            while(1);

         } break;
         case L2X0_SET_DEBUG : {
            printk("L2 Set Debug\n");
            while(1);
         } break;

         case LTZVISOR_READ_SYSCALL : {
            printk("LTZVISOR_READ_SYSCALL\n");
            r0 =  (*((volatile uint32_t *)(r1)));
            //r0 = read32((volatile void*)r1);
         } break;

         case LTZVISOR_WRITE_SYSCALL : {
          //  printk("LTZVISOR_WRITE_SYSCALL\n");
            //write32( (volatile void*)r1, r2);
          //  printk("VAL: %x ---- ADDR: %x\n", r1, r2);
            (*((volatile uint32_t *)(r1))) = (r2);
         } break;

         case PRCESSOR_ID : {
           r0 = READ_PROCESSSOR_ID();
         } break;

         case PRCESSOR_ID2 : {
           r0 = READ_PROCESSSOR_ID();
         } break;

         case PROCESSOR_MPIDR: {
           r0 = READ_PROCESSOR_MPIDR();
           printk("MPDIR: %x\n",r0);
         } break;

         case MMU_READ_WRITE : {
           r0 = MMU_READ_WRITE_F(r1);
         } break;
         case 0x60000003 : {
            Print_CPUInterface();
            Print_CPUDestributor();
         } break;

         default: ;
           //while(1){
            //  for(i = 0; i< 100000; i++);
            //    printk(" KERNEL CODE HERE \n");
            //}
  }
}

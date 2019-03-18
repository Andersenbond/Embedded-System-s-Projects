#include <types.h>


#define L2X0_ENABLE	    0x80000000
#define L2X0_DISABLE	  0x80000001
#define L2X0_SET_DEBUG	0x80000002



void board_smc(int r0, int r1, int r3, int r4, int r5, int r6, int r7, int r8){
    switch(r0) {
         case: (L2X0_ENABLE){
            printk("L2 Enable\n");
         } break;
         case: (L2X0_DISABLE){
           printk("L2 Disable\n");

         } break;
         case: (L2X0_SET_DEBUG){
           printk("L2 Set Debug\n");

  //       } break;
    //     case (L2X0_ENABLE){
      //   } break;

         default:
         printk("L2 Default\n");

}

#include <cortex_a9.h>
#include <types.h>
#include <ltz_imx6_scu.h>

ltz_imx6_scu_config(void){
    SCU_ACCESS_CONTROL_SAC_WR(LTZ_LINUX_SCU_ACC_CTR_SAC_VAL);
    SCU_NON_SEC_ACCESS_CTR_WR(LTZ_LINUX_SCU_NON_SEC_CTR_VAL);
    scu_enable();
}

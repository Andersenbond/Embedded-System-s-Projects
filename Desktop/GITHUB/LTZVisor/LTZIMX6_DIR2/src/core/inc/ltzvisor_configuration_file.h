
#ifndef __LTZVISOR_CONFIGURATION_FILE_H
#define __LTZVISOR_CONFIGURATION_FILE_H

//#include <types.h>
//#define MULTI_CORE
//#define SINGLE_CORE

#define CORE_NS_GUEST                (0x0002)
#define SHIFT_CP15                   (20*0x4)
#define CONTEXT_SHIFT                (0x0100)

#define RUNNING_OS_DATA_BASE_ADDR    (0x00907000)
#define CURRENT_OS_RUNNING_BASE_ADDR (RUNNING_OS_DATA_BASE_ADDR + 0x00)
#define MAX_OS_RUNNING_BASE_ADDR     (RUNNING_OS_DATA_BASE_ADDR + 0x04)
#define NEXT_OS_RUNNING_BASE_ADDR    (RUNNING_OS_DATA_BASE_ADDR + 0x08)
#define NS_START_DONE_BASE_ADDR      (RUNNING_OS_DATA_BASE_ADDR + 0x0A)


#define CURRENT_OS_RUNNING   (*(volatile uint32_t*) CURRENT_OS_RUNNING_BASE_ADDR)
#define MAX_OS_RUNNING       (*(volatile uint32_t*) MAX_OS_RUNNING_BASE_ADDR    )
#define NEXT_OS_RUNNING      (*(volatile uint32_t*) NEXT_OS_RUNNING_BASE_ADDR   )
#define NS_START_DONE        (*(volatile uint32_t*) NS_START_DONE_BASE_ADDR     )


#endif

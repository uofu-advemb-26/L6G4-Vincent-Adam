#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "FreeRTOSConfig_examples_common.h"
#undef configGENERATE_RUN_TIME_STATS
#define configGENERATE_RUN_TIME_STATS             1
#undef configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY                  1
#define configRUN_TIME_COUNTER_TYPE               uint64_t
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  
#define configMINIMAL_STACK_SIZE                ( configSTACK_DEPTH_TYPE ) 512 
#define portGET_RUN_TIME_COUNTER_VALUE()          time_us_64()

// This example uses a common include to avoid repetition


#endif

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "pico/cyw43_arch.h"

void busy_busy(void)
{
    for (int i = 0; ; i++);
}

void busy_yield(void)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

#endif
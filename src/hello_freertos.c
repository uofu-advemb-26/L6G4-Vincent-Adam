/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include <semphr.h>
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;
SemaphoreHandle_t semaphore;

#define SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 4UL )
#define SUB_TASK_HIGH_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define SUB_TASK_MED_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define SUB_TASK_LOW_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

#define SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define SUB_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void sub_task_low(__unused void *params) {
        printf("Starting low prioiryt task... attempting to take semaphore\n");
        if (xSemaphoreTake(semaphore, (TickType_t) 100 ) == pdTRUE){
            printf("Semaphore taken in low priority task. Delaying...\n");
            vTaskDelay(1000);
            xSemaphoreGive(semaphore);
            printf("Low priroity work done\n");
        }
        else {
            printf("Low priority task unable to acquire semaphore...\n");
        }

    while (true) {
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
void sub_task_med(__unused void *params) {
    printf("med priority starving system attempt");
    while (1) {
    }
}
void sub_task_high(__unused void *params) {

    printf("High proiorty task attmepting to take sempahore...\n");
    if (xSemaphoreTake(semaphore, (TickType_t)10000 ) == pdTRUE){
        printf("Semaphore taken in high priority task. giving...\n");
        xSemaphoreGive(semaphore);
        printf("Hight priority work done and sempahore released... \n");
    }
    else{
        printf("High priority task was unable to acquire semaphore...\n");
    }
    while (1) {
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

}

void supervisor_task(__unused void *params) {
    TaskHandle_t subtask_high, subtask_med, subtask_low;

    xTaskCreate(sub_task_low, "SubThreadLow",
                SUB_TASK_STACK_SIZE, NULL, SUB_TASK_LOW_PRIORITY, &subtask_low);
    printf("Created low priority tatsk \n");

    vTaskDelay(5000); // increased delay from low to med to force low pri to take semaphore

    xTaskCreate(sub_task_med, "SubThreadMed",
                SUB_TASK_STACK_SIZE, NULL, SUB_TASK_MED_PRIORITY, &subtask_med);

    xTaskCreate(sub_task_high, "SubThreadHigh",
                SUB_TASK_STACK_SIZE, NULL, SUB_TASK_HIGH_PRIORITY, &subtask_high);

    while(1){
        taskYIELD();
    }
}

int main( void )
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);

    printf("\n\n");
    semaphore = xSemaphoreCreateMutex();
    if (!semaphore){
        printf("Error creating semaphore\n");
        return 0;
    }
    printf("Semaphore created succesfully.\n");

    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task;

    xTaskCreate(supervisor_task, "SupervisorThread",
                SUPERVISOR_TASK_STACK_SIZE, NULL, SUPERVISOR_TASK_PRIORITY, &task);
    printf("Starting scheduler\n");
    vTaskStartScheduler();
    return 0;
}

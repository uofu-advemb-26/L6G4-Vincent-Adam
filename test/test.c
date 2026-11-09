#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include <FreeRTOS.h>
#include "task.h"
#include <pico/time.h>

#include "helpers.h"

#define TEST_RUNNER_PRIORITY ( tskIDLE_PRIORITY + 5UL )

#define SUB_TASK_HIGH_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define SUB_TASK_MED_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define SUB_TASK_LOW_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

#define SUB_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void setUp(void) {}

void tearDown(void) {}

void test_variable_assignment()
{
    int x = 1;
    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
}

void test_multiplication(void)
{
    int x = 30;
    int y = 6;
    int z = x / y;
    TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
}

/*
Write tests for two threads running the following scenarios. Try to predict the runtime for each thread.

    Threads with same priority:
        Both run busy_busy.
        Both run busy_yield
        One run busy_busy one run busy_yield
    Threads with different priority.
        Both run busy_busy.
            Higher priority starts first.
            Lower priority starts first.
        Both run busy_yield.

Make sure you are setting priorities according to the priority order presented in the documentation.*/

/**
 * Model a test runner function that spins up task threads similarly to how the solution code does it
 */
void test_runner(TaskFunction_t task1_func, TaskFunction_t task2_func, uint64_t taskDelay_ms, 
        BaseType_t task1_priority, BaseType_t task2_priority, uint64_t *t1_time, uint64_t *t2_time) {
    
    TaskHandle_t task1, task2;
    TickType_t start_ticks = xTaskGetTickCount();
    uint64_t start_count = portGET_RUN_TIME_COUNTER_VALUE();

    xTaskCreate(task1_func, "Task 1",
                SUB_TASK_STACK_SIZE, NULL, task1_priority, &task1);
    vTaskDelay(taskDelay_ms/portTICK_PERIOD_MS);
    xTaskCreate(task2_func, "Task 2",
                SUB_TASK_STACK_SIZE, NULL, task2_priority, &task2);
    
    vTaskDelay(taskDelay_ms/portTICK_PERIOD_MS);

    TaskStatus_t task1_stat, task2_stat;
    vTaskGetInfo(task1, &task1_stat, pdTRUE, eInvalid);
    vTaskGetInfo(task2, &task2_stat, pdTRUE, eInvalid);

    *t1_time = task1_stat.ulRunTimeCounter;
    *t2_time = task2_stat.ulRunTimeCounter;

    printf("Task 1 time: %llu\n", *t1_time);
    printf("Task 2 time: %llu\n", *t2_time);

    vTaskDelete(task1);
    vTaskDelete(task2);
}

void both_busy_busy(void) {
    uint64_t t1_time = 0, t2_time = 0;
    test_runner(busy_busy, busy_busy, 0, SUB_TASK_LOW_PRIORITY, SUB_TASK_LOW_PRIORITY, 
        &t1_time, &t2_time);

    TEST_ASSERT(t1_time > 400000 && t1_time < 600000);
    TEST_ASSERT(t2_time > 400000 && t2_time < 600000);
}

void test_supervisor(__unused void *params) {
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(both_busy_busy);
        // RUN_TEST(test_multiplication);
        sleep_ms(5000);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    // while (1) {
    //     sleep_ms(5000); // Give time for TTY to attach.
    //     printf("Start tests\n");
    //     UNITY_BEGIN();
    //     RUN_TEST(test_variable_assignment);
    //     RUN_TEST(test_multiplication);
    //     sleep_ms(5000);
    //     UNITY_END();
    // }
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(test_supervisor, "Test manager thread", 
        configMINIMAL_STACK_SIZE, NULL, 
        TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}

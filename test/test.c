#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "loop.h"
#include <FreeRTOS.h>

#include <task.h>
#include <unity.h>
#include "loop.h"

#define DELAY 1000
#define STATE_STATUS 1

#define TEST_RUNNER_PRIORITY ( tskIDLE_PRIORITY + 5UL )
#define LEFT_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define LEFT_TASK_PRIORITY ( TEST_RUNNER_PRIORITY - 1UL )
#define RIGHT_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define RIGHT_TASK_PRIORITY ( TEST_RUNNER_PRIORITY - 1UL )

void setUp(void) {}

void tearDown(void) {}

void activity_2(void){
    int counter = 0;
    int status;
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);

    // Testing Failed Condition
    printf("Taking allocated resource and running task");
    xSemaphoreTake(semaphore, portMAX_DELAY);
    status = do_loop(semaphore, &counter, "activity_2", DELAY);
    TEST_ASSERT_NOT_EQUAL_INT(pdTRUE, status);
    TEST_ASSERT_NOT_EQUAL(counter, counter++);

    // Testing Pass condition
    counter = 0;
    printf("Releasing allocated resource and running task");
    xSemaphoreGive(semaphore);
    status = do_loop(semaphore, &counter, "activity_2", DELAY);
    TEST_ASSERT_EQUAL_INT(pdTRUE, status);
    TEST_ASSERT_EQUAL(counter, counter++);
}

void deadlock_activity(void)
{
    TaskHandle_t left_thread, right_thread;
    SemaphoreHandle_t left = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t right = xSemaphoreCreateCounting(1, 1);

    struct DeadlockArgs left_args = {left, right, 0, 0, 0};
    struct DeadlockArgs right_args = {right, left, 0, 0, 0};

    BaseType_t left_status =
        xTaskCreate(deadlock, "Left", LEFT_TASK_STACK_SIZE,
                    (void *)&left_args, LEFT_TASK_PRIORITY, &left_thread);

    BaseType_t right_status =
        xTaskCreate(deadlock, "Right", RIGHT_TASK_STACK_SIZE,
                    (void *)&right_args, RIGHT_TASK_PRIORITY, &right_thread);

    vTaskDelay(DELAY);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(left), 0);
    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(right), 0);
    TEST_ASSERT_EQUAL_INT(left_args.state_1, STATE_STATUS);
    TEST_ASSERT_EQUAL_INT(left_args.state_2, STATE_STATUS);
    TEST_ASSERT_EQUAL_INT(left_args.state_3, STATE_STATUS);
    TEST_ASSERT_EQUAL_INT(right_args.state_1, STATE_STATUS);
    TEST_ASSERT_EQUAL_INT(right_args.state_2, STATE_STATUS);
    TEST_ASSERT_EQUAL_INT(right_args.state_3, STATE_STATUS);

    vTaskDelete(left_thread);
    vTaskDelete(right_thread);
}

void test_orphaned_lock(void){
    int numerator = 4;
    int denominator = 2; 
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    int result = orphaned_lock(semaphore, 500, &numerator, &denominator);
    TEST_ASSERT_EQUAL_INT(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semaphore));

    numerator = 4;
    denominator = 3;
    
    result = orphaned_lock(semaphore, 500, &numerator, &denominator);
    TEST_ASSERT_NOT_EQUAL_INT(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore));
}

void runner_thread(__unused void *args)
{
    for (;;) {
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(activity_2);
        RUN_TEST(deadlock_activity);
        RUN_TEST(test_orphaned_lock);
        UNITY_END();
        sleep_ms(10000);
    }
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000);
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(runner_thread, "TestRunner",
                configMINIMAL_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();
	return 0;
}

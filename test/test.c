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

void setUp(void) {}

void tearDown(void) {}

void activity_2(void){
    int counter = 0;
    int status;
    SemaphoreHandle_t semaphore;
    semaphore = xSemaphoreCreateCounting(1, 1);

    // Testing Failed Condition
    xSemaphoreTake(semaphore, portMAX_DELAY);
    status = do_loop(semaphore, &counter, "activity_2", DELAY);
    TEST_ASSERT_NOT_EQUAL_INT(pdTRUE, status);
    TEST_ASSERT_NOT_EQUAL(counter, counter++);

    //Testing Pass condition
    counter = 0;
    xSemaphoreGive(semaphore);
    status = do_loop(semaphore, &counter, "activity_2", DELAY);
    TEST_ASSERT_EQUAL_INT(pdTRUE, status);
    TEST_ASSERT_EQUAL(counter, counter++);
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(activity_2);
    // RUN_TEST(test_multiplication);
    sleep_ms(5000);
    return UNITY_END();
}

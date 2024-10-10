#ifndef MY_HEADER_H
#define MY_HEADER_H

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

int do_loop(SemaphoreHandle_t semaphore,
            int *counter,
            const char *src,
            TickType_t timeout);

struct DeadlockArgs {
    SemaphoreHandle_t lock_A;
    SemaphoreHandle_t lock_B;
    int state_1;
    int state_2;
    int state_3;
};

void deadlock(void *);
int orphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *numerator, int *denominator);
int un_orphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *numerator, int *denominator);

#endif
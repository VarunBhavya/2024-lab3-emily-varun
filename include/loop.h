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


#endif
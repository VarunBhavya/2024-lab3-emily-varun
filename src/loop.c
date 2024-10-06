#include "loop.h"

// Critical Section
int do_loop(SemaphoreHandle_t semaphore,
            int *counter,
            const char *src,
            TickType_t timeout)
{ 
    // Checking for Timeout
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;  

    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", src, *counter);
        xSemaphoreGive(semaphore);
    }
    
    return pdTRUE;
}

void deadlock(void *args)
{
    struct DeadlockArgs *dargs = (struct DeadlockArgs *)args;
    dargs->state_1 = 1;
    // Locking A
    {
        dargs->state_2 = 1;
        xSemaphoreTake(dargs->lock_A, portMAX_DELAY);
        vTaskDelay(100);
        {
            // Locking B
            dargs->state_3 = 1;
            xSemaphoreTake(dargs->lock_B, portMAX_DELAY);
            vTaskDelay(100);
        }
        xSemaphoreGive(dargs->lock_B);
    }
    xSemaphoreGive(dargs->lock_A);
    vTaskSuspend(NULL);
}


int orphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *numerator, int *denominator)
{
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;

    if (*numerator % *denominator != 0){
        printf("Number not divisible");
        return 0;
    }
    printf("Number divisible");
    xSemaphoreGive(semaphore);
    return pdTRUE;
}
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
    // Initially all states are passed as 0 but as the function moves through different locking scenarious we maske it true i.e 1
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

    // We try to not clear semaphore and return 0 to create a deadlock situation if numbers are not divisible
    if (*numerator % *denominator != 0){
        printf("Number not divisible\n");
        return 0;
    }
    printf("Number divisible\n");
    xSemaphoreGive(semaphore);
    return pdTRUE;
}

int un_orphaned_lock(SemaphoreHandle_t semaphore, TickType_t timeout, int *numerator, int *denominator)
{
    if (xSemaphoreTake(semaphore, timeout) == pdFALSE)
        return pdFALSE;
    // We fixed the bug by simply adding an if-else statement and make sure the function always release the resource.
    if (*numerator % *denominator != 0){
        printf("Number not divisible\n");
    }
    else if(*numerator % *denominator == 0){
        printf("Number divisible\n");
    }
    xSemaphoreGive(semaphore);
    return pdTRUE;
}
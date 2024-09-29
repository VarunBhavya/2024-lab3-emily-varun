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
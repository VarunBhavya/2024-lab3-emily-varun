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

    // Main Thread
    if (src == "main") {
        (*counter)++;
    }

    // Side Thread
    else if (src="side"){
        *counter = (*counter) + 1;
    }

    {
        printf("hello world from %s! Count %d\n", src, *counter);
        xSemaphoreGive(semaphore);
    }
    
    return pdTRUE;
}
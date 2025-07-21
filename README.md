# cmsisv2-static-alloc

A lightweight, portable header-only utility that provides static allocation macros for [CMSIS-RTOS v2](https://arm-software.github.io/CMSIS_5/RTOS2/html/index.html).  
Suitable for embedded systems using FreeRTOS via CMSIS on STM32 and other ARM Cortex-M platforms.

## Features

- Static thread creation
- Static message queues
- Static mutexes and semaphores
- Static timers and memory pools
- Works with CMSIS-RTOS v2 (`cmsis_os2.h`)
- No dynamic memory allocation, suitable for safety-critical applications

## Requirements

- CMSIS-RTOS v2 (`cmsis_os2.h`)
- You must include `cmsis_os2.h` before including this header

## Example 

```c
#include "cmsis_os2.h"
#include "rtos_static_alloc.h"

OS_THREAD_STATIC(myTask, 256, osPriorityNormal)
void myTaskFn(void*) {
  // Task code here
}
osThreadId_t tid = osThreadNew(myTaskFn, NULL, &myTask_attributes);


OS_MUTEX_STATIC(myMutex)
osMutexId_t mid = osMutexNew(&myMutex_attributes);


OS_SEMAPHORE_STATIC(mySemaphore)
osSemaphoreId_t sid = osSemaphoreNew(5, 2, &mySemaphore_attributes);  // max 5, initial 2


typedef struct {
    uint8_t data[16];
} Block;
OS_MEMORY_POOL_STATIC(myPool, 8, Block)
osMemoryPoolId_t pid = osMemoryPoolNew(8, sizeof(Block), &myPool_attributes);

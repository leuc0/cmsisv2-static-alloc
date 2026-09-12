# cmsisv2-static-alloc

A lightweight, header-only utility that provides static allocation macros for [CMSIS-RTOS v2](https://arm-software.github.io/CMSIS_5/RTOS2/html/index.html) on FreeRTOS.
Suitable for embedded systems on STM32 and other ARM Cortex-M platforms, including builds that turn dynamic allocation off entirely.

## Features

- Static thread creation
- Static message queues
- Static mutexes and semaphores
- Static timers
- Static event flags
- Static memory pools
- Compile-time checks (stack size, queue length)
- No dynamic memory allocation, suitable for safety-critical applications, and works with `configSUPPORT_DYNAMIC_ALLOCATION` set to 0

## Requirements

- CMSIS-RTOS v2 on FreeRTOS (the macros use the FreeRTOS static-allocation types)
- `configSUPPORT_STATIC_ALLOCATION` set to 1
- The header is self-contained: it includes the CMSIS-RTOS v2 and FreeRTOS headers it needs, so it can be included on its own
- `OS_MEMORY_POOL_STATIC` additionally requires `freertos_mpool.h` (from the CMSIS-RTOS v2 FreeRTOS layer) to be included first

Each macro expands to a full set of declarations ending in a semicolon, so use it without a trailing semicolon.

## Example

```c
#include "cmsis_os2_static_alloc.h"

// Thread
OS_THREAD_STATIC(myTask, 256, osPriorityNormal)
void myTaskFn(void*) {
  // Task code here
}
osThreadId_t tid = osThreadNew(myTaskFn, NULL, &myTask_attributes);

// Message queue
OS_MESSAGE_QUEUE_STATIC(myQueue, 8, uint32_t)
osMessageQueueId_t qid = osMessageQueueNew(8, sizeof(uint32_t), &myQueue_attributes);

// Mutex
OS_MUTEX_STATIC(myMutex)
osMutexId_t mid = osMutexNew(&myMutex_attributes);

// Semaphore
OS_SEMAPHORE_STATIC(mySemaphore)
osSemaphoreId_t sid = osSemaphoreNew(5, 2, &mySemaphore_attributes);  // max 5, initial 2

// Timer
OS_TIMER_STATIC(myTimer)
void timerCallback(void *arg) {
    // Timer callback logic
}
osTimerId_t timid = osTimerNew(timerCallback, osTimerOnce, NULL, &myTimer_attributes);

// Event flags
OS_EVENT_FLAGS_STATIC(myFlags)
osEventFlagsId_t fid = osEventFlagsNew(&myFlags_attributes);

// Memory pool (requires freertos_mpool.h)
#include "freertos_mpool.h"
typedef struct {
    uint8_t data[16];
} Block;
OS_MEMORY_POOL_STATIC(myPool, 8, Block)
osMemoryPoolId_t pid = osMemoryPoolNew(8, sizeof(Block), &myPool_attributes);
```

## License

MIT

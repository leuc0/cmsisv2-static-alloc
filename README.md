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

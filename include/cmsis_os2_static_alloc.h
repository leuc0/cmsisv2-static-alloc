/**
 * @file cmsis_os2_static_alloc.h
 * @brief Static allocation helper macros for CMSIS-RTOS v2 (FreeRTOS).
 *
 * @defgroup RTOS_Static_Allocation CMSIS-RTOS v2 Static Allocation Macros
 * @ingroup RTOS
 * @{
 *
 * Convenience macros that statically allocate CMSIS-RTOS v2 resources (threads,
 * message queues, mutexes, semaphores, timers, event flags, and memory pools).
 * Each macro declares the control block and any backing storage at compile time
 * and emits a `name_attributes` struct to pass to the matching osXxxNew() call,
 * so no dynamic allocation is needed and the build can run with
 * configSUPPORT_DYNAMIC_ALLOCATION set to 0.
 *
 * The macros use the FreeRTOS static-allocation types, so this header targets
 * the FreeRTOS implementation of CMSIS-RTOS v2. It is self-contained: it pulls in
 * the CMSIS-RTOS v2 and FreeRTOS headers it needs, so it can be included on its
 * own. configSUPPORT_STATIC_ALLOCATION must be 1.
 *
 * Each macro expands to a complete set of declarations terminated by a semicolon,
 * so invoke it without a trailing semicolon, e.g. OS_THREAD_STATIC(worker, 256,
 * osPriorityNormal).
 */

#pragma once
#ifndef CMSISV2_RTOS_STATIC_ALLOC_H_
#define CMSISV2_RTOS_STATIC_ALLOC_H_

#include "cmsis_os2.h"      /* osThreadAttr_t, osPriority_t, osMessageQueueAttr_t, ... */
#include "FreeRTOS.h"       /* configMINIMAL_STACK_SIZE, static-allocation support     */
#include "task.h"           /* StackType_t, StaticTask_t                               */
#include "queue.h"          /* StaticQueue_t                                           */
#include "semphr.h"         /* StaticSemaphore_t                                       */
#include "timers.h"         /* StaticTimer_t                                           */
#include "event_groups.h"   /* StaticEventGroup_t                                      */
#include <assert.h>         /* static_assert (C11 / C++)                               */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Statically allocate a thread.
 *
 * @param name           Thread name (also used to generate identifiers).
 * @param stack_words    Stack size in words (not bytes).
 * @param priority_level Thread priority (osPriority_t).
 */
#define OS_THREAD_STATIC(name, stack_words, priority_level)                \
    static_assert((stack_words) >= configMINIMAL_STACK_SIZE,               \
                  "OS_THREAD_STATIC: stack_words is below configMINIMAL_STACK_SIZE"); \
    static StackType_t name##_stack[stack_words];                          \
    static StaticTask_t name##_cb;                                         \
    static const osThreadAttr_t name##_attributes = {                      \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb),                                                 \
        name##_stack,                                                      \
        sizeof(name##_stack),                                              \
        (osPriority_t)(priority_level),                                    \
        0,                                                                 \
        0                                                                  \
    };

/**
 * @brief Statically allocate a message queue.
 *
 * @param name        Queue name.
 * @param queue_size  Number of items in the queue.
 * @param type        Data type of each item.
 */
#define OS_MESSAGE_QUEUE_STATIC(name, queue_size, type)                    \
    static_assert((queue_size) > 0, "OS_MESSAGE_QUEUE_STATIC: queue_size must be > 0"); \
    static StaticQueue_t name##_cb;                                        \
    static uint8_t name##_queue_mem[(queue_size) * sizeof(type)];          \
    static const osMessageQueueAttr_t name##_attributes = {                \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb),                                                 \
        name##_queue_mem,                                                  \
        sizeof(name##_queue_mem)                                           \
    };

/**
 * @brief Statically allocate a mutex.
 *
 * @param name Mutex name.
 */
#define OS_MUTEX_STATIC(name)                                              \
    static StaticSemaphore_t name##_cb;                                    \
    static const osMutexAttr_t name##_attributes = {                       \
        #name,                                                             \
        osMutexPrioInherit,                                                \
        &name##_cb,                                                        \
        sizeof(name##_cb)                                                  \
    };

/**
 * @brief Statically allocate a semaphore.
 *
 * Prepares a static control block for a CMSIS-RTOS v2 semaphore. This macro does not
 * create the semaphore instance, call `osSemaphoreNew()` with the desired count values
 * and this attribute struct.
 *
 * @param name Semaphore name.
 *
 * @note You must pass `max_count` and `initial_count` explicitly to `osSemaphoreNew()`.
 *
 * @code
 *  OS_SEMAPHORE_STATIC(my_semaphore);
 *  osSemaphoreId_t sem_id = osSemaphoreNew(5, 2, &my_semaphore_attributes); // max 5, starts with 2
 * @endcode
 */
#define OS_SEMAPHORE_STATIC(name)                                          \
    static StaticSemaphore_t name##_cb;                                    \
    static const osSemaphoreAttr_t name##_attributes = {                   \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb)                                                  \
    };

/**
 * @brief Statically allocate a timer.
 *
 * @param name Timer name.
 */
#define OS_TIMER_STATIC(name)                                              \
    static StaticTimer_t name##_cb;                                        \
    static const osTimerAttr_t name##_attributes = {                       \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb)                                                  \
    };

/**
 * @brief Statically allocate an event flags object.
 *
 * @param name Event flags name.
 */
#define OS_EVENT_FLAGS_STATIC(name)                                        \
    static StaticEventGroup_t name##_cb;                                   \
    static const osEventFlagsAttr_t name##_attributes = {                  \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb)                                                  \
    };

/**
 * @brief Statically allocate a memory pool.
 *
 * @param name       Pool name.
 * @param pool_size  Number of elements in the pool.
 * @param type       Data type of each element.
 *
 * @note Requires the CMSIS-RTOS v2 FreeRTOS header `freertos_mpool.h` to be
 *       included before use (it defines MemPool_t and MEMPOOL_ARR_SIZE). The
 *       backing store is sized with MEMPOOL_ARR_SIZE, which accounts for the
 *       per-block free-list overhead rather than just pool_size * sizeof(type).
 */
#define OS_MEMORY_POOL_STATIC(name, pool_size, type)                       \
    static MemPool_t name##_cb;                                            \
    static uint8_t name##_mem[MEMPOOL_ARR_SIZE((pool_size), sizeof(type))];\
    static const osMemoryPoolAttr_t name##_attributes = {                  \
        #name,                                                             \
        0,                                                                 \
        &name##_cb,                                                        \
        sizeof(name##_cb),                                                 \
        name##_mem,                                                        \
        sizeof(name##_mem)                                                 \
    };

#ifdef __cplusplus
}
#endif

#endif /* CMSISV2_RTOS_STATIC_ALLOC_H_ */

/** @} */ // end of RTOS_Static_Allocation

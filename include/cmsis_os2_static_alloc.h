/**
 * @file rtos_static_alloc.h
 * @brief Static allocation helper macros for CMSIS-RTOS v2.
 *
 * @defgroup RTOS_Static_Allocation CMSIS-RTOS v2 Static Allocation Macros
 * @ingroup RTOS
 * @{
 *
 * Provides convenient macros to statically allocate CMSIS-RTOS v2 resources,
 * such as threads, message queues, mutexes, semaphores, timers, and memory pools.
 * These macros allow allocation of control blocks and memory buffers at compile time,
 * avoiding dynamic memory usage.
 */

#pragma once
#ifndef CMSISV2_RTOS_STATIC_ALLOC_H_
#define CMSISV2_RTOS_STATIC_ALLOC_H_

#ifndef CMSIS_OS2_H_
#warning "cmsis_os2.h must be included before this file"
#endif

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
 * create the semaphore instance — call `osSemaphoreNew()` with the desired count values
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
 * @brief Statically allocate a memory pool.
 *
 * @param name       Pool name.
 * @param pool_size  Number of elements in the pool.
 * @param type       Data type of each element.
 */
#define OS_MEMORY_POOL_STATIC(name, pool_size, type)                       \
    static StaticMemoryPool_t name##_cb;                                   \
    static uint8_t name##_mem[(pool_size) * sizeof(type)];                 \
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
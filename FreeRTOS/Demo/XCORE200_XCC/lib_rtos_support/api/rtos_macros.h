// Copyright (c) 2019, XMOS Ltd, All rights reserved

#ifndef RTOS_MACROS_H_
#define RTOS_MACROS_H_

#include "rtos_support_rtos_config.h"
#include "xcore_c.h"

/*
 * Inserts a compile time memory barrier
 */
#define RTOS_MEMORY_BARRIER() asm volatile( "" ::: "memory" )

/*
 * Returns the number of 32-bit stack words required by the given thread entry function.
 *
 * This will not just "work" if there is any recursion or function pointers in the thread.
 * For function pointers the fptrgroup attribute may be used. For recursive functions, the
 * "stackfunction" #pragma may be used.
 *
 * Example FreeRTOS usage:
 * xTaskCreate( vTask, "task_name", RTOS_THREAD_STACK_SIZE(vTask), pvParameters, uxPriority, &pxTaskHandle );
 */
#define RTOS_THREAD_STACK_SIZE(thread_entry) \
    ({ \
        uint32_t stack_size; \
        asm volatile ( \
                "ldc %0, " _XCORE_C_STR(thread_entry) ".nstackwords + " _XCORE_C_STR(RTOS_SUPPORT_INTERRUPT_STACK_GROWTH) "\n\t" \
                : "=r"(stack_size) /* output 0 is stack_size */ \
                : /* there are no inputs */ \
                : /* nothing gets clobbered */ \
        ); \
        stack_size; \
    })

#endif /* RTOS_MACROS_H_ */

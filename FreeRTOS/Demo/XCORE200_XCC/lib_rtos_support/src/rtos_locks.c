// Copyright (c) 2019, XMOS Ltd, All rights reserved

#include "rtos_support.h"
#include "xassert.h"

lock_t rtos_locks[RTOS_LOCK_COUNT] = {
#if RTOS_LOCK_COUNT >= 1
        -1,
#endif
#if RTOS_LOCK_COUNT >= 2
        -1,
#endif
#if RTOS_LOCK_COUNT >= 3
        -1,
#endif
#if RTOS_LOCK_COUNT >= 4
        -1
#endif
};

int rtos_lock_counters[RTOS_LOCK_COUNT] = {0};

void rtos_locks_initialize(void)
{
    int i;

    for (i = 0; i < RTOS_LOCK_COUNT; i++) {
        lock_alloc(&rtos_locks[i]);
        xassert(rtos_locks[i] != 0);
    }
}

<<<<<<< HEAD
// Copyright (c) 2019, XMOS Ltd, All rights reserved

#include "rtos_support.h"
#include "xassert.h"
=======
// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include "rtos_support.h"
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

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
<<<<<<< HEAD
        lock_alloc(&rtos_locks[i]);
        xassert(rtos_locks[i] != 0);
    }
}
=======
        rtos_locks[i] = lock_alloc();
        xassert(rtos_locks[i] != 0);
    }
}

/*
 * Ensure that these normally inline functions exist
 * when compiler optimizations are disabled.
 */
extern inline int rtos_lock_acquire(int lock_id);
extern inline int rtos_lock_release(int lock_id);
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

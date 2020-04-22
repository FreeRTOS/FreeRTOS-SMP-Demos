// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_lock_impl.h"
extern lock_t _lock_alloc(void);

#include "xcore_c_lock.h"
extern xcore_c_error_t lock_alloc(lock_t *l);
extern xcore_c_error_t lock_free(lock_t *l);
extern xcore_c_error_t lock_acquire(lock_t l);
extern xcore_c_error_t lock_release(lock_t l);

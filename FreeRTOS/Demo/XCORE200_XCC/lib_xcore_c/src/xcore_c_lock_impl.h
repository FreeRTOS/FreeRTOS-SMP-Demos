// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_lock_impl_h__
#define __xcore_c_lock_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_resource_impl.h"
#include <xs1.h>

/**
 * lock is an opaque type that denotes a hardware lock which provide a mutex function.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned lock_t;

inline lock_t _lock_alloc(void)
{
  lock_t l;
  _RESOURCE_ALLOC(l, XS1_RES_TYPE_LOCK);
  return l;
}

#endif // !defined(__XC__)

#endif // __xcore_c_lock_impl_h__

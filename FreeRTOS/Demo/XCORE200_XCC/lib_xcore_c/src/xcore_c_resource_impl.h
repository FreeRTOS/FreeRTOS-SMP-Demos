// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_resource_impl_h__
#define __xcore_c_resource_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xassert.h"
#include "xcore_c_macros.h"

/** generic resource handle
 *
 *  This is an opaque base of the types 'chanend', 'port' and 'timer'.
 *  It is used to form a list to pass into select_wait_ordered()
 *  and select_no_wait_ordered().
 *
 *  Users must not access its raw underlying type.
 */
typedef uint32_t resource_t;

typedef void(*callback_function)(void);

/** wrapped select callback function
 *
 *  This is an opaque type returned by the SELECT_CALLBACK() macro.
 *
 *  Users must not access its raw underlying type.
 */
typedef callback_function select_callback_t;

/** wrapped interrupt callback function
 *
 *  This is an opaque type returned by the INTERRUPT_CALLBACK() macro.
 *
 *  Users must not access its raw underlying type.
 */
typedef callback_function interrupt_callback_t;

extern void _select_non_callback(void);  // Implemented in xcore_c_select.S

inline void _resource_enable_trigger(resource_t r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

inline void _resource_disable_trigger(resource_t r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

extern void _resource_setup_callback(resource_t r, void *data, callback_function func, uint32_t type);

inline void _resource_setup_interrupt_callback(resource_t r, void *data, interrupt_callback_t intrpt)
{
  _resource_setup_callback(r, data, intrpt, 0xA);  // Raise interrupts instead of events
}

inline void _resource_setup_select_callback(resource_t r, void *data, select_callback_t callback)
{
  _resource_setup_callback(r, data, callback, 0x2);  // Raise events instead of interrupts
}

inline void _resource_setup_select(resource_t r, uint32_t value)
{
  _resource_setup_select_callback(r, (void*)value, _select_non_callback);
}

#define _RESOURCE_ALLOC(res, id) asm volatile( "getr %0, " _XCORE_C_STR(id) : "=r" (res))

inline void _resource_free(resource_t r)
{
  asm volatile("freer res[%0]" :: "r" (r));
}

#define _RESOURCE_SETCI(res, c) asm volatile( "setc res[%0], " _XCORE_C_STR(c) :: "r" (res))

#endif // !defined(__XC__)

#endif // __xcore_c_resource_impl_h__

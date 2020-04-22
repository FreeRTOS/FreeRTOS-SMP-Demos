// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_hwtimer_impl_h__
#define __xcore_c_hwtimer_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <hwtimer.h>
#ifdef __DOXYGEN__
// Copy typedefs from hwtimer.h for use by doxygen
/**
 * hwtimer_t is an opaque type.
 *
 *  The hwtimer_t type can be used just like the timer type. It gives a unique
 *  hardware timer to use (as opposed to the default timer in xC which is
 *  allocated based on a shared hardware timer per logical core).
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned hwtimer_t;
#endif
#include "xcore_c_resource_impl.h"
#include <xs1.h>

inline void _hwtimer_realloc_xc_timer(void)
{
  // __init_threadlocal_timer has resource ID in r2 and it may be zero.
  // Implement a checked version here instead.
  unsigned tmr, addr;
  _RESOURCE_ALLOC(tmr, XS1_RES_TYPE_TIMER);
  asm volatile( "ecallf %0" :: "r" (tmr));
  asm volatile( "ldaw %0, dp[__timers]" : "=r" (addr));
  asm volatile( "get r11, id" ::: /* clobbers */ "r11");
  asm volatile( "stw  %0, %1[r11]" : : "r" (tmr), "r" (addr));
}

extern void __free_threadlocal_timer(void);
inline void _hwtimer_free_xc_timer(void)
{
  __free_threadlocal_timer();
}

inline hwtimer_t _hwtimer_alloc(void)
{
  hwtimer_t t;
  _RESOURCE_ALLOC(t, XS1_RES_TYPE_TIMER);
  return t;
}

inline void _hwtimer_free(hwtimer_t t)
{
  _resource_free((resource_t)t);
}

inline void _hwtimer_get_time(hwtimer_t t, uint32_t *now)
{
  asm volatile("in %0, res[%1]" : "=r" (*now): "r" (t));
}

inline void _hwtimer_change_trigger_time(hwtimer_t t, uint32_t time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

inline void _hwtimer_set_trigger_time(hwtimer_t t, uint32_t time)
{
  _RESOURCE_SETCI(t, XS1_SETC_COND_AFTER);
  _hwtimer_change_trigger_time(t, time);
}

inline void _hwtimer_clear_trigger_time(hwtimer_t t)
{
  _RESOURCE_SETCI(t, XS1_SETC_COND_NONE);
  // hwtimer_get_time() will respond immediately
}

#endif // !defined(__XC__)

#endif // __xcore_c_hwtimer_impl_h__

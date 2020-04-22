// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chan_impl_h__
#define __xcore_c_chan_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xccompat.h>
#ifdef __DOXYGEN__
// Copy typedefs from xccompat.h for use by doxygen
/** Opaque type for use in C/C++ code.
 *
 * It enables a xC function prototyped as taking a parameter of type chanend to
 * be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned chanend;

/** Opaque type for use in C/C++ code.
 *
 * It enables a xC function prototyped as taking a parameter of type
 * streaming_chanend_t to be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned streaming_chanend_t;
#endif
#include <xs1.h>
#include "xcore_c_resource_impl.h"

/** An opaque type for handling transactions
 *
 *  Users must not access its raw underlying type.
 */
typedef struct transacting_chanend_t {
#ifndef __DOXYGEN__
  streaming_chanend_t c;
  unsigned last_out;
#endif // __DOXYGEN__
} transacting_chanend_t;

inline streaming_chanend_t _s_chanend_alloc(void)
{
  streaming_chanend_t c;
  _RESOURCE_ALLOC(c, XS1_RES_TYPE_CHANEND);
  return c;
}

inline void _s_chanend_free(streaming_chanend_t c)
{
  _resource_free((resource_t)c);
}

inline void _s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}
#endif // !defined(__XC__)

inline void _s_chan_out_word(streaming_chanend_t c, uint32_t data)
{
  asm volatile("out res[%0], %1" :: "r" (c), "r" (data));
}

inline void _s_chan_out_byte(streaming_chanend_t c, uint8_t data)
{
  asm volatile("outt res[%0], %1" :: "r" (c), "r" (data));
}

inline uint32_t _s_chan_in_word(streaming_chanend_t c)
{
  uint32_t data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

inline uint8_t _s_chan_in_byte(streaming_chanend_t c)
{
  uint8_t data;
  asm volatile("int %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

inline void _s_chan_out_ct(streaming_chanend_t c, uint8_t ct)
{
  asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct));
}

inline void _s_chan_out_ct_end(streaming_chanend_t c)
{
  asm volatile("outct res[%0], " _XCORE_C_STR(XS1_CT_END) :: "r" (c));
}

inline void _s_chan_check_ct(streaming_chanend_t c, uint8_t ct)
{
  asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct));
}

inline void _s_chan_check_ct_end(streaming_chanend_t c)
{
  asm volatile("chkct res[%0], " _XCORE_C_STR(XS1_CT_END) :: "r" (c));
}

#if !defined(__XC__) || defined(__DOXYGEN__)
// Manage direction changes.
// As specified in the Tools Development Guide, the last_out state is managed
// to control when CT_END tokens are sent or expected.
inline void _t_chan_change_to_input(transacting_chanend_t *tc)
{
  if (tc->last_out) {
    _s_chan_out_ct_end(tc->c);
    tc->last_out = 0;
  }
}
inline void _t_chan_change_to_output(transacting_chanend_t *tc)
{
  if (!tc->last_out) {
    _s_chan_check_ct_end(tc->c);
    tc->last_out = 1;
  }
}
#endif // !defined(__XC__)

#endif // __xcore_c_chan_impl_h__


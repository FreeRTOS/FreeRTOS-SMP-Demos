// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_impl_h__
#define __xcore_c_port_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include <xccompat.h>
#ifdef __DOXYGEN__
// Copy typedefs from xccompat.h for use by doxygen
/**
 * port is an opaque type for use in C/C++ code.
 * It enables a xC function prototyped as taking a parameter of type port to
 * be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned port;
#endif
#include <xs1.h>
#include "xcore_c_resource_impl.h"

inline void _port_set_transfer_width(port p, size_t width)
{
  asm volatile("settw res[%0], %1" :: "r" (p), "r" (width));
}

inline port _port_alloc(unsigned id)
{
  _RESOURCE_SETCI((resource_t)id, XS1_SETC_INUSE_ON);
  return id;
}

inline void _port_reset(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INUSE_ON);
}

inline void _port_free(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INUSE_OFF);
}

inline void _port_set_buffered(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_BUF_BUFFERS);
}

inline void _port_set_unbuffered(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_BUF_NOBUFFERS);
}

#if USE_XCORE_CLOCK_TYPE
inline void _port_set_clock(port p, clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (p), "r" (clk));
}
#endif

inline void _port_set_inout_data(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_DATAPORT);
}

inline void _port_set_out_clock(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_CLOCKPORT);
}

inline void _port_set_out_ready(port p, port ready_source)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_READYPORT);
  asm volatile("setrdy res[%0], %1" :: "r" (p), "r" (ready_source));
}

inline void _port_set_invert(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INV_INVERT);
}

inline void _port_set_no_invert(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INV_NOINVERT);
}

inline void _port_set_sample_falling_edge(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_SDELAY_SDELAY);
}

inline void _port_set_sample_rising_edge(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_SDELAY_NOSDELAY);
}

inline void _port_set_master(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_MS_MASTER);
}

inline void _port_set_slave(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_MS_SLAVE);
}

inline void _port_set_no_ready(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_NOREADY);
}

inline void _port_set_ready_strobed(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_STROBED);
}

inline void _port_set_ready_handshake(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_HANDSHAKE);
}

inline int16_t _port_get_trigger_time(port p)
{
  int16_t ts;
  asm volatile("getts %0, res[%1]" : "=r" (ts) : "r" (p));
  return ts;
}

inline void _port_set_trigger_time(port p, int16_t t)
{
  asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t));
}

inline void _port_clear_trigger_time(port p)
{
  asm volatile("clrpt res[%0]" :: "r" (p));
}

inline void _port_set_trigger_in_equal(port p, uint32_t d)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_EQ);
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (d));
}

inline void _port_set_trigger_in_not_equal(port p, uint32_t d)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_NEQ);
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (d));
}

inline void _port_clear_trigger_in(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_NONE);
}

inline uint32_t _port_peek(port p)
{
  uint32_t data;
  asm volatile("peek %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

inline void _port_out(port p, uint32_t data)
{
  asm volatile("out res[%0], %1" :: "r" (p), "r" (data));
}

inline uint32_t _port_in(port p)
{
  uint32_t data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

inline void _port_out_shift_right(port p, uint32_t *data)
{
  // We read-write data
  asm volatile("outshr res[%1], %0" : "+r" (*data) : "r" (p));
}

inline void _port_in_shift_right(port p, uint32_t *data)
{
  // We read-write data
  asm volatile("inshr %0, res[%1]" : "+r" (*data) : "r" (p));
}

inline size_t _port_endin(port p)
{
  size_t num;
  asm volatile("endin %0, res[%1]" : "=r" (num) : "r" (p));
  return num;
}

inline void _port_clear_buffer(port p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RUN_CLRBUF);
}

#endif // !defined(__XC__)

#endif // __xcore_c_port_impl_h__

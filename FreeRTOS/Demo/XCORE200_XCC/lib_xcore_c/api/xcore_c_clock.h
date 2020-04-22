// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_h__
#define __xcore_c_clock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xcore_c_clock_impl.h"
#include "xcore_c_exception_impl.h"
#include "xcore_c_resource_impl.h"
#include <xs1.h>

/** A clock block identifier
 *
 * Clock resources must be allocated by name rather than from a pool.
 */
typedef enum {
  clock_ref = XS1_CLKBLK_REF,
  clock_1 = XS1_CLKBLK_1,
  clock_2 = XS1_CLKBLK_2,
  clock_3 = XS1_CLKBLK_3,
  clock_4 = XS1_CLKBLK_4,
  clock_5 = XS1_CLKBLK_5,
} clock_id_t;


/** Allocates a clock
 *
 *  This function enables a specified clock block and returns a clock
 *  variable denoting the clock.
 *
 *  \param clk  Clock variable representing the initialised clock
 *
 *  \param id   The id of the clock to allocate
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 *  \exception  ET_LOAD_STORE         invalid *\*clk* argument.
 */
inline xcore_c_error_t clock_alloc(clock *clk, clock_id_t id)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                *clk = id; \
                                _RESOURCE_SETCI(*clk, XS1_SETC_INUSE_ON); \
                              } while (0) );
}

/** Deallocate a clock
 *
 *  \param clk  The clock to be freed
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 *  \exception  ET_LOAD_STORE         invalid *\*clk* argument.
 */
inline xcore_c_error_t clock_free(clock *clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _RESOURCE_SETCI(*clk, XS1_SETC_INUSE_OFF); \
                                *clk = 0; \
                              } while (0) );
}

/** Start a clock
 *
 *  This function starts a clock running.
 *
 *  \param clk  The clock to start running
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_start(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( _RESOURCE_SETCI(clk, XS1_SETC_RUN_STARTR) );
}

/** Stop a clock
 *
 *  This function waits until the clock is low and then pauses a clock.
 *
 *  \param clk  The clock to stop
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_stop(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( _RESOURCE_SETCI(clk, XS1_SETC_RUN_STOPR) );
}

/** Configure a clock's source to a 1-bit port
 *
 *  A clock can be a 1-bit port, the reference clock or the xCORE clock. Note
 *  that if the xCORE clock is used then a non-zero divide must be used for
 *  ports to function correctly.
 *
 *  \param clk  The clock to configure
 *
 *  \param p  The 1-bit port to set as the clock input. Attempting to set a
 *            port which is not 1-bit as the input will cause an exception.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock or port,
 *                                    or the clock is running,
 *                                    or p not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_set_source_port(clock clk, port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p)) );
}

/** Configure a clock's source to be the 100MHz reference clock
 *
 *  \param clk  The clock to configure
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_set_source_clk_ref(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_REF)) );
}

/** Configure a clock's source to be the xCORE clock.
 *
 *  *Note*: When using the xCORE clock as the clock input a divide of > 0 must
 *  be used for the ports to function correclty.
 *
 *  \param clk  The clock to configure
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_set_source_clk_xcore(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_XCORE)) );
}

/** Configure the divider for a clock.
 *
 *  A clock can divide its input signal by an integer value which this function
 *  specifies. The XS2 architecture supports dividing the signal from a 1-bit
 *  port while the XS1 architecture will raise a trap if a non-zero divide is
 *  used with a 1-bit port input.
 *
 *  If the clock has been started then this will raise an exception.
 *
 *  If the divide is 0 then the value signal will be passed through the clock.
 *  If the value is non-zero then the clock output will be divided by 2*divide.
 *
 *  \param clk    The clock to configure
 *
 *  \param divide The divide value
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_set_divide(clock clk, uint8_t divide)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setd res[%0], %1" :: "r" (clk), "r" (divide)) );
}

/** Sets a clock to use a 1-bit port for the ready-in signal.
 *
 *  If the port is not a 1-bit port then an exception is raised. The ready-in
 *  port controls when data is sampled from the pins.
 *
 *  \param clk          The clock to configure.
 *
 *  \param ready_source The 1-bit port to use for the ready-in signal.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or ready_source not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline xcore_c_error_t clock_set_ready_src(clock clk, port ready_source)
{
  RETURN_EXCEPTION_OR_ERROR( _clock_set_ready_src(clk, ready_source) );
}

#endif // !defined(__XC__)

#endif // __xcore_c_clock_h__

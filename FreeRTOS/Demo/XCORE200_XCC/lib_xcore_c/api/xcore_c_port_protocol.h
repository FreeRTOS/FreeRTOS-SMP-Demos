// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_protocol_h__
#define __xcore_c_port_protocol_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xcore_c_port_impl.h"
#include "xcore_c_clock_impl.h"
#include "xcore_c_exception_impl.h"

/** Configure a port to be a clocked input port in handshake mode.
 *
 *  If the ready-in or ready-out ports are not 1-bit ports, an exception is raised.
 *  The ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. The port samples its pins on its sampling edge
 *  when both the ready-in and ready-out ports are asserted.
 *
 *  By default the port's sampling edge is the rising edge of clock. This can be
 *  changed by the function port_set_sample_falling_edge().
 *
 *  *Note*: A handshaken port must be buffered, so this function will also make
 *          the port buffered.
 *
 *  \param p         The port to configure
 *  \param readyin   A 1-bit port to use for the ready-in signal
 *  \param readyout  A 1-bit port to use for the ready-out signal
 *  \param clk       The clock used to configure the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin/readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
 inline xcore_c_error_t
 port_protocol_in_handshake(port p, port readyin, port readyout, clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in input mode. */ \
                                (void) _port_in(p); \
                                _clock_set_ready_src(clk, readyin); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_handshake(p); \
                                _port_clear_buffer(p); \
                                _port_set_out_ready(readyout, p); \
                              } while (0) );
}

/** configures a port to be a clocked output port in handshake mode.
 *
 *  if the ready-in or ready-out ports are not 1-bit ports an exception is
 *  raised. the port drives the initial value on its pins until an
 *  output statement changes the value driven.
 *
 *  the ready-in port is read on the sampling edge of the port. outputs are driven
 *  on the next falling edge of the clock where the previous value read from the
 *  ready-in port was high.
 *
 *  on the falling edge of the clock the ready-out port is driven high
 *  if data is output on that edge, otherwise it is driven low.
 *
 *  by default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function port_set_sample_falling_edge().
 *
 *  *note*: a handshaken port must be buffered, so this function will also make
 *          the port buffered.
 *
 *  \param p        the port to configure
 *  \param readyin  a 1-bit port to use for the ready-in signal
 *  \param readyout a 1-bit port to use for the ready-out signal
 *  \param clk      the clock used to configure the port
 *  \param initial  the initial value to output on the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin/readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
inline xcore_c_error_t
port_protocol_out_handshake(port p, port readyin, port readyout, clock clk, uint32_t initial)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in output mode. */ \
                                _port_out(p, initial); \
                                _clock_set_ready_src(clk, readyin); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_handshake(p); \
                                _port_clear_buffer(p); \
                                _port_set_out_ready(readyout, p); \
                              } while (0) );
}

/** configures a port to be a clocked input port in strobed master mode.
 *
 *  if the ready-out port is not a 1-bit port, an exception is raised.
 *  the ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. the port samples its pins on its sampling edge
 *  after the ready-out port is asserted.
 *
 *  by default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p         the port to configure
 *  \param readyout  a 1-bit port to use for the ready-out signal
 *  \param clk       the clock used to configure the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
inline xcore_c_error_t
port_protocol_in_strobed_master(port p, port readyout, clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in input mode. */ \
                                (void) _port_in(p); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_strobed(p); \
                                _port_set_master(p); \
                                _port_clear_buffer(p); \
                                _port_set_out_ready(readyout, p); \
                              } while (0) );
}

/** configures a port to be a clocked output port in strobed master mode.
 *
 *  if the ready-out port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. outputs are driven on the next
 *  falling edge of the clock. on the falling edge of the clock the ready-out
 *  port is driven high if data is output on that edge, otherwise it is driven
 *  low.
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p        the port to configure
 *  \param readyout a 1-bit port to use for the ready-out signal
 *  \param clk      the clock used to configure the port
 *  \param initial  the initial value to output on the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
inline xcore_c_error_t
port_protocol_out_strobed_master(port p, port readyout, clock clk, uint32_t initial)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in output mode. */ \
                                _port_out(p, initial); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_strobed(p); \
                                _port_set_master(p); \
                                _port_clear_buffer(p); \
                                _port_set_out_ready(readyout, p); \
                              } while (0) );
}

/** configures a port to be a clocked input port in strobed slave mode.
 *
 *  if the ready-in port is not a 1-bit port, an exception is raised.
 *  the port samples its pins on its sampling edge when the ready-in signal is
 *  high. by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p       the port to configure
 *  \param readyin a 1-bit port to use for the ready-in signal
 *  \param clk     the clock used to configure the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
inline xcore_c_error_t
port_protocol_in_strobed_slave(port p, port readyin, clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in input mode. */ \
                                (void) _port_in(p); \
                                _clock_set_ready_src(clk, readyin); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_strobed(p); \
                                _port_set_slave(p); \
                                _port_clear_buffer(p); \
                              } while (0) ); \
}

/** configures a port to be a clocked output port in strobed slave mode.
 *
 *  if the ready-in port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. the ready-in port is read on the
 *  port's sampling edge. outputs are driven on the next falling edge
 *  of the clock where the previous value read from the ready-in port is high.
 *  by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p       the port to configure
 *  \param readyin a 1-bit port to use for the ready-in signal
 *  \param clk     the clock used to configure the port
 *  \param initial the initial value to output on the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
inline xcore_c_error_t
port_protocol_out_strobed_slave(port p, port readyin, clock clk, uint32_t initial)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_inout_data(p); \
                                _port_set_buffered(p); \
                                /* ensure port is in output mode. */ \
                                _port_out(p, initial); \
                                _clock_set_ready_src(clk, readyin); \
                                _port_set_clock(p, clk); \
                                _port_set_ready_strobed(p); \
                                _port_set_slave(p); \
                              } while (0) );
}

#endif // !defined(__XC__)

#endif // __xcore_c_port_protocol_h__

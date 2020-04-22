// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_h__
#define __xcore_c_port_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include "xcore_c_port_impl.h"
#include "xcore_c_resource_impl.h"
#include "xcore_c_exception_impl.h"

/** A port identifier
 *
 * Port resources must be allocated by name rather than from a poll.
 */
typedef enum {
  port_1A  = XS1_PORT_1A,
  port_1B  = XS1_PORT_1B,
  port_1C  = XS1_PORT_1C,
  port_1D  = XS1_PORT_1D,
  port_1E  = XS1_PORT_1E,
  port_1F  = XS1_PORT_1F,
  port_1G  = XS1_PORT_1G,
  port_1H  = XS1_PORT_1H,
  port_1I  = XS1_PORT_1I,
  port_1J  = XS1_PORT_1J,
  port_1K  = XS1_PORT_1K,
  port_1L  = XS1_PORT_1L,
  port_1M  = XS1_PORT_1M,
  port_1N  = XS1_PORT_1N,
  port_1O  = XS1_PORT_1O,
  port_1P  = XS1_PORT_1P,
  port_4A  = XS1_PORT_4A,
  port_4B  = XS1_PORT_4B,
  port_4C  = XS1_PORT_4C,
  port_4D  = XS1_PORT_4D,
  port_4E  = XS1_PORT_4E,
  port_4F  = XS1_PORT_4F,
  port_8A  = XS1_PORT_8A,
  port_8B  = XS1_PORT_8B,
  port_8C  = XS1_PORT_8C,
  port_8D  = XS1_PORT_8D,
  port_16A = XS1_PORT_16A,
  port_16B = XS1_PORT_16B,
  port_16C = XS1_PORT_16C,
  port_16D = XS1_PORT_16D,
  port_32A = XS1_PORT_32A,
  port_32B = XS1_PORT_32B,
} port_id_t;


/** Enumeration to declare how the port was set up */
typedef enum {
  PORT_UNBUFFERED = 0,
  PORT_BUFFERED = 1,
} port_type_t;

/** Allocates a port.
 *
 *  Either this function or port_alloc_buffered() must be called once for each
 *  variable of type ``port`` before use. port_free() must be called afterwards.
 *
 *  The port's state is set to: input, unbuffered, inout_data, no_invert,
 *  rising_edge, master, no_ready, no triggers, clocked by XS1_CLKBLK_REF.
 *
 *  \param p    Port variable representing the initialised port
 *  \param id   Value that identifies which port to drive
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*p* argument.
 */
inline xcore_c_error_t port_alloc(port *p, port_id_t id)
{
  RETURN_EXCEPTION_OR_ERROR( *p = _port_alloc(id) );
}

/** Reset a port.
 *
 *  Clears a ports settings back to the default state at port_alloc().
 *
 *  \param p    The port to be reset
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*p* argument.
 */
inline xcore_c_error_t port_reset(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_reset(p) );
}

/** Allocates a port to buffer and serialise/deserialise data.
 *
 *  Either this function or port_alloc() must be called once for each variable
 *  of type ``port`` before use. port_free() must be called afterwards.
 *
 *  \param p               Port variable representing the initialised port
 *  \param id         Value that identifies which port to drive
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*p* argument.
 */
inline xcore_c_error_t port_alloc_buffered(port *p, port_id_t id, size_t transfer_width)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                *p = _port_alloc(id); \
                                _port_set_buffered(id); \
                                _port_set_transfer_width(id, transfer_width); \
                              } while (0) );
}

/** Deallocate a port
 *
 *  \param p    The port to be freed
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*p* argument.
 */
inline xcore_c_error_t port_free(port *p)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_free(*p); \
                                *p = 0; \
                              } while (0) );
}

/** Change the transfer width of a port.
 *
 *  The default transfer width is the same as the physical port width.
 *
 *  **A port must have been set to buffered if the width is different from the
 *  physical port width**
 *
 *  \param p               The port to change the transfer width of
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port,
 *                                    or the port is unbuffered.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_transfer_width(port p, size_t transfer_width)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_transfer_width(p, transfer_width) );
}

/** Sets a port to be buffered.
 *
 *  Configures a port into buffered mode where it can automatically serialise or
 *  deserialise data.
 *
 *  \param p    The port to set as buffered
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_buffered(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_buffered(p) );
}

/** Sets a port to be unbuffered (default state).
 *
 *  Configures a port into unbuffered mode. Note that before this is called, a
 *  a port needs to have its transfer width equal to the port width and be
 *  configured as a master port.
 *
 *  \param p    The port to set as unbuffered
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_unbuffered(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_unbuffered(p) );
}

/** Set the clock clocking a port.
 *
 *  This function changes the clock used for a port's control functions.
 *  The default clock is 'XS1_CLKBLK_REF'.
 *
 *  \param p    Port whose clock is being changed.
 *  \param clk  Clock to attach to the port.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port, clock,
 *                                    or clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
#if USE_XCORE_CLOCK_TYPE
inline xcore_c_error_t port_set_clock(port p, clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_clock(p, clk) );
}
#endif

/** Set a port drive out the data value (default state).
 *
 *  \param p    Port to change the mode of
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_inout_data(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_inout_data(p) );
}

/** Set a port to drive out its clocking signal.
 *
 *  This function configures the port to drive the clock signal instead of its
 *  own data values. The clock signal that is driven out is configured using the
 *  port_set_clock() function.
 *
 *  \param p    Port to change the mode of
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_out_clock(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_out_clock(p) );
}

/** Set a port to drive out the ready signal of another port.
 *
 *  This function configures the port to drive the ready signal of another port
 *  instead of its own data values.
 *
 *  \param p              Port to change the mode of. This must be a 1-bit
 *                        port or this function will trap.
 *  \param ready_source   The port whose ready signal will be output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *                                    or p not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_out_ready(port p, port ready_source)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_out_ready(p, ready_source) );

}

/** Set the port to invert its data.
 *
 *  This function configures a port to invert the data on the pin. This can be
 *  reverted by calling port_set_no_invert().
 *
 *  \param p   Port to set its data to be inverted. This must be a 1-bit port
 *             or a trap will be raised.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
  *                                   or p not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_invert(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_invert(p) );
}

/** Set the port to not invert its data (default state).
 *
 *  This function configures a port to not invert the data on the pin.
 *
 *  \param p   Port to set the data to not be inverted.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_no_invert(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_no_invert(p) );
}

/** Set the port to sample on the falling edge.
 *
 *  The default is for a port to sample data on the rising edge of the clock.
 *  This function changes the port to sample on the falling edge instead.
 *  This change can be reverted by calling port_set_sample_rising_edge().
 *
 *  \param p    Port to change its sampling edge.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_sample_falling_edge(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_sample_falling_edge(p) );
}

/** Set the port to sample on the rising edge (default state).
 *
 *  This function restores a port to sampling data on the rising edge of the clock.
 *
 *  \param p    Port to change its sampling edge.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_sample_rising_edge(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_sample_rising_edge(p) );
}

/** Set the port to master mode (default state).
 *
 *  This function configures a port to be a master. This is only relevant when
 *  using ready signals (port_set_ready_strobed() / port_set_ready_handshake()).
 *
 *  It is highly recommended to use the ``port_protocol_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p    Port to set as a master
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_master(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_master(p) );
}

/** Set the port to slave mode.
 *
 *  This function configures a port to be a master. This is only relevant when
 *  using a ready strobe (port_set_ready_strobed())
 *
 *  *Note*: the port must be set to use a ready strobe, otherwise this function
 *          will raise an exception.
 *
 *  It is highly recommended to use the ``port_protocol_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to set as a slave
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_slave(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_slave(p) );
}

/** Set the port to use no ready signals (default state).
 *
 *  This function changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  *Note*: the port must be a ``master`` port otherwise this function will raise
 *          an exception.
 *
 *  It is highly recommended to use the ``port_protocol_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p    Port to change to not use ready signals
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_no_ready(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_no_ready(p) );
}

/** Set the port to use a single strobe.
 *
 *  This function changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  *Note*: the port must be a buffered port otherwise this function will raise
 *          an exception.
 *
 *  It is highly recommended to use the ``port_protocol_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p    Port to change to not use ready signals
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_ready_strobed(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_ready_strobed(p) );
}

/** Set the port to be fully handshaken.
 *
 *  This function changes a port to use both a ready input and drive a ready
 *  output in order to control when data is sampled or written.
 *
 *  *Note*: the port must be a master buffered port otherwise this function will
 *          raise an exception.
 *
 *  It is highly recommended to use the ``port_protocol_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to change to not use ready signals
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_ready_handshake(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_ready_handshake(p) );
}

/** Get the timestamp of the last operation on a port.
 *
 *  This function gets the timestamp of the last input or output operation
 *  on a port.
 *
 *  \param p    The port to get the timestamp from
 *  \param t    The timestamp of the last operation
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*t* argument.
 */
inline xcore_c_error_t port_get_trigger_time(port p, int16_t *t)
{
  RETURN_EXCEPTION_OR_ERROR( *t = _port_get_trigger_time(p) );
}

/** Set the timestamp at which the port will input/output data.
 *
 *  This function sets the time condition for the next input or output on
 *  a port.
 *  If the port is unbuffered or the buffer is empty/full a call to port_in()
 *  or port_out() will pause until the specified time.
 *  The trigger is cleared by a input/output or by calling port_clear_trigger_time().
 *
 *  \param p    The port to set the condition on
 *  \param t    The port timestamp to match
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline xcore_c_error_t port_set_trigger_time(port p, int16_t t)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_trigger_time(p, t) );
}

/** Clear the timestamp trigger on a port.
 *
 *  This function clears any trigger_time condition on the port so the next input or
 *  output will happen unconditionally in respect to the timestamp.
 *  This function does not clear the trigger_in condition on the port.
 *
 *  \param p    the port to clear the trigger_time on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_clear_trigger_time(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_clear_trigger_time(p) );
}

/** Setup an event to trigger on a port when its input value matches.
 *
 *  On a unbuffered port the trigger will apply to all future inputs
 *  until the trigger is set again.
 *  On an buffered port the trigger will only hold for the next input
 *  after which the trigger_in_equal will be cleared.
 *
 *  \param p    The port to set the trigger on
 *  \param v    The value to match
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_trigger_in_equal(port p, uint32_t v)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_trigger_in_equal(p, v) );
}

/** Setup an event to trigger on a port when its input value does not matches.
 *
 *  On a unbuffered port the trigger will apply to all future inputs
 *  until the trigger is set again.
 *  On an buffered port the trigger will only hold for the next input
 *  after which the trigger_in_not_equal will be cleared.
 *
 *  \param p    The port to set the trigger on
 *  \param v    The value not to match
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_set_trigger_in_not_equal(port p, uint32_t v)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_trigger_in_not_equal(p, v) );
}

/** Clear the in trigger on a port.
 *
 *  This function clears any trigger_in condition on the port so the next input
 *  will happen unconditionally in respect to the input value.
 *  This function does not clear the trigger_time condition on the port.
 *
 *  \param p    The port to clear the trigger_in on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_clear_trigger_in(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_clear_trigger_in(p) );
}

/** Peek at the value on a a port.
 *
 *  Peeking a port returns the current value on the pins of a port, regardless
 *  of whether the port is an output or input and without affecting its direciton.
 *  Peek will not pause, regardless of any triggers that have been set.
 *
 *  \param p    Port to be peeked
 *  \param data The current value on the pins
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_peek(port p, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR( *data = _port_peek(p) );
}

/** Outputs a value onto a port.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be
 *  stored in the buffer, and be serialised onto the output pins.
 *
 *  If there is a time trigger setup and the port is unbuffered or the buffer is full
 *  the call will pause until the specified time.
 *
 *  \param p      Port to output to
 *  \param data   Value to output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_out(port p, uint32_t data)
{
  RETURN_EXCEPTION_OR_ERROR( _port_out(p, data) );
}

/** Input a value from a port.
 *
 *  For unbuffered port with no trigger, the data will be whatever is on the input pins.
 *  For unbuffered port with a trigger, the data will be the value read when the
 *  trigger fired. The call will pause if the trigger has not yet fired.
 *  For buffered port, this function will pause until the buffer is filled up with
 *  deserialised data.
 *
 *  \param p    Port to input from
 *  \param data The inputted data
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in(port p, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR( *data = _port_in(p) );
}

/** Outputs a value onto a port and shift the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be stored
 *  in the buffer, and be serialised onto the output pins.
 *
 *  If there is a time trigger setup and the port is unbuffered or the buffer is full
 *  the call will pause until the specified time.
 *
 *  \param p      Port to output to
 *  \param data   data is shifted right by the transfer width of the port,
 *                with the bits shifting out onto the port.
 *                The remaining shifted bits are returned in data.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_out_shift_right(port p, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR( _port_out_shift_right(p, data) );
}

/** Input a value from a port and shift the data.
 *
 *  For unbuffered port with no trigger, the data will be whatever is on the input pins.
 *  For unbuffered port with a trigger, the data will be the value read when the
 *  trigger fired. The call will pause if the trigger has not yet fired.
 *  For buffered port, this function will pause until the buffer is filled up with
 *  deserialised data.
 *
 *  \param p      Port to input from
 *  \param data   The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_shift_right(port p, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR( _port_in_shift_right(p, data) );
}

/** Outputs a value onto a port at a specified port timestamp.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port timestamp to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p      Port to output to
 *  \param t      The timestamp to do the output on
 *  \param data   Value to output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_out_at_time(port p, int16_t t, uint32_t data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_time(p, t); \
                                _port_out(p, data); \
                              } while (0) );
}

/** Input data from a port when its counter is at a specific time.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time ``t``. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p      Port to input from
 *  \param t      The timestamp to do input on
 *  \param data   The inputted data
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_at_time(port p, int16_t t, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_time(p, t); \
                                *data = _port_in(p); \
                              } while (0) );
}

/** Outputs a value onto a port at a specified time and shifts the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port counter to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p      Port to output to
 *  \param t      The timestamp of the output
 *  \param data   data is shifted right by the transfer width of the port,
 *                with the bits shifting out onto the port.
 *                The remaining shifted bits are returned in data.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_out_shift_right_at_time(port p, int16_t t, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_time(p, t); \
                                _port_out_shift_right(p, data); \
                              } while (0) );
}

/** Input data from a port at a specific time and shift the data.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time ``t``. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p      Port to input from
 *  \param t      The timestamp to do input on
 *  \param data   The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_shift_right_at_time(port p, int16_t t, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_time(p, t); \
                                _port_in_shift_right(p, data); \
                              } while (0) );
}


/** Input data from a port when its pins match a specific value.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   The inputted data
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_when_pinseq(port p, port_type_t pt, uint32_t value, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_in_equal(p , value); \
                                *data = _port_in(p); \
                                if (pt==PORT_UNBUFFERED) _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Input data from a port when its pins do not match a specific value.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this macro will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   The inputted data
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_when_pinsneq(port p, port_type_t pt, uint32_t value, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_in_not_equal(p , value); \
                                *data = _port_in(p); \
                                if (pt==PORT_UNBUFFERED) _port_clear_trigger_in(p); \
                              } while (0) );
}


/** Input data from a port when its pins match a specific value and shift the data.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   The input data shifted right by the transfer width of the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_shift_right_when_pinseq(port p, port_type_t pt, uint32_t value, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_in_equal(p , value); \
                                _port_in_shift_right(p, data); \
                                if (pt==PORT_UNBUFFERED) _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Input data from a port when its pins do not
 *  match a specific value  and shift the data.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this macro will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   The input data shifted right by the transfer width of the port
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t port_in_shift_right_when_pinsneq(port p, port_type_t pt, uint32_t value, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _port_set_trigger_in_not_equal(p , value); \
                                _port_in_shift_right(p, data); \
                                if (pt==PORT_UNBUFFERED) _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Clears the buffer used by a port.
 *
 *  Any data sampled by the port which has not been input by the processor is
 *  discarded. Any data output by the processor which has not been driven by the
 *  port is discarded. If the port is in the process of serialising output, it
 *  is interrupted immediately. If a pending output would have caused a change
 *  in direction of the port then that change of direction does not take place.
 *  If the port is driving a value on its pins when this function is called then
 *  it continues to drive the value until an output statement changes the value
 *  driven.
 *
 *  \param p    The port whose buffer is to be cleared
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline xcore_c_error_t port_clear_buffer(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_clear_buffer(p) );
}

/** Ends the current input on a buffered port.
 *
 *  The number of bits sampled by the port but not yet input by the processor is
 *  returned. This count includes both data in the transfer register and data in
 *  the shift register used for deserialisation.
 *  Subsequent inputs on the port return transfer-width bits of data
 *  until there is less than one transfer-width bits of data remaining.
 *  Any remaining data can be read with one further input, which
 *  returns transfer-width bits of data with the remaining buffered data
 *  in the most significant bits of this value.
 *
 *  \param p    The port to end the current input on
 *  \param num  The number of bits of data remaining
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*num* argument.
 */
inline xcore_c_error_t port_endin(port p, size_t *num)
{
  RETURN_EXCEPTION_OR_ERROR( *num = _port_endin(p) );
}

/** Force an input on a buffered port.
 *
 *  The function will perform an input on a buffered port even if the buffer is
 *  only partially full.
 *
 *  \param p    The port to do the input on
 *  \param num  A variable to be filled with number of bits inputted
 *  \param data The inputted data

 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *\*num* or *\*data* argument.
 */
inline xcore_c_error_t port_force_input(port p, size_t *num, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                *num = _port_endin(p); \
                                *data = _port_in(p); \
                              } while (0) );
}

/** Setup select events on a port.
 *
 *  Configures a port to trigger select events when ready. By default a
 *  port will trigger when there is data available. The trigger event can be
 *  changed using the port_set_trigger_*() function.
 *
 *  Once the select event is setup you need to call port_enable_trigger() to enable it.
 *
 *  \param p        The port to setup the select event on
 *  \param enum_id  The value to be returned by select_wait() et al when the
 *                  port event is triggered.
 *                  On XS1 bit 16 must be set (see ENUM_ID_BASE)
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t port_setup_select(port p, uint32_t enum_id)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_select(p, enum_id); \
                                _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Setup select events on a port where the events are handled by a function.
 *
 *  Same as port_setup_select() except that a callback function is used
 *  rather than the event being passed back to the select_wait() et al functions.
 *
 *  Once the event is setup you need to call port_enable_trigger() to enable it.
 *
 *  \param p      The port to setup the select event on
 *  \param data   The value to be passed to the select_callback_t function
 *                On XS1 bit 16 must be set (see ENUM_ID_BASE)
 *  \param func   The select_callback_t function to handle events
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in data.
 */
inline xcore_c_error_t port_setup_select_callback(port p, void *data, select_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_select_callback(p, data, func); \
                                _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Setup interrupt event on a port.
 *
 *  Once the event is setup you need to call port_enable_trigger() to enable it.
 *
 *  \param p     The port to setup the interrupt event on
 *  \param data  The value to be passed to the interrupt_callback_t function
 *               On XS1 bit 16 must be set (see ENUM_ID_BASE)
 *  \param func  The interrupt_callback_t function to handle events
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in data.
 */
inline xcore_c_error_t port_setup_interrupt_callback(port p, void *data, interrupt_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_interrupt_callback(p, data, func); \
                                _port_clear_trigger_in(p); \
                              } while (0) );
}

/** Enable select & interrupt events on a port.
 *
 *  Prior to enabling, port_setup_select(), port_setup_select_callback() or
 *  port_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using port_disable_trigger()
 *  and port_enable_trigger().
 *  When the event fires, the value must be read from the port to clear the event.
 *
 *  \param p    The port to enable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline xcore_c_error_t port_enable_trigger(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_enable_trigger(p) );
}

/** Disable select & interrupt events for a given port.
 *
 *  This function prevents any further events being triggered by a given port.
 *
 *  \param p    The port to disable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline xcore_c_error_t port_disable_trigger(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_disable_trigger(p) );
}

#endif // !defined(__XC__)

#endif // __xcore_c_port_h__


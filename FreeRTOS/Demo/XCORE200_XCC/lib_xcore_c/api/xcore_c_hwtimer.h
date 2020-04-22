// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_hwtimer_h__
#define __xcore_c_hwtimer_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xcore_c_hwtimer_impl.h"
#include "xcore_c_resource_impl.h"
#include "xcore_c_exception_impl.h"

/** Deallocate the xC timer resource for a thread
 *
 *  This function deallcoates the hardware timer automatically allocated
 *  for xC use. Each logical core is allocated a hardware timer that is
 *  multiplexed and used by the xC 'timer' interface. This multiplexed
 *  timer is not accessible from C.
 *  If the logical core is not running any xC code, or any xC code
 *  is not making use of the 'timer' resource type, the allocated hardware
 *  timer may be retrieved for use as a hwtimer_t.
 *
 *  **This call must be paired with a call to hwtimer_realloc_xc_timer()
 *  prior to the logical core completing its tasks**
 *
 *  **The xScope link also requires a hardware timer**
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   timer has already been deallocated.
 */
inline xcore_c_error_t hwtimer_free_xc_timer(void)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_free_xc_timer() );
}

/** Reallocate the xC timer resource for a thread
 *
 *  This function reallcoates a logical core's xC hardware timer that was
 *  deallocated by a call to hwtimer_free_xc_timer().
 *
 *  **There must be an available hw timer when this call is made,
 *  otherwise an exception will be raised when the logical core completes**
 *
 *  \return     error_none.
 *
 *  \exception  ET_ECALL   no available hw timer, reallocation failed.
 */
inline xcore_c_error_t hwtimer_realloc_xc_timer(void)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_realloc_xc_timer() );
}

/** Allocates and initialise a timer.
 *
 *  This function allocates a hardware timer.
 *  If there are no timers available, then the function will return 0.
 *  This macro is to be called once on every variable of the type ``timer``.
 *  When the timer is no longer required, hwtimer_free() must be called
 *  to deallocate it.
 *
 *  \param t    Timer variable representing the initialised timer
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *\*t* argument.
 */
inline xcore_c_error_t hwtimer_alloc(hwtimer_t *t)
{
  RETURN_EXCEPTION_OR_ERROR( *t = _hwtimer_alloc() );
}

/** Deallocate a timer.
 *
 *  This function frees the hardware timer.
 *
 *  \param t    The timer to be freed
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *\*t* argument.
 */
inline xcore_c_error_t hwtimer_free(hwtimer_t *t)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _hwtimer_free(*t); \
                                *t = 0; \
                              } while (0) );
}

/** Get the current time from the timer.
 *
 *  If there is a trigger time setup, the call will stall until after the trigger time.
 *  For select and interrupt event, calling hwtimer_get_time() will clear the event.
 *
 *  \param t    The timer on which to input
 *  \param now  The time value (a 32-bit value)
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *\*now* argument.
 */
inline xcore_c_error_t hwtimer_get_time(hwtimer_t t, uint32_t *now)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_get_time(t, now) );
}

/** Setup an event trigger on a timer.
 *
 *  This will cause hwtimer_get_time() to pause until the specified time.
 *  The trigger may be cleared using hwtimer_clear_trigger_time().
 *
 *  **hwtimer_wait_until(), hwtimer_delay(), hwtimer_setup_select()
 *  hwtimer_setup_select_callback() and hwtimer_setup_interrupt_callback()
 *  call hwtimer_set_trigger_time()**
 *
 *  \param t     The timer to setup a event trigger on.
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline xcore_c_error_t hwtimer_set_trigger_time(hwtimer_t t, uint32_t time)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_set_trigger_time(t, time) );
}

/** Change the time at which a timer trigger will fire.
 *
 *  This function modifies the time at which a previously setup triggers fire.
 *  It is used to set a new trigger time after a select or interrupt event has occurred.
 *
 *  \param t     The timer to change
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline xcore_c_error_t hwtimer_change_trigger_time(hwtimer_t t, uint32_t time)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_change_trigger_time(t, time) );
}

/** Clear an event trigger on a timer.
 *
 *  Makes sure no triggers are setup on a timer.
 *  Should be called when a timer is no longer being used for select and interrupt events.
 *  Both hwtimer_wait_until() and hwtimer_delay() call hwtimer_clear_trigger_time().
 *
 *  \param t    The timer to tear down events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */

inline xcore_c_error_t hwtimer_clear_trigger_time(hwtimer_t t)
{
  RETURN_EXCEPTION_OR_ERROR( _hwtimer_clear_trigger_time(t) );
}

/** Wait until after a specified time.
 *
 *  **This will destroy any select or interrupt event triggers set on this resource**
 *
 *  \param t      The timer to use for timing
 *  \param until  The time to wait until
 *  \param now    The time we actually waited until
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *\*now* argument.
 */
inline xcore_c_error_t hwtimer_wait_until(hwtimer_t t, uint32_t until, uint32_t *now)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _hwtimer_set_trigger_time(t, until); \
                                _hwtimer_get_time(t, now); \
                                _hwtimer_clear_trigger_time(t); \
                              } while (0) );
}

/** Delay for a specified time using a specific timer.
 *
 *  **This will destroy any select or interrupt event triggers set on this resource**
 *
 *  \param t    The timer resource to use
 *  \param period The amount of time to wait (in reference time ticks, usually 10ns steps)
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *\*now* argument.
 */
inline xcore_c_error_t hwtimer_delay(hwtimer_t t, uint32_t period)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                uint32_t start; \
                                _hwtimer_get_time(t, &start); \
                                uint32_t until = start + period; \
                                _hwtimer_set_trigger_time(t, until); \
                                uint32_t dummy; \
                                _hwtimer_get_time(t, &dummy); \
                                _hwtimer_clear_trigger_time(t); \
                              } while (0) );
}

/** Setup select events on a timer.
 *
 *  Configures a timer to trigger select events when the timer has reached
 *  the specified time. It is used in combination with select_wait() et al
 *  functions, returning the enum_id when the event is triggered.
 *
 *  Once the select event is setup you need to call hwtimer_enable_trigger() to enable it.
 *
 *  \param t        The timer to setup the select event on
 *  \param time     The time at which the timer will trigger an event. The default
 *                  timer ticks are at a 10ns resolution.
 *  \param enum_id  The value to be returned by select_wait() et al when the
 *                  timer event is triggered.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t hwtimer_setup_select(hwtimer_t t, uint32_t time, uint32_t enum_id)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_select(t, enum_id); \
                                _hwtimer_set_trigger_time(t, time); \
                              } while (0) );
}

/** Setup select event on a timer where the events are handled by a function.
 *
 *  Same as hwtimer_setup_select() except that a callback function is used
 *  rather than the event being passed back to the select_wait() et al functions.
 *
 *  Once the event is setup you need to call hwtimer_enable_trigger() to enable it.
 *
 *  \param t      The timer to setup the select event on
 *  \param time   The time at which the timer will trigger an event. The default
 *                timer ticks are at a 10ns resolution.
 *  \param data   The value to be passed to the select_callback_t function
 *  \param func   The select_callback_t function to handle the event
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t
hwtimer_setup_select_callback(hwtimer_t t, uint32_t time, void *data, select_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_select_callback(t, data, func);
                                _hwtimer_set_trigger_time(t, time);
                              } while (0) );
}

/** Setup interrupt event on a timer.
 *
 *  Once the event is setup you need to call hwtimer_enable_trigger() to enable it.
 *
 *  \param t      The timer to setup the events on
 *  \param time   The time at which the timer will trigger an event. The default
 *                timer ticks are at a 10ns resolution.
 *  \param data   The value to be passed to the interrupt_callback_t function
 *  \param func   The interrupt_callback_t function to handle the events
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t
hwtimer_setup_interrupt_callback(hwtimer_t t, uint32_t time, void *data, interrupt_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _resource_setup_interrupt_callback(t, data, func);
                                _hwtimer_set_trigger_time(t, time);
                              } while (0) );
}

/** Enable select & interrupt events on a timer.
 *
 *  Prior to enabling, hwtimer_setup_select(), hwtimer_setup_select_callback() or
 *  hwtimer_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using hwtimer_disable_trigger()
 *  and hwtimer_enable_trigger().
 *  When the event fires, hwtimer_get_time() must be called to clear the event.
 *  The time of the next event is set using hwtimer_change_trigger_time().
 *  
 *  \param t    The timer to enable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline xcore_c_error_t hwtimer_enable_trigger(hwtimer_t t)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_enable_trigger(t) );
}

/** Disable select & interrupt events for a given timer.
 *
 *  This function prevents any further select or interrupt events being triggered
 *  by a given timer.
 *
 *  **This does not clear the trigger setup**
 *
 *  \param t    The timer to disable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline xcore_c_error_t hwtimer_disable_trigger(hwtimer_t t)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_disable_trigger(t) );
}

#endif // !defined(__XC__)

#endif // __xcore_c_hwtimer_h__

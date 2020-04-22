// Copyright (c) 2020, XMOS Ltd, All rights reserved

#include "FreeRTOS.h"
#include "task.h"

#include "TimerDemo.h"

static void _hwtimer_get_trigger_time( hwtimer_t t, uint32_t *time )
{
	asm volatile("getd %0, res[%1]" : "=r" (*time): "r" (t));
}

static xcore_c_error_t hwtimer_get_trigger_time( hwtimer_t t, uint32_t *time )
{
	RETURN_EXCEPTION_OR_ERROR( _hwtimer_get_trigger_time( t, time ) );
}

DEFINE_RTOS_INTERRUPT_CALLBACK( pxTimerDemoISR, pvData )
{
hwtimer_t xTimer = ( hwtimer_t ) pvData;
uint32_t ulNow;

	hwtimer_get_trigger_time( xTimer, &ulNow );

	/* The full demo includes a software timer demo/test that requires
	prodding periodically from a periodic interrupt. */
	vTimerPeriodicISRTests();

	ulNow += configCPU_CLOCK_HZ / configTICK_RATE_HZ;
	hwtimer_change_trigger_time( xTimer, ulNow );
}

void vInitialiseTimerISRForTimerDemo( void )
{
uint32_t ulNow;
hwtimer_t xTimer;

	hwtimer_alloc( &xTimer );
	hwtimer_get_time( xTimer, &ulNow );
	ulNow += configCPU_CLOCK_HZ / configTICK_RATE_HZ;
	hwtimer_setup_interrupt_callback( xTimer, ulNow, ( void * ) xTimer, RTOS_INTERRUPT_CALLBACK( pxTimerDemoISR ) );
	hwtimer_enable_trigger( xTimer );
}

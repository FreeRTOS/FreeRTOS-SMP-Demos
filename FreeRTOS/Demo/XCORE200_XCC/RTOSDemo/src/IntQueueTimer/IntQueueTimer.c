// Copyright (c) 2019, XMOS Ltd, All rights reserved

#include "FreeRTOS.h"
#include "task.h"

#include "IntQueue.h"

static hwtimer_t xIntQueueTimer0;
static hwtimer_t xIntQueueTimer1;

static void _hwtimer_get_trigger_time( hwtimer_t t, uint32_t *time )
{
	asm volatile("getd %0, res[%1]" : "=r" (*time): "r" (t));
}

static xcore_c_error_t hwtimer_get_trigger_time( hwtimer_t t, uint32_t *time )
{
	RETURN_EXCEPTION_OR_ERROR( _hwtimer_get_trigger_time( t, time ) );
}

DEFINE_RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR, pvData )
{
hwtimer_t xTimer = ( hwtimer_t ) pvData;
uint32_t ulNow;
int xYieldRequired;

	hwtimer_get_time( xTimer, &ulNow );
	hwtimer_get_trigger_time( xTimer, &ulNow );

	if ( xTimer == xIntQueueTimer0 )
	{
		ulNow += configCPU_CLOCK_HZ / 100;
		xYieldRequired = xFirstTimerHandler();
	}
	else
	{
		ulNow += configCPU_CLOCK_HZ / 150;
		xYieldRequired = xSecondTimerHandler();
	}

	hwtimer_change_trigger_time( xTimer, ulNow );

	portYIELD_FROM_ISR( xYieldRequired );
}

void vInitialiseTimerForIntQueueTest( void )
{
uint32_t ulNow;

	hwtimer_alloc( &xIntQueueTimer0 );
	hwtimer_get_time( xIntQueueTimer0, &ulNow );
	ulNow += configCPU_CLOCK_HZ / 100;
	hwtimer_setup_interrupt_callback( xIntQueueTimer0, ulNow, ( void * ) xIntQueueTimer0, RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR ) );
	hwtimer_enable_trigger( xIntQueueTimer0 );

	hwtimer_alloc( &xIntQueueTimer1 );
	hwtimer_get_time( xIntQueueTimer1, &ulNow );
	ulNow += configCPU_CLOCK_HZ / 150;
	hwtimer_setup_interrupt_callback( xIntQueueTimer1, ulNow, ( void * ) xIntQueueTimer1, RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR ) );
	hwtimer_enable_trigger( xIntQueueTimer1 );
}


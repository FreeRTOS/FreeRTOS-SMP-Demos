// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved

#include "FreeRTOS.h"
#include "task.h"

#include <xcore/hwtimer.h>
#include <xcore/triggerable.h>

#include "IntQueue.h"

static hwtimer_t xIntQueueTimer0;
static hwtimer_t xIntQueueTimer1;

DEFINE_RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR, pvData )
{
	hwtimer_t xTimer = ( hwtimer_t ) pvData;
	uint32_t ulNow;
	int xYieldRequired;

	ulNow = hwtimer_get_time( xTimer );
    ulNow = hwtimer_get_trigger_time( xTimer );

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

	xIntQueueTimer0 = hwtimer_alloc();
	ulNow = hwtimer_get_time( xIntQueueTimer0 );
	ulNow += configCPU_CLOCK_HZ / 100;
	triggerable_setup_interrupt_callback( xIntQueueTimer0, ( void * ) xIntQueueTimer0, RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR ) );
	hwtimer_set_trigger_time( xIntQueueTimer0, ulNow );
	triggerable_enable_trigger( xIntQueueTimer0 );

	xIntQueueTimer1 = hwtimer_alloc();
	ulNow = hwtimer_get_time( xIntQueueTimer1 );
	ulNow += configCPU_CLOCK_HZ / 150;
	triggerable_setup_interrupt_callback( xIntQueueTimer1, ( void * ) xIntQueueTimer1, RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR ) );
	hwtimer_set_trigger_time( xIntQueueTimer1, ulNow );
	triggerable_enable_trigger( xIntQueueTimer1 );
}


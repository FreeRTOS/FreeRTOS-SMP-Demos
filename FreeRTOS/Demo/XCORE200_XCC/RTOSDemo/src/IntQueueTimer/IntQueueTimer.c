<<<<<<< HEAD
// Copyright (c) 2019, XMOS Ltd, All rights reserved
=======
// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

#include "FreeRTOS.h"
#include "task.h"

<<<<<<< HEAD
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
=======
#include <xcore/hwtimer.h>
#include <xcore/triggerable.h>

#include "IntQueue.h"

DEFINE_RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR, pvData )
{
	static int xCount0, xCount1;
	hwtimer_t xTimer = ( hwtimer_t ) pvData;
	uint32_t ulNow;
	int xYieldRequired = pdFALSE;

	ulNow = hwtimer_get_time( xTimer );
    ulNow = hwtimer_get_trigger_time( xTimer );
	ulNow += configCPU_CLOCK_HZ / 50;

	if( ++xCount0 == 2 )
	{
		xCount0 = 0;
		taskENTER_CRITICAL_FROM_ISR();
		if( xFirstTimerHandler() != pdFALSE )
		{
			xYieldRequired = pdTRUE;
		}
		taskEXIT_CRITICAL_FROM_ISR( 0 );
	}

	if( ++xCount1 == 3 )
	{
		xCount1 = 0;
		taskENTER_CRITICAL_FROM_ISR();
		if( xSecondTimerHandler() != pdFALSE )
		{
			xYieldRequired = pdTRUE;
		}
		taskEXIT_CRITICAL_FROM_ISR( 0 );
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
	}

	hwtimer_change_trigger_time( xTimer, ulNow );

	portYIELD_FROM_ISR( xYieldRequired );
}

void vInitialiseTimerForIntQueueTest( void )
{
uint32_t ulNow;
<<<<<<< HEAD

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
=======
uint32_t ulState;
hwtimer_t xIntQueueTimer;

	/*
	 * Disable interrupts here so we stay on the same core
	 */
	ulState = portDISABLE_INTERRUPTS();
	{
		xIntQueueTimer = hwtimer_alloc();
		ulNow = hwtimer_get_time( xIntQueueTimer );
		ulNow += configCPU_CLOCK_HZ / 50;
	    triggerable_setup_interrupt_callback( xIntQueueTimer, ( void * ) xIntQueueTimer, RTOS_INTERRUPT_CALLBACK( pxIntQueueTimerISR ) );
	    hwtimer_set_trigger_time( xIntQueueTimer, ulNow );
	    triggerable_enable_trigger( xIntQueueTimer );
	}
	portRESTORE_INTERRUPTS( ulState );
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
}


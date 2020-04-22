// Copyright (c) 2019, XMOS Ltd, All rights reserved

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "portable.h"

/* Demo application include files. */
#include "partest.h"

void led_driver(uint16_t led_value);

static chanend c_write;
static chanend c_read;
static int this_tile;
static uint16_t led_bitmap = 0xFFFF;

typedef enum {
	LED_OFF = pdFALSE,
	LED_ON  = pdTRUE,
	LED_TOGGLE
} led_value_t;

typedef union {
	struct {
		uint32_t tile : 8;
		uint32_t led : 8;
		led_value_t value : 8;
		uint32_t reserved : 8;
	};
	uint32_t raw;
} led_cmd_t;

DEFINE_RTOS_INTERRUPT_CALLBACK( pxLEDUpdateISR, pvData )
{
	led_cmd_t xCmd;
	uint16_t mask;

	(void) pvData;

	//debug_printf("led isr\n");
	xCmd.raw = _s_chan_in_word( c_read );
	_s_chan_check_ct_end( c_read );

	//debug_printf("Received LED update from tile %x\n", xCmd.tile);

	mask = 1 << xCmd.led;
	if (xCmd.value == LED_ON)
	{
		led_bitmap |= mask;
	}
	else if (xCmd.value == LED_OFF)
	{
		led_bitmap &= mask;
	}
	else if (xCmd.value == LED_TOGGLE)
	{
		led_bitmap ^= mask;
	}

	led_driver( led_bitmap );
}

/* ParTest contains FreeRTOS standard parallel port IO routines. */

void vParTestInitialiseXCORE( int tile, chanend xTile0Chan, chanend xTile1Chan, chanend xTile2Chan, chanend xTile3Chan )
{
	this_tile = tile;

	if( tile == 0 )
	{
		chanend_alloc(&c_read);
		chan_out_word(xTile1Chan, c_read);

		chanend_alloc(&c_write);

		chanend_setup_interrupt_callback( c_read, NULL, RTOS_INTERRUPT_CALLBACK( pxLEDUpdateISR ) );
		chanend_enable_trigger( c_read );
	}
	else
	{
		chan_in_word(xTile0Chan, ( uint32_t * ) &c_read);
		chanend_alloc(&c_write);
	}

	chanend_set_dest(c_write, c_read);
}

/*-----------------------------------------------------------*/

void vParTestSetLED( UBaseType_t uxLED, BaseType_t xValue )
{
	uint32_t ulState;

	//debug_printf("tile %d sending led\n", this_tile);

	led_cmd_t xCmd;

	xCmd.led = uxLED;
	xCmd.value = xValue;
	xCmd.tile = this_tile;

	ulState = portDISABLE_INTERRUPTS();
	_s_chan_out_word( c_write, xCmd.raw );
	_s_chan_out_ct_end( c_write );
	portRESTORE_INTERRUPTS(ulState);
}

/*-----------------------------------------------------------*/

void vParTestToggleLED( UBaseType_t uxLED )
{
	vParTestSetLED( uxLED, LED_TOGGLE );
}

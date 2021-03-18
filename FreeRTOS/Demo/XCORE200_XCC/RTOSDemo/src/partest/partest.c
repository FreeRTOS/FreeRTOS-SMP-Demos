<<<<<<< HEAD
// Copyright (c) 2019, XMOS Ltd, All rights reserved
=======
// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "portable.h"
<<<<<<< HEAD
=======
#include <xcore/chanend.h>
#include <xcore/triggerable.h>
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

/* Demo application include files. */
#include "partest.h"

void led_driver(uint16_t led_value);

<<<<<<< HEAD
static chanend c_write;
static chanend c_read;
=======
static chanend_t c_write;
static chanend_t c_read;
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
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
<<<<<<< HEAD
	xCmd.raw = _s_chan_in_word( c_read );
	_s_chan_check_ct_end( c_read );
=======
	xCmd.raw = chanend_in_word( c_read );
	chanend_check_end_token( c_read );
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

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

<<<<<<< HEAD
void vParTestInitialiseXCORE( int tile, chanend xTile0Chan, chanend xTile1Chan, chanend xTile2Chan, chanend xTile3Chan )
=======
void vParTestInitialiseXCORE( int tile, chanend_t xTile0Chan, chanend_t xTile1Chan, chanend_t xTile2Chan, chanend_t xTile3Chan )
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
{
	this_tile = tile;

	if( tile == 0 )
	{
<<<<<<< HEAD
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
=======
		c_read = chanend_alloc();
		chanend_out_word(xTile1Chan, c_read);

		c_write = chanend_alloc();

		triggerable_setup_interrupt_callback( c_read, NULL, RTOS_INTERRUPT_CALLBACK( pxLEDUpdateISR ) );
		triggerable_enable_trigger( c_read );
	}
	else
	{
		c_read = chanend_in_word(xTile0Chan);
		c_write = chanend_alloc();
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
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
<<<<<<< HEAD
	_s_chan_out_word( c_write, xCmd.raw );
	_s_chan_out_ct_end( c_write );
=======
	chanend_out_word( c_write, xCmd.raw );
	chanend_out_end_token( c_write );
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
	portRESTORE_INTERRUPTS(ulState);
}

/*-----------------------------------------------------------*/

void vParTestToggleLED( UBaseType_t uxLED )
{
	vParTestSetLED( uxLED, LED_TOGGLE );
}

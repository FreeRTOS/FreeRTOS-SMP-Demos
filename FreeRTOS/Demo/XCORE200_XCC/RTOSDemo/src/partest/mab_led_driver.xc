// Copyright (c) 2019, XMOS Ltd, All rights reserved

#include <stdint.h>
#include <platform.h>

out port p_led_0_7 = PORT_LED0_TO_7;
out port p_led_8 = PORT_LED8;
out port p_led_9 = PORT_LED9;
out port p_led_10_12 = PORT_LED10_TO_12;
out port p_led_oe_n = PORT_LED_OEN;

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define LED_COUNT 13

void led_driver(uint16_t led_value)
{
	p_led_0_7 <: 0;
	p_led_8 <: 0;
	p_led_9 <: 0;
	p_led_10_12 <: 0;
	p_led_oe_n <: 0;

	p_led_0_7 <: (unsigned)( led_value & 0x00FF );
	p_led_8 <: (unsigned)( ( led_value >> 8 ) & 0x0001 );
	p_led_9 <: (unsigned)( ( led_value >> 9 ) & 0x0001 );
	p_led_10_12 <: (unsigned)( ( led_value >> 10 ) & 0x0007 );
}

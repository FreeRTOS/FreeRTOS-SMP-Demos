// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_port_impl.h"
extern void _port_setc(port p, uint32_t c);
extern void _port_set_transfer_width(port p, size_t width);
extern port _port_alloc(unsigned id);
extern void _port_reset(port p);
extern void _port_free(port p);
extern void _port_set_buffered(port p);
extern void _port_set_unbuffered(port p);
#if USE_XCORE_CLOCK_TYPE
extern void _port_set_clock(port p, clock clk);
#endif
extern void _port_set_inout_data(port p);
extern void _port_set_out_clock(port p);
extern void _port_set_out_ready(port p, port ready_source);
extern void _port_set_invert(port p);
extern void _port_set_no_invert(port p);
extern void _port_set_sample_falling_edge(port p);
extern void _port_set_sample_rising_edge(port p);
extern void _port_set_master(port p);
extern void _port_set_slave(port p);
extern void _port_set_no_ready(port p);
extern void _port_set_ready_strobed(port p);
extern void _port_set_ready_handshake(port p);
extern int16_t _port_get_trigger_time(port p);
extern void _port_set_trigger_time(port p, int16_t t);
extern void _port_clear_trigger_time(port p);
extern void _port_set_trigger_in_equal(port p, uint32_t d);
extern void _port_set_trigger_in_not_equal(port p, uint32_t d);
extern void _port_clear_trigger_in(port p);
extern uint32_t _port_peek(port p);
extern void _port_out(port p, uint32_t data);
extern uint32_t _port_in(port p);
extern void _port_out_shift_right(port p, uint32_t *data);
extern void _port_in_shift_right(port p, uint32_t *data);
extern size_t _port_endin(port p);
extern void _port_clear_buffer(port p);

#include "xcore_c_port.h"
extern xcore_c_error_t port_alloc(port *p, port_id_t id);
extern xcore_c_error_t port_reset(port p);
extern xcore_c_error_t port_alloc_buffered(port *p, port_id_t id, size_t transfer_width);
extern xcore_c_error_t port_free(port *p);
extern xcore_c_error_t port_set_transfer_width(port p, size_t transfer_width);
extern xcore_c_error_t port_set_buffered(port p);
extern xcore_c_error_t port_set_unbuffered(port p);
#if USE_XCORE_CLOCK_TYPE
extern xcore_c_error_t port_set_clock(port p, clock clk);
#endif
extern xcore_c_error_t port_set_inout_data(port p);
extern xcore_c_error_t port_set_out_clock(port p);
extern xcore_c_error_t port_set_out_ready(port p, port ready_source);
extern xcore_c_error_t port_set_invert(port p);
extern xcore_c_error_t port_set_no_invert(port p);
extern xcore_c_error_t port_set_sample_falling_edge(port p);
extern xcore_c_error_t port_set_sample_rising_edge(port p);
extern xcore_c_error_t port_set_master(port p);
extern xcore_c_error_t port_set_slave(port p);
extern xcore_c_error_t port_set_no_ready(port p);
extern xcore_c_error_t port_set_ready_strobed(port p);
extern xcore_c_error_t port_set_ready_handshake(port p);
extern xcore_c_error_t port_get_trigger_time(port p, int16_t *ts);
extern xcore_c_error_t port_set_trigger_time(port p, int16_t count);
extern xcore_c_error_t port_clear_trigger_time(port p);
extern xcore_c_error_t port_set_trigger_in_equal(port p, uint32_t v);
extern xcore_c_error_t port_set_trigger_in_not_equal(port p, uint32_t v);
extern xcore_c_error_t port_clear_trigger_in(port p);
extern xcore_c_error_t port_peek(port p, uint32_t *data);
extern xcore_c_error_t port_out(port p, uint32_t data);
extern xcore_c_error_t port_in(port p, uint32_t *data);
extern xcore_c_error_t port_out_shift_right(port p, uint32_t *data);
extern xcore_c_error_t port_in_shift_right(port p, uint32_t *data);
extern xcore_c_error_t port_out_at_time(port p, int16_t t, uint32_t data);
extern xcore_c_error_t port_in_at_time(port p, int16_t t, uint32_t *data);
extern xcore_c_error_t port_out_shift_right_at_time(port p, int16_t t, uint32_t *data);
extern xcore_c_error_t port_in_shift_right_at_time(port p, int16_t t, uint32_t *data);
extern xcore_c_error_t port_in_when_pinseq(port p, port_type_t pt, uint32_t value, uint32_t *data);
extern xcore_c_error_t port_in_when_pinsneq(port p, port_type_t pt, uint32_t value, uint32_t *data);
extern xcore_c_error_t port_in_shift_right_when_pinseq(port p, port_type_t pt, uint32_t value, uint32_t *data);
extern xcore_c_error_t port_in_shift_right_when_pinsneq(port p, port_type_t pt, uint32_t value, uint32_t *data);
extern xcore_c_error_t port_clear_buffer(port p);
extern xcore_c_error_t port_endin(port p, size_t *num);
extern xcore_c_error_t port_force_input(port p, size_t *num, uint32_t *data);
extern xcore_c_error_t port_setup_select(port p, uint32_t enum_id);
extern xcore_c_error_t port_setup_select_callback(port p, void *data, select_callback_t func);
extern xcore_c_error_t port_setup_interrupt_callback(port p, void *data, interrupt_callback_t func);
extern xcore_c_error_t port_enable_trigger(port p);
extern xcore_c_error_t port_disable_trigger(port p);

#if USE_XCORE_CLOCK_TYPE
#include "xcore_c_port_protocol.h"
extern xcore_c_error_t port_protocol_in_handshake(port p, port readyin, port readyout, clock clk);
extern xcore_c_error_t port_protocol_out_handshake(port p, port readyin, port readyout, clock clk, uint32_t initial);
extern xcore_c_error_t port_protocol_in_strobed_master(port p, port readyout, clock clk);
extern xcore_c_error_t port_protocol_out_strobed_master(port p, port readyout, clock clk, uint32_t initial);
extern xcore_c_error_t port_protocol_in_strobed_slave(port p, port readyin, clock clk);
extern xcore_c_error_t port_protocol_out_strobed_slave(port p, port readyin, clock clk, uint32_t initial);
#endif

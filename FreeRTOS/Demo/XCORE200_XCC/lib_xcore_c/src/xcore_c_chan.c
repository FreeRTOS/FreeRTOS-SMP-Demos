// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_chan_impl.h"
extern streaming_chanend_t _s_chanend_alloc(void);
extern void _s_chanend_free(streaming_chanend_t c);
extern void _s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst);
extern void _s_chan_out_word(streaming_chanend_t c, uint32_t data);
extern void _s_chan_out_byte(streaming_chanend_t c, uint8_t data);
extern uint32_t _s_chan_in_word(streaming_chanend_t c);
extern uint8_t _s_chan_in_byte(streaming_chanend_t c);
extern void _s_chan_out_ct(streaming_chanend_t c, uint8_t ct);
extern void _s_chan_out_ct_end(streaming_chanend_t c);
extern void _s_chan_check_ct(streaming_chanend_t c, uint8_t ct);
extern void _s_chan_check_ct_end(streaming_chanend_t c);
extern void _t_chan_change_to_input(transacting_chanend_t *tc);
extern void _t_chan_change_to_output(transacting_chanend_t *tc);

#include "xcore_c_chanend.h"
extern xcore_c_error_t s_chanend_alloc(streaming_chanend_t *c);
extern xcore_c_error_t s_chanend_free(streaming_chanend_t *c);
extern xcore_c_error_t s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst);
extern streaming_chanend_t s_chanend_convert(chanend c);
extern xcore_c_error_t chanend_alloc(chanend *c);
extern xcore_c_error_t chanend_free(chanend *c);
extern chanend chanend_convert(streaming_chanend_t c);
extern xcore_c_error_t chanend_set_dest(chanend c, chanend dst);
extern xcore_c_error_t chanend_setup_select(chanend c, uint32_t enum_id);
extern xcore_c_error_t chanend_setup_select_callback(chanend c, void *data, select_callback_t func);
extern xcore_c_error_t chanend_setup_interrupt_callback(chanend c, void *data, interrupt_callback_t intrpt);
extern xcore_c_error_t chanend_enable_trigger(chanend c);
extern xcore_c_error_t chanend_disable_trigger(chanend c);

#include "xcore_c_channel_streaming.h"
extern xcore_c_error_t s_chan_alloc(streaming_channel_t *c);
extern xcore_c_error_t s_chan_free(streaming_channel_t *c);
extern xcore_c_error_t s_chan_out_word(streaming_chanend_t c, uint32_t data);
extern xcore_c_error_t s_chan_out_byte(streaming_chanend_t c, uint8_t data);
extern xcore_c_error_t s_chan_out_buf_word(streaming_chanend_t c, const uint32_t buf[], size_t n);
extern xcore_c_error_t s_chan_out_buf_byte(streaming_chanend_t c, const uint8_t buf[], size_t n);
extern xcore_c_error_t s_chan_in_word(streaming_chanend_t c, uint32_t *data);
extern xcore_c_error_t s_chan_in_byte(streaming_chanend_t c, uint8_t *data);
extern xcore_c_error_t s_chan_in_buf_word(streaming_chanend_t c, uint32_t buf[], size_t n);
extern xcore_c_error_t s_chan_in_buf_byte(streaming_chanend_t c, uint8_t buf[], size_t n);
extern xcore_c_error_t s_chan_out_ct(streaming_chanend_t c, uint8_t ct);
extern xcore_c_error_t s_chan_out_ct_end(streaming_chanend_t c);
extern xcore_c_error_t s_chan_check_ct(streaming_chanend_t c, uint8_t ct);
extern xcore_c_error_t s_chan_check_ct_end(streaming_chanend_t c);

#include "xcore_c_channel.h"
extern xcore_c_error_t chan_alloc(channel_t *c);
extern xcore_c_error_t chan_free(channel_t *c);
extern xcore_c_error_t chan_out_word(chanend c, uint32_t data);
extern xcore_c_error_t chan_out_byte(chanend c, uint8_t data);
extern xcore_c_error_t chan_out_buf_word(chanend c, const uint32_t buf[], size_t n);
extern xcore_c_error_t chan_out_buf_byte(chanend c, const uint8_t buf[], size_t n);
extern xcore_c_error_t chan_in_word(chanend c, uint32_t *data);
extern xcore_c_error_t chan_in_byte(chanend c, uint8_t *data);
extern xcore_c_error_t chan_in_buf_word(chanend c, uint32_t buf[], size_t n);
extern xcore_c_error_t chan_in_buf_byte(chanend c, uint8_t buf[], size_t n);

#include "xcore_c_channel_transaction.h"
extern xcore_c_error_t chan_init_transaction_master(chanend *c, transacting_chanend_t *tc);
extern xcore_c_error_t chan_init_transaction_slave(chanend *c, transacting_chanend_t *tc);
extern xcore_c_error_t chan_complete_transaction(chanend *c, transacting_chanend_t *tc);
extern xcore_c_error_t t_chan_out_word(transacting_chanend_t *c, uint32_t data);
extern xcore_c_error_t t_chan_out_byte(transacting_chanend_t *c, uint8_t data);
extern xcore_c_error_t t_chan_out_buf_word(transacting_chanend_t *c, const uint32_t buf[], size_t n);
extern xcore_c_error_t t_chan_out_buf_byte(transacting_chanend_t *c, const uint8_t buf[], size_t n);
extern xcore_c_error_t t_chan_in_word(transacting_chanend_t *c, uint32_t *data);
extern xcore_c_error_t t_chan_in_byte(transacting_chanend_t *c, uint8_t *data);
extern xcore_c_error_t t_chan_in_buf_word(transacting_chanend_t *c, uint32_t buf[], size_t n);
extern xcore_c_error_t t_chan_in_buf_byte(transacting_chanend_t *c, uint8_t buf[], size_t n);

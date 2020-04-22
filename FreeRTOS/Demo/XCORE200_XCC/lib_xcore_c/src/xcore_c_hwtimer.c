// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_hwtimer_impl.h"
extern void _hwtimer_free_xc_timer(void);
extern void _hwtimer_realloc_xc_timer(void);
extern hwtimer_t _hwtimer_alloc(void);
extern void _hwtimer_free(hwtimer_t t);
extern void _hwtimer_get_time(hwtimer_t t, uint32_t *now);
extern void _hwtimer_change_trigger_time(hwtimer_t t, uint32_t time);
extern void _hwtimer_set_trigger_time(hwtimer_t t, uint32_t time);
extern void _hwtimer_clear_trigger_time(hwtimer_t t);

#include "xcore_c_hwtimer.h"
extern xcore_c_error_t hwtimer_free_xc_timer(void);
extern xcore_c_error_t hwtimer_realloc_xc_timer(void);
extern xcore_c_error_t hwtimer_alloc(hwtimer_t *t);
extern xcore_c_error_t hwtimer_free(hwtimer_t *t);
extern xcore_c_error_t hwtimer_get_time(hwtimer_t t, uint32_t *now);
extern xcore_c_error_t hwtimer_set_trigger_time(hwtimer_t t, uint32_t time);
extern xcore_c_error_t hwtimer_clear_trigger_time(hwtimer_t t);
extern xcore_c_error_t hwtimer_change_trigger_time(hwtimer_t t, uint32_t time);
extern xcore_c_error_t hwtimer_wait_until(hwtimer_t t, uint32_t until, uint32_t *now);
extern xcore_c_error_t hwtimer_delay(hwtimer_t t, uint32_t period);

extern xcore_c_error_t hwtimer_setup_select(hwtimer_t t, uint32_t time, uint32_t enum_id);
extern xcore_c_error_t hwtimer_setup_select_callback(hwtimer_t t, uint32_t time, void *data, select_callback_t callback);
extern xcore_c_error_t hwtimer_setup_interrupt_callback(hwtimer_t t, uint32_t time, void *data, interrupt_callback_t intrpt);
extern xcore_c_error_t hwtimer_enable_trigger(hwtimer_t t);
extern xcore_c_error_t hwtimer_disable_trigger(hwtimer_t t);

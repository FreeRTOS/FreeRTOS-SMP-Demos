// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_clock_impl.h"
extern void _clock_set_ready_src(clock clk, port ready_source);

#include "xcore_c_clock.h"
extern xcore_c_error_t clock_alloc(clock *clk, clock_id_t id);
extern xcore_c_error_t clock_free(clock *clk);
extern xcore_c_error_t clock_start(clock clk);
extern xcore_c_error_t clock_stop(clock clk);
extern xcore_c_error_t clock_set_source_port(clock clk, port p);
extern xcore_c_error_t clock_set_source_clk_ref(clock clk);
extern xcore_c_error_t clock_set_source_clk_xcore(clock clk);
extern xcore_c_error_t clock_set_divide(clock clk, uint8_t divide);
extern xcore_c_error_t clock_set_ready_src(clock clk, port ready_source);

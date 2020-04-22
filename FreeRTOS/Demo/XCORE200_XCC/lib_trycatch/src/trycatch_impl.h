// Copyright (c) 2013-2018, XMOS Ltd, All rights reserved

#ifndef _trycatch_impl_h_
#define _trycatch_impl_h_

#include <xccompat.h>
#include <stdint.h>
#include <setjmp.h>

typedef struct typecatch_try_t {
  jmp_buf buf;
  intptr_t prev;
  intptr_t old_handler;
} typecatch_try_t;

#ifdef __XC__
#define ADDRESS_OF
#else
#define ADDRESS_OF &
#endif

#if defined(__cplusplus)
extern "C" {
#endif
void trycatch_enter(REFERENCE_PARAM(typecatch_try_t, buf));

void trycatch_exit(void);

int trycatch_catch(REFERENCE_PARAM(exception_t, exception));
#if defined(__cplusplus)
}
#endif

__attribute__((noreturn)) void trycatch_throw(unsigned type, unsigned data);

#endif //_trycatch_impl_h_

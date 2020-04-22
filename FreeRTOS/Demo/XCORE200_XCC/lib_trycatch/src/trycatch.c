// Copyright (c) 2013-2016, XMOS Ltd, All rights reserved

#include <setjmp.h>
#include "trycatch.h"
#include <xs1.h>

typedef struct trycatch_state_t {
  exception_t exception;
  typecatch_try_t *try;
} trycatch_state_t;

__thread trycatch_state_t trycatch_state;

__attribute__((noreturn)) void trycatch_throw(unsigned type, unsigned data) {
  trycatch_state.exception.type = type;
  trycatch_state.exception.data = data;
  longjmp(trycatch_state.try->buf, 2);
}

void trycatch_trap_handler();

static intptr_t getkep(void) {
  register intptr_t r11 asm("r11");
  asm volatile ("get %0, kep" : "=r"(r11));
  return r11;
}

static void setkep(intptr_t handler) {
  register intptr_t r11 asm("r11") = handler;
  asm volatile("set kep, %0" : /* no outputs */ : "r"(r11));
}

void trycatch_enter(typecatch_try_t *try) {
  // Register exception handler.
  try->old_handler = getkep();
  setkep((intptr_t)&trycatch_trap_handler);
  try->prev = (intptr_t)trycatch_state.try;
  trycatch_state.try = try;
}

void trycatch_exit(void) {
  // Restore old handler
  setkep(trycatch_state.try->old_handler);
  trycatch_state.try = (typecatch_try_t*)trycatch_state.try->prev;
}

int trycatch_catch(exception_t *exception) {
  if (trycatch_state.exception.type == XS1_ET_NONE) {
    return 0;
  }
  *exception = trycatch_state.exception;
  trycatch_state.exception.type = XS1_ET_NONE;
  return 1;
}

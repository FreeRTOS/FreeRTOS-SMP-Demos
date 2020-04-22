// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_select_impl_h__
#define __xcore_c_select_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_macros.h"
#include "xcore_c_resource_impl.h"

// _SELECT_CALLBACK_STACK_SIZE also defined in xcore_c_select.S
#ifdef __XS2A__
# define _SELECT_CALLBACK_STACK_SIZE  2
# define ISSUE_MODE_SINGLE .issue_mode single
#else
# define _SELECT_CALLBACK_STACK_SIZE  1
# define ISSUE_MODE_SINGLE
#endif

#define _SELECT_CALLBACK(callback) \
    _xcore_c_select_callback_ ## callback

#define _DECLARE_SELECT_CALLBACK(callback, data) \
    void _SELECT_CALLBACK(callback)(void);\
    void callback(void* data)

#define _DEFINE_SELECT_CALLBACK_DEF(callback) \
    .globl _xcore_c_select_callback_common; \
    .globl _SELECT_CALLBACK(callback); \
    .align _XCORE_C_CODE_ALIGNMENT; \
    .type  _SELECT_CALLBACK(callback),@function; \
    ISSUE_MODE_SINGLE; \
    .cc_top _SELECT_CALLBACK(callback).function,_SELECT_CALLBACK(callback); \
    _SELECT_CALLBACK(callback):; \
      _XCORE_C_ENTSP( _SELECT_CALLBACK_STACK_SIZE ); \
      ldap r11, callback; \
      add r1, r11, 0; \
      ldap r11, _xcore_c_select_callback_common; \
      bau r11; \
    .cc_bottom _SELECT_CALLBACK(callback).function; \
    .set   _SELECT_CALLBACK(callback).nstackwords, _SELECT_CALLBACK_STACK_SIZE + callback.nstackwords; \
    .globl _SELECT_CALLBACK(callback).nstackwords; \
    .set   _SELECT_CALLBACK(callback).maxcores, 1 $M _xcore_c_select_callback_common.maxcores $M callback.maxcores; \
    .globl _SELECT_CALLBACK(callback).maxcores; \
    .set   _SELECT_CALLBACK(callback).maxtimers, 0 $M _xcore_c_select_callback_common.maxtimers $M callback.maxtimers; \
    .globl _SELECT_CALLBACK(callback).maxtimers; \
    .set   _SELECT_CALLBACK(callback).maxchanends, 0 $M _xcore_c_select_callback_common.maxchanends $M callback.maxchanends; \
    .globl _SELECT_CALLBACK(callback).maxchanends; \
    .size  _SELECT_CALLBACK(callback), . - _SELECT_CALLBACK(callback); \

#define _DEFINE_SELECT_CALLBACK(callback, data) \
    asm(_XCORE_C_STR(_DEFINE_SELECT_CALLBACK_DEF(callback))); \
    _DECLARE_SELECT_CALLBACK(callback, data)

#endif // !defined(__XC__)

#endif // __xcore_c_select_impl_h__


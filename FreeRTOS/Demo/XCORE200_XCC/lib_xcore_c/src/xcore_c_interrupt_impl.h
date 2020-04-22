// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_interrupt_impl_h__
#define __xcore_c_interrupt_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include "xcore_c_macros.h"
#include "xcore_c_resource_impl.h"

// The user may define a larger kstack for their own worse case use.
#ifndef XCORE_C_KSTACK_WORDS
#define XCORE_C_KSTACK_WORDS 0
#endif

#ifdef __XS2A__
# define ISSUE_MODE_SINGLE .issue_mode single
#else
# define ISSUE_MODE_SINGLE
#endif

#define _INTERRUPT_PERMITTED(root_function) \
    _xcore_c_interrupt_permitted_ ## root_function

#define _DECLARE_INTERRUPT_PERMITTED(ret, root_function, ...) \
    void _INTERRUPT_PERMITTED(root_function) (__VA_ARGS__);\
    ret root_function(__VA_ARGS__)

#define _DEFINE_INTERRUPT_PERMITTED_DEF(grp, root_function) \
    .weak  _fptrgroup.grp.nstackwords.group; \
    .max_reduce _fptrgroup.grp.nstackwords, _fptrgroup.grp.nstackwords.group, 0; \
    .set _kstack_words, _XCORE_C_STACK_ALIGN(_fptrgroup.grp.nstackwords $M XCORE_C_KSTACK_WORDS); \
    .globl _xcore_c_interrupt_permitted_common; \
    .globl _INTERRUPT_PERMITTED(root_function); \
    .align _XCORE_C_CODE_ALIGNMENT; \
    .type  _INTERRUPT_PERMITTED(root_function),@function; \
    ISSUE_MODE_SINGLE; \
    .cc_top _INTERRUPT_PERMITTED(root_function).function,_INTERRUPT_PERMITTED(root_function); \
    _INTERRUPT_PERMITTED(root_function):; \
      _XCORE_C_ENTSP(_XCORE_C_STACK_ALIGN(3)); \
      stw r5, sp[2]; \
      stw r4, sp[1]; \
      ldc r4, _kstack_words; \
      ldap r11, root_function; \
      add r5, r11, 0; \
      ldap r11, _xcore_c_interrupt_permitted_common; \
      bau r11; \
    .cc_bottom _INTERRUPT_PERMITTED(root_function).function; \
    .set   _INTERRUPT_PERMITTED(root_function).nstackwords, _XCORE_C_STACK_ALIGN(3) + _kstack_words + _xcore_c_interrupt_permitted_common.nstackwords + root_function.nstackwords; \
    .globl _INTERRUPT_PERMITTED(root_function).nstackwords; \
    .set   _INTERRUPT_PERMITTED(root_function).maxcores, 1 $M _xcore_c_interrupt_permitted_common.maxcores $M root_function.maxcores; \
    .globl _INTERRUPT_PERMITTED(root_function).maxcores; \
    .set   _INTERRUPT_PERMITTED(root_function).maxtimers, 0 $M _xcore_c_interrupt_permitted_common.maxtimers $M root_function.maxtimers; \
    .globl _INTERRUPT_PERMITTED(root_function).maxtimers; \
    .set   _INTERRUPT_PERMITTED(root_function).maxchanends, 0 $M _xcore_c_select_callback_common.maxchanends $M root_function.maxchanends; \
    .globl _INTERRUPT_PERMITTED(root_function).maxchanends; \
    .size  _INTERRUPT_PERMITTED(root_function), . - _INTERRUPT_PERMITTED(root_function); \

#define _DEFINE_INTERRUPT_PERMITTED(grp, ret, root_function, ...) \
    asm(_XCORE_C_STR(_DEFINE_INTERRUPT_PERMITTED_DEF(grp, root_function))); \
    _DECLARE_INTERRUPT_PERMITTED(ret, root_function, __VA_ARGS__)


#define _INTERRUPT_CALLBACK(intrpt) \
    _xcore_c_interrupt_callback_ ## intrpt

#define _DECLARE_INTERRUPT_CALLBACK(intrpt, data) \
    void _INTERRUPT_CALLBACK(intrpt)(void);\
    void intrpt(void* data)

#define _DEFINE_INTERRUPT_CALLBACK_DEF(grp, intrpt) \
    .globl _xcore_c_interrupt_callback_common; \
    .weak _fptrgroup.grp.nstackwords.group; \
    .add_to_set _fptrgroup.grp.nstackwords.group, _INTERRUPT_CALLBACK(intrpt).nstackwords, _INTERRUPT_CALLBACK(intrpt); \
    .globl _INTERRUPT_CALLBACK(intrpt); \
    .align _XCORE_C_CODE_ALIGNMENT; \
    .type  _INTERRUPT_CALLBACK(intrpt),@function; \
    ISSUE_MODE_SINGLE; \
    .cc_top _INTERRUPT_CALLBACK(intrpt).function,_INTERRUPT_CALLBACK(intrpt); \
    _INTERRUPT_CALLBACK(intrpt):; \
      _XCORE_C_SINGLE_ISSUE /* Do we know what KEDI is set to? */; \
      kentsp _XCORE_C_STACK_ALIGN(7); \
      stw r11, sp[5]; \
      stw r1, sp[3]; \
      ldap r11, intrpt; \
      add r1, r11, 0; \
      ldap r11, _xcore_c_interrupt_callback_common; \
      bau r11; \
    .cc_bottom _INTERRUPT_CALLBACK(intrpt).function; \
    .set   _INTERRUPT_CALLBACK(intrpt).nstackwords, _XCORE_C_STACK_ALIGN(7) + _xcore_c_interrupt_callback_common.nstackwords + intrpt.nstackwords; \
    .globl _INTERRUPT_CALLBACK(intrpt).nstackwords; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxcores, 1 $M _xcore_c_interrupt_callback_common.maxcores $M intrpt.maxcores; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxcores; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxtimers, 0 $M _xcore_c_interrupt_callback_common.maxtimers $M intrpt.maxtimers; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxtimers; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxchanends, 0 $M _xcore_c_interrupt_callback_common.maxchanends $M intrpt.maxchanends; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxchanends; \
    .size  _INTERRUPT_CALLBACK(intrpt), . - _INTERRUPT_CALLBACK(intrpt); \

#define _DEFINE_INTERRUPT_CALLBACK(grp, intrpt, data) \
    asm(_XCORE_C_STR(_DEFINE_INTERRUPT_CALLBACK_DEF(grp, intrpt))); \
    _DECLARE_INTERRUPT_CALLBACK(intrpt, data)

#endif // __xcore_c_interrupt_impl_h__


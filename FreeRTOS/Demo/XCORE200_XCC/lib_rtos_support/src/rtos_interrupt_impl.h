<<<<<<< HEAD
// Copyright (c) 2020, XMOS Ltd, All rights reserved
=======
// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

/*
 * This file extends the interrupt support in lib_xcore_c to support
 * interrupts in an RTOS environment where thread context needs to
 * be saved on the stack.
 */

#ifndef RTOS_INTERRUPT_IMPL_H_
#define RTOS_INTERRUPT_IMPL_H_

<<<<<<< HEAD
#include "xcore_c_interrupt_impl.h"
#include "rtos_support_rtos_config.h"
=======
#include "rtos_support_rtos_config.h"
#include <xcore/interrupt_wrappers.h>
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

#define _DEFINE_RTOS_KERNEL_ENTRY_DEF(root_function) \
    .weak  _fptrgroup.rtos_isr.nstackwords.group; \
    .max_reduce _fptrgroup.rtos_isr.nstackwords, _fptrgroup.rtos_isr.nstackwords.group, 0; \
<<<<<<< HEAD
    .set _kstack_words, _XCORE_C_STACK_ALIGN(_fptrgroup.rtos_isr.nstackwords); \
    .globl _xcore_c_interrupt_permitted_common; \
    .globl _INTERRUPT_PERMITTED(root_function); \
    .align _XCORE_C_CODE_ALIGNMENT; \
    .type  _INTERRUPT_PERMITTED(root_function),@function; \
    .issue_mode single; \
    .cc_top _INTERRUPT_PERMITTED(root_function).function,_INTERRUPT_PERMITTED(root_function); \
    _INTERRUPT_PERMITTED(root_function):; \
      _XCORE_C_ENTSP(_XCORE_C_STACK_ALIGN(3)); \
=======
    .set _kstack_words, _XCORE_STACK_ALIGN(_fptrgroup.rtos_isr.nstackwords); \
    .globl __xcore_interrupt_permitted_common; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_PERMITTED(root_function),@function; \
    .issue_mode single; \
    .cc_top _XCORE_INTERRUPT_PERMITTED(root_function).function,_XCORE_INTERRUPT_PERMITTED(root_function); \
    _XCORE_INTERRUPT_PERMITTED(root_function):; \
      _XCORE_ENTSP(_XCORE_STACK_ALIGN(3)); \
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
      stw r5, sp[2]; \
      stw r4, sp[1]; \
      ldc r4, _kstack_words; \
      ldap r11, root_function; \
      add r5, r11, 0; \
<<<<<<< HEAD
      ldap r11, _xcore_c_interrupt_permitted_common; \
      bau r11; \
    .cc_bottom _INTERRUPT_PERMITTED(root_function).function; \
    /* The stack size for this function must be big enough for: */ \
    /*  - This wrapper function: _XCORE_C_STACK_ALIGN(3) + _xcore_c_interrupt_permitted_common.nstackwords  */ \
    /*  - The size of the stack required by the root function: root_function.nstackwords */ \
    /*  - The size of the stack required by the ISR group: _kstack_words */ \
    .set   _INTERRUPT_PERMITTED(root_function).nstackwords, _XCORE_C_STACK_ALIGN(3) + _xcore_c_interrupt_permitted_common.nstackwords + root_function.nstackwords + _kstack_words; \
    .globl _INTERRUPT_PERMITTED(root_function).nstackwords; \
    .set   _INTERRUPT_PERMITTED(root_function).maxcores, 1 $M _xcore_c_interrupt_permitted_common.maxcores $M root_function.maxcores; \
    .globl _INTERRUPT_PERMITTED(root_function).maxcores; \
    .set   _INTERRUPT_PERMITTED(root_function).maxtimers, 0 $M _xcore_c_interrupt_permitted_common.maxtimers $M root_function.maxtimers; \
    .globl _INTERRUPT_PERMITTED(root_function).maxtimers; \
    .set   _INTERRUPT_PERMITTED(root_function).maxchanends, 0 $M _xcore_c_select_callback_common.maxchanends $M root_function.maxchanends; \
    .globl _INTERRUPT_PERMITTED(root_function).maxchanends; \
    .size  _INTERRUPT_PERMITTED(root_function), . - _INTERRUPT_PERMITTED(root_function); \

#define _DEFINE_RTOS_KERNEL_ENTRY(ret, root_function, ...) \
    asm(_XCORE_C_STR(_DEFINE_RTOS_KERNEL_ENTRY_DEF(root_function))); \
    _DECLARE_INTERRUPT_PERMITTED(ret, root_function, __VA_ARGS__)

#define _DECLARE_RTOS_INTERRUPT_CALLBACK(intrpt, data) \
    void _INTERRUPT_CALLBACK(intrpt)(void);\
=======
      ldap r11, __xcore_interrupt_permitted_common; \
      bau r11; \
    .cc_bottom _XCORE_INTERRUPT_PERMITTED(root_function).function; \
    /* The stack size for this function must be big enough for: */ \
    /*  - This wrapper function: _XCORE_STACK_ALIGN(3) + __xcore_interrupt_permitted_common.nstackwords  */ \
    /*  - The size of the stack required by the root function: root_function.nstackwords */ \
    /*  - The size of the stack required by the ISR group: _kstack_words */ \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).nstackwords, _XCORE_STACK_ALIGN(3) + __xcore_interrupt_permitted_common.nstackwords + root_function.nstackwords + _kstack_words; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).nstackwords; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxcores, 1 $M __xcore_interrupt_permitted_common.maxcores $M root_function.maxcores; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxcores; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxtimers, 0 $M __xcore_interrupt_permitted_common.maxtimers $M root_function.maxtimers; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxtimers; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxchanends, 0 $M _xcore_c_select_callback_common.maxchanends $M root_function.maxchanends; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxchanends; \
    .size  _XCORE_INTERRUPT_PERMITTED(root_function), . - _XCORE_INTERRUPT_PERMITTED(root_function); \

#define _DEFINE_RTOS_KERNEL_ENTRY(ret, root_function, ...) \
    asm(RTOS_STRINGIFY(_DEFINE_RTOS_KERNEL_ENTRY_DEF(root_function))); \
    _XCORE_DECLARE_INTERRUPT_PERMITTED(ret, root_function, __VA_ARGS__)

#define _DECLARE_RTOS_INTERRUPT_CALLBACK(intrpt, data) \
    void _XCORE_INTERRUPT_CALLBACK(intrpt)(void);\
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
    void intrpt(void *data)

#define _DEFINE_RTOS_INTERRUPT_CALLBACK_DEF(intrpt) \
    .weak _fptrgroup.rtos_isr.nstackwords.group; \
<<<<<<< HEAD
    .add_to_set _fptrgroup.rtos_isr.nstackwords.group, _INTERRUPT_CALLBACK(intrpt).nstackwords, _INTERRUPT_CALLBACK(intrpt); \
    .globl _INTERRUPT_CALLBACK(intrpt); \
    .align _XCORE_C_CODE_ALIGNMENT; \
    .type  _INTERRUPT_CALLBACK(intrpt),@function; \
    .issue_mode dual; \
    .cc_top _INTERRUPT_CALLBACK(intrpt).function,_INTERRUPT_CALLBACK(intrpt); \
    _INTERRUPT_CALLBACK(intrpt):; \
=======
    .add_to_set _fptrgroup.rtos_isr.nstackwords.group, _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords, _XCORE_INTERRUPT_CALLBACK(intrpt); \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_CALLBACK(intrpt),@function; \
    .issue_mode dual; \
    .cc_top _XCORE_INTERRUPT_CALLBACK(intrpt).function,_XCORE_INTERRUPT_CALLBACK(intrpt); \
    _XCORE_INTERRUPT_CALLBACK(intrpt):; \
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
      /* Extend the stack by enough words to store the thread context. */ \
      extsp RTOS_SUPPORT_INTERRUPT_STACK_GROWTH; \
      /* We need to use R1 and R11 now so save them where the RTOS wants */ \
      /* them. The RTOS provided function rtos_interrupt_callback_common */ \
      /* will save the rest of the registers. */ \
      stw r1, sp[RTOS_SUPPORT_INTERRUPT_R1_STACK_OFFSET]; \
      stw r11, sp[RTOS_SUPPORT_INTERRUPT_R11_STACK_OFFSET]; \
      ldap r11, intrpt; \
      mov r1, r11; \
      ldap r11, rtos_interrupt_callback_common; \
      bau r11; \
<<<<<<< HEAD
    .cc_bottom _INTERRUPT_CALLBACK(intrpt).function; \
    .set   _INTERRUPT_CALLBACK(intrpt).nstackwords, intrpt.nstackwords; \
    .globl _INTERRUPT_CALLBACK(intrpt).nstackwords; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxcores, 1 $M intrpt.maxcores; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxcores; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxtimers, 0 $M intrpt.maxtimers; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxtimers; \
    .set   _INTERRUPT_CALLBACK(intrpt).maxchanends, 0 $M intrpt.maxchanends; \
    .globl _INTERRUPT_CALLBACK(intrpt).maxchanends; \
    .size  _INTERRUPT_CALLBACK(intrpt), . - _INTERRUPT_CALLBACK(intrpt); \

#define _DEFINE_RTOS_INTERRUPT_CALLBACK(intrpt, data) \
    asm(_XCORE_C_STR(_DEFINE_RTOS_INTERRUPT_CALLBACK_DEF(intrpt))); \
=======
    .cc_bottom _XCORE_INTERRUPT_CALLBACK(intrpt).function; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords, intrpt.nstackwords; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxcores, 1 $M intrpt.maxcores; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxcores; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxtimers, 0 $M intrpt.maxtimers; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxtimers; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxchanends, 0 $M intrpt.maxchanends; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxchanends; \
    .size  _XCORE_INTERRUPT_CALLBACK(intrpt), . - _XCORE_INTERRUPT_CALLBACK(intrpt); \

#define _DEFINE_RTOS_INTERRUPT_CALLBACK(intrpt, data) \
    asm(RTOS_STRINGIFY(_DEFINE_RTOS_INTERRUPT_CALLBACK_DEF(intrpt))); \
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588
    _DECLARE_RTOS_INTERRUPT_CALLBACK(intrpt, data)


#endif /* RTOS_INTERRUPT_IMPL_H_ */

// Copyright (c) 2014-2016, XMOS Ltd, All rights reserved
#ifndef __xassert_h__
#define __xassert_h__

#ifdef __xassert_conf_h_exists__
#include "xassert_conf.h"
#endif

#ifdef __debug_conf_h_exists__
#include "debug_conf.h"
#endif

#ifndef XASSERT_UNIT
#define XASSERT_UNIT APPLICATION
#endif

#ifndef XASSERT_ENABLE_ASSERTIONS
#define XASSERT_ENABLE_ASSERTIONS 1
#endif

#ifndef XASSERT_ENABLE_DEBUG
#define XASSERT_ENABLE_DEBUG 0
#endif

#ifndef XASSERT_ENABLE_LINE_NUMBERS
#define XASSERT_ENABLE_LINE_NUMBERS 0
#endif

#define XASSERT_JOIN0(x,y) x ## y
#define XASSERT_JOIN(x,y) XASSERT_JOIN0(x,y)

#if XASSERT_JOIN(XASSERT_ENABLE_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_ASSERTIONS0 1
#endif

#if XASSERT_JOIN(XASSERT_DISABLE_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_ASSERTIONS0 0
#endif

#if !defined(XASSERT_ENABLE_ASSERTIONS0)
#  define XASSERT_ENABLE_ASSERTIONS0 XASSERT_ENABLE_ASSERTIONS
#endif

#if XASSERT_JOIN(XASSERT_ENABLE_DEBUG_,XASSERT_UNIT)
#  define XASSERT_ENABLE_DEBUG0 1
#endif

#if XASSERT_JOIN(XASSERT_DISABLE_DEBUG_,XASSERT_UNIT)
#  define XASSERT_ENABLE_DEBUG0 0
#endif

#if !defined(XASSERT_ENABLE_DEBUG0)
#  define XASSERT_ENABLE_DEBUG0 XASSERT_ENABLE_DEBUG
#endif

#if XASSERT_ENABLE_DEBUG0
#  include "print.h"
#endif

#if XASSERT_ENABLE_LINE_NUMBERS
#  define xassert_print_line do { printstr(" (" __FILE__ ":");    \
                                  printint(__LINE__);             \
                                  printstr(")\n");                \
                                } while(0)
#else
#  define xassert_print_line do { printstr("\n"); } while(0)
#endif


#if XASSERT_ENABLE_ASSERTIONS0
#  if XASSERT_ENABLE_DEBUG0
#    define xassert(e) do { if (!(e)) {\
       printstr(#e); xassert_print_line; \
      __builtin_trap();} \
      } while(0)
#  else
#    define xassert(e) do { if (!(e)) __builtin_trap();} while(0)
#  endif
#else
#  define xassert(e)   // disabled
#endif

#if XASSERT_ENABLE_ASSERTIONS0
#  if XASSERT_ENABLE_DEBUG0
#    define unreachable(msg) do { printstr(msg); xassert_print_line; __builtin_trap();} while(0)
#  else
#    define unreachable(msg) do { __builtin_trap();} while(0)
#  endif
#else
#  define unreachable(msg) do { __builtin_unreachable();} while(0)
#endif

#if XASSERT_ENABLE_DEBUG0
#  define fail(msg) do { printstr(msg); xassert_print_line; __builtin_trap();} while(0)
#else
#  define fail(msg) do { __builtin_trap();} while(0)
#endif

inline int xassert_msg(const char msg[]) { return 1; }

#ifdef __XC__
#define _msg(x) xassert_msg(x)
#define  msg(x) xassert_msg(x)
#else
#define _msg(x) x
#define  msg(x) x
#endif

#if !defined(assert) && !XASSERT_DISABLE_ASSERT_DEF
#define assert(...) xassert(__VA_ARGS__)
#endif

#endif // __xassert_h__

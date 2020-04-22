// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_macros_h__
#define __xcore_c_macros_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#define _XCORE_C_STR1(...) #__VA_ARGS__
#define _XCORE_C_STR(x) _XCORE_C_STR1(x)

#ifdef __XS2A__
#define _XCORE_C_CODE_ALIGNMENT   4
#else
#define _XCORE_C_CODE_ALIGNMENT   2
#endif

#ifdef __XS2A__
#define _XCORE_C_STACK_ALIGN(n)   ((n+1)/2)*2
#else
#define _XCORE_C_STACK_ALIGN(n)   n
#endif

#ifdef __XS2A__
#define _XCORE_C_ENTSP(n)   nop; entsp n
#else
#define _XCORE_C_ENTSP(n)   entsp n
#endif

#ifdef __XS2A__
#define _XCORE_C_SINGLE_ISSUE   nop; entsp 0
#else
#define _XCORE_C_SINGLE_ISSUE
#endif

#endif // __xcore_c_macros_h__

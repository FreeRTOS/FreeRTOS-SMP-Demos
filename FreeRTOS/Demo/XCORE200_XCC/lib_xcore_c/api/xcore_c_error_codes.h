// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_error_codes_h__
#define __xcore_c_error_codes_h__

#include <xs1.h>

/** Errors returned when XCORE_C_NO_EXCEPTION policy is active
 *
 *  All errors (apart from error_none) are caught hardware exceptions.
 *  See 'The XMOS XS1/XS2 Architecture' for details regarding the exceptions.
 */
 typedef enum {
  error_none                = XS1_ET_NONE,
  error_link_error          = XS1_ET_LINK_ERROR,
  error_illegal_pc          = XS1_ET_ILLEGAL_PC,
  error_illegal_instruction = XS1_ET_ILLEGAL_INSTRUCTION,
  error_illegal_resource    = XS1_ET_ILLEGAL_RESOURCE,
  error_load_store          = XS1_ET_LOAD_STORE,
  error_illegal_ps          = XS1_ET_ILLEGAL_PS,
  error_arithmetic          = XS1_ET_ARITHMETIC,
  error_ecall               = XS1_ET_ECALL,
  error_resource_dep        = XS1_ET_RESOURCE_DEP,
  error_kcall               = XS1_ET_KCALL,
} xcore_c_error_t;

#endif // __xcore_c_error_codes_h__

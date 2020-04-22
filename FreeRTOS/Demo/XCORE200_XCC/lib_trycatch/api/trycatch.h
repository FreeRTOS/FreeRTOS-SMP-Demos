// Copyright (c) 2013-2016, XMOS Ltd, All rights reserved

#ifndef _trycatch_h_
#define _trycatch_h_

/** \file trycatch.h
 *  This file contains macros that allow you to handle hardware and
 *  thrown exceptions raised on the current logical core.
 */

/** Structure describing an exception.
 *
 *  The following hardware exception types are defined by xs1.h:
 *
 *    - XS1_ET_NONE
 *    - XS1_ET_LINK_ERROR
 *    - XS1_ET_ILLEGAL_PC
 *    - XS1_ET_ILLEGAL_INSTRUCTION
 *    - XS1_ET_ILLEGAL_RESOURCE
 *    - XS1_ET_LOAD_STORE
 *    - XS1_ET_ILLEGAL_PS
 *    - XS1_ET_ARITHMETIC
 *    - XS1_ET_ECALL
 *    - XS1_ET_RESOURCE_DEP
 *    - XS1_ET_KCALL
 *
 *  Please see 'The XMOS XS* Architecture' document for further details.
 *
 *  Runtime errors in software are trapped using an XS1_ET_ECALL hardware
 *  exception.
 *
 *  Values greater than 255 should be used for a 'thrown' exception type.
 *
 * \cond DOXYGEN_IGNORED_BLOCK
 *
 * In the above list of exception types the word error is deliberately spelt
 * with a zero to work around an xdoc bug #17453 - the exception type is really:
 * XS1_ET_LINK_ERROR
 *
 * \endcond
 */
typedef struct exception_t {
  unsigned type;  /**< Exception type */
  unsigned data;  /**< Exception data */
} exception_t;


#include "trycatch_impl.h"

/** Macro to execute a block of code catching any raised exceptions.
 *  The TRY macro must be immediately followed by a CATCH macro as
 *  follows:
 *  \code
 *  exception_t exception;
 *  TRY { ... } CATCH(exception) { ... }
 *  \endcode
 *  If an exception is raised, execution jumps to the code inside the
 *  catch block. The operand of the CATCH macro is populated with information
 *  about the raised exception. The catch block is not executed if no exception
 *  is raised.
 *
 *  The TRY and CATCH macros are implemented using setjmp() and longjmp() and
 *  have the following limitations:
 *
 *    - xCORE resources allocated inside the TRY block may not be freed if an
 *      exception is raised.
 *    - If an exception is raised the values of local variables changed
 *      inside the TRY block are indeterminate.
 *    - If the code inside the TRY block spawns task onto additional logical
 *      cores, exceptions on these logical cores will not be caught.
 *    - The compiler may remove code that has no other side effects beyond
 *      raising an exception.
 */
#define TRY \
  { \
    typecatch_try_t trycatch_try; \
    if (setjmp(trycatch_try.buf) == 0) { \
      trycatch_enter(ADDRESS_OF trycatch_try); \
      do

/** Macro for catching an exception.
 *  This must be used in conjunction with the TRY macros, see documentation of
 *  TRY for more details.
 *
 * \param exception   the unique exception variable for this TRY-CATCH block
 */
#define CATCH(exception) \
      while(0); \
    } \
    trycatch_exit(); \
  } \
  if (!trycatch_catch(ADDRESS_OF exception)) {} else


/** Macro for throwing an exception.
 *  This must only be called from within an active TRY-CATCH block as
 *  follows:
 *  \code
 *  void mayThow(void) {
 *    exception_t e = {256,0};
 *    THROW(e);
 *  }
 *  void noThow(void) {
 *    exception_t e;
 *    TRY
 *      mayThow();
 *    CATCH(e)
 *      assert(e.type == 256);
 *  }
 *  \endcode
 *
 *  \param exception   the exception to be thrown
 */
#define THROW(exception) \
  trycatch_throw(exception.type, exception.data)

#endif //_trycatch_h_

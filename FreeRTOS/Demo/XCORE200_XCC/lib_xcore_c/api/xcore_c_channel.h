// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_h__
#define __xcore_c_channel_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include "xcore_c_channel_streaming.h"
#include "xcore_c_chan_impl.h"
#include "xcore_c_exception_impl.h"

/** Helper type for passing around both ends of a channel.
*/
typedef streaming_channel_t channel_t;

/** Allocate a channel.
 *
 *  This function allocates two hardware chan-ends and joins them.
 *  If there are not enough chan-ends available the function returns a
 *  channel_t with both fields set to 0.
 *  When the channel_t is no longer required, chan_free() must be
 *  called to deallocate it.
 *
 *  **The chan-ends must be accessed on the same tile**
 *
 *  \param c    channel_t variable holding the two initialised and
 *              joined chan-ends or 0s.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline xcore_c_error_t chan_alloc(channel_t *c)
{
  return s_chan_alloc((streaming_channel_t*)c);
}

/** Deallocate a channel.
 *
 *  This function frees the two hardware chan-ends.
 *
 *  \param c  channel_t to free
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chanend.
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline xcore_c_error_t chan_free(channel_t *c)
{
  // Not implemented in terms of s_chan_free() as we have already hand-shook a CT_END.
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chanend_free(c->end_a); \
                                c->end_a = 0; \
                                _s_chanend_free(c->end_b); \
                                c->end_b = 0; \
                              } while (0) );
}

/** Output a word over a channel.
 *
 *  \param c    The chan-end
 *
 *  \param data The word to be output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error_t chan_out_word(chanend c, uint32_t data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_word(c, data); \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                              } while (0) );
}

/** Output a byte over a channel.
 *
 *  \param c    The chan-end
 *
 *  \param data The byte to be output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error_t chan_out_byte(chanend c, uint8_t data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_byte(c, data); \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                              } while (0) );
}

/** Output a block of data over a channel.
 *
 *  \param c    The chan-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of words to send
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline xcore_c_error_t chan_out_buf_word(chanend c, const uint32_t buf[], size_t n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                                for (size_t i = 0; i < n; i++) { \
                                  _s_chan_out_word(c, buf[i]); \
                                } \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                              } while (0) );
}

/** Output a block of data over a channel.
 *
 *  \param c    The chan-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of bytes to send
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline xcore_c_error_t chan_out_buf_byte(chanend c, const uint8_t buf[], size_t n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                                for (size_t i = 0; i < n; i++) { \
                                  _s_chan_out_byte(c, buf[i]); \
                                } \
                                _s_chan_out_ct_end(c); \
                                _s_chan_check_ct_end(c); \
                              } while (0) );
}

/** Input a word from a channel.
 *
 *  \param c    The chan-end
 *
 *  \param data The inputted word
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t chan_in_word(chanend c, uint32_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_ct_end(c); \
                                *data = _s_chan_in_word(c); \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_ct_end(c); \
                              } while (0) );
}

/** Input a byte from a channel.
 *
 *  \param c    The chan-end
 *
 *  \param data The inputted byte
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *\*data* argument.
 */
inline xcore_c_error_t chan_in_byte(chanend c, uint8_t *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_ct_end(c); \
                                *data = _s_chan_in_byte(c); \
                                _s_chan_check_ct_end(c); \
                                _s_chan_out_ct_end(c); \
                              } while (0) );
}

/** Input a block of data from a channel.
 *
 *  \param c    The chan-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of words to receive
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline xcore_c_error_t chan_in_buf_word(chanend c, uint32_t buf[], size_t n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_check_ct_end(c);
                                _s_chan_out_ct_end(c);
                                for (size_t i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_word(c); \
                                } \
                                _s_chan_check_ct_end(c);
                                _s_chan_out_ct_end(c);
                              } while (0) );
}

/** Input a block of data from a channel.
 *
 *  \param c    The chan-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline xcore_c_error_t chan_in_buf_byte(chanend c, uint8_t buf[], size_t n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _s_chan_check_ct_end(c);
                                _s_chan_out_ct_end(c);
                                for (size_t i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_byte(c); \
                                } \
                                _s_chan_check_ct_end(c);
                                _s_chan_out_ct_end(c);
                              } while (0) );
}

#endif // !defined(__XC__)

#endif // __xcore_c_channel_h__

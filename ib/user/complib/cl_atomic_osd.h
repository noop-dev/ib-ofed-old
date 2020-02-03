/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 *
 * This software is available to you under the OpenIB.org BSD license
 * below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * $Id: cl_atomic_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_ATOMIC_OSD_H_
#define _CL_ATOMIC_OSD_H_


#include "cl_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE int32_t CL_API
cl_atomic_inc(
	IN	atomic32_t* const	p_value )
{
	return( InterlockedIncrement( (LONG*)p_value ) );
}


CL_INLINE int32_t CL_API
cl_atomic_dec(
	IN	atomic32_t* const	p_value )
{
	return( InterlockedDecrement( (LONG*)p_value ) );
}


CL_INLINE int32_t CL_API
cl_atomic_add(
	IN	atomic32_t* const	p_value,
	IN	const int32_t		increment )
{
	/* Return the incremented value. */
	return( InterlockedExchangeAdd( (long*)p_value, increment ) + increment );
}


CL_INLINE int32_t CL_API
cl_atomic_sub(
	IN	atomic32_t* const	p_value,
	IN	const int32_t		decrement )
{
	/* Return the decremented value. */
	return( InterlockedExchangeAdd( (long*)p_value, -decrement ) - decrement );
}


CL_INLINE int32_t CL_API
cl_atomic_xchg(
	IN	atomic32_t* const	p_value,
	IN	const int32_t		new_value )
{
	return( InterlockedExchange( (long*)p_value, new_value ) );
}


CL_INLINE int32_t CL_API
cl_atomic_comp_xchg(
	IN	atomic32_t* const	p_value,
	IN	const int32_t		compare,
	IN	const int32_t		new_value )
{
	return( InterlockedCompareExchange( (long*)p_value, new_value, compare ) );
}


#ifdef __cplusplus
}	// extern "C"
#endif

#endif // _CL_ATOMIC_OSD_H_

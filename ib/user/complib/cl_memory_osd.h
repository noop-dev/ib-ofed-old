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
 * $Id: cl_memory_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Defines platform specific memory related functions.
 *
 * Environment:
 *	Windows User Mode
 */


#ifndef _CL_MEMORY_OSD_H_
#define _CL_MEMORY_OSD_H_


#include <complib/cl_types.h>


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE void CL_API
cl_memset(
	IN	void* const		p_memory, 
	IN	const uint8_t	fill, 
	IN	const size_t	count )
{
	RtlFillMemory( p_memory, count, fill );
}


CL_INLINE void* CL_API
cl_memcpy(
	IN	void* const			p_dest, 
	IN	const void* const	p_src, 
	IN	const size_t		count )
{
	RtlCopyMemory( p_dest, p_src, count );
	return p_dest;
}


CL_INLINE int32_t CL_API
cl_memcmp(
	IN	const void* const	p_memory1,
	IN	const void* const	p_memory2, 
	IN	const size_t		count )
{
	return( memcmp( p_memory1, p_memory2, count ) );
}


#define _CL_MEMCLR_DEFINED_
CL_INLINE void CL_API
cl_memclr(
	IN	void* const		p_memory,
	IN	const size_t	count )
{
	RtlSecureZeroMemory( p_memory, count );
}


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* _CL_MEMORY_OSD_H_ */


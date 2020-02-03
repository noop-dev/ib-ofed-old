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
 *	Defines kernel-only memory related functions.
 *
 * Environment:
 *	Windows Kernel Mode
 */


#ifndef _CL_MEMORY_OSD_H_
#define _CL_MEMORY_OSD_H_


#include <complib/cl_types.h>


#ifdef __cplusplus
extern "C"
{
#endif

	
CL_INLINE void
cl_memset( 
	IN	void* const		p_memory, 
	IN	const uint8_t	fill, 
	IN	const size_t	count )
{
	RtlFillMemory( p_memory, count, fill );
}


CL_INLINE void*
cl_memcpy( 
	IN	void* const			p_dest, 
	IN	const void* const	p_src, 
	IN	const size_t		count )
{
	RtlCopyMemory( p_dest, p_src, count );
	return p_dest;
}


CL_INLINE int32_t
cl_memcmp( 
	IN	const void* const	p_memory1,
	IN	const void* const	p_memory2, 
	IN	const size_t		count )
{
	return( memcmp( p_memory1, p_memory2, count ) );
}


CL_INLINE uint32_t
cl_get_pagesize( void )
{
	return (PAGE_SIZE);
}


#ifdef CL_NTDDK
CL_INLINE uint64_t
cl_get_physaddr(
	IN	void *vaddr )
{
	return MmGetPhysicalAddress( vaddr ).QuadPart;
}
#endif


CL_INLINE cl_status_t
cl_check_for_read(
	IN	const void* const	vaddr,
	IN	const size_t		count )
{
	__try
	{
		ProbeForRead( ( void *)vaddr, count, sizeof(void*) );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return CL_INVALID_PERMISSION;
	}
	return CL_SUCCESS;
}


CL_INLINE cl_status_t
cl_check_for_write(
	IN	void* const		vaddr,
	IN	const size_t	count )
{
	__try
	{
		/*
		 * We use ProbeForRead instead of ProbeForWrite because
		 * the additional checks to make sure the pages can be written
		 * are not guaranteed to still hold by the time we copy into
		 * the buffer.
		 *
		 * Furthermore, even if the pages don't change, ProbeForWrite will
		 * cause all pages to be paged in, and these pages could be paged out
		 * before the copy, requiring the copy to page them in once again.
		 *
		 * Micky Snir (mailto:mickys@microsoft.com) recommended *not* using
		 * ProbeForWrite because the page validity/permissions can change after
		 * the call and the actual access.
		 */
		ProbeForRead( vaddr, count, sizeof(void*) );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return CL_INVALID_PERMISSION;
	}
	return CL_SUCCESS;
}


CL_INLINE cl_status_t
cl_copy_to_user(
	IN	void* const			p_dest,
	IN	const void* const	p_src,
	IN	const size_t		count )
{
	/*
	 * The memory copy must be done within a try/except block as the
	 * memory could be changing while the buffer is copied.
	 */
	__try
	{
		/*
		 * We use ProbeForRead instead of ProbeForWrite because
		 * the additional checks to make sure the pages can be written
		 * are not guaranteed to still hold by the time we copy into
		 * the buffer.
		 *
		 * Furthermore, even if the pages don't change, ProbeForWrite will
		 * cause all pages to be paged in, and these pages could be paged out
		 * before the copy, requiring the copy to page them in once again.
		 *
		 * Micky Snir (mailto:mickys@microsoft.com) recommended *not* using
		 * ProbeForWrite because the page validity/permissions can change after
		 * the call and the actual access.
		 */
		ProbeForRead( p_dest, count, 1 );
		cl_memcpy( p_dest, p_src, count );
		return CL_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return CL_INVALID_PERMISSION;
	}
}


CL_INLINE cl_status_t
cl_copy_from_user(
	IN	void* const			p_dest,
	IN	const void* const	p_src,
	IN	const size_t		count )
{
	/*
	 * The memory copy must be done within a try/except block as the
	 * memory could be changing while the buffer is copied.
	 */
	__try
	{
		ProbeForRead( (void*)p_src, count, 1 );
		cl_memcpy( p_dest, p_src, count );
		return CL_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return CL_INVALID_PERMISSION;
	}
}


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* _CL_MEMORY_OSD_H_ */


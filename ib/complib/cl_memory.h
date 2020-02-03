/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Copyright (c) 1996-2003 Intel Corporation. All rights reserved. 
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
 * $Id: cl_memory.h 4993 2009-10-25 09:07:53Z xalex $
 */


/*
 * Abstract:
 *	Declaration of generic memory allocation calls.
 *
 * Environment:
 *	All
 */


#ifndef _CL_MEMORY_H_
#define _CL_MEMORY_H_


#include <complib/cl_types.h>
#include <complib/cl_memory_osd.h>


/****h* Public/Memory Management
* NAME
*	Memory Management
*
* DESCRIPTION
*	The memory management functionality provides memory manipulation
*	functions as well as powerful debugging tools.
*
*	The Allocation Tracking functionality provides a means for tracking memory
*	allocations in order to detect memory leaks.
*
*	Memory allocation tracking stores the file name and line number where
*	allocations occur. Gathering this information does have an adverse impact
*	on performance, and memory tracking should therefore not be enabled in
*	release builds of software.
*
*	Memory tracking is compiled into the debug version of the library,
*	and can be enabled for the release version as well. To Enable memory
*	tracking in a release build of the public layer, users should define
*	the MEM_TRACK_ON keyword for compilation.
*********/


#ifdef __cplusplus
extern "C"
{
#endif


/****i* Component Library: Memory Management/__cl_mem_track
* NAME
*	__cl_mem_track
*
* DESCRIPTION
*	The __cl_mem_track function enables or disables memory allocation tracking.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
__cl_mem_track(
	IN	const boolean_t	start );
/*
* PARAMETERS
*	start
*		[in] Specifies whether to start or stop memory tracking.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	This function performs all necessary initialization for tracking
*	allocations.  Users should never call this function, as it is called by
*	the component library framework.
*
*	If the Start parameter is set to TRUE, the function starts tracking memory
*	usage if not already started. When set to FALSE, memory tracking is stoped
*	and all remaining allocations are displayed to the applicable debugger, if
*	any.
*
*	Starting memory tracking when it is already started has no effect.
*	Likewise, stoping memory tracking when it is already stopped has no effect.
*
* SEE ALSO
*	Memory Management, cl_mem_display
**********/


/****f* Component Library: Memory Management/cl_mem_display
* NAME
*	cl_mem_display
*
* DESCRIPTION
*	The cl_mem_display function displays all tracked memory allocations to
*	the applicable debugger.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_mem_display( void );
/*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Each tracked memory allocation is displayed along with the file name and
*	line number that allocated it.
*
*	Output is sent to the platform's debugging target, which may be the
*	system log file.
*
* SEE ALSO
*	Memory Management
**********/


/****i* Component Library: Memory Management/__cl_malloc_trk
* NAME
*	__cl_malloc_trk
*
* DESCRIPTION
*	The __cl_malloc_trk function allocates and tracks a block of memory.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
__cl_malloc_trk(
	IN	const char* const	p_file_name,
	IN	const int32_t		line_num,
	IN	const size_t		bytes,
	IN	const boolean_t		pageable );
/*
* PARAMETERS
*	p_file_name
*		[in] Name of the source file initiating the allocation.
*
*	line_num
*		[in] Line number in the specified file where the allocation is
*		initiated
*
*	size
*		[in] Size of the requested allocation.
*
*	pageable
*		[in] On operating systems that support pageable vs. non pageable
*		memory in the kernel, set to TRUE to allocate memory from paged pool.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*	This function is should not be called directly.  The cl_malloc macro will
*	redirect users to this function when memory tracking is enabled.
*
* SEE ALSO
*	Memory Management, __cl_malloc_ntrk, __cl_zalloc_trk, __cl_free_trk
**********/


/****i* Component Library: Memory Management/__cl_zalloc_trk
* NAME
*	__cl_zalloc_trk
*
* DESCRIPTION
*	The __cl_zalloc_trk function allocates and tracks a block of memory
*	initialized to zero.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
__cl_zalloc_trk(
	IN	const char* const	p_file_name,
	IN	const int32_t		line_num,
	IN	const size_t		bytes,
	IN	const boolean_t		pageable );
/*
* PARAMETERS
*	p_file_name
*		[in] Name of the source file initiating the allocation.
*
*	line_num
*		[in] Line number in the specified file where the allocation is
*		initiated
*
*	size
*		[in] Size of the requested allocation.
*
*	pageable
*		[in] On operating systems that support pageable vs. non pageable
*		memory in the kernel, set to TRUE to allocate memory from paged pool.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*	This function should not be called directly.  The cl_zalloc macro will
*	redirect users to this function when memory tracking is enabled.
*
* SEE ALSO
*	Memory Management, __cl_zalloc_ntrk, __cl_malloc_trk, __cl_free_trk
**********/


/****i* Component Library: Memory Management/__cl_malloc_ntrk
* NAME
*	__cl_malloc_ntrk
*
* DESCRIPTION
*	The __cl_malloc_ntrk function allocates a block of memory.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
__cl_malloc_ntrk(
	IN	const size_t		size,
	IN	const boolean_t		pageable );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
*	pageable
*		[in] On operating systems that support pageable vs. non pageable
*		memory in the kernel, set to TRUE to allocate memory from paged pool.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*	This function is should not be called directly.  The cl_malloc macro will
*	redirect users to this function when memory tracking is not enabled.
*
* SEE ALSO
*	Memory Management, __cl_malloc_trk, __cl_zalloc_ntrk, __cl_free_ntrk
**********/


/****i* Component Library: Memory Management/__cl_zalloc_ntrk
* NAME
*	__cl_zalloc_ntrk
*
* DESCRIPTION
*	The __cl_zalloc_ntrk function allocates a block of memory
*	initialized to zero.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
__cl_zalloc_ntrk(
	IN	const size_t		bytes,
	IN	const boolean_t		pageable );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
*	pageable
*		[in] On operating systems that support pageable vs. non pageable
*		memory in the kernel, set to TRUE to allocate memory from paged pool.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*	This function should not be called directly.  The cl_zalloc macro will
*	redirect users to this function when memory tracking is not enabled.
*
* SEE ALSO
*	Memory Management, __cl_zalloc_trk, __cl_malloc_ntrk, __cl_free_ntrk
**********/


/****i* Component Library: Memory Management/__cl_free_trk
* NAME
*	__cl_free_trk
*
* DESCRIPTION
*	The __cl_free_trk function deallocates a block of tracked memory.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
__cl_free_trk(
	IN	void* const	p_memory );
/*
* PARAMETERS
*	p_memory
*		[in] Pointer to a memory block.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	The p_memory parameter is the pointer returned by a previous call to
*	__cl_malloc_trk, or __cl_zalloc_trk.
*
*	__cl_free_trk has no effect if p_memory is NULL.
*
*	This function should not be called directly.  The cl_free macro will
*	redirect users to this function when memory tracking is enabled.
*
* SEE ALSO
*	Memory Management, __cl_free_ntrk, __cl_malloc_trk, __cl_zalloc_trk
**********/


/****i* Component Library: Memory Management/__cl_free_ntrk
* NAME
*	__cl_free_ntrk
*
* DESCRIPTION
*	The __cl_free_ntrk function deallocates a block of memory.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
__cl_free_ntrk(
	IN	void* const	p_memory );
/*
* PARAMETERS
*	p_memory
*		[in] Pointer to a memory block.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	The p_memory parameter is the pointer returned by a previous call to
*	__cl_malloc_ntrk, or __cl_zalloc_ntrk.
*
*	__cl_free_ntrk has no effect if p_memory is NULL.
*
*	This function should not be called directly.  The cl_free macro will
*	redirect users to this function when memory tracking is not enabled.
*
* SEE ALSO
*	Memory Management, __cl_free_ntrk, __cl_malloc_trk, __cl_zalloc_trk
**********/


/****f* Component Library: Memory Management/cl_malloc
* NAME
*	cl_malloc
*
* DESCRIPTION
*	The cl_malloc function allocates a block of memory.
*
* SYNOPSIS
*/
void*
cl_malloc(
	IN	const size_t	size );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*
* SEE ALSO
*	Memory Management, cl_free, cl_zalloc, cl_palloc, cl_pzalloc,
*	cl_memset, cl_memclr, cl_memcpy, cl_memcmp
**********/


/****f* Component Library: Memory Management/cl_zalloc
* NAME
*	cl_zalloc
*
* DESCRIPTION
*	The cl_zalloc function allocates a block of memory initialized to zero.
*
* SYNOPSIS
*/
void*
cl_zalloc(
	IN	const size_t	size );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*
* SEE ALSO
*	Memory Management, cl_free, cl_malloc, cl_palloc, cl_pzalloc,
*	cl_memset, cl_memclr, cl_memcpy, cl_memcmp
**********/


/****f* Component Library: Memory Management/cl_palloc
* NAME
*	cl_palloc
*
* DESCRIPTION
*	The cl_palloc function allocates a block of memory from paged pool if the
*	operating system supports it.  If the operating system does not distinguish
*	between pool types, cl_palloc is identical to cl_malloc.
*
* SYNOPSIS
*/
void*
cl_palloc(
	IN	const size_t	size );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*
* SEE ALSO
*	Memory Management, cl_free, cl_malloc, cl_zalloc, cl_pzalloc,
*	cl_memset, cl_memclr, cl_memcpy, cl_memcmp
**********/


/****f* Component Library: Memory Management/cl_pzalloc
* NAME
*	cl_pzalloc
*
* DESCRIPTION
*	The cl_pzalloc function allocates a block of memory from paged pool if the
*	operating system supports it and initializes it to zero.  If the operating
*	system does not distinguish between pool types, cl_pzalloc is identical
*	to cl_zalloc.
*
* SYNOPSIS
*/
void*
cl_pzalloc(
	IN	const size_t	size );
/*
* PARAMETERS
*	size
*		[in] Size of the requested allocation.
*
* RETURN VALUES
*	Pointer to allocated memory if successful.
*
*	NULL otherwise.
*
* NOTES
*	Allocated memory follows alignment rules specific to the different
*	environments.
*
* SEE ALSO
*	Memory Management, cl_free, cl_malloc, cl_zalloc, cl_palloc,
*	cl_memset, cl_memclr, cl_memcpy, cl_memcmp
**********/


/****f* Component Library: Memory Management/cl_free
* NAME
*	cl_free
*
* DESCRIPTION
*	The cl_free function deallocates a block of memory.
*
* SYNOPSIS
*/
void
cl_free(
	IN	void* const	p_memory );
/*
* PARAMETERS
*	p_memory
*		[in] Pointer to a memory block.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	The p_memory parameter is the pointer returned by a previous call to
*	cl_malloc, or cl_zalloc.
*
*	cl_free has no effect if p_memory is NULL.
*
* SEE ALSO
*	Memory Management, cl_alloc, cl_zalloc
**********/


/****f* Component Library: Memory Management/cl_memset
* NAME
*	cl_memset
*
* DESCRIPTION
*	The cl_memset function sets every byte in a memory range to a given value.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_memset(
	IN	void* const		p_memory,
	IN	const uint8_t	fill,
	IN	const size_t	count );
/*
* PARAMETERS
*	p_memory
*		[in] Pointer to a memory block.
*
*	fill
*		[in] Byte value with which to fill the memory.
*
*	count
*		[in] Number of bytes to set.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Memory Management, cl_memclr, cl_memcpy, cl_memcmp
**********/


#ifndef _CL_MEMCLR_DEFINED_
/****f* Component Library: Memory Management/cl_memclr
* NAME
*	cl_memclr
*
* DESCRIPTION
*	The cl_memclr function sets every byte in a memory range to zero.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_memclr(
	IN	void* const		p_memory,
	IN	const size_t	count )
{
	cl_memset( p_memory, 0, count );
}
/*
* PARAMETERS
*	p_memory
*		[in] Pointer to a memory block.
*
*	count
*		[in] Number of bytes to set.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Memory Management, cl_memset, cl_memcpy, cl_memcmp
**********/
#endif


/****f* Component Library: Memory Management/cl_memcpy
* NAME
*	cl_memcpy
*
* DESCRIPTION
*	The cl_memcpy function copies a given number of bytes from
*	one buffer to another.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
cl_memcpy(
	IN	void* const			p_dest,
	IN	const void* const	p_src,
	IN	const size_t		count );
/*
* PARAMETERS
*	p_dest
*		[in] Pointer to the buffer being copied to.
*
*	p_src
*		[in] Pointer to the buffer being copied from.
*
*	count
*		[in] Number of bytes to copy from the source buffer to the
*		destination buffer.
*
* RETURN VALUE
*	Returns a pointer to the destination buffer.
*
* SEE ALSO
*	Memory Management, cl_memset, cl_memclr, cl_memcmp
**********/


/****f* Component Library: Memory Management/cl_memcmp
* NAME
*	cl_memcmp
*
* DESCRIPTION
*	The cl_memcmp function compares two memory buffers.
*
* SYNOPSIS
*/
CL_EXPORT int32_t CL_API
cl_memcmp(
	IN	const void* const	p_mem,
	IN	const void* const	p_ref,
	IN	const size_t		count );
/*
* PARAMETERS
*	p_mem
*		[in] Pointer to a memory block being compared.
*
*	p_ref
*		[in] Pointer to the reference memory block to compare against.
*
*	count
*		[in] Number of bytes to compare.
*
* RETURN VALUES
*	Returns less than zero if p_mem is less than p_ref.
*
*	Returns greater than zero if p_mem is greater than p_ref.
*
*	Returns zero if the two memory regions are the identical.
*
* SEE ALSO
*	Memory Management, cl_memset, cl_memclr, cl_memcpy
**********/


#ifdef CL_KERNEL

/****f* Component Library: Memory Management/cl_get_pagesize
* NAME
*	cl_get_pagesize
*
* DESCRIPTION
*	Returns the number of bytes in a OS defined page.
*
* SYNOPSIS
*/
CL_EXPORT uint32_t CL_API
cl_get_pagesize( void );
/*
* PARAMETERS
*	NONE
*
* RETURN VALUES
*	Returns the number of bytes in a page as defined by the Operating
*	System.
*
* SEE ALSO
*	Memory Management
**********/


/****f* Component Library: Memory Management/cl_get_physaddr
* NAME
*	cl_get_physaddr
*
* DESCRIPTION
*	Returns the Physical address for a kernel virtual address.
*
* SYNOPSIS
*/
CL_EXPORT uint64_t CL_API
cl_get_physaddr(
	IN	void *vaddr );
/*
* PARAMETERS
*	p_addr
*		[in] Pointer to virtual to which the physical address is required.
*
* RETURN VALUES
*	Returns the physical address for a virtual address.
*
* NOTES
*	This call is only available in kernel mode.
*
* SEE ALSO
*	Memory Management
**********/


/****f* Component Library: Memory Management/cl_check_for_read
* NAME
*	cl_check_for_read
*
* DESCRIPTION
*	Checks a user-mode virtual address for read access.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_check_for_read(
	IN	const void* const	vaddr,
	IN	const size_t		count );
/*
* PARAMETERS
*	vaddr
*		[in] Virtual address to check for read access.
*
*	count
*		[in] Number of bytes of the buffer at the specified address
*		to validate.
*
* RETURN VALUES
*	CL_SUCCESS if the virtual address is valid for a read of the specified
*	size.
*
*	CL_INVALID_PERMISSION if the virtual address or the size is not valid.
*
* NOTES
*	This call is only available in the kernel.  The buffer can only be accessed
*	in the context of the application thread (i.e. in the path of an IOCTL
*	request).  Callers cannot be holding a spinlock when calling this function.
*
* SEE ALSO
*	Memory Management, cl_check_for_write, cl_copy_to_user, cl_copy_from_user
*********/


/****f* Component Library: Memory Management/cl_check_for_write
* NAME
*	cl_check_for_write
*
* DESCRIPTION
*	Checks a user-mode virtual address for write access.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_check_for_write(
	IN	void* const		vaddr,
	IN	const size_t	count );
/*
* PARAMETERS
*	vaddr
*		[in] Virtual address to check for write access.
*
*	count
*		[in] Number of bytes of the buffer at the specified
*		address to validate.
*
* RETURN VALUES
*	CL_SUCCESS if the virtual address is valid for a write of the specified
*	size.
*
*	CL_INVALID_PERMISSION if the virtual address or the size is not valid.
*
* NOTES
*	This call is only available in the kernel.  The buffer can only be accessed
*	in the context of the application thread (i.e. in the path of an IOCTL
*	request).  Callers cannot be holding a spinlock when calling this function.
*
* SEE ALSO
*	Memory Management, cl_check_for_read, cl_copy_to_user, cl_copy_from_user
*********/


/****f* Component Library: Memory Management/cl_copy_to_user
* NAME
*	cl_copy_to_user
*
* DESCRIPTION
*	Copies data into a user-mode buffer, performing access checks.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_copy_to_user(
	IN	void* const			p_dest,
	IN	const void* const	p_src,
	IN	const size_t		count );
/*
* PARAMETERS
*	p_dest
*		[in] User-mode virtual address to which to copy data.
*
*	p_src
*		[in] Pointer to the buffer being copied from.
*
*	count
*		[in] Number of bytes to copy from the source buffer to the
*		destination buffer.
*
* RETURN VALUES
*	CL_SUCCESS if the user-mode buffer virtual address is valid as the
*	destination of the copy.
*
*	CL_INVALID_PERMISSION if the virtual address or the count is not valid.
*
* NOTES
*	This call is only available in the kernel.  The buffer can only be accessed
*	in the context of the application thread (i.e. in the path of an IOCTL
*	request).  Callers cannot be holding a spinlock when calling this function.
*
* SEE ALSO
*	Memory Management, cl_check_for_read, cl_check_for_write, cl_copy_from_user
*********/


/****f* Component Library: Memory Management/cl_copy_from_user
* NAME
*	cl_copy_from_user
*
* DESCRIPTION
*	Copies data from a user-mode buffer, performing access checks.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_copy_from_user(
	IN	void* const			p_dest,
	IN	const void* const	p_src,
	IN	const size_t		count );
/*
* PARAMETERS
*	p_dest
*		[in] Pointer to the buffer being copied to.
*
*	p_src
*		[in] User-mode virtual address from which to copy data.
*
*	count
*		[in] Number of bytes to copy from the source buffer to the
*		destination buffer.
*
* RETURN VALUES
*	CL_SUCCESS if the user-mode buffer virtual address is valid as the
*	source of the copy.
*
*	CL_INVALID_PERMISSION if the virtual address or the count is not valid.
*
* NOTES
*	This call is only available in the kernel.  The buffer can only be accessed
*	in the context of the application thread (i.e. in the path of an IOCTL
*	request).  Callers cannot be holding a spinlock when calling this function.
*
* SEE ALSO
*	Memory Management, cl_check_for_read, cl_check_for_write, cl_copy_to_user
*********/

#endif	/* CL_KERNEL */

#if defined( CL_NO_TRACK_MEM ) && defined( CL_TRACK_MEM )
	#error Conflict: Cannot define both CL_NO_TRACK_MEM and CL_TRACK_MEM.
#endif

/*
 * Turn on memory allocation tracking in debug builds if not explicitly
 * disabled or already turned on.
 */
#if defined( _DEBUG_ ) && \
	!defined( CL_NO_TRACK_MEM ) && \
	!defined( CL_TRACK_MEM )
	#define CL_TRACK_MEM
#endif


/*
 * Define allocation macro.
 */

/*	From WinDDK: "Avoid calling ExAllocatePoolWithTag with memory size == 0. 
	Doing so will result in pool header wastage"
 	Verifier with low mem simulation will crash with  memory size == 0
*/
#define ExAllocatePoolWithTagSafeEx( pageable, size, tag ) \
			(size == 0 ? NULL : ExAllocatePoolWithTag(pageable, size, tag))

#define ExAllocatePoolWithTagSafeExNonPaged(size, tag ) \
			(size == 0 ? NULL : ExAllocatePoolWithTag(NonPagedPool, size, tag ))		
			
#define ExAllocatePoolWithTagSafeExPaged(size, tag ) \
				(size == 0 ? NULL : ExAllocatePoolWithTag(PagedPool, size, tag ))


#if defined( CL_TRACK_MEM )

#define cl_malloc( a )	\
	__cl_malloc_trk( __FILE__, __LINE__, a, FALSE )

#define cl_zalloc( a )	\
	__cl_zalloc_trk( __FILE__, __LINE__, a, FALSE )

#define cl_palloc( a )	\
	__cl_malloc_trk( __FILE__, __LINE__, a, TRUE )

#define cl_pzalloc( a )	\
	__cl_zalloc_trk( __FILE__, __LINE__, a, TRUE )

#define cl_free( a )	\
	__cl_free_trk( a )

#else	/* !defined( CL_TRACK_MEM ) */

#define cl_malloc( a )	\
	__cl_malloc_ntrk( a, FALSE )

#define cl_zalloc( a )	\
	__cl_zalloc_ntrk( a, FALSE )

#define cl_palloc( a )	\
	__cl_malloc_ntrk( a, TRUE )

#define cl_pzalloc( a )	\
	__cl_zalloc_ntrk( a, TRUE )

#define cl_free( a )	\
	__cl_free_ntrk( a )

#endif	/* defined( CL_TRACK_MEM ) */


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _CL_MEMORY_H_ */

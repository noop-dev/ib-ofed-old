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
 * $Id: cl_syscallback.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	System Callback abstractions.
 *
 * Environment:
 *	All
 */


#ifndef _CL_SYS_CALLBACK_H_
#define _CL_SYS_CALLBACK_H_


#include <complib/cl_types.h>

/****h* Component Library/System Callback
* NAME
*	System Callback
*
* DESCRIPTION
*	The System Callback provider uses threads from a system thread-pool to
*	invoke specified callback functions.
*
*	Callbacks can be queued in a low- or high-priority queue for processing.
*
*	cl_thread_suspend and cl_thread_stall can be used to delay or stall the
*	callback thread.
*
*	Environments that do not have a native system thread-pool emulate this
*	functionality to provide cross-environment support.
*
*	The cl_sys_callback_item_t structure should be treated as opaque and be
*	manipulated only through the provided functions.
*********/


/****d* Component Library: System Callback/cl_pfn_sys_callback_t
* NAME
*	cl_pfn_sys_callback_t
*
* DESCRIPTION
*	The cl_pfn_sys_callback_t function type defines the prototype for
*	functions invoked by the system callback provider.
*
* SYNOPSIS
*/
typedef void
(CL_API *cl_pfn_sys_callback_t)(
	IN	void*	get_context,
	IN	void*	queue_context );
/*
* PARAMETERS
*	get_context
*		[in] Value of the get_context parameter specified in a call
*		to cl_sys_callback_get.
*
*	queue_context
*		[in] Value of the queue_context parameter specified in a call
*		to cl_sys_callback_queue.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	This function type is provided as function prototype reference for
*	the function provided by users as a parameter to the
*	cl_sys_callback_queue function.
*
* SEE ALSO
*	System Callback, cl_sys_callback_queue
*********/


/* Include platform specific system callback support. */
#include <complib/cl_syscallback_osd.h>


#ifdef __cplusplus
extern "C"
{
#endif


/****i* Component Library: System Callback/cl_sys_callback_construct
* NAME
*	cl_sys_callback_construct
*
* DESCRIPTION
*	The cl_sys_callback_construct function is called to initialize the state
*	of the system callback provider.
*
* SYNOPSIS
*/
void
__cl_sys_callback_construct( void );
/*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	This function is called internally when initializing the component
*	library for use.  Users should never call this function directly.
*
*	Calling cl_sys_callback_construct is a prerequisite to calling any other
*	system callback function.
*
*	Allows calling cl_sys_callback_init, cl_sys_callback_destroy, and
*	cl_is_sys_callback_inited.
*
* SEE ALSO
*	System Callback, cl_sys_callback_init, cl_sys_callback_destroy,
*	cl_is_sys_callback_inited
*********/


/****f* Component Library: System Callback/cl_is_sys_callback_inited
* NAME
*	cl_is_sys_callback_inited
*
* DESCRIPTION
*	The cl_is_sys_callback_inited function returns whether the system
*	callback provider was initialized successfully
*
* SYNOPSIS
*/
boolean_t
__cl_is_sys_callback_inited( void );
/*
* RETURN VALUES
*	TRUE if the system callback provider was initialized successfully.
*
*	FALSE otherwise.
*
* NOTES
*	Allows checking the state of the system callback provider to determine
*	if invoking member functions is appropriate.
*
* SEE ALSO
*	System Callback
*********/


/****i* Component Library: System Callback/cl_sys_callback_init
* NAME
*	cl_sys_callback_init
*
* DESCRIPTION
*	The cl_sys_callback_init function is called to initialize the system
*	callback provider.
*
* SYNOPSIS
*/
cl_status_t
__cl_sys_callback_init( void );
/*
* RETURN VALUES
*	CL_SUCCESS if the system callback provider was initialized successfully.
*
*	CL_INSUFFICIENT_MEMORY if there was not enough memory to inititalize
*	the system callback provider.
*
*	CL_ERROR if the system callback provider's threads could not be created.
*
* NOTES
*	This function is called internally when initializing the component
*	library for use.  Users should never call this function directly.
*
* SEE ALSO
*	System Callback, cl_sys_callback_construct, cl_sys_callback_destroy
*********/


/****i* Component Library: System Callback/cl_sys_callback_destroy
* NAME
*	cl_sys_callback_destroy
*
* DESCRIPTION
*	The cl_sys_callback_destroy function is called to destroy the system
*	callback provider.
*
* SYNOPSIS
*/
void
__cl_sys_callback_destroy( void );
/*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	This function is called internally when destroying the component
*	library after use.  Users should never call this function directly.
*
*	All threads and resources allocated by the system callback provider
*	are freed.
*
*	This function should only be called after calling either
*	cl_sys_callback_construct or cl_sys_callback_construct.
*
* SEE ALSO
*	System Callback, cl_sys_callback_construct, cl_sys_callback_construct
*********/


/****f* Component Library: System Callback/cl_sys_callback_get
* NAME
*	cl_sys_callback_get
*
* DESCRIPTION
*	The cl_sys_callback_get function retrieves a system callback item.
*
* SYNOPSIS
*/
CL_EXPORT cl_sys_callback_item_t* CL_API
cl_sys_callback_get(
	IN	const void* const get_context );
/*
* PARAMETERS
*	get_context
*		[in] Context value to pass into the callback function.
*
* RETURN VALUES
*	Returns a pointer to a system callback item if successful.
*
*	Returns NULL if the call fails.
*
* NOTES
*	A system callback item must be released with a call to cl_sys_callback_put.
*
*	Care must be taken to prevent a system callback item from being returned
*	to the pool while it is queued. Callers of cl_sys_callback_queue must not
*	return the system callback item to the pool until their callback has been
*	invoked.
*
*	In Windows 2000 Kernel Mode, the get_context is a pointer to the device
*	object for which the system callback is being used.
*
* SEE ALSO
*	System Callback, SysCallbackPut, SysCallbackQueue
*********/


/****f* Component Library: System Callback/cl_sys_callback_put
* NAME
*	cl_sys_callback_put
*
* DESCRIPTION
*	The cl_sys_callback_put function releases the specified
*	system callback item.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_sys_callback_put(
	IN	cl_sys_callback_item_t* const	p_item );
/*
* PARAMETERS
*	p_item
*		[in] Pointer to a system callback item to release.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	The p_item parameter points to a system callback item returned by
*	a previous call to cl_sys_callback_get.
*
*	The specified system callback item must not be queued when making
*	a call to this function.  This function can, however, be called
*	from the callback function.
*
* SEE ALSO
*	System Callback, cl_sys_callback_get, cl_sys_callback_queue
*********/


/****f* Component Library: System Callback/cl_sys_callback_queue
* NAME
*	cl_sys_callback_queue
*
* DESCRIPTION
*	The cl_sys_callback_queue function queues the specified system callback item
*	for execution.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_sys_callback_queue(
	IN	cl_sys_callback_item_t* const	p_item,
	IN	cl_pfn_sys_callback_t			pfn_callback,
	IN	const void* const				queue_context,
	IN	const boolean_t					high_priority );
/*
* PARAMETERS
*	p_item
*		[in] Pointer to a system callback item.
*
*	pfn_callback
*		[in] Pointer to a function to be invoked by the system callback module.
*		See the cl_pfn_sys_callback_t function type definition for details
*		about the callback function.
*
*	queue_context
*		[in] Value passed to the system callback function.
*
*	high_priority
*		[in] Specifies whether the request should be queued in the high- or
*		low-priority queue.
*
* RETURN VALUES
*	CL_SUCCESS if the system callback item was successfully queued.
*
*	CL_ERROR otherwise.
*
* NOTES
*	A thread from the system thread pool will invoke the specified callback
*	function with the get_context value specified in the call to
*	cl_sys_callback_get and the specified context as parameters.
*
*	The high priority queue is processed before the low priority queue. There
*	is no fairness algorithm implemented for removing items from the queues.
*
*	Care should be taken to only queue a given system callback item once
*	at a time.
*
* SEE ALSO
*	System Callback, cl_sys_callback_get, cl_pfn_sys_callback_t
*********/


#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif	/* _CL_SYS_CALLBACK_H_ */

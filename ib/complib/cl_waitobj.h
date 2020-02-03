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
 * $Id: cl_waitobj.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 * 	Declaration of wait object.
 * 
 * Environment:
 * 	All
 */


#ifndef _CL_WAITOBJ_H_
#define _CL_WAITOBJ_H_


#include <complib/cl_waitobj_osd.h>


/****h* Component Library/Wait Object
* NAME
*	Wait Object
*
* DESCRIPTION
*	The Wait Object provides the capability for a user mode process to
*	create and manipulate a wait object that can also be manipulated from
*	kernel mode.
*
* SEE ALSO
*	Structures:
*		cl_waitobj_handle_t
*
*	User Mode Initialization/Destruction:
*		cl_waitobj_create
*		cl_waitobj_destroy
*
*	Kernel Mode Access:
*		cl_waitobj_ref
*		cl_waitobj_deref
*
*	Manipulation:
*		cl_waitobj_signal
*		cl_waitobj_reset
*		cl_waitobj_wait_on
******/


/****d* Component Library: Wait Object/cl_waitobj_handle_t
* NAME
*	cl_waitobj_handle_t
*
* DESCRIPTION
*	Defines the handle for an OS wait object.
*
* NOTES
*	The wait object handle should be treated as opaque and is defined
*	differently depending on the target environment.
*
* SEE ALSO
*	Wait Object, cl_waitobj_create, cl_waitobj_destroy,
*	cl_waitobj_ref, cl_waitobj_deref, cl_waitobj_signal,
*	cl_waitobj_reset, cl_waitobj_wait_on
******/


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


#if defined(CL_KERNEL)

/****f* Component Library: Wait Object/cl_waitobj_ref
* NAME
*	cl_waitobj_ref
* 
* DESCRIPTION
*	The cl_waitobj_ref function validates a user mode wait object handle 
*	and returns a kernel mode wait object handle.  A reference is taken
*	on the object to prevent its destruction even if the user mode 
*	application destroys it.
*								
* SYNOPSIS
*/
CL_EXPORT cl_waitobj_handle_t CL_API
cl_waitobj_ref(
	IN	void					*h_user_wait_obj );
/* 
* PARAMETERS
*	h_user_wait_obj
*		[in] A wait object handle passed from user mode. 
*
* RETURN VALUES
*	Returns a kernel wait object handle upon success.  The returned handle 
*	should only be used as parameters to kernel mode calls.
*
*	Returns NULL in case of failure.
*
* NOTES
*	This function is only available in kernel mode.
*
* SEE ALSO
*	Wait Object, cl_waitobj_handle_t, cl_waitobj_deref,
*	cl_waitobj_signal, cl_waitobj_reset, cl_waitobj_wait_on
******/


/****f* Component Library: Wait Object/cl_waitobj_deref
* NAME
*	cl_waitobj_deref
* 
* DESCRIPTION
*	The cl_waitobj_deref function release a reference on a kernel mode 
*	wait object handle and allows the wait object to be destroyed.
*								
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_waitobj_deref(
	IN	cl_waitobj_handle_t  	h_kernel_wait_obj );
/* 
* PARAMETERS
*	h_kernel_wait_obj
*		[in] A wait object handle returned by a previous call to cl_waitobj_ref. 
*
* RETURN VALUES
*	This function does not return a value.
*
* NOTES
*	This function is only available in kernel mode.
*
* SEE ALSO
*	Wait Object, cl_waitobj_handle_t, cl_waitobj_ref, 
*	cl_waitobj_signal, cl_waitobj_reset, cl_waitobj_wait_on
******/

#else	/* CL_KERNEL */

/****f* Component Library: Wait Object/cl_waitobj_create
* NAME
*	cl_waitobj_create
* 
* DESCRIPTION
*	The cl_waitobj_create function creates a wait object.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_waitobj_create(
	IN	const boolean_t				manual_reset, 
	OUT	cl_waitobj_handle_t* const	ph_wait_obj );
/* 
* PARAMETERS
*	manual_reset
*		[in] If FALSE, indicates that the event resets itself after releasing 
*		a single waiter.  If TRUE, the event remains in the signalled state 
*		until explicitly reset by a call to cl_event_reset.
*
*	ph_wait_obj
*		[out] Pointer to a wait object handle set upon successful creation.
*
* RETURN VALUES
*	CL_SUCCESS if the wait object was created successfully.
*
*	CL_ERROR if the wait object creation failed.
*
* NOTES
* 	This function is only available in user mode.
*
* SEE ALSO
*	Wait Object, cl_waitobj_handle_t, cl_waitobj_destroy, 
*	cl_waitobj_signal, cl_waitobj_reset, cl_waitobj_wait_on
******/


/****f* Component Library: Wait Object/cl_waitobj_destroy
* NAME
* 	cl_waitobj_destroy
*
* DESCRIPTION
*	The cl_waitobj_destroy function destroys a wait object.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_waitobj_destroy(
	IN	cl_waitobj_handle_t	h_wait_obj );
/* 
* PARAMETERS
* 	h_wait_obj
* 		[in] A handle to the wait object to destroy, obtained by a pervious
*		call to cl_waitobj_create.
*
* RETURN VALUES
*	CL_SUCCESS if the wait object handle is destroyed.
*
*	CL_INVALID_PARAMETER if the wait object handle is invalid.
*
* NOTES
* 	This function is only available in user mode.
*
* SEE ALSO
*	Wait Object, cl_waitobj_handle_t, cl_waitobj_create, 
*	cl_waitobj_signal, cl_waitobj_reset, cl_waitobj_wait_on
*********/

#endif	/* CL_KERNEL */

/****f* Component Library: Wait Object/cl_waitobj_signal
* NAME
*	cl_waitobj_signal
* 
* DESCRIPTION
*	The cl_waitobj_signal function sets a wait object to the signalled 
*	state and releases one or more waiting threads.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_waitobj_signal(
	IN	cl_waitobj_handle_t	h_wait_obj );
/* 
* PARAMETERS
* 	h_wait_obj
* 		[in] A handle to the wait object that needs to be signaled.
* 
* RETURN VALUES
*	CL_SUCCESS if the event was successfully signalled.
*
*	CL_ERROR otherwise.
*
* NOTES
*	For auto-reset wait objects, the wait object is reset automatically once 
*	a wait operation is satisfied. 
*
*	Triggering the wait object multiple times does not guarantee that the same 
*	number of wait operations are satisfied. This is because wait objects are 
*	either in a signalled on non-signalled state, and triggering a wait object 
*	that is already in the signalled state has no effect.
*
*	In kernel mode, a pointer to a cl_event_t can safely be used instead of
*	a wait object handle.
*
* SEE ALSO
*	Wait Object, cl_waitobj_create, cl_waitobj_destroy,
*	cl_waitobj_ref, cl_waitobj_deref,
*	cl_waitobj_reset, cl_waitobj_wait_on
*********/


/****f* Component Library: Wait Object/cl_waitobj_reset
* NAME
*	cl_waitobj_reset
*
* DESCRIPTION
*	The cl_waitobj_reset function sets an wait object to the non-signalled state.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_waitobj_reset(
	IN	cl_waitobj_handle_t	h_wait_obj );
/*
* PARAMETERS
* 	h_wait_obj
* 		[in] A handle to the wait object that needs to reset.
*
* RETURN VALUES
*	CL_SUCCESS if the wait object was successfully reset.
*
*	CL_ERROR otherwise.
*
* NOTES
*	In kernel mode, a pointer to a cl_event_t can safely be used instead of
*	a wait object handle.
*
* SEE ALSO
*	Wait Object, cl_waitobj_create, cl_waitobj_destroy,
*	cl_waitobj_ref, cl_waitobj_deref,
*	cl_waitobj_signal, cl_waitobj_wait_on
*********/


/****f* Component Library: Wait Object/cl_waitobj_wait_on
* NAME
*	cl_waitobj_wait_on
*
* DESCRIPTION
*	The cl_waitobj_wait_on function waits for the specified wait object to be 
*	triggered for a minimum amount of time.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_waitobj_wait_on(
	IN	cl_waitobj_handle_t		h_wait_obj,
	IN	const uint32_t			wait_us,
	IN	const boolean_t			interruptible );
/*
* PARAMETERS
* 	h_wait_obj
* 		[in] A handle to the wait object on which to wait.
*
*	wait_us 
*		[in] Number of microseconds to wait.
*
*	interruptible
*		[in] Indicates whether the wait operation can be interrupted
*		by external signals.
*
* RETURN VALUES
*	CL_SUCCESS if the wait operation succeeded in response to the wait object 
*	being set.
*
*	CL_TIMEOUT if the specified time period elapses.
*
*	CL_NOT_DONE if the wait was interrupted by an external signal.
*
*	CL_ERROR if the wait operation failed.
*
* NOTES
*	If wait_us is set to EVENT_NO_TIMEOUT, the function will wait until the 
*	wait object is triggered and never timeout.
*
*	If the timeout value is zero, this function simply tests the state of 
*	the wait object.
*
*	If the wait object is already in the signalled state at the time of the call
*	to cl_waitobj_wait_on, the call completes immediately with CL_SUCCESS.
*
*	In kernel mode, a pointer to a cl_event_t can safely be used instead of
*	a wait object handle.
*
* SEE ALSO
*	Wait Object, cl_waitobj_create, cl_waitobj_destroy,
*	cl_waitobj_ref, cl_waitobj_deref, 
*	cl_waitobj_signal, cl_waitobj_reset
*********/


#ifdef __cplusplus
}	/* extern "C" */
#endif	/* __cplusplus */


#endif 	/* _CL_WAITOBJ_H_ */

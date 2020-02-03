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
 * $Id: cl_waitobj_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_WAITOBJ_OSD_H_
#define _CL_WAITOBJ_OSD_H_


#include <complib/cl_types.h>
#include <complib/cl_event.h>


typedef PKEVENT	cl_waitobj_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

	
CL_INLINE cl_waitobj_handle_t
cl_waitobj_ref(
	IN	void					*h_user_wait_obj )
{
	cl_waitobj_handle_t	h_kevent = NULL;

	/* 
	 * Assumption that if the call fails, the h_kevent parameter is unchanged,
	 * or set to NULL.
	 */
	ObReferenceObjectByHandle( h_user_wait_obj, STANDARD_RIGHTS_ALL,
		*ExEventObjectType, UserMode, (PVOID*)&h_kevent, NULL );

	return h_kevent;
}


CL_INLINE void
cl_waitobj_deref(
	IN	cl_waitobj_handle_t		h_kernel_wait_obj )
{
	ObDereferenceObject( h_kernel_wait_obj );
}


CL_INLINE cl_status_t
cl_waitobj_signal(
	IN	cl_waitobj_handle_t	h_wait_obj )
{
	return cl_event_signal( (cl_event_t*)h_wait_obj );
}


CL_INLINE cl_status_t
cl_waitobj_reset( 
	IN	cl_waitobj_handle_t	h_wait_obj )
{
	return cl_event_reset( (cl_event_t*)h_wait_obj );
}


CL_INLINE cl_status_t
cl_waitobj_wait_on( 
	IN	cl_waitobj_handle_t		h_wait_obj,
	IN	const uint32_t			wait_us,
	IN	const boolean_t			interruptible )
{
	return cl_event_wait_on( (cl_event_t*)h_wait_obj, wait_us, interruptible );
}


#ifdef __cplusplus
}	/* extern "C" */
#endif	/* __cplusplus */


#endif 	/* _CL_WAITOBJ_OSD_H_ */

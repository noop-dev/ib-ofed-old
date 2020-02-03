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

typedef cl_event_t  cl_waitobj_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


CL_INLINE cl_status_t CL_API
cl_waitobj_create(
	IN	const boolean_t				manual_reset, 
	OUT	cl_waitobj_handle_t* const	ph_wait_obj )
{
	cl_event_construct( ph_wait_obj );
	return cl_event_init( ph_wait_obj, manual_reset );
}


CL_INLINE cl_status_t CL_API
cl_waitobj_destroy(
	IN	cl_waitobj_handle_t	h_wait_obj )
{
	/*
	 * Note that we can take the address of the function parameter *only*
	 * because the wait object (and cl_event_t) is just a HANDLE, so
	 * copying it works.
	 */
	cl_event_destroy( &h_wait_obj );
	return CL_SUCCESS;
}


CL_INLINE cl_status_t CL_API
cl_waitobj_signal(
	IN	cl_waitobj_handle_t	h_wait_obj )
{
	/*
	 * Note that we can take the address of the function parameter *only*
	 * because the wait object (and cl_event_t) is just a HANDLE, so
	 * copying it works.
	 */
	return cl_event_signal( &h_wait_obj );
}


CL_INLINE cl_status_t CL_API
cl_waitobj_reset(
	IN	cl_waitobj_handle_t	h_wait_obj )
{
	/*
	 * Note that we can take the address of the function parameter *only*
	 * because the wait object (and cl_event_t) is just a HANDLE, so
	 * copying it works.
	 */
	return cl_event_reset( &h_wait_obj );
}


CL_INLINE cl_status_t CL_API
cl_waitobj_wait_on(
	IN	cl_waitobj_handle_t		h_wait_obj,
	IN	const uint32_t			wait_us,
	IN	const boolean_t			interruptible )
{
	/*
	 * Note that we can take the address of the function parameter *only*
	 * because the wait object (and cl_event_t) is just a HANDLE, so
	 * copying it works.
	 */
	return cl_event_wait_on( &h_wait_obj, wait_us, interruptible );
}


#ifdef __cplusplus
}	/* extern "C" */
#endif	/* __cplusplus */


#endif 	/* _CL_WAITOBJ_OSD_H_ */

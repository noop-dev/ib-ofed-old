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
 * $Id: cl_event_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_EVENT_OSD_H_
#define _CL_EVENT_OSD_H_


#include "complib/cl_types.h"
#include "complib/cl_memory.h"


/* Simple definition, eh? */
typedef KEVENT		cl_event_t;


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE void
cl_event_construct( 
	IN	cl_event_t* const	p_event )
{
	CL_ASSERT( p_event );

	cl_memclr( p_event, sizeof(cl_event_t) );
}


CL_INLINE cl_status_t
cl_event_init( 
	IN	cl_event_t* const	p_event,
	IN	const boolean_t		manual_reset )
{
	CL_ASSERT( p_event );

	cl_event_construct( p_event );

	if( manual_reset )
		KeInitializeEvent( p_event, NotificationEvent, FALSE );
	else
		KeInitializeEvent( p_event, SynchronizationEvent, FALSE );

	return( CL_SUCCESS );
}


CL_INLINE void
cl_event_destroy( 
	IN	cl_event_t* const	p_event )
{
	UNUSED_PARAM( p_event );
}


CL_INLINE cl_status_t
cl_event_signal( 
	IN	cl_event_t* const	p_event )
{
	CL_ASSERT( p_event );
	CL_ASSERT( KeGetCurrentIrql() <= DISPATCH_LEVEL );

	KeSetEvent( p_event, 0, FALSE );

	return( CL_SUCCESS );
}


CL_INLINE cl_status_t
cl_event_reset( 
	IN	cl_event_t* const	p_event )
{
	CL_ASSERT( p_event );
	CL_ASSERT( KeGetCurrentIrql() <= DISPATCH_LEVEL );

	KeClearEvent( p_event );

	return( CL_SUCCESS );
}


#ifdef __cplusplus
}	// extern "C"
#endif

#endif // _CL_EVENT_OSD_H_
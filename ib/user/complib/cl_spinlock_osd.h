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
 * $Id: cl_spinlock_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_SPINLOCK_OSD_H_
#define _CL_SPINLOCK_OSD_H_


#include "cl_types.h"


/* Spinlock object definition. */
typedef struct _cl_spinlock
{
	CRITICAL_SECTION	crit_sec;
	/* The flag is necessary to conditionally destroy the critical section. */
	boolean_t			initialized;

} cl_spinlock_t;


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE void CL_API
cl_spinlock_construct(
	IN	cl_spinlock_t* const	p_spinlock )
{
	p_spinlock->initialized = FALSE;
}


CL_INLINE cl_status_t CL_API
cl_spinlock_init(
	IN	cl_spinlock_t* const	p_spinlock )
{
	CL_ASSERT( p_spinlock );

	cl_spinlock_construct( p_spinlock );

	/*
	 * Documentation recommends a spin count ~4K.
	 * High bit set to force pre-allocation of event.
	 */
	if( !InitializeCriticalSectionAndSpinCount( 
		&p_spinlock->crit_sec, 0x80001000 ) )
	{
		return( CL_ERROR );
	}

	p_spinlock->initialized = TRUE;
	return( CL_SUCCESS );
}


CL_INLINE void CL_API
cl_spinlock_destroy(
	IN	cl_spinlock_t* const	p_spinlock )
{
	CL_ASSERT( p_spinlock );

	if( p_spinlock->initialized )
	{
		p_spinlock->initialized = FALSE;
		DeleteCriticalSection( &p_spinlock->crit_sec );
	}
}


CL_INLINE void CL_API
cl_spinlock_acquire(
	IN	cl_spinlock_t* const	p_spinlock )
{
	CL_ASSERT( p_spinlock && p_spinlock->initialized );

	EnterCriticalSection( &p_spinlock->crit_sec );
}


CL_INLINE void CL_API
cl_spinlock_release(
	IN	cl_spinlock_t* const	p_spinlock )
{
	CL_ASSERT( p_spinlock && p_spinlock->initialized );

	LeaveCriticalSection( &p_spinlock->crit_sec );
}


#ifdef __cplusplus
}	// extern "C"
#endif

#endif // _CL_SPINLOCK_OSD_H_

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
 * $Id: cl_irqlock_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Implementation of IRQ lock object.
 *
 * Environment:
 *	Windows Kernel Mode
 */


#ifndef _CL_IRQLOCK_OSD_H_
#define _CL_IRQLOCK_OSD_H_


#include <complib/cl_spinlock.h>


typedef struct _cl_irqlock
{
	PKINTERRUPT		p_interrupt;
	KIRQL			irql;

}	cl_irqlock_t;


typedef struct _KINTERRUPT		cl_interrupt_t;


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE void
cl_irqlock_construct( 
	IN	cl_irqlock_t* const		p_irqlock )
{
	p_irqlock->p_interrupt = NULL;
}


CL_INLINE cl_status_t
cl_irqlock_init( 
	IN	cl_irqlock_t* const		p_irqlock,
	IN	cl_interrupt_t* const	p_interrupt )
{
	cl_irqlock_construct( p_irqlock );
	p_irqlock->p_interrupt = p_interrupt;
	return CL_SUCCESS;
}


CL_INLINE void
cl_irqlock_destroy( 
	IN	cl_irqlock_t* const		p_irqlock )
{
	p_irqlock->p_interrupt = NULL;
}


CL_INLINE void
cl_irqlock_acquire( 
	IN	cl_irqlock_t* const		p_irqlock )
{
#if WINVER > 0x500
	p_irqlock->irql = KeAcquireInterruptSpinLock( p_irqlock->p_interrupt );
#else
	UNUSED_PARAM( p_irqlock );
#pragma warning( push, 3 )
	ASSERT( 0 );
#pragma warning( pop )
#endif
}


CL_INLINE void
cl_irqlock_release(
	IN	cl_irqlock_t* const		p_irqlock )
{
#if WINVER > 0x500
	KeReleaseInterruptSpinLock( p_irqlock->p_interrupt, p_irqlock->irql );
#else
	UNUSED_PARAM( p_irqlock );
#pragma warning( push, 3 )
	ASSERT( 0 );
#pragma warning( pop )
#endif
}


#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif /* _CL_IRQLOCK_OSD_H_ */

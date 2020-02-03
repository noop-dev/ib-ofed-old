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
 * $Id: cl_irqlock.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 * 	Declaration of interrupt level IRQ lock object.
 * 
 * Environment:
 * 	All
 */


#ifndef _CL_IRQLOCK_H_
#define _CL_IRQLOCK_H_


#include <complib/cl_irqlock_osd.h>


/****h* Component Library/Irqlock
* NAME
*	Irqlock
*
* DESCRIPTION
*	Irqlock provides synchronization at interrupt level between threads for 
*	exclusive access to a resource.
*
*	The irqlock functions manipulate a cl_irqlock_t structure which should 
*	be treated as opaque and should be manipulated only through the provided 
*	functions.
*
* SEE ALSO
*	Structures:
*		cl_irqlock_t
*
*	Initialization:
*		cl_irqlock_construct, cl_irqlock_init, cl_irqlock_destroy
*
*	Manipulation
*		cl_irqlock_acquire, cl_irqlock_release
*********/


#ifdef __cplusplus
extern "C"
{
#endif


/****f* Component Library: Irqlock/cl_irqlock_construct
* NAME
*	cl_irqlock_construct
*
* DESCRIPTION
*	The cl_irqlock_construct function initializes the state of a 
*	IRQ lock.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_irqlock_construct( 
	IN	cl_irqlock_t* const		p_irqlock );
/*
* PARAMETERS
*	p_irqlock 
*		[in] Pointer to a IRQ lock structure whose state to initialize.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Allows calling cl_irqlock_destroy without first calling 
*	cl_irqlock_init.
*
*	Calling cl_irqlock_construct is a prerequisite to calling any other
*	IRQ lock function except cl_irqlock_init.
*
* SEE ALSO
*	Irqlock, cl_irqlock_init, cl_irqlock_destroy
*********/


/****f* Component Library: Irqlock/cl_irqlock_init
* NAME
*	cl_irqlock_init
*
* DESCRIPTION
*	The cl_irqlock_init function initializes a IRQ lock for use.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_irqlock_init( 
	IN	cl_irqlock_t* const		p_irqlock,
	IN	cl_interrupt_t* const	p_interrupt );
/*
* PARAMETERS
*	p_irqlock 
*		[in] Pointer to a IRQ lock structure to initialize.
*
*	p_interrupt
*		[in] Platform specific pointer conveying information about the
*		interrupt vector and level with which to synchronize.
*
* RETURN VALUES
*	CL_SUCCESS if initialization succeeded.
*
*	CL_ERROR if initialization failed. Callers should call 
*	cl_irqlock_destroy to clean up any resources allocated during 
*	initialization.
*
* NOTES
*	Initialize the IRQ lock structure. Allows calling cl_irqlock_aquire 
*	and cl_irqlock_release.
*
*	In Linux, the p_interrupt parameter is currently ignored.
*
*	In Windows, the p_interrupt parameter is a pointer to a KINTERRUPT object,
*	the value of which is supplied by a call to IoConnectInterrupt.
*
* SEE ALSO
*	Irqlock, cl_irqlock_construct, cl_irqlock_destroy, 
*	cl_irqlock_acquire, cl_irqlock_release
*********/


/****f* Component Library: Irqlock/cl_irqlock_destroy
* NAME
*	cl_irqlock_destroy
*
* DESCRIPTION
*	The cl_irqlock_destroy function performs all necessary cleanup of a 
*	IRQ lock.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_irqlock_destroy( 
	IN	cl_irqlock_t* const		p_irqlock );
/*
* PARAMETERS
*	p_irqlock 
*		[in] Pointer to a IRQ lock structure to destroy.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Performs any necessary cleanup of a IRQ lock. This function must only 
*	be called if either cl_irqlock_construct or cl_irqlock_init has been 
*	called.
*
* SEE ALSO
*	Irqlock, cl_irqlock_construct, cl_irqlock_init
*********/


/****f* Component Library: Irqlock/cl_irqlock_acquire
* NAME
*	cl_irqlock_acquire
*
* DESCRIPTION
*	The cl_irqlock_acquire function acquires a IRQ lock.
*	This version of lock does not prevent an interrupt from 
*	occuring on the processor on which the code is being
*	executed. To protect from an interrupt level resource
*	use the cl_irqlock_acquire_irq function.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_irqlock_acquire( 
	IN	cl_irqlock_t* const		p_irqlock );
/*
* PARAMETERS
*	p_irqlock 
*		[in] Pointer to a IRQ lock structure to acquire.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Irqlock, cl_irqlock_release
*********/


/****f* Component Library: Irqlock/cl_irqlock_release
* NAME
*	cl_irqlock_release
*
* DESCRIPTION
*	The cl_irqlock_release function releases a IRQ lock object.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_irqlock_release(
	IN	cl_irqlock_t* const		p_irqlock );
/*
* PARAMETERS
*	p_irqlock 
*		[in] Pointer to a IRQ lock structure to release.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Releases a IRQ lock after a call to cl_irqlock_acquire.
*
* SEE ALSO
*	Irqlock, cl_irqlock_acquire
*********/


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _CL_IRQLOCK_H_ */

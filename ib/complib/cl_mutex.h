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
 * $Id: cl_mutex.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of mutex object.
 *
 * Environment:
 *	All
 */


#ifndef _CL_MUTEX_H_
#define _CL_MUTEX_H_


#include <complib/cl_mutex_osd.h>


/****h* complib/Mutex
* NAME
*	Mutex
*
* DESCRIPTION
*	Mutex provides synchronization between threads for exclusive access to
*	a resource.
*
*	The Mutex functions manipulate a cl_mutex_t structure which should
*	be treated as opaque and should be manipulated only through the provided
*	functions.
*
* SEE ALSO
*	Structures:
*		cl_mutex_t
*
*	Initialization:
*		cl_mutex_construct, cl_mutex_init, cl_mutex_destroy
*
*	Manipulation
*		cl_mutex_acquire, cl_mutex_release
*********/


#ifdef __cplusplus
extern "C"
{
#endif


/****f* Component Library: Mutex/cl_mutex_construct
* NAME
*	cl_mutex_construct
*
* DESCRIPTION
*	The cl_mutex_construct function initializes the state of a
*	mutex.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_mutex_construct(
	IN	cl_mutex_t* const	p_mutex );
/*
* PARAMETERS
*	p_mutex
*		[in] Pointer to a mutex structure whose state to initialize.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Allows calling cl_semphore_destroy without first calling
*	cl_mutex_init.
*
*	Calling cl_mutex_construct is a prerequisite to calling any other
*	mutex function except cl_mutex_init.
*
* SEE ALSO
*	Mutex, cl_semphore_init cl_mutex_destroy
*********/


/****f* Component Library: Mutex/cl_mutex_init
* NAME
*	cl_mutex_init
*
* DESCRIPTION
*	The cl_mutex_init function initializes a mutex for use.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_mutex_init(
	IN	cl_mutex_t* const	p_mutex );
/*
* PARAMETERS
*	p_mutex
*		[in] Pointer to a mutex structure to initialize.
*
* RETURN VALUES
*	CL_SUCCESS if initialization succeeded.
*
*	CL_ERROR if initialization failed. Callers should call
*	cl_mutex_destroy to clean up any resources allocated during
*	initialization.
*
* NOTES
*	Initializes the mutex structure. Allows calling cl_mutex_aquire
*	and cl_mutex_release. The cl_mutex is always created in the unlocked state.
*
* SEE ALSO
*	Mutex, cl_mutex_construct, cl_mutex_destroy,
*	cl_mutex_acquire, cl_mutex_release
*********/


/****f* Component Library: Mutex/cl_mutex_destroy
* NAME
*	cl_mutex_destroy
*
* DESCRIPTION
*	The cl_mutex_destroy function performs all necessary cleanup of a
*	mutex.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_mutex_destroy(
	IN	cl_mutex_t* const	p_mutex );
/*
* PARAMETERS
*	p_mutex
*		[in] Pointer to a mutex structure to destroy.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Performs any necessary cleanup of a mutex. This function must only
*	be called if either cl_mutex_construct or cl_mutex_init has been
*	called.
*
* SEE ALSO
*	Mutex, cl_mutex_construct, cl_mutex_init
*********/


/****f* Component Library: Mutex/cl_mutex_acquire
* NAME
*	cl_mutex_acquire
*
* DESCRIPTION
*	The cl_mutex_acquire function acquires a mutex.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_mutex_acquire(
	IN	cl_mutex_t* const	p_mutex );
/*
* PARAMETERS
*	p_mutex
*		[in] Pointer to a mutex structure to acquire.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Mutex, cl_mutex_release
*********/


/****f* Component Library: Mutex/cl_mutex_release
* NAME
*	cl_mutex_release
*
* DESCRIPTION
*	The cl_mutex_release function releases a mutex object.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_mutex_release(
	IN	cl_mutex_t* const	p_mutex );
/*
* PARAMETERS
*	p_mutex
*		[in] Pointer to a mutex structure to release.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Releases a mutex after a call to cl_mutex_acquire.
*
* SEE ALSO
*	Mutex, cl_mutex_acquire
*********/


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _CL_MUTEX_H_ */

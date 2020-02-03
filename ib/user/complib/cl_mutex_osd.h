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
 * $Id: cl_mutex_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of mutex object.
 *
 * Environment:
 *	Windows User Mode
 */


#ifndef _CL_MUTEX_OSD_H_
#define _CL_MUTEX_OSD_H_


#include <complib/cl_types.h>


typedef HANDLE	cl_mutex_t;


#ifdef __cplusplus
extern "C"
{
#endif


CL_INLINE void CL_API
cl_mutex_construct(
	IN	cl_mutex_t* const	p_mutex )
{
	*p_mutex = NULL;
}


CL_INLINE cl_status_t CL_API
cl_mutex_init(
	IN	cl_mutex_t* const	p_mutex )
{
	*p_mutex = CreateMutex( NULL, FALSE, NULL );
	if( *p_mutex )
		return CL_SUCCESS;
	else
		return CL_ERROR;
}


CL_INLINE void CL_API
cl_mutex_destroy(
	IN	cl_mutex_t* const	p_mutex )
{
	CloseHandle( *p_mutex );
}


CL_INLINE void CL_API
cl_mutex_acquire(
	IN	cl_mutex_t* const	p_mutex )
{
	WaitForSingleObject( *p_mutex, INFINITE );
}


CL_INLINE void CL_API
cl_mutex_release(
	IN	cl_mutex_t* const	p_mutex )
{
	ReleaseMutex( *p_mutex );
}


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _CL_MUTEX_OSD_H_ */

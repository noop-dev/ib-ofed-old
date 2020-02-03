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
 * $Id: cl_thread_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_THREAD_OSD_H_
#define _CL_THREAD_OSD_H_


#include "cl_types.h"
#include "cl_timer.h"


/* OS specific information about the thread. */
typedef struct _cl_thread_osd
{
	HANDLE			h_thread;
	DWORD			thread_id;

} cl_thread_osd_t;


#ifdef __cplusplus
extern "C"
{
#endif

CL_INLINE void CL_API
cl_thread_suspend(
	IN	const uint32_t	pause_ms )
{
	Sleep( pause_ms );
}


CL_INLINE void CL_API
cl_thread_stall(
	IN const uint32_t pause_us )
{
	uint64_t	end_time;

	end_time = cl_get_time_stamp() + pause_us;

	/* Spin. */
	while( cl_get_time_stamp() < end_time )
		;
}


CL_INLINE boolean_t CL_API
cl_is_blockable( void )
{
	return TRUE;
}


#ifdef __cplusplus
}	// extern "C"
#endif


#endif // _CL_THREAD_OSD_H_

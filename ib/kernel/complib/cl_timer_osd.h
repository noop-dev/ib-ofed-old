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
 * $Id: cl_timer_osd.h 6049 2010-07-05 14:17:17Z xalex $
 */




#ifndef _CL_TIMER_OSD_H_
#define _CL_TIMER_OSD_H_


#include "complib/cl_types.h"


/* Timer object definition. */
typedef struct _cl_timer
{
	KTIMER					timer;
	KDPC					dpc;
	cl_pfn_timer_callback_t	pfn_callback;
	const void				*context;
	uint64_t				timeout_time;
	KSPIN_LOCK				spinlock;
	KSPIN_LOCK				cb_lock;

} cl_timer_t;


#ifdef __cplusplus
extern "C"
{
#endif

/* 100ns to s conversion */
#define HUNDREDNS_TO_SEC	CL_CONST64(10000000)
/* s to µs conversion */
#define SEC_TO_MICRO		CL_CONST64(1000000)

CL_INLINE uint64_t CL_API
cl_get_time_stamp( void )
{
	LARGE_INTEGER	tick_count, frequency;

	tick_count = KeQueryPerformanceCounter( &frequency );
	return( SEC_TO_MICRO * tick_count.QuadPart / frequency.QuadPart );
}

CL_INLINE uint32_t CL_API
cl_get_time_stamp_sec( void )
{
	return( (uint32_t)(KeQueryInterruptTime() / HUNDREDNS_TO_SEC) );
}

CL_INLINE uint64_t CL_API
cl_get_tick_count( void )
{
	LARGE_INTEGER	tick_count;

	tick_count = KeQueryPerformanceCounter( NULL );
	return tick_count.QuadPart;
}

CL_INLINE uint64_t CL_API
cl_get_tick_freq( void )
{
	LARGE_INTEGER	frequency;

	KeQueryPerformanceCounter( &frequency );
	return frequency.QuadPart;
}

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	// _CL_TIMER_OSD_H_

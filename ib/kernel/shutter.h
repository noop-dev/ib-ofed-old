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
 * $Id: shutter.h 1611 2006-08-20 14:48:55Z sleybo $
 */


#pragma once


// Define the max numbers of operations that can be simultaniously done
#define MAX_OPERATIONS	0x10000000

typedef struct _shutter_t {
	long cnt;
	KEVENT event;

}	shutter_t;

static inline void shutter_init(shutter_t* p_shutter)
{
	p_shutter->cnt = 0;
	KeInitializeEvent( &p_shutter->event, SynchronizationEvent, FALSE );
}


static inline void shutter_sub(shutter_t * p_shutter,long Val)
{
    long res = 0;
    ASSERT(Val < 0);
	res = InterlockedExchangeAdd( &p_shutter->cnt,Val );
	if ((res+Val) == -MAX_OPERATIONS)
		KeSetEvent( &p_shutter->event, 0, FALSE );
}

// if RC == true, one can proceed
static inline BOOLEAN shutter_add(shutter_t * p_shutter,long Val)
{
    long res = 0, int_res;
	
    ASSERT(Val > 0);

	for(;;) {
		res = p_shutter->cnt;
		if (res < 0) {
			return FALSE;
		}
		
		int_res = InterlockedCompareExchange(&p_shutter->cnt, res+Val, res );
		if (int_res == res) {
			return TRUE;
		}
		if (int_res < 0) {
			return FALSE;
		}

	}

}

static inline void shutter_loose(shutter_t * p_shutter)
{
	long res = InterlockedDecrement( &p_shutter->cnt );
	if (res == -MAX_OPERATIONS)
		KeSetEvent( &p_shutter->event, 0, FALSE );
}

// if RC > 0, one can proceed
static inline int shutter_use(shutter_t * p_shutter)
{
	BOOLEAN ret = shutter_add(p_shutter, 1);
	if (ret) return 1;
	return -1;
		
}


static inline void shutter_shut(shutter_t * p_shutter)
{
    long res = 0;
    //
    //  ASSERT not calling shu twice.
    //
    ASSERT(p_shutter->cnt - MAX_OPERATIONS >=  (-MAX_OPERATIONS));
    
	// Mark the counter as locked
	res = InterlockedExchangeAdd(&p_shutter->cnt, -MAX_OPERATIONS);
	ASSERT(res >= 0);
	if (res) 
	{
		// We are now waiting for the object to reach -MAX_OPERATIONS
		ASSERT( KeGetCurrentIrql() < DISPATCH_LEVEL );
		KeWaitForSingleObject( &p_shutter->event, Executive, KernelMode, FALSE, NULL );
	}
}

static inline void shutter_alive(shutter_t * p_shutter)
{
    long res = 0;
    
	// Mark the counter as alive
	res = InterlockedExchangeAdd(&p_shutter->cnt, MAX_OPERATIONS);
	ASSERT(res < 0);
}



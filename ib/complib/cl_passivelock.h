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
 * $Id: cl_passivelock.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	This file contains the passive lock, which synchronizes passive threads.
 *	The passive lock allows multiple readers to access a resource
 *	simultaneously, exclusive from a single thread writting.  It is similar
 *	in behavior to the passive lock, but works at passive only.
 *
 * Environment:
 *	All
 */


#ifndef _CL_PASSIVE_LOCK_H_
#define _CL_PASSIVE_LOCK_H_


#include <complib/cl_event.h>
#include <complib/cl_atomic.h>


/****h* Component Library/Passive Lock
* NAME
*	Passive Lock
*
* DESCRIPTION
*	The Passive Lock provides synchronization between multiple threads that
*	are sharing the lock with a single thread holding the lock exclusively.
*
*	Passive lock works exclusively between threads and cannot be used in
*	situations where the caller cannot be put into a waiting state.
*
*	The passive lock functions operate a cl_plock_t structure which should
*	be treated as opaque and should be manipulated only through the provided
*	functions.
*
* SEE ALSO
*	Structures:
*		cl_plock_t
*
*	Initialization:
*		cl_plock_construct, cl_plock_init, cl_plock_destroy
*
*	Manipulation
*		cl_plock_acquire, cl_plock_excl_acquire, cl_plock_release
*********/


/****s* Component Library: Passive Lock/cl_plock_t
* NAME
*	cl_plock_t
*
* DESCRIPTION
*	Passive Lock structure.
*
*	The cl_plock_t structure should be treated as opaque and should
*	be manipulated only through the provided functions.
*
* SYNOPSIS
*/
typedef struct _cl_plock
{
	cl_event_t		reader_event;
	cl_event_t		writer_event;
	atomic32_t		reader_count;

} cl_plock_t;
/*
* FIELDS
*	reader_event
*		Event used to synchronize shared access to the lock.
*
*	writer_event
*		Event used to synchronize exclusive access to the lock.
*
*	reader_count
*		Number of threads holding the lock for shared access.
*
* SEE ALSO
*	Passive Lock
*********/


/****f* Component Library: Passive Lock/cl_plock_construct
* NAME
*	cl_plock_construct
*
* DESCRIPTION
*	The cl_plock_construct function initializes the state of a
*	passive lock.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_plock_construct(
	IN	cl_plock_t* const	p_lock )
{
	CL_ASSERT( p_lock );

	p_lock->reader_count = 0;
	cl_event_construct( &p_lock->reader_event );
	cl_event_construct( &p_lock->writer_event );
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure whose state to initialize.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Allows calling cl_plock_destroy without first calling cl_plock_init.
*
*	Calling cl_plock_construct is a prerequisite to calling any other
*	passive lock function except cl_plock_init.
*
* SEE ALSO
*	Passive Lock, cl_plock_init, cl_plock_destroy
*********/


/****f* Component Library: Passive Lock/cl_plock_destroy
* NAME
*	cl_plock_destroy
*
* DESCRIPTION
*	The cl_plock_destroy function performs any necessary cleanup
*	of a passive lock.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_plock_destroy(
	IN	cl_plock_t* const	p_lock )
{
	CL_ASSERT( p_lock );
	CL_ASSERT( p_lock->reader_count == 0 );

	cl_event_destroy( &p_lock->writer_event );
	cl_event_destroy( &p_lock->reader_event );
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure whose state to initialize.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	cl_plock_destroy performs any necessary cleanup of the specified
*	passive lock.
*
*	This function must only be called if cl_plock_construct or
*	cl_plock_init has been called. The passive lock must not be held
*	when calling this function.
*
* SEE ALSO
*	Passive Lock, cl_plock_construct, cl_plock_init
*********/


/****f* Component Library: Passive Lock/cl_plock_init
* NAME
*	cl_plock_init
*
* DESCRIPTION
*	The cl_plock_init function initializes a passive lock.
*
* SYNOPSIS
*/
CL_INLINE cl_status_t CL_API
cl_plock_init(
	IN	cl_plock_t* const	p_lock )
{
	cl_status_t	status;

	CL_ASSERT( p_lock );

	cl_plock_construct( p_lock );

	status = cl_event_init( &p_lock->writer_event, FALSE );
	if( status != CL_SUCCESS )
	{
		cl_plock_destroy( p_lock );
		return( status );
	}

	status = cl_event_init( &p_lock->reader_event, FALSE );
	if( status != CL_SUCCESS )
	{
		cl_plock_destroy( p_lock );
		return( status );
	}

	/*
	 * Set the writer event to signalled so that the first
	 * wait operation succeeds.
	 */
	status = cl_event_signal( &p_lock->writer_event );
	if( status != CL_SUCCESS )
	{
		cl_plock_destroy( p_lock );
		return( status );
	}

	/*
	 * Set the reader event to signalled so that the first
	 * wait operation succeeds.
	 */
	status = cl_event_signal( &p_lock->reader_event );
	if( status != CL_SUCCESS )
	{
		cl_plock_destroy( p_lock );
		return( status );
	}

	return( CL_SUCCESS );
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure to initialize.
*
* RETURN VALUES
*	CL_SUCCESS if the passive lock was initialized successfully.
*
*	CL_ERROR otherwise.
*
* NOTES
*	Allows calling cl_plock_acquire, cl_plock_release,
*	cl_plock_excl_acquire, and cl_plock_excl_release.
*
* SEE ALSO
*	Passive Lock, cl_plock_construct, cl_plock_destroy,
*	cl_plock_excl_acquire, cl_plock_excl_release,
*	cl_plock_acquire, cl_plock_release
*********/


/****f* Component Library: Passive Lock/cl_plock_acquire
* NAME
*	cl_plock_acquire
*
* DESCRIPTION
*	The cl_plock_acquire function acquires a passive lock for
*	shared access.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_plock_acquire(
	IN	cl_plock_t* const	p_lock )
{
	cl_status_t	status;

	CL_ASSERT( p_lock );

	status =
		cl_event_wait_on( &p_lock->reader_event, EVENT_NO_TIMEOUT, FALSE );
	CL_ASSERT( status == CL_SUCCESS );

	/*
	 * Increment the reader count to block a thread trying for exclusive
	 * access.
	 */
	cl_atomic_inc( &p_lock->reader_count );
#ifdef DBG_PASSIVE_LOCKS
	cl_dbg_out( "cl_plock_acquire: ReaderCount = %u\n",
		p_lock->reader_count );
#endif
	/*
	 * Release the reader event to satisfy the wait of another reader
	 * or a writer.
	 */
	cl_event_signal( &p_lock->reader_event );
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure to acquire.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Passive Lock, cl_plock_release, cl_plock_excl_acquire
*********/


/****f* Component Library: Passive Lock/cl_plock_excl_acquire
* NAME
*	cl_plock_excl_acquire
*
* DESCRIPTION
*	The cl_plock_excl_acquire function acquires exclusive access
*	to a passive lock.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_plock_excl_acquire(
	IN	cl_plock_t* const	p_lock )
{
	cl_status_t	status;

	CL_ASSERT( p_lock );

	/* Acquire the reader event.  This will block new readers. */
	status =
		cl_event_wait_on( &p_lock->reader_event, EVENT_NO_TIMEOUT, FALSE );
	CL_ASSERT( status == CL_SUCCESS );

	/* Wait for the writer event until all readers have exited. */
	while( p_lock->reader_count )
	{
#ifdef DBG_PASSIVE_LOCKS
		cl_dbg_out( "cl_plock_excl_acquire: ReaderCount = %u\n",
			p_lock->reader_count );
#endif
		status =
			cl_event_wait_on( &p_lock->writer_event, EVENT_NO_TIMEOUT, FALSE );
		CL_ASSERT( status == CL_SUCCESS );
	}

#ifdef DBG_PASSIVE_LOCKS
	cl_dbg_out( "cl_plock_excl_acquire: Exit\n" );
#endif
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure to acquire exclusively.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Passive Lock, cl_plock_release, cl_plock_acquire
*********/


/****f* Component Library: Passive Lock/cl_plock_release
* NAME
*	cl_plock_release
*
* DESCRIPTION
*	The cl_plock_release function releases a passive lock from
*	shared or exclusive access.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_plock_release(
	IN	cl_plock_t* const	p_lock )
{
	CL_ASSERT( p_lock );

	if( p_lock->reader_count )
	{

		/*
		 * Decrement the count to allow a thread waiting for exclusive
		 * access to continue.
		 */
		cl_atomic_dec( &p_lock->reader_count );

		#ifdef DBG_PASSIVE_LOCKS
			cl_dbg_out( "cl_plock_release: ReaderCount = %u\n",
				p_lock->reader_count );
		#endif

		/* Release a writer, if any. */
		cl_event_signal( &p_lock->writer_event );
	}
	else
	{
		/* Release threads waiting to acquire the lock. */
		cl_event_signal( &p_lock->reader_event );
		cl_event_signal( &p_lock->writer_event );

		#ifdef DBG_PASSIVE_LOCKS
			cl_dbg_out( "cl_plock_release: Exit\n" );
		#endif
	}
}
/*
* PARAMETERS
*	p_lock
*		[in] Pointer to a cl_plock_t structure to release.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Passive Lock, cl_plock_acquire, cl_plock_excl_acquire
*********/


#endif /* _CL_PASSIVE_LOCK_H_ */

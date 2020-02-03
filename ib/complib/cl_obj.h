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
 * $Id: cl_obj.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of basic objects and relationships.
 *
 * Environment:
 *	All
 */


#if !defined(__CL_OBJ_H__)
#define __CL_OBJ_H__

#include <complib/cl_async_proc.h>
#include <complib/cl_atomic.h>
#include <complib/cl_event.h>
#include <complib/cl_qlist.h>
#include <complib/cl_qpool.h>
#include <complib/cl_spinlock.h>


/****h* Component Library/Object
* NAME
*	Object
*
* DESCRIPTION
*	Object describes a basic class that can be used to track accesses to an
*	object and provides automatic cleanup of an object that is dependent
*	on another object.
*
*	Dependencies between objects are described using a relationship.  A
*	child object is considered dependent on a parent object.  Destruction of
*	a parent object automatically results in the destruction of any child
*	objects associated with the parent.
*
*	The relationship between parent and child objects is many to many.
*	Parents can have multiple child objects, and a child can be dependent on
*	multiple parent objects.  In the latter case, destruction of any parent
*	object results in the destruction of the child object.
*
*	Other relationships between objects are described using references.  An
*	object that takes a reference on a second object prevents the second object
*	from being deallocated as long as the reference is held.
*
* SEE ALSO
*	Types
*		cl_destroy_type_t
*
*	Structures:
*		cl_obj_t, cl_obj_rel_t
*
*	Callbacks:
*		cl_pfn_obj_call_t
*
*	Initialization/Destruction:
*		cl_obj_mgr_create, cl_obj_mgr_destroy,
*		cl_obj_construct, cl_obj_init, cl_obj_destroy, cl_obj_deinit
*
*	Object Relationships:
*		cl_obj_ref, cl_obj_deref,
*		cl_rel_alloc, cl_rel_free, cl_obj_insert_rel, cl_obj_remove_rel
*
*	Object Manipulation:
*		cl_obj_reset
*********/



/* Forward declaration. */
typedef struct _cl_obj *__p_cl_obj_t;



/****s* Component Library: Object/cl_obj_mgr_t
* NAME
*	cl_obj_mgr_t
*
* DESCRIPTION
*	The global object manager.
*
*	The manager must be created before constructing any other objects, and all
*	objects must be destroyed before the object manager is destroyed.
*
*	The manager is used to maintain the list of all objects currently active
*	in the system.  It provides a pool of relationship items used to
*	describe parent-child, or dependent, relationships between two objects.
*	The manager contains an asynchronous processing thread that is used to
*	support asynchronous object destruction.
*
* SYNOPSIS
*/
typedef struct _cl_obj_mgr
{
	cl_qlist_t					obj_list;
	cl_spinlock_t				lock;

	cl_async_proc_t				async_proc_mgr;

	cl_qpool_t					rel_pool;

}	cl_obj_mgr_t;
/*
* FIELDS
*	obj_list
*		List of all object's in the system.  Object's are inserted into this
*		list when constructed and removed when freed.
*
*	lock
*		A lock used by the object manager for synchronization to the obj_list.
*
*	async_proc_mgr
*		An asynchronous processing manager used to process asynchronous
*		destruction requests.  Users wishing to synchronize the execution of
*		specific routines with object destruction may queue work requests to
*		this processing manager.
*
*	rel_pool
*		Pool of items used to describe dependent relationships.  Users may
*		obtain relationship objects from this pool when forming relationships,
*		but are not required to do so.
*
* SEE ALSO
*	Object, cl_obj_mgr_create, cl_obj_mgr_destroy,
*	cl_obj_construct, cl_obj_deinit,
*	cl_qlist_t, cl_spinlock_t, cl_async_proc_t, cl_qpool_t
*********/



#ifdef __cplusplus
extern "C" {
#endif



/****f* Component Library: Object/cl_obj_mgr_create
* NAME
*	cl_obj_mgr_create
*
* DESCRIPTION
*	This routine creates an object manager used to track all objects by
*	the user.  The object manager assists with debugging efforts by identifying
*	objects that are not destroyed properly.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_obj_mgr_create(void);
/*
* PARAMETERS
*	None.
*
* RETURN VALUE
*	CL_SUCCESS
*		The object manager was succesfully created.
*
*	CL_INSUFFICIENT_MEMORY
*		The object manager could not be allocated.
*
* NOTES
*	This call must succeed before invoking any other object-related function.
*
* SEE ALSO
*	Object, cl_obj_mgr_destroy
*********/



/****f* Component Library: Object/cl_obj_mgr_destroy
* NAME
*	cl_obj_mgr_destroy
*
* DESCRIPTION
*	This routine destroys the object manager created through cl_obj_mgr_create.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_mgr_destroy(void);
/*
* PARAMETERS
*	None.
*
* RETURN VALUE
*	None.
*
* NOTES
*	When the object manager is destroyed, it will display information about all
*	objects that have not yet been destroyed.
*
* SEE ALSO
*	Object, cl_obj_mgr_create
*********/


/****d* Component Library: Object/cl_pfn_obj_call_t
* NAME
*	cl_pfn_obj_call_t
*
* DESCRIPTION
*	The cl_pfn_obj_call_t function type defines the prototype for functions
*	used to return objects to the user.
*
* SYNOPSIS
*/
typedef void
(CL_API *cl_pfn_obj_call_t)(
	IN				struct _cl_obj				*p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] Pointer to a cl_obj_t.  This is the object being returned to
*		the user.
*
* RETURN VALUES
*	None.
*
* NOTES
*	This function type is provided as a prototype for functions provided
*	by users as parameters to the cl_obj_init function.
*
* SEE ALSO
*	Object, cl_obj_init, cl_obj_t
*********/


/****d* Component Library: Object/cl_destroy_type_t
* NAME
*	cl_destroy_type_t
*
* DESCRIPTION
*	Indicates the type of destruction to perform on an object.
*
* SYNOPSIS
*/
typedef enum _cl_destroy_type
{
	CL_DESTROY_ASYNC,
	CL_DESTROY_SYNC

}	cl_destroy_type_t;
/*
* VALUES
*	CL_DESTROY_ASYNC
*		Indicates that the object should be destroyed asynchronously.  Objects
*		destroyed asynchronously complete initial destruction processing, then
*		return the calling thread.  Once their reference count goes to zero,
*		they are queue onto an asynchronous thread to complete destruction
*		processing.
*
*	CL_DESTROY_SYNC
*		Indicates that the object should be destroyed synchronously.  Objects
*		destroyed synchronously wait (block) until their reference count goes
*		to zero.  Once their reference count goes to zero, destruction
*		processing is completed by the calling thread.
*
* SEE ALSO
*	Object, cl_obj_init, cl_obj_destroy, cl_obj_deinit, cl_obj_t
*********/



/****s* Component Library: Object/cl_obj_t
* NAME
*	cl_obj_t
*
* DESCRIPTION
*	Object structure.
*
* SYNOPSIS
*/
typedef struct _cl_obj
{
	cl_pool_item_t				pool_item;	/* Must be first. */
	uint32_t					type;
	cl_state_t					state;
	cl_destroy_type_t			destroy_type;

	cl_async_proc_item_t		async_item;
	cl_event_t					event;

	cl_pfn_obj_call_t			pfn_destroying;
	cl_pfn_obj_call_t			pfn_cleanup;
	cl_pfn_obj_call_t			pfn_free;

	cl_spinlock_t				lock;

	cl_qlist_t					parent_list;
	cl_qlist_t					child_list;

	atomic32_t					ref_cnt;

}	cl_obj_t;
/*
* FIELDS
*	pool_item
*		Used to track the object with the global object manager.  We use
*		a pool item, rather than a list item, to let users store the object
*		in a pool.
*
*	type
*		Stores a user-specified object type.
*
*	state
*		Records the current state of the object, such as initialized,
*		destroying, etc.
*
*	destroy_type
*		Specifies the type of destruction, synchronous or asynchronous, to
*		perform on this object.
*
*	async_item
*		Asynchronous item used when destroying the object asynchronously.
*		This item is queued to an asynchronous thread to complete destruction
*		processing.
*
*	event
*		Event used when destroying the object synchronously.  A call to destroy
*		the object will wait on this event until the destruction has completed.
*
*	pfn_destroying
*		User-specified callback invoked to notify a user that an object has
*		been marked for destruction.  This callback is invoked directly from
*		the thread destroying the object and is used to notify a user that
*		a parent object has invoked a child object's destructor.
*
*	pfn_cleanup
*		User-specified callback invoked as an object is undergoing destruction.
*		For object's destroyed asynchronously, this callback is invoked from
*		the context of the asynchronous destruction thread.  Users may block
*		in the context of this thread; however, further destruction processing
*		will not continue until this callback returns.
*
*	pfn_free
*		User-specified callback invoked to notify a user that an object has
*		been destroyed and is ready for deallocation.  Users should either
*		call cl_obj_deinit or cl_obj_reset from within this callback.
*
*	lock
*		A lock provided by the object.
*
*	parent_list
*		A list of relationships to parent objects that an object is dependent
*		on.
*
*	child_list
*		A list of all child objects that are dependent on this object.
*		Destroying this object will result in all related objects maintained
*		in the child list also being destroyed.
*
*	ref_cnt
*		A count of the number of objects still referencing this object.
*
* SEE ALSO
*	Object, cl_obj_construct, cl_obj_init, cl_obj_destroy,
*	cl_obj_deinit, cl_pfn_obj_call_t, cl_destroy_type_t,
*	cl_pool_item_t, cl_state_t, cl_async_proc_item_t,
*	cl_event_t, cl_spinlock_t, cl_qlist_t, atomic32_t
*********/



/****f* Component Library: Object/cl_obj_construct
* NAME
*	cl_obj_construct
*
* DESCRIPTION
*	This routine prepares an object for use.  The object must be successfully
*	initialized before being used.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_construct(
	IN				cl_obj_t * const			p_obj,
	IN		const	uint32_t					obj_type );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to construct.
*
*	obj_type
*		[in] A user-specified type associated with the object.  This type
*		is recorded by the object for debugging purposes and may be accessed
*		by the user.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This call must succeed before invoking any other function on an object.
*
* SEE ALSO
*	Object, cl_obj_init, cl_obj_destroy, cl_obj_deinit.
*********/


/****f* Component Library: Object/cl_obj_init
* NAME
*	cl_obj_init
*
* DESCRIPTION
*	This routine initializes an object for use.  Upon the successful completion
*	of this call, the object is ready for use.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_obj_init(
	IN				cl_obj_t * const			p_obj,
	IN				cl_destroy_type_t			destroy_type,
	IN		const	cl_pfn_obj_call_t			pfn_destroying OPTIONAL,
	IN		const	cl_pfn_obj_call_t			pfn_cleanup OPTIONAL,
	IN		const	cl_pfn_obj_call_t			pfn_free );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to initialize.
*
*	destroy_type
*		[in] Specifies the destruction model used by this object.
*
*	pfn_destroying
*		[in] User-specified callback invoked to notify a user that an object has
*		been marked for destruction.  This callback is invoked directly from
*		the thread destroying the object and is used to notify a user that
*		a parent object has invoked a child object's destructor.
*
*	pfn_cleanup
*		[in] User-specified callback invoked to an object is undergoing
*		destruction.  For object's destroyed asynchronously, this callback
*		is invoked from the context of the asynchronous destruction thread.
*		Users may block in the context of this thread; however, further
*		destruction processing will not continue until this callback returns.
*
*	pfn_free
*		[in] User-specified callback invoked to notify a user that an object has
*		been destroyed and is ready for deallocation.  Users should either
*		call cl_obj_deinit or cl_obj_reset from within this callback.
*
* RETURN VALUE
*	CL_SUCCESS
*		The object was successfully initialized.
*
*	CL_INSUFFICIENT_MEMORY
*		The object could not allocate the necessary memory resources to
*		complete initialization.
*
* NOTES
*	The three destruction callbacks are used to notify the user of the progress
*	of the destruction, permitting the user to perform an additional processing.
*	Pfn_destroying is used to notify the user that the object is being
*	destroyed.  It is called after an object has removed itself from
*	relationships with its parents, but before it destroys any child objects
*	that it might have.
*
*	Pfn_cleanup is invoked after all child objects have been destroyed, and
*	there are no more references on the object itself.  For objects destroyed
*	asynchronously, pfn_cleanup is invoked from an asynchronous destruction
*	thread.
*
*	Pfn_free is called to notify the user that the destruction of the object has
*	completed.  All relationships have been removed, and all child objects have
*	been destroyed.  Relationship items (cl_obj_rel_t) that were used to
*	identify parent objects are returned to the user through the p_parent_list
*	field of the cl_obj_t structure.
*
* SEE ALSO
*	Object, cl_obj_construct, cl_obj_destroy, cl_obj_deinit,
*	cl_obj_t, cl_destroy_type_t, cl_pfn_obj_call_t,
*********/


/****f* Component Library: Object/cl_obj_destroy
* NAME
*	cl_obj_destroy
*
* DESCRIPTION
*	This routine destroys the specified object.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_destroy(
	IN				cl_obj_t *					p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to destroy.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This routine starts the destruction process for the specified object.  For
*	additional information regarding destruction callbacks, see the following
*	fields in cl_obj_t and parameters in cl_obj_init: pfn_destroying,
*	pfn_cleanup, and pfn_free.
*
*	In most cases, after calling this routine, users should call cl_obj_deinit
*	from within their pfn_free callback routine.
*
* SEE ALSO
*	Object, cl_obj_construct, cl_obj_init, cl_obj_deinit,
*	cl_obj_t, cl_destroy_type_t, cl_pfn_obj_call_t
*********/



/****f* Component Library: Object/cl_obj_deinit
* NAME
*	cl_obj_deinit
*
* DESCRIPTION
*	Release all resources allocated by an object.  This routine should
*	typically be called from a user's pfn_free routine.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_deinit(
	IN				cl_obj_t * const			p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to free.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This call must be invoked to release the object from the global object
*	manager.
*
* SEE ALSO
*	Object, cl_obj_construct, cl_obj_init, cl_obj_destroy, cl_obj_t
*********/



/****f* Component Library: Object/cl_obj_reset
* NAME
*	cl_obj_reset
*
* DESCRIPTION
*	Reset an object's state.  This is called after cl_obj_destroy has
*	been called on a object, but before cl_obj_deinit has been invoked.
*	After an object has been reset, it is ready for re-use.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_reset(
	IN				cl_obj_t * const			p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to reset.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This routine allows an object to be initialized once, then destroyed
*	and re-used multiple times.  This permits the user to allocate and
*	maintain a pool of objects.  The objects may be reset and returned to
*	the pool, rather than freed, after being destroyed.  The objects would
*	not be freed until the pool itself was destroyed.
*
* SEE ALSO
*	Object, cl_obj_destroy, cl_obj_free, cl_obj_t
*********/



/****f* Component Library: Object/cl_obj_ref
* NAME
*	cl_obj_ref
*
* DESCRIPTION
*	Increments the reference count on an object and returns the updated count.
*	This routine is thread safe, but does not result in locking the object.
*
* SYNOPSIS
*/
CL_EXPORT int32_t CL_API
cl_obj_ref(
	IN				cl_obj_t * const			p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to reference.
*
* RETURN VALUE
*	The updated reference count.
*
* SEE ALSO
*	Object, cl_obj_t, cl_obj_deref
*********/



/****f* Component Library: Object/cl_obj_deref
* NAME
*	cl_obj_deref
*
* DESCRIPTION
*	Decrements the reference count on an object and returns the updated count.
*	This routine is thread safe, but results in locking the object.
*
* SYNOPSIS
*/
CL_EXPORT int32_t CL_API
cl_obj_deref(
	IN				cl_obj_t * const			p_obj );
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object to dereference.
*
* RETURN VALUE
*	The updated reference count.
*
* SEE ALSO
*	Object, cl_obj_t, cl_obj_ref
*********/


/****f* Component Library: Object/cl_obj_type
* NAME
*	cl_obj_type
*
* DESCRIPTION
*	Returns the type of an object.
*
* SYNOPSIS
*/
CL_INLINE uint32_t CL_API
cl_obj_type(
	IN				cl_obj_t * const			p_obj )
{
	return p_obj->type;
}
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object whose type to return.
*
* RETURN VALUE
*	The type of the object, as specified in the call to cl_obj_init.
*
* SEE ALSO
*	Object, cl_obj_t, cl_obj_init
*********/


/****f* Component Library: Object/cl_obj_lock
* NAME
*	cl_obj_lock
*
* DESCRIPTION
*	Acquires an object's lock.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_obj_lock(
	IN				cl_obj_t * const			p_obj )
{
	CL_ASSERT( p_obj->state == CL_INITIALIZED ||
		p_obj->state == CL_DESTROYING );
	cl_spinlock_acquire( &p_obj->lock );
}
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object whose lock to acquire.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Object, cl_obj_t, cl_obj_unlock
*********/


/****f* Component Library: Object/cl_obj_unlock
* NAME
*	cl_obj_unlock
*
* DESCRIPTION
*	Releases an object's lock previously acquired by a call to cl_obj_lock.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_obj_unlock(
	IN				cl_obj_t * const			p_obj )
{
	CL_ASSERT( p_obj->state == CL_INITIALIZED ||
		p_obj->state == CL_DESTROYING );
	cl_spinlock_release( &p_obj->lock );
}
/*
* PARAMETERS
*	p_obj
*		[in] A pointer to the object whose lock to release.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*	Object, cl_obj_t, cl_obj_lock
*********/


/****s* Component Library: Object/cl_obj_rel_t
* NAME
*	cl_obj_rel_t
*
* DESCRIPTION
*	Identifies a dependent relationship between two objects.
*
* SYNOPSIS
*/
typedef struct _cl_obj_rel
{
	cl_pool_item_t				pool_item;		/* Must be first. */
	struct _cl_obj				*p_parent_obj;

	cl_list_item_t				list_item;
	struct _cl_obj				*p_child_obj;

}	cl_obj_rel_t;
/*
* FIELDS
*	pool_item
*		An item used to store the relationship in a free pool maintained
*		by the object manager.  This field is also used by the parent object
*		to store the relationship in its child_list.
*
*	p_parent_obj
*		A reference to the parent object for the relationship.
*
*	list_item
*		This field is used by the child object to store the relationship in
*		its parent_list.
*
*	p_child_obj
*		A reference to the child object for the relationship.
*
* NOTES
*	This structure is used to define all dependent relationships.  Dependent
*	relationships are those where the destruction of a parent object result in
*	the destruction of child objects.  For other types of relationships, simple
*	references between objects may be used.
*
*	Relationship items are stored in lists maintained by both the parent
*	and child objects.  References to both objects exist while the
*	relationship is maintained.  Typically, relationships are defined by
*	the user by calling cl_obj_insert_rel, but are destroyed automatically
*	via an object's destruction process.
*
* SEE ALSO
*	Object, cl_rel_alloc, cl_rel_free, cl_obj_insert_rel, cl_obj_remove_rel,
*	cl_obj_destroy
*********/



/****f* Component Library: Object/cl_rel_alloc
* NAME
*	cl_rel_alloc
*
* DESCRIPTION
*	Retrieves an object relationship item from the object manager.
*
* SYNOPSIS
*/
CL_EXPORT cl_obj_rel_t* CL_API
cl_rel_alloc(void);
/*
* PARAMETERS
*	None.
*
* RETURN VALUE
*	A reference to an allocated relationship object, or NULL if no relationship
*	object could be allocated.
*
* NOTES
*	This routine retrieves a cl_obj_rel_t structure from a pool maintained
*	by the object manager.  The pool automatically grows as needed.
*
*	Relationship items are used to describe a dependent relationship between
*	a parent and child object.  In cases where a child has a fixed number of
*	relationships, the user may be able to allocate and manage the cl_obj_rel_t
*	structures more efficiently than obtaining the structures through this call.
*
* SEE ALSO
*	Object, cl_rel_free, cl_obj_insert_rel, cl_obj_remove_rel, cl_obj_destroy
*********/



/****f* Component Library: Object/cl_rel_free
* NAME
*	cl_rel_free
*
* DESCRIPTION
*	Return a relationship object to the global object manager.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_rel_free(
	IN				cl_obj_rel_t * const		p_rel );
/*
* PARAMETERS
*	p_rel
*		[in] A reference to the relationship item to free.
*
* RETURN VALUE
*	None.
*
* NOTES
*	Relationship items must not be freed until both the parent and child
*	object have removed their references to one another.  Relationship items
*	may be freed after calling cl_obj_remove_rel or after the associated
*	child object's free callback has been invoked.  In the latter case, the
*	invalid relationship items are referenced by the child object's parent_list.
*
* SEE ALSO
*	Object, cl_rel_alloc, cl_obj_insert_rel, cl_obj_remove_rel, cl_obj_destroy
*********/



/****f* Component Library: Object/cl_obj_insert_rel
* NAME
*	cl_obj_insert_rel
*
* DESCRIPTION
*	Forms a relationship between two objects, with the existence of the child
*	object dependent on the parent.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_obj_insert_rel(
	IN				cl_obj_rel_t * const		p_rel,
	IN				cl_obj_t * const			p_parent_obj,
	IN				cl_obj_t * const			p_child_obj );
/*
* PARAMETERS
*	p_rel
*		[in] A reference to an unused relationship item.
*
*	p_parent_obj
*		[in] A reference to the parent object.
*
*	p_child_obj
*		[in] A reference to the child object.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This call inserts a relationship between the parent and child object.
*	The relationship allows for the automatic destruction of the child object
*	if the parent is destroyed.
*
*	A given object can have multiple parent and child objects, but the
*	relationships must form into an object tree.  That is, there cannot be any
*	cycles formed through the parent-child relationships.  (For example, an
*	object cannot be both the parent and a child of a second object.)
*
* SEE ALSO
*	Object, cl_rel_alloc, cl_rel_free, cl_obj_remove_rel, cl_obj_destroy
*********/



/****f* Component Library: Object/cl_obj_insert_rel_parent_locked
* NAME
*	cl_obj_insert_rel_parent_locked
*
* DESCRIPTION
*	Forms a relationship between two objects, with the existence of the child
*	object dependent on the parent.  The parent's object lock is held.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_obj_insert_rel_parent_locked(
	IN				cl_obj_rel_t * const		p_rel,
	IN				cl_obj_t * const			p_parent_obj,
	IN				cl_obj_t * const			p_child_obj );
/*
* PARAMETERS
*	p_rel
*		[in] A reference to an unused relationship item.
*
*	p_parent_obj
*		[in] A reference to the parent object.
*
*	p_child_obj
*		[in] A reference to the child object.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This call inserts a relationship between the parent and child object.
*	The relationship allows for the automatic destruction of the child object
*	if the parent is destroyed.
*
*	A given object can have multiple parent and child objects, but the
*	relationships must form into an object tree.  That is, there cannot be any
*	cycles formed through the parent-child relationships.  (For example, an
*	object cannot be both the parent and a child of a second object.)
*
*	This call requires the caller to already hold the parent object's lock.
*
* SEE ALSO
*	Object, cl_rel_alloc, cl_rel_free, cl_obj_remove_rel, cl_obj_destroy
*********/



/****f* Component Library: Object/cl_obj_remove_rel
* NAME
*	cl_obj_remove_rel
*
* DESCRIPTION
*	Manually removes a relationship between two objects.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_obj_remove_rel(
	IN				cl_obj_rel_t * const		p_rel );
/*
* PARAMETERS
*	p_rel
*		[in] A reference to the relationship to remove.
*
* RETURN VALUE
*	None.
*
* NOTES
*	This routine permits a user to manually remove a dependent relationship
*	between two objects.  When removing a relationship using this call, the
*	user must ensure that objects referenced by the relationship are not
*	destroyed, either directly or indirectly via a parent.
*
* SEE ALSO
*	Object, cl_rel_alloc, cl_rel_free, cl_obj_insert_rel, cl_obj_destroy
*********/


#ifdef __cplusplus
}
#endif


#endif /* __CL_OBJ_H__ */

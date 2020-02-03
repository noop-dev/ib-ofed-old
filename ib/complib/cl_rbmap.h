/*++
Copyright © InfiniCon Systems, Inc.  All rights reserved.

THIS SOFTWARE IS PROVIDED BY INFINICON SYSTEMS, INC. ("INFINICON") TO EACH
PERSON OR COMPANY ("RECIPIENT") ON AN "AS IS" BASIS.  ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL INFINICON BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED OR ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

Any agreements between InfiniCon and the Recipient shall apply to Recipient's
use of the Software.
--*/


/*
 * Abstract:
 *	Declaration of primitive red/black map, a red/black tree where the caller
 *	always provides all necessary storage.
 *
 *	This tree implementation exposes functions required for the client to
 *	manually walk the map, allowing clients to implement various methods
 *	of comparisson.
 *
 * Environment:
 *	All
 *
 * $Revision$
 */


#ifndef _CL_RBMAP_H_
#define _CL_RBMAP_H_


#include <complib/cl_types.h>


/****h* Component Library/RB Map
* NAME
*	RB Map
*
* DESCRIPTION
*	RB map implements a binary tree that stores user provided cl_rbmap_item_t
*	structures.  Each item stored in a RB map has a unique key
*	(duplicates are not allowed).  RB map provides the ability to
*	efficiently search for an item given a key.
*
*	RB map does not allocate any memory, and can therefore not fail
*	any operations due to insufficient memory.  RB map can thus be useful
*	in minimizing the error paths in code.
*
*	RB map is not thread safe, and users must provide serialization when
*	adding and removing items from the map.
*
*	The RB map functions operate on a cl_rbmap_t structure which should be
*	treated as opaque and should be manipulated only through the provided
*	functions.
*
* SEE ALSO
*	Structures:
*		cl_rbmap_t, cl_rbmap_item_t
*
*	Initialization:
*		cl_rbmap_init
*
*	Iteration:
*		cl_rbmap_root, cl_rbmap_end, cl_rbmap_left, cl_rbmap_right, cl_rbmap_up
*
*	Manipulation:
*		cl_rbmap_insert, cl_rbmap_get, cl_rbmap_remove_item, cl_rbmap_remove,
*		cl_rbmap_reset, cl_rbmap_merge, cl_rbmap_delta
*
*	Search:
*		cl_rbmap_apply_func
*
*	Attributes:
*		cl_rbmap_count, cl_is_rbmap_empty,
*********/


/****i* Component Library: RB Map/cl_map_color_t
* NAME
*	cl_map_color_t
*
* DESCRIPTION
*	The cl_map_color_t enumerated type is used to note the color of
*	nodes in a map.
*
* SYNOPSIS
*/
typedef enum _cl_map_color
{
	CL_MAP_RED,
	CL_MAP_BLACK

} cl_map_color_t;
/*
* VALUES
*	CL_MAP_RED
*		The node in the map is red.
*
*	CL_MAP_BLACK
*		The node in the map is black.
*
* SEE ALSO
*	RB Map, cl_rbmap_item_t
*********/


/****s* Component Library: RB Map/cl_rbmap_item_t
* NAME
*	cl_rbmap_item_t
*
* DESCRIPTION
*	The cl_rbmap_item_t structure is used by maps to store objects.
*
*	The cl_rbmap_item_t structure should be treated as opaque and should
*	be manipulated only through the provided functions.
*
* SYNOPSIS
*/
typedef struct _cl_rbmap_item
{
	struct _cl_rbmap_item		*p_left;
	struct _cl_rbmap_item		*p_right;
	struct _cl_rbmap_item		*p_up;
	cl_map_color_t				color;
#ifdef _DEBUG_
	struct _cl_rbmap			*p_map;
#endif

} cl_rbmap_item_t;
/*
* FIELDS
*	p_left
*		Pointer to the map item that is a child to the left of the node.
*
*	p_right
*		Pointer to the map item that is a child to the right of the node.
*
*	p_up
*		Pointer to the map item that is the parent of the node.
*
*	color
*		Indicates whether a node is red or black in the map.
*
* NOTES
*	None of the fields of this structure should be manipulated by users, as
*	they are crititcal to the proper operation of the map in which they
*	are stored.
*
*	To allow storing items in either a quick list, a quick pool, or a quick
*	map, the map implementation guarantees that the map item can be safely
*	cast to a pool item used for storing an object in a quick pool, or cast to
*	a list item used for storing an object in a quick list.  This removes the
*	need to embed a map item, a list item, and a pool item in objects that need
*	to be stored in a quick list, a quick pool, and a RB map.
*
* SEE ALSO
*	RB Map, cl_rbmap_insert, cl_rbmap_key, cl_pool_item_t, cl_list_item_t
*********/


/****s* Component Library: RB Map/cl_rbmap_t
* NAME
*	cl_rbmap_t
*
* DESCRIPTION
*	Quick map structure.
*
*	The cl_rbmap_t structure should be treated as opaque and should
*	be manipulated only through the provided functions.
*
* SYNOPSIS
*/
typedef struct _cl_rbmap
{
	cl_rbmap_item_t	root;
	cl_rbmap_item_t	nil;
	cl_state_t		state;
	size_t			count;

} cl_rbmap_t;
/*
* PARAMETERS
*	root
*		Map item that serves as root of the map.  The root is set up to
*		always have itself as parent.  The left pointer is set to point to
*		the item at the root.
*
*	nil
*		Map item that serves as terminator for all leaves, as well as providing
*		the list item used as quick list for storing map items in a list for
*		faster traversal.
*
*	state
*		State of the map, used to verify that operations are permitted.
*
*	count
*		Number of items in the map.
*
* SEE ALSO
*	RB Map
*********/


#ifdef __cplusplus
extern "C" {
#endif


/****f* Component Library: RB Map/cl_rbmap_count
* NAME
*	cl_rbmap_count
*
* DESCRIPTION
*	The cl_rbmap_count function returns the number of items stored
*	in a RB map.
*
* SYNOPSIS
*/
CL_INLINE size_t CL_API
cl_rbmap_count(
	IN	const cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );
	CL_ASSERT( p_map->state == CL_INITIALIZED );
	return( p_map->count );
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure whose item count to return.
*
* RETURN VALUE
*	Returns the number of items stored in the map.
*
* SEE ALSO
*	RB Map, cl_is_rbmap_empty
*********/


/****f* Component Library: RB Map/cl_is_rbmap_empty
* NAME
*	cl_is_rbmap_empty
*
* DESCRIPTION
*	The cl_is_rbmap_empty function returns whether a RB map is empty.
*
* SYNOPSIS
*/
CL_INLINE boolean_t CL_API
cl_is_rbmap_empty(
	IN	const cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );
	CL_ASSERT( p_map->state == CL_INITIALIZED );

	return( p_map->count == 0 );
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure to test for emptiness.
*
* RETURN VALUES
*	TRUE if the RB map is empty.
*
*	FALSE otherwise.
*
* SEE ALSO
*	RB Map, cl_rbmap_count, cl_rbmap_reset
*********/


/****f* Component Library: RB Map/cl_rbmap_reset
* NAME
*	cl_rbmap_reset
*
* DESCRIPTION
*	The cl_rbmap_reset function removes all items in a RB map,
*	leaving it empty.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_rbmap_reset(
	IN	cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );
	CL_ASSERT( p_map->state == CL_INITIALIZED );

	p_map->root.p_left = &p_map->nil;
	p_map->count = 0;
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure to empty.
*
* RETURN VALUES
*	This function does not return a value.
*
* SEE ALSO
*	RB Map, cl_rbmap_remove, cl_rbmap_remove_item
*********/


/****f* Component Library: RB Map/cl_rbmap_init
* NAME
*	cl_rbmap_init
*
* DESCRIPTION
*	The cl_rbmap_init function initialized a RB map for use.
*
* SYNOPSIS
*/
CL_INLINE void CL_API
cl_rbmap_init(
	IN	cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );

	/* special setup for the root node */
	p_map->root.p_left = &p_map->nil;
	p_map->root.p_right = &p_map->nil;
	p_map->root.p_up = &p_map->root;
	p_map->root.color = CL_MAP_BLACK;

	/* Setup the node used as terminator for all leaves. */
	p_map->nil.p_left = &p_map->nil;
	p_map->nil.p_right = &p_map->nil;
	p_map->nil.p_up = &p_map->nil;
	p_map->nil.color = CL_MAP_BLACK;

#ifdef _DEBUG_
	p_map->root.p_map = p_map;
	p_map->nil.p_map = p_map;
#endif

	p_map->state = CL_INITIALIZED;

	p_map->count = 0;
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure to initialize.
*
* RETURN VALUES
*	This function does not return a value.
*
* NOTES
*	Allows calling RB map manipulation functions.
*
* SEE ALSO
*	RB Map, cl_rbmap_insert, cl_rbmap_remove
*********/


/****f* Component Library: RB Map/cl_rbmap_root
* NAME
*	cl_rbmap_root
*
* DESCRIPTION
*	The cl_rbmap_root function returns the root of a RB map.
*
* SYNOPSIS
*/
CL_INLINE cl_rbmap_item_t* const CL_API
cl_rbmap_root(
	IN	const cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );
	return( p_map->root.p_left );
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure whose root to return.
*
* RETURN VALUE
*	Pointer to the end of the map.
*
* NOTES
*	cl_rbmap_end is useful for determining the validity of map items returned
*	by cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, or cl_rbmap_prev.  If the map
*	item pointer returned by any of these functions compares to the end, the
*	end of the map was encoutered.
*	When using cl_rbmap_head or cl_rbmap_tail, this condition indicates that
*	the map is empty.
*
* SEE ALSO
*	RB Map, cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, cl_rbmap_prev,
*	cl_rbmap_end, cl_rbmap_left, cl_rbmap_right, cl_rbmap_up
*********/


/****f* Component Library: RB Map/cl_rbmap_end
* NAME
*	cl_rbmap_end
*
* DESCRIPTION
*	The cl_rbmap_end function returns the end of a RB map.
*
* SYNOPSIS
*/
CL_INLINE const cl_rbmap_item_t* const CL_API
cl_rbmap_end(
	IN	const cl_rbmap_t* const	p_map )
{
	CL_ASSERT( p_map );
	CL_ASSERT( p_map->state == CL_INITIALIZED );
	/* Nil is the end of the map. */
	return( &p_map->nil );
}
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure whose end to return.
*
* RETURN VALUE
*	Pointer to the end of the map.
*
* NOTES
*	cl_rbmap_end is useful for determining the validity of map items returned
*	by cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, or cl_rbmap_prev.  If the map
*	item pointer returned by any of these functions compares to the end, the
*	end of the map was encoutered.
*	When using cl_rbmap_head or cl_rbmap_tail, this condition indicates that
*	the map is empty.
*
* SEE ALSO
*	RB Map, cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, cl_rbmap_prev
*	cl_rbmap_root, cl_rbmap_left, cl_rbmap_right, cl_rbmap_up
*********/


/****f* Component Library: RB Map/cl_rbmap_left
* NAME
*	cl_rbmap_left
*
* DESCRIPTION
*	The cl_rbmap_left function returns the map item to the left
*	of the specified map item.
*
* SYNOPSIS
*/
CL_INLINE cl_rbmap_item_t* CL_API
cl_rbmap_left(
	IN	const cl_rbmap_item_t* const	p_item )
{
	CL_ASSERT( p_item );
	return( (cl_rbmap_item_t*)p_item->p_left );
}
/*
* PARAMETERS
*	p_item
*		[in] Pointer to a map item whose predecessor to return.
*
* RETURN VALUES
*	Pointer to the map item to the left in a RB map.
*
*	Pointer to the map end if no item is to the left.
*
* SEE ALSO
*	RB Map, cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, cl_rbmap_end,
*	cl_rbmap_item_t
*********/


/****f* Component Library: RB Map/cl_rbmap_right
* NAME
*	cl_rbmap_right
*
* DESCRIPTION
*	The cl_rbmap_right function returns the map item to the right
*	of the specified map item.
*
* SYNOPSIS
*/
CL_INLINE cl_rbmap_item_t* CL_API
cl_rbmap_right(
	IN	const cl_rbmap_item_t* const	p_item )
{
	CL_ASSERT( p_item );
	return( (cl_rbmap_item_t*)p_item->p_right );
}
/*
* PARAMETERS
*	p_item
*		[in] Pointer to a map item whose predecessor to return.
*
* RETURN VALUES
*	Pointer to the map item to the right in a RB map.
*
*	Pointer to the map end if no item is to the right.
*
* SEE ALSO
*	RB Map, cl_rbmap_head, cl_rbmap_tail, cl_rbmap_next, cl_rbmap_end,
*	cl_rbmap_item_t
*********/


/****f* Component Library: RB Map/cl_rbmap_insert
* NAME
*	cl_rbmap_insert
*
* DESCRIPTION
*	The cl_rbmap_insert function inserts a map item into a RB map.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_rbmap_insert(
	IN	cl_rbmap_t* const		p_map,
	IN	cl_rbmap_item_t* const	p_insert_at,
	IN	cl_rbmap_item_t* const	p_item,
	IN	boolean_t				left );
/*
* PARAMETERS
*	p_map
*		[in] Pointer to a cl_rbmap_t structure into which to add the item.
*
*	p_insert_at
*		[in] Pointer to a cl_rbmap_item_t structure to serve as parent
*		to p_item.
*
*	p_item
*		[in] Pointer to a cl_rbmap_item_t stucture to insert into the RB map.
*
*	left
*		[in] Indicates that p_item should be inserted to the left of p_insert_at.
*
* RETURN VALUE
*	Pointer to the item in the map with the specified key.  If insertion
*	was successful, this is the pointer to the item.  If an item with the
*	specified key already exists in the map, the pointer to that item is
*	returned.
*
* NOTES
*	Insertion operations may cause the RB map to rebalance.
*
* SEE ALSO
*	RB Map, cl_rbmap_remove, cl_rbmap_item_t
*********/


/****f* Component Library: RB Map/cl_rbmap_remove_item
* NAME
*	cl_rbmap_remove_item
*
* DESCRIPTION
*	The cl_rbmap_remove_item function removes the specified map item
*	from a RB map.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_rbmap_remove_item(
	IN	cl_rbmap_t* const		p_map,
	IN	cl_rbmap_item_t* const	p_item );
/*
* PARAMETERS
*	p_item
*		[in] Pointer to a map item to remove from its RB map.
*
* RETURN VALUES
*	This function does not return a value.
*
*	In a debug build, cl_rbmap_remove_item asserts that the item being removed
*	is in the specified map.
*
* NOTES
*	Removes the map item pointed to by p_item from its RB map.
*
* SEE ALSO
*	RB Map, cl_rbmap_remove, cl_rbmap_reset, cl_rbmap_insert
*********/


#ifdef __cplusplus
}
#endif


#endif	/* _CL_RBMAP_H_ */

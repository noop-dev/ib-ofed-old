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
 * $Id: cl_types.h 2073 2007-11-13 11:38:40Z leonid $
 */


/*
 * Abstract:
 *	Defines standard return codes, keywords, macros, and debug levels.
 *
 * Environment:
 *	All supported platforms
 */


#ifndef _CL_TYPES_H_
#define _CL_TYPES_H_


#include <complib/cl_types_osd.h>


typedef uint16_t		net16_t;
typedef uint32_t		net32_t;
typedef uint64_t		net64_t;



/****d* Component Library: Pointer Manipulation/offsetof
* NAME
*	offsetof
*
* DESCRIPTION
*	The offsetof macro returns the offset of a member within a structure.
*
* SYNOPSIS
*	uintn_t
*	offsetof(
*		IN TYPE,
*		IN MEMBER );
*
* PARAMETERS
*	TYPE
*		[in] Name of the structure containing the specified member.
*
*	MEMBER
*		[in] Name of the member whose offset in the specified structure
*		is to be returned.
*
* RETURN VALUE
*	Number of bytes from the beginning of the structure to the
*	specified member.
*
* SEE ALSO
*	PARENT_STRUCT
*********/
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((uintn_t) &((TYPE *)0)->MEMBER)
#endif


/****d* Component Library: Pointer Manipulation/PARENT_STRUCT
* NAME
*	PARENT_STRUCT
*
* DESCRIPTION
*	The PARENT_STRUCT macro returns a pointer to a structure
*	given a name and pointer to one of its members.
*
* SYNOPSIS
*	PARENT_TYPE*
*	PARENT_STRUCT(
*		IN void* const p_member,
*		IN PARENT_TYPE,
*		IN MEMBER_NAME );
*
* PARAMETERS
*	p_member
*		[in] Pointer to the MEMBER_NAME member of a PARENT_TYPE structure.
*
*	PARENT_TYPE
*		[in] Name of the structure containing the specified member.
*
*	MEMBER_NAME
*		[in] Name of the member whose address is passed in the p_member
*		parameter.
*
* RETURN VALUE
*	Pointer to a structure of type PARENT_TYPE whose MEMBER_NAME member is
*	located at p_member.
*
* SEE ALSO
*	offsetof
*********/
#ifndef PARENT_STRUCT
#define PARENT_STRUCT(p_member, PARENT_TYPE, MEMBER_NAME) \
	((PARENT_TYPE*)((uint8_t*)(p_member) - offsetof(PARENT_TYPE, MEMBER_NAME)))
#endif

/****d* Component Library/Parameter Keywords
* NAME
*	Parameter Keywords
*
* DESCRIPTION
*	The Parameter Keywords can be used to clarify the usage of function
*	parameters to users.
*
* VALUES
*	IN
*		Designates that the parameter is used as input to a function.
*
*	OUT
*		Designates that the parameter's value will be set by the function.
*
*	OPTIONAL
*		Designates that the parameter is optional, and may be NULL.
*		The OPTIONAL keyword, if used, follows the parameter name.
*
* EXAMPLE
*	// Function declaration.
*	void*
*	my_func(
*	    IN void* const p_param1,
*	    OUT void** const p_handle OPTIONAL );
*
* NOTES
*	Multiple keywords can apply to a single parameter. The IN and OUT
*	keywords precede the parameter type. The OPTIONAL
*	keyword, if used, follows the parameter name.
*********/
#ifndef		IN
#define		IN			/* Function input parameter */
#endif
#ifndef		OUT
#define		OUT			/* Function output parameter */
#endif
#ifndef		OPTIONAL
#define		OPTIONAL	/* Optional function parameter - NULL if not used */
#endif


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%                  Function Returns And Completion Codes					 %%
%%																			 %%
%% The text for any addition to this enumerated type must be added to the	 %%
%% string array defined in <cl_statustext.c>.								 %%
%%																			 %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


/****d* Component Library/Data Types
* NAME
*	Data Types
*
* DESCRIPTION
*	The component library provides and uses explicitly sized types.
*
* VALUES
*	char
*		8-bit, defined by compiler.
*
*	void
*		0-bit, defined by compiler.
*
*	int8_t
*		8-bit signed integer.
*
*	uint8_t
*		8-bit unsigned integer.
*
*	int16_t
*		16-bit signed integer.
*
*	uint16_t
*		16-bit unsigned integer.
*
*	net16_t
*		16-bit network byte order value.
*
*	int32_t
*		32-bit signed integer.
*
*	uint32_t
*		32-bit unsigned integer.
*
*	net32_t
*		32-bit network byte order value.
*
*	int64_t
*		64-bit signed integer.
*
*	uint64_t
*		64-bit unsigned integer.
*
*	net64_t
*		64-bit network byte order value.
*
*	intn_t
*		Signed natural sized integer.  32-bit on a 32-bit platform, 64-bit on
*		a 64-bit platform.
*
*	uintn_t
*		Unsigned natural sized integer.  32-bit on a 32-bit platform, 64-bit on
*		a 64-bit platform.
*
*	boolean_t
*		integral sized.  Set to TRUE or FALSE and used in logical expressions.
*
* NOTES
*	Pointer types are not defined as these provide no value and can potentially
*	lead to naming confusion.
*********/


/****d* Component Library: Data Types/cl_status_t
* NAME
*	cl_status_t
*
* DESCRIPTION
*	The cl_status_t return types are used by the component library to
*	provide detailed function return values.
*
* SYNOPSIS
*/
#ifdef CL_KERNEL

typedef enum _cl_status
{
	CL_SUCCESS 					= STATUS_SUCCESS,
	CL_ERROR						= STATUS_DRIVER_INTERNAL_ERROR,
	CL_INVALID_STATE				= STATUS_INVALID_DEVICE_STATE,
	CL_INVALID_OPERATION			= STATUS_NOT_SUPPORTED,
	CL_INVALID_SETTING			= STATUS_INVALID_PARAMETER_MIX,
	CL_INVALID_PARAMETER			= STATUS_INVALID_PARAMETER,
	CL_INSUFFICIENT_RESOURCES	= STATUS_INSUFFICIENT_RESOURCES,
	CL_INSUFFICIENT_MEMORY		= STATUS_NO_MEMORY,
	CL_INVALID_PERMISSION			= STATUS_ACCESS_DENIED,
	CL_COMPLETED					= STATUS_EVENT_DONE,
	CL_NOT_DONE					= STATUS_ABANDONED,
	CL_PENDING						= STATUS_PENDING,
	CL_TIMEOUT						= STATUS_TIMEOUT,
	CL_CANCELED					= STATUS_CANCELLED,
	CL_REJECT						= STATUS_REQUEST_NOT_ACCEPTED,
	CL_OVERRUN						= STATUS_DATA_OVERRUN,
	CL_NOT_FOUND					= STATUS_NOT_FOUND,
	CL_UNAVAILABLE				= STATUS_DEVICE_NOT_READY,
	CL_BUSY						= STATUS_DEVICE_BUSY,
	CL_DISCONNECT					= STATUS_LOCAL_DISCONNECT,
	CL_DUPLICATE					= STATUS_DUPLICATE_NAME,
	CL_INVALID_REQUEST			= STATUS_INVALID_DEVICE_REQUEST,
	CL_INVALID_HANDLE			= STATUS_INVALID_HANDLE,
	CL_CONNECTION_INVALID		= STATUS_CONNECTION_INVALID

} cl_status_t;

#else

typedef enum _cl_status
{
	CL_SUCCESS = 0,
	CL_ERROR,
	CL_INVALID_STATE,
	CL_INVALID_OPERATION,
	CL_INVALID_SETTING,
	CL_INVALID_PARAMETER,
	CL_INSUFFICIENT_RESOURCES,
	CL_INSUFFICIENT_MEMORY,
	CL_INVALID_PERMISSION,
	CL_COMPLETED,
	CL_NOT_DONE,
	CL_PENDING,
	CL_TIMEOUT,
	CL_CANCELED,
	CL_REJECT,
	CL_OVERRUN,
	CL_NOT_FOUND,
	CL_UNAVAILABLE,
	CL_BUSY,
	CL_DISCONNECT,
	CL_DUPLICATE,
	CL_INVALID_REQUEST,

	CL_STATUS_COUNT			/* should be the last value */

} cl_status_t;

#endif

/*
* SEE ALSO
*	Data Types, CL_STATUS_MSG
*********/


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


#ifndef CL_KERNEL
/* Status values above converted to text for easier printing. */
CL_EXPORT const char* cl_status_text[CL_STATUS_COUNT];
#endif

#ifndef cl_panic
/****f* Component Library: Error Trapping/cl_panic
* NAME
*	cl_panic
*
* DESCRIPTION
*	Halts execution of the current process.  Halts the system if called in
*	from the kernel.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_panic(
	IN	const char* const	message,
	IN	... );
/*
* PARAMETERS
*	message
*		[in] ANSI string formatted identically as for a call to the standard C
*		function printf describing the cause for the panic.
*
*	...
*		[in] Extra parameters for string formatting, as defined for the
*		standard C function printf.
*
* RETURN VALUE
*	This function does not return.
*
* NOTES
*	The formatting of the message string is the same as for printf
*
*	cl_panic sends the message to the current message logging target.
*********/
#endif	/* cl_panic */


/****d* Component Library: Data Types/CL_STATUS_MSG
* NAME
*	CL_STATUS_MSG
*
* DESCRIPTION
*	The CL_STATUS_MSG macro returns a textual representation of
*	an cl_status_t code.
*
* SYNOPSIS
*	const char*
*	CL_STATUS_MSG(
*		IN cl_status_t errcode );
*
* PARAMETERS
*	errcode
*		[in] cl_status_t code for which to return a text representation.
*
* RETURN VALUE
*	Pointer to a string containing a textual representation of the errcode
*	parameter.
*
* NOTES
*	This function performs boundary checking on the cl_status_t value,
*	masking off the upper 24-bits. If the value is out of bounds, the string
*	"invalid status code" is returned.
*
* SEE ALSO
*	cl_status_t
*********/
#ifndef CL_KERNEL
#define CL_STATUS_MSG( errcode ) \
	((errcode < CL_STATUS_COUNT)?cl_status_text[errcode]:"invalid status code")
#endif	


#if !defined( FALSE )
#define FALSE	0
#endif	/* !defined( TRUE ) */


#if !defined( TRUE )
#define TRUE	(!FALSE)
#endif	/* !defined( TRUE ) */


/****d* Component Library: Unreferenced Parameters/UNUSED_PARAM
* NAME
*	UNUSED_PARAM
*
* DESCRIPTION
*	The UNUSED_PARAM macro can be used to eliminates compiler warnings related
*	to intentionally unused formal parameters in function implementations.
*
* SYNOPSIS
*	UNUSED_PARAM( P )
*
* EXAMPLE
*	void my_func( int32_t value )
*	{
*		UNUSED_PARAM( value );
*	}
*********/


/****d* Component Library/Object States
* NAME
*	Object States
*
* DESCRIPTION
*	The object states enumerated type defines the valid states of components.
*
* SYNOPSIS
*/
typedef enum _cl_state
{
	CL_UNINITIALIZED = 1,
	CL_INITIALIZED,
	CL_DESTROYING,
	CL_DESTROYED

} cl_state_t;
/*
* VALUES
*	CL_UNINITIALIZED
*		Indicates that initialization was not invoked successfully.
*
*	CL_INITIALIZED
*		Indicates initialization was successful.
*
*	CL_DESTROYING
*		Indicates that the object is undergoing destruction.
*
*	CL_DESTROYED
*		Indicates that the object's destructor has already been called.  Most
*		objects set their final state to CL_DESTROYED before freeing the
*		memory associated with the object.
*********/


/****d* Component Library: Object States/cl_is_state_valid
* NAME
*	cl_is_state_valid
*
* DESCRIPTION
*	The cl_is_state_valid function returns whether a state has a valid value.
*
* SYNOPSIS
*/
CL_INLINE boolean_t CL_API
cl_is_state_valid(
	IN	const cl_state_t	state )
{
	return( (state == CL_UNINITIALIZED) || (state == CL_INITIALIZED) ||
		(state == CL_DESTROYING) || (state == CL_DESTROYED) );
}
/*
* PARAMETERS
*	state
*		State whose value to validate.
*
* RETURN VALUES
*	TRUE if the specified state has a valid value.
*
*	FALSE otherwise.
*
* NOTES
*	This function is used in debug builds to check for valid states.  If an
*	uninitialized object is passed, the memory for the state may cause the
*	state to have an invalid value.
*
* SEE ALSO
*	Object States
*********/


#ifdef __cplusplus
}
#endif	/* __cplusplus */


#endif /* _DATA_TYPES_H_ */

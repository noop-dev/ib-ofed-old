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
 * $Id: cl_reqmgr.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of asynchronous request manager.  The request manager does
 *	not return resources, only notifies the user when resources are available.
 *
 * Environment:
 *	All
 */


#ifndef _CL_REQ_MGR_H_
#define _CL_REQ_MGR_H_


#include <complib/cl_qpool.h>


/****h* Component Library/Request Manager
* NAME
*	Request Manager
*
* DESCRIPTION
*	The Request Manager manages synchronous as well as asynchronous
*	requests for objects.
*
*	Request manager does not supply the objects, but merely returns whether
*	objects are available to satisfy requests. This allows users to use
*	various sources for objects.
*
*	While the request manager manages synchronous and asynchronous requests
*	for objects, it does not itself operate asynchronously. Instead, the
*	cl_req_mgr_resume function returns information for resuming asynchronous
*	requests. If a call to cl_req_mgr_resume returns CL_SUCCESS, additional
*	requests may be able to resume. It is recommended that users flush
*	pending requests by calling cl_req_mgr_resume while CL_SUCCESS is returned.
*
*	The request manager functions operates on a cl_req_mgr_t structure which
*	should be treated as opaque and should be manipulated only through the
*	provided functions.
*
* SEE ALSO
*	Types:
*		cl_req_type_t
*
*	Structures:
*		cl_req_mgr_t
*
*	Callbacks:
*		cl_pfn_req_cb_t, cl_pfn_reqmgr_get_count_t
*
*	Initialization/Destruction:
*		cl_req_mgr_construct, cl_req_mgr_init, cl_req_mgr_destroy
*
*	Manipulation:
*		cl_req_mgr_get, cl_req_mgr_resume
*
*	Attributes:
*		cl_is_req_mgr_inited, cl_req_mgr_count
*********/


/****d* Component Library: Request Manager/cl_pfn_req_cb_t
* NAME
*	cl_pfn_req_cb_t
*
* DESCRIPTION
*	The cl_pfn_req_cb_t function type defines the prototype for functions
*	used to store a function pointer to a user defined function.
*
* SYNOPSIS
*/
typedef void
(CL_API *cl_pfn_req_cb_t)( void );
/*
* PARAMETERS
*	This function does not take parameters.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Function pointers specified by this parameter do not have to match the
*	defined syntax, as these callbacks are never invoked directly by the
*	request manager.  When specifying a function with a different prototype,
*	cast the function pointer to this type.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_get, cl_req_mgr_resume
*********/


/****d* Component Library: Request Manager/cl_req_type_t
* NAME
*	cl_req_type_t
*
* DESCRIPTION
*	The cl_req_type_t enumerated type describes the type of request.
*
* SYNOPSIS
*/
typedef enum _cl_req_type
{
	REQ_GET_SYNC,
	REQ_GET_ASYNC,
	REQ_GET_PARTIAL_OK

} cl_req_type_t;
/*
* VALUES
*	REQ_GET_SYNC
*		Synchronous request.
*
*	REQ_GET_ASYNC
*		Asynchronous requests for which all objects are required at once.
*
*	REQ_GET_PARTIAL_OK
*		Asynchronous requests that may be broken into multiple smaller requests.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_get
*********/


/****d* Component Library: Request Manager/cl_pfn_reqmgr_get_count_t
* NAME
*	cl_pfn_reqmgr_get_count_t
*
* DESCRIPTION
*	The cl_pfn_reqmgr_get_count_t function type defines the prototype for
*	functions used to retrieve the number of available objects in a pool.
*
* SYNOPSIS
*/
typedef size_t
(CL_API *cl_pfn_reqmgr_get_count_t)(
	IN	void*	context );
/*
* PARAMETERS
*	Context
*		[in] Context provided in a call to cl_req_mgr_init by
*		the get_context parameter.
*
* RETURN VALUE
*	Returns the number of objects available in an object pool for which
*	requests are managed by a request manager.
*
* NOTES
*	This function type is provided as function prototype reference for the
*	function passed into cl_req_mgr_init. This function is invoked by the
*	request manager when trying to fulfill requests for resources, either
*	through a call to cl_req_mgr_get or cl_req_mgr_resume.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_init, cl_req_mgr_get, cl_req_mgr_resume
*********/


/****s* Component Library: Request Manager/cl_req_mgr_t
* NAME
*	cl_req_mgr_t
*
* DESCRIPTION
*	Quick composite pool structure.
*
*	The cl_req_mgr_t structure should be treated as opaque and should be
*	manipulated only through the provided functions.
*
* SYNOPSIS
*/
typedef struct _cl_req_mgr
{
	cl_pfn_reqmgr_get_count_t	pfn_get_count;
	const void					*get_context;
	cl_qlist_t					request_queue;
	cl_qpool_t					request_pool;

} cl_req_mgr_t;
/*
* FIELDS
*	pfn_get_count
*		Pointer to the count callback function.
*
*	get_context
*		Context to pass as single parameter to count callback.
*
*	request_queue
*		Pending requests for elements.
*
*	request_pool
*		Pool of request structures for storing requests in the request queue.
*
* SEE ALSO
*	Request Manager
*********/


#ifdef __cplusplus
extern "C"
{
#endif


/****f* Component Library: Request Manager/cl_req_mgr_construct
* NAME
*	cl_req_mgr_construct
*
* DESCRIPTION
*	The cl_req_mgr_construct function constructs a request manager.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_req_mgr_construct(
	IN	cl_req_mgr_t* const	p_req_mgr );
/*
* PARAMETERS
*	p_req_mgr
*		[in] Pointer to a cl_req_mgr_t structure to construct.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	cl_req_mgr_construct allows calling cl_req_mgr_destroy without first
*	calling cl_req_mgr_init.
*
*	Calling cl_req_mgr_construct is a prerequisite to calling any other
*	request manager function except cl_req_mgr_init.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_init, cl_req_mgr_destroy
*********/


/****f* Component Library: Request Manager/cl_req_mgr_init
* NAME
*	cl_req_mgr_init
*
* DESCRIPTION
*	The cl_req_mgr_init function initializes a request manager for use.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_req_mgr_init(
	IN	cl_req_mgr_t* const			p_req_mgr,
	IN	cl_pfn_reqmgr_get_count_t	pfn_get_count,
	IN	const void* const			get_context );
/*
* PARAMETERS
*	p_req_mgr
*		[in] Pointer to a cl_req_mgr_t structure to initialize.
*
*	pfn_get_count
*		[in] Callback function invoked by the request manager to get the
*		number of objects available in a pool of objects for which the
*		request manager is managing requests.
*		See the cl_pfn_req_mgr_get_count_t function type declaration for
*		details about the callback function.
*
*	get_context
*		[in] Context to pass into the function specified by the
*		pfn_get_count parameter.
*
* RETURN VALUES
*	CL_SUCCESS if the request manager was successfully initialized.
*
*	CL_INSUFFICIENT_MEMORY if there was not enough memory to initialize
*	the request manager.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_construct, cl_req_mgr_destroy, cl_req_mgr_get,
*	cl_req_mgr_resume, cl_pfn_req_mgr_get_count_t
*********/


/****f* Component Library: Request Manager/cl_req_mgr_destroy
* NAME
*	cl_req_mgr_destroy
*
* DESCRIPTION
*	The cl_req_mgr_destroy function destroys a request manager.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_req_mgr_destroy(
	IN	cl_req_mgr_t* const	p_req_mgr );
/*
* PARAMETERS
*	p_req_mgr
*		[in] Pointer to a cl_req_mgr_t structure to destroy.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	cl_req_mgr_destroy frees all memory allocated by the request manager.
*	Further operations on the request manager should not be attempted.
*
*	This function should only be called after a call to cl_req_mgr_construct
*	or cl_req_mgr_init.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_construct, cl_req_mgr_init
*********/


/****f* Component Library: Request Manager/cl_req_mgr_get
* NAME
*	cl_req_mgr_get
*
* DESCRIPTION
*	The cl_req_mgr_get function handles synchronous and asynchronous
*	requests for objects.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_req_mgr_get(
	IN		cl_req_mgr_t* const	p_req_mgr,
	IN OUT	size_t* const		p_count,
	IN		const cl_req_type_t	req_type,
	IN		cl_pfn_req_cb_t		pfn_callback,
	IN		const void* const	context1,
	IN		const void* const	context2 );
/*
* PARAMETERS
*	p_req_mgr
*		[in] Pointer to a cl_req_mgr_t structure from which to check
*		for resources.
*
*	p_count
*		[in/out] On input, contains the number of objects requested.
*		On output, contains the number of objects available.
*
*	req_type
*		[in] Enumerated type describing the type of request. Valid values are:
*			ReqGetSync
*				Synchronous request.
*			ReqGetAsync
*				Asynchronous requests for which all objects are required at
*				once.
*			ReqGetAsyncPartialOk
*				Asynchronous requests that may be broken into multiple smaller
*				requests.
*
*	pfn_callback
*		[in] Pointer to a callback function for use by the caller. This
*		callback function is never invoked by the request manager.
*
*	context1
*		[in] First of two contexts for a resource request.
*
*	context2
*		[in] Second of two contexts for a resource request.
*
* RETURN VALUES
*	CL_SUCCESS if all objects requested are available.
*
*	CL_PENDING if the request could not be completed in its entirety.
*	The p_count parameter contains the number of objects immediately available.
*
*	CL_INSUFFICIENT_RESOURCES if the request could not be completed due to
*	insufficient objects being available.
*
*	CL_INSUFFICIENT_MEMORY if the request failed due to a lack of system memory.
*
* NOTES
*	Upon successful completion of this function, the p_count parameter contains
*	the number of objects available.
*
*	Synchronous requests fail if there are any asynchronous requests pending,
*	or if there are not enough resources to immediately satisfy the request in
*	its entirety .
*
*	Asynchronous requests fail if there is insufficient system memory to
*	queue them.
*
*	Once an asynchronous request is queued, use cl_req_mgr_resume to retrieve
*	information for resuming queued requests.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_resume
*********/


/****f* Component Library: Request Manager/cl_req_mgr_resume
* NAME
*	cl_req_mgr_resume
*
* DESCRIPTION
*	The cl_req_mgr_resume function attempts to resume queued requests.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_req_mgr_resume(
	IN	cl_req_mgr_t* const		p_req_mgr,
	OUT	size_t* const			p_count,
	OUT	cl_pfn_req_cb_t* const	ppfn_callback,
	OUT	const void** const		p_context1,
	OUT	const void** const		p_context2 );
/*
* PARAMETERS
*	p_req_mgr
*		[in] Pointer to a cl_req_mgr_t structure from which to resume requests.
*
*	p_count
*		[out] Contains the number of objects available for a resuming request.
*
*	ppfn_callback
*		[out] Contains the pfn_callback value for the resuming request, as
*		provided to the call to the cl_req_mgr_get function.
*
*	p_context1
*		[out] Contains the context1 value for the resuming request, as provided
*		to the call to the cl_req_mgr_get function.
*
*	p_context2
*		[out] Contains the context2 value for the resuming request, as provided
*		to the call to the cl_req_mgr_get function.
*
* RETURN VALUES
*	CL_SUCCESS if a request was completed.
*
*	CL_PENDING if a request was continued, but not completed.
*
*	CL_INSUFFICIENT_RESOURCES if a request could not be continued due to
*	a lack of resources.
*
*	CL_NOT_DONE if there were no pending requests.
*
* NOTES
*	cl_req_mgr_resume resumes at most one request. Further requests may be
*	able to be resumed if this call returns CL_SUCCESS.
*
* SEE ALSO
*	Request Manager, cl_req_mgr_get
*********/


#ifdef __cplusplus
}
#endif


#endif	/* _CL_REQ_MGR_H_ */

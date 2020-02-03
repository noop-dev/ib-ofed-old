/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Copyright (c) 1996-2003 Intel Corporation. All rights reserved. 
 * Portions Copyright (c) 2008 Microsoft Corporation.  All rights reserved.
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
 * $Id: ib_uvp.h 2860 2008-07-27 17:51:14Z leonid $
 */

#ifndef __IB_UAL_UVP_H__
#define __IB_UAL_UVP_H__

#include <complib/cl_types.h>
#include <iba/ib_types.h>
#include <iba/ib_ci.h>
#include <initguid.h>

/****h* UAL_UVP_Interface/user-mode Verbs
* NAME
*	User-mode Verbs -- User-mode Verbs implements the HCA specific
*	user-mode functions to plug in to the Usermode Access Layer
*	Architecture (UAL)
*
* COPYRIGHT
*	Copyright© 2001 Intel Corporation - All Rights Reserved.
* DESCRIPTION
*	The user-mode Verbs Interface defines the mechanism for a HCA vendor
*	to plug into the User-mode Access Layer (UAL) architecture.
*	Access Layer API is what is exposed to the user-mode applications.
*	The	interface described here is not Verbs API. In this interface model,
*	UAL provides a generic mechanism to exchange vendor specific info
*	in the implementation of verbs within the UAL architecture. UAL provides
*	the support for callback processing. For instance, AL provides a
*	QP error callback when a qp incurs error. Such asynchronous events are
*	handled with the support of UAL and not by the vendor interface described
*	here.
*
*	For verbs related AL APIs, UAL packages the parameters in an IOCTL
*	and sends it to the kernel AL. In the UAL design, this is broken down
*	into 3 steps.
*
*	a. Pre-ioctl step
*		A vendor specified pre-ioctl function is called with relevant input
*		parameters including a private buffer template (ci_umv_buf_t)
*		for the vendor to communicate with the corresponding HCA driver.
*		For calls that does not go to the HCA driver (for e.g. ib_open_ca())
*		no private buffer will be passed.
*	b. Sending IOCTL to kernel AL
*		Following step (a), UAL prepares an IOCTL with the relevant parameters
*		including the vendor's private buffer. UAL/user-mode proxy does not
*		interpret the contents of the private buffer.
*		UAL sends the IOCTL to the user-mode proxy in kernel. The proxy
*		interfaces with kernel AL to act on behalf the user. AL passes the
*		parameters to the Verbs Provider Driver and the results are returned
*		back to UAL.
*	c. Post-ioctl step.
*		Following the return from IOCTL in step (b), UAL calls a
*		vendor-specified post-ioctl function with relevant parameters.
*		UAL will call the post-ioctl function whether or not step (b)
*		succeeded. The ioctl itself could have successfully returned but
*		a vendor-specific status in ci_umv_buf_t may indicate a failure.
*		UAL also passes the ioctl status to the vendor library so that
*		the appropriate action can be taken in the post call.
*
*	Use of ci_umv_buf_t and pre/post return values
*
*		1. ci_umv_buf is provided by UAL as a unique buffer template for
*		a given verbs call. Vendor could keep any info relevant to
*		the specific verbs call in this buffer. This buffer is sufficient
*		for uniquely identifying which call it is intended for. For instance,
*		the umv buffer set up by vendor in a uvp_pre_create_qp() could later
*		tell the uvp_post_create_qp_t() which QP it is intended for.
*
*		2. The success of pre/post-ioctl step to UAL means IB_SUCCESS.
*		Any value other than IB_SUCCESS is treated as failure.
*
*		3. The Vendor could exchange a status in ci_umv_buf_t. However, this
*		interface does not enumerate the status in ci_umv_buf_t.
*		However, the vendor could check the status in ci_umv_buf_t
*		returned from the pre-ioctl	step and act accordingly.
*
* AUTHOR
*	Intel Corporation
* CREATION DATE
*	XX.XX.XX
* NOTES
*	1. For user mode verbs that require a kernel transition, handles passed
*	to kernel are validated in the	user-mode proxy running in kernel.
*	Those Verbs that are entirely done in user mode that would affect
*	speed path do not perform consistency checks. So invalid pointers
*	would lead to application crash with core dumps.
*
*******
*
*/

/******/

/******/

/*
*
*	Vendor-specific handles
*
*/

/****f* user-mode Verbs/unsupported functions
* NAME
*	1. Register physical memory region with HCA (ci_register_pmr)
*	2. Modify physical memory region with HCA (ci_modify_pmr)
*	3. Create Special QP	(ci_create_spl_qp)
*
*	For all these functions, the vendor does NOT provide support
*	and UAL will return IB_UNSUPPORTED to the caller of Access Layer.
*
* SYNOPSIS
*/

/********/

/****f* user-mode Verbs/uvp_pre_open_ca_t
* NAME
*	uvp_pre_open_ca_t -- Pre-ioctl operation for user-mode ib_open_ca()
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_open_ca_t) (
	IN		const	ib_net64_t					ca_guid,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_ca_handle_t				*ph_uvp_ca);

/*
* DESCRIPTION
*	uvp_pre_open_ca_t() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_open_ca() in user-mode.
*
*
* PARAMETERS
*	ca_guid
*		[in] The HCA adapter's EUI64 identifier. Clients would use other
*		enumeration API's to locate all available adapters and their
*		guids in a system, e.g. GetCaGuids(), maintained by the IB
*		Access Layer.
*
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains any vendor-specific
*		record to be exchanged with the vendor's HCA driver.

*	ph_uvp_ca
*		[out] (OPTIONAL) Vendor's Handle to the newly opened instance of the CA.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		Invalid GUID.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t, uvp_pre_modify_ca,
*	uvp_post_modify_ca_t,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_open_ca_t
* NAME
*	uvp_post_open_ca_t -- Post-ioctl operation for user-mode ib_open_ca()
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_open_ca_t) (
	IN		const	ib_net64_t					ca_guid,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_ca_handle_t				*ph_uvp_ca,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_post_open_ca_t() is implemented by vendor. It is the post-ioctl routine
*	for the AL call ib_open_ca() in user-mode.
*
* PARAMETERS
*	ca_guid
*		[in] The HCA adapter's EUI64 identifier.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_uvp_ca
*		[in out] Pointer to vendor's handle to the newly opened instance of
*		the CA. If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in] This contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_open_ca).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	IB_SUCCESS
*		The HCA is return handle is valid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t, uvp_pre_modify_ca,
*	uvp_post_modify_ca_t,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_query_ca
* NAME
*	uvp_pre_query_ca -- Pre-ioctl operation for user-mode ib_query_ca()
*
* SYNOPSIS
*/
typedef ib_api_status_t
(AL_API *uvp_pre_query_ca) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				ib_ca_attr_t				*p_ca_attr,
	IN				size_t						byte_count,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_pre_query_ca() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_query_ca() in user-mode.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*	p_ca_attr
*		[in] Pointer to the user's CA attribute buffer.
*	byte_count
*		[in] User-supplied size of the CA attribute buffer.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains any vendor-specific
*		record to be exchanged with the vendor's HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INVALID_CA_HANDLE
*		CA handle is invalid
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_post_query_ca_t, uvp_pre_modify_ca,
*	uvp_post_modify_ca_t,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_query_ca_t
* NAME
*	uvp_post_query_ca_t -- Post-ioctl operation for user-mode ib_query_ca()
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_ca_t) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status,
	IN				ib_ca_attr_t				*p_ca_attr,
	IN				size_t						byte_count,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_post_query_ca_t() is implemented by vendor. It is the post-ioctl routine
*	for the AL call ib_query_ca() in user-mode. UAL provides the results
*	of querying the CA attributes to the vendor's post-ioctl routine.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_ca_attr
*		[in] CA attribute of this Host Channel adapter (as returned by
*		from ioctl to kernel AL).
*	byte_count
*		[in] Number of bytes in ca_attr buffer.
*	p_umv_buf
*		[in ] This contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_ca).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_pre_query_ca, uvp_pre_modify_ca,
*	uvp_post_modify_ca_t,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_modify_ca
* NAME
*	uvp_pre_modify_ca -- Pre-ioctl operation for user-mode ib_modify_ca()
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_modify_ca) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				uint8_t						port_num,
	IN				ib_ca_mod_t					ca_mod,
	IN		const	ib_port_attr_mod_t* const	p_port_attr_mod );

/*
* DESCRIPTION
*	uvp_pre_modify_ca() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_modify_ca() in user-mode.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*	port_num
*		[in] An index to the port that is being modified.  The port_num matches
*		the index of the port as returned through the ib_query_ca call.
*	ca_mod
*		[in] A mask of the attributes and counters to modify.
*	p_port_attr_mod
*		[in] A list of the specific port attribute information to modify.  For
*		the access layer to modify an attribute, its corresponding bit must be
*		set in the ca_mod parameter.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INVALID_CA_HANDLE
*		CA handle is invalid.
*	IB_INVALID_PARAMETER
*		One or more parameters is invalid.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t,
*	uvp_post_modify_ca_t,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_modify_ca_t
* NAME
*	uvp_post_modify_ca_t -- Post-ioctl operation for user-mode ib_modify_ca()
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_modify_ca_t) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_modify_ca_t() is implemented by vendor. It is the post-ioctl routine
*	for the AL call ib_modify_ca() in user-mode.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*	ioctl_status
*		[in] The ioctl status of the AL API.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t,
*	uvp_pre_modify_ca,	uvp_pre_close_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_close_ca_t
* NAME
*	uvp_pre_close_ca_t -- Pre-ioctl operation for user-mode ib_close_ca().
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_close_ca_t) (
	IN				ib_ca_handle_t				h_uvp_ca );

/*
* DESCRIPTION
*	uvp_pre_close_ca_t() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_close_ca() in user-mode.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*
* RETURN VALUE
*	IB_SUCCESS
*		Successfully completed the pre-ioctl.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t,
*	uvp_pre_modify_ca,	uvp_post_modify_ca_t, uvp_post_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_close_ca_t
* NAME
*	uvp_post_close_ca_t -- Post-ioctl operation for user-mode ib_close_ca().
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_close_ca_t) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_close_ca_t() is implemented by vendor. It is the post-ioctl routine
*	for the AL call ib_close_ca().
*	UAL calls this function in the context of the asynchronous callback
*	from AL notifying the successful destruction of CA.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*
* RETURN VALUE
*	IB_SUCCESS
*		The post-ioctl for ib_close_ca() successfully completed.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_open_ca_t, uvp_post_open_ca_t, uvp_pre_query_ca, uvp_post_query_ca_t,
*	uvp_pre_modify_ca,	uvp_post_modify_ca_t, uvp_pre_close_ca_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_ci_call
* NAME
*	uvp_pre_ci_call -- Pre-ioctl function to ib_ci_call
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_ci_call) (
	IN		const	ib_ca_handle_t				h_uvp_ca,
	IN		const	void**				const	handle_array	OPTIONAL,
	IN				uint32_t					num_handles,
	IN				ib_ci_op_t*			const	p_ci_op,
	IN	OUT			ci_umv_buf_t				*p_umv_buf);
/*
* DESCRIPTION
*	uvp_pre_ci_call() is implemented by vendor. It is the pre-ioctl
*	routine for ib_ci_call().
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the CA
*	handle_array
*		[in] An array of uvp handles.  For valid types, refer to ib_ci.h or
*		ib_al.h.  This is an optional parameter.
*	num_handles
*		[in] The number of handles in the array.
*	p_ci_op
*		[in] The operation that is requested by the client.  For more info,
*		refer ib_types.h
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl is successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_ci_call
*
********/

/********/

/****f* user-mode Verbs/uvp_post_ci_call
* NAME
*	uvp_post_ci_call -- Post-ioctl function to ib_ci_call
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_ci_call) (
	IN		const	ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status,
	IN		const	void*  *		const	handle_array	OPTIONAL,
	IN				uint32_t					num_handles,
	IN				ib_ci_op_t*			const	p_ci_op,
	IN	OUT			ci_umv_buf_t				*p_umv_buf);
/*
* DESCRIPTION
*	uvp_post_ci_call() is implemented by vendor. It is the pre-ioctl
*	routine for ib_ci_call().
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the CA
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	handle_array
*		[in] An array of uvp handles.  For valid types, refer to ib_ci.h or
*		ib_al.h.  This is an optional parameter.
*	num_handles
*		[in] The number of handles in the array.
*	p_ci_op
*		[in] The operation that is requested by the client.  For more info,
*		refer ib_types.h
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_ci_call
*
********/


/********/

/****f* user-mode Verbs/uvp_pre_allocate_pd
* NAME
*	uvp_pre_allocate_pd -- Pre-ioctl function to allocate PD
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_allocate_pd) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_pd_handle_t				*ph_uvp_pd);

/*
* DESCRIPTION
*	uvp_pre_allocate_pd() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_alloc_pd() in user-mode.
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library handle to the open instance of the CA
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_pd
*		[out] (OPTIONAL) Vendor's Handle to the newly created protection domain.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INVALID_CA_HANDLE
*		CA handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_allocate_pd_t, uvp_pre_deallocate_pd, uvp_post_deallocate_pd_t
*
*******/

/********/

/****f* user-mode Verbs/uvp_post_allocate_pd_t
* NAME
*	uvp_post_allocate_pd_t -- Post-ioctl function to allocate PD
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_allocate_pd_t) (
	IN				ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_pd_handle_t				*ph_uvp_pd,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_allocate_pd_t() is implemented by vendor. It is the post-ioctl
*	routine for the AL call ib_alloc_pd().
*
* PARAMETERS
*	h_uvp_ca
*		[in] Vendor's user-mode library CA handle.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_uvp_pd
*		[in out] The vendor library handle to the newly created protection domain.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in] This contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_allocate_pd).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_allocate_pd, uvp_pre_deallocate_pd, uvp_post_deallocate_pd_t
*
*******/

/********/

/****f* user-mode Verbs/uvp_pre_deallocate_pd
* NAME
*	uvp_pre_deallocate_pd -- Pre-ioctl function to deallocate PD
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_deallocate_pd) (
	IN		const	ib_pd_handle_t				h_uvp_pd );

/*
* DESCRIPTION
*	uvp_pre_deallocate_pd() is implemented by vendor. It is the pre-ioctl
*	routine for the AL call ib_deallocate_pd().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's user-mode library PD handle.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_allocate_pd, uvp_post_allocate_pd_t, uvp_post_deallocate_pd_t
*
*******/

/********/

/****f* user-mode Verbs/uvp_post_deallocate_pd_t
* NAME
*	uvp_post_deallocate_pd_t -- Post-ioctl function to deallocate PD
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_deallocate_pd_t) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_deallocate_pd_t() is implemented by the vendor. It is the
*	post-ioctl routine for the AL call ib_dealloc_pd().
*
*	When all the resouces associated with a PD are destroyed,
*	UAL invokes this post-ioctl routine to deallocate PD. Since the
*	completion of the resource deallocation (e.g QP/CQ) is asynchronous,
*	this function is called from a UAL asynchronous callback
*	processing thread.
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's user-mode library PD handle.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_allocate_pd, uvp_post_allocate_pd_t, uvp_pre_deallocate_pd
*
*******/

/********/

/****f* user-mode Verbs/uvp_pre_create_av
* NAME
*	uvp_pre_create_av -- Pre-ioctl function to create AV
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_create_av) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN		const	ib_av_attr_t				*p_addr_vector,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_av_handle_t				*ph_uvp_av);
/*
* DESCRIPTION
*	uvp_pre_create_av() is implemented by vendor. It is the pre-ioctl
*	routine for ib_create_av().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's user-mode library handle to the Protection domain
*		to which this AV is associated.
*	p_addr_vector
*		[in] Parameters to create the address vector.  If the grh of the
*		address vector is valid, then the grh resv1 field should be set to
*		the index of the src_gid.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_av
*		[out] (OPTIONAL) Vendor's Handle to the newly created address vector.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl is successful.
*	IB_INVALID_SETTING
*		Values in the vector is not valid
*	IB_INVALID_PD_HANDLE
*		The PD handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_create_av_t, uvp_pre_query_av, uvp_post_query_av_t, uvp_pre_modify_av,
*	uvp_post_modify_av_t, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_create_av_t
* NAME
*	uvp_post_create_av_t -- Post-ioctl function to create AV
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_create_av_t) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_av_handle_t				*ph_uvp_av,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_post_create_av_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_create_av().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's user-mode library handle to the Protection domain
*		to which this AV is associated
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_uvp_av
*		[in out] Vendor's address vector handle.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_create_av).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_pre_query_av, uvp_post_query_av_t, uvp_pre_modify_av,
*	uvp_post_modify_av_t, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_query_av
* NAME
*	uvp_pre_query_av -- Pre-ioctl operation for ib_query_ca()
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_query_av) (
	IN		const	ib_av_handle_t				h_uvp_av,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_query_av() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_query_av() in user-mode.
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's handle to the address vector in user-mode library
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INVALID_AV_HANDLE
*		AV handle was invalid
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_post_query_av_t, uvp_pre_modify_av,
*	uvp_post_modify_av_t, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
*********/

/********/
/****f* user-mode Verbs/uvp_post_query_av_t
* NAME
*
*	Vendor-specific post-ioctl operation for user-mode ib_query_ca()
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_av_t) (
	IN		const	ib_av_handle_t				h_uvp_av,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_av_attr_t				*p_addr_vector,
	IN	OUT			ib_pd_handle_t				*ph_pd,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_query_av_t() is implemented by vendor. It is the post-ioctl routine
*	for the AL call ib_query_av() in user-mode.
*	UAL provides the results of the query to the vendor library in this
*	post-ioctl routine.
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's handle to the address vector in user-mode library
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_addr_vector
*		[in out] AV attribute (as returned by the ioctl).
*	ph_pd
*		[out] The vendor library PD handle associated with this AV.
*	p_umv_buf
*		[in]
*		On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_av).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_pre_query_av, uvp_pre_modify_av,
*	uvp_post_modify_av_t, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_modify_av
* NAME
*	uvp_pre_modify_av -- Pre-ioctl function to modify AV
*
* SYNOPSIS
*/
typedef ib_api_status_t
(AL_API *uvp_pre_modify_av) (
	IN		const	ib_av_handle_t				h_uvp_av,
	IN		const	ib_av_attr_t				*p_addr_vector,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_pre_modify_av() is implemented by vendor. It is the pre-ioctl routine
*	for ib_modify_av().
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's AV handle in user-mode library.
*	p_addr_vector
*		[in] Parameters to modify the address vector handle.  If the grh of the
*		address vector is valid, then the grh resv1 field should be set to
*		the index of the src_gid.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl is successful.
*	IB_INVALID_SETTING
*		Values in the vector is not valid.
*	IB_INVALID_AV_HANDLE
*		The AV handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_pre_query_av, uvp_post_query_av_t,
*	uvp_post_modify_av_t, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_modify_av_t
* NAME
*	uvp_post_modify_av_t -- Post-ioctl function to modify AV
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_modify_av_t) (
	IN		const	ib_av_handle_t				h_uvp_av,
	IN				ib_api_status_t				ioctl_status,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_modify_av_t() is implemented by vendor to modify the attributes
*	of AV. It is the post-ioctl routine for ib_modify_av().
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's av handle in user-mode library.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_modify_av).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_pre_query_av, uvp_post_query_av_t,
*	uvp_pre_modify_av, uvp_pre_destroy_av, uvp_post_destroy_av_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_destroy_av
* NAME
*	uvp_pre_destroy_av -- Pre-ioctl function to destroy AV
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_destroy_av) (
	IN		const	ib_av_handle_t				h_uvp_av );

/*
* DESCRIPTION
*	uvp_pre_destroy_av() is implemented by vendor to destroy the AV.
*	It is the pre-ioctl routine for ib_destroy_av().
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's AV handle in user-mode library.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl is successful.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_pre_query_av, uvp_post_query_av_t,
*	uvp_pre_modify_av, uvp_post_modify_av_t, uvp_post_destroy_av_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_destroy_av_t
* NAME
*	uvp_post_destroy_av_t -- Post-ioctl function to destroy AV
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_destroy_av_t) (
	IN		const	ib_av_handle_t				h_uvp_av,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_destroy_av_t() is implemented by vendor. It is the post-ioctl
*	routine for ib_destroy_av().
*
* PARAMETERS
*	h_uvp_av
*		[in] Vendor's AV handle in user-mode library.
*	p_umv_buf
*		[in out]
*		On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_destroy_av).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_av, uvp_post_create_av_t, uvp_pre_query_av, uvp_post_query_av_t,
*	uvp_pre_modify_av, uvp_post_modify_av_t, uvp_pre_destroy_av
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_create_srq
* NAME
*	uvp_pre_create_srq -- Pre-ioctl function to Create a Shared Queue Pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_create_srq) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN		const	ib_srq_attr_t* const			p_srq_attr,
	IN	OUT 		ci_umv_buf_t				*p_umv_buf,
		OUT 		ib_srq_handle_t				*ph_uvp_srq);

/*
* DESCRIPTION
*	uvp_pre_create_srq() is implemented by vendor. It is the pre-ioctl routine
*	for ib_create_srq().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle in user-mode library.
*	p_srq_attr
*		[in] Initial attributes with which the srq must be created.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_srq
*		[out] (OPTIONAL) Vendor's Handle to the newly created SRQ.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INVALID_PD_HANDLE
*		The PD handle is invalid.
*	IB_UNSUPPORTED
*		The specified queue pair type was not supported by the channel adapter.
*	IB_INVALID_MAX_WRS
*		The requested maximum send or receive work request depth could not be
*		supported.
*	IB_INVALID_MAX_SGE
*		The requested maximum number of scatter-gather entries for the send or
*		receive queue could not be supported.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*	IB_INVALID_PARAMETER
*		At least one parameter is invalid.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_create_srq_t, uvp_pre_query_srq, uvp_post_query_srq_t, uvp_pre_modify_srq,
*	uvp_post_modify_srq_t, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_create_srq_t
* NAME
*	uvp_post_create_srq_t -- Post-ioctl function to Create a Queue Pair.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_create_srq_t) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN				ib_api_status_t			ioctl_status,
	IN	OUT 		ib_srq_handle_t				*ph_uvp_srq,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_create_srq_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_create_srq().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle in user-mode library.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_uvp_srq
*		[in out] Vendor's srq handle for the newly created srq.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_create_srq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_pre_query_srq, uvp_post_query_srq_t, uvp_pre_modify_srq,
*	uvp_post_modify_srq_t, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_modify_srq
* NAME
*	uvp_pre_modify_srq -- Pre-ioctl function to Modify attributes of the
*						 specified srq.
*
* SYNOPSIS
*
*/

typedef ib_api_status_t
(AL_API *uvp_pre_modify_srq) (
	IN		const	ib_srq_handle_t			h_uvp_srq,
	IN		const	ib_srq_attr_t * const		p_srq_attr,
	IN		const	ib_srq_attr_mask_t			srq_attr_mask,
	IN	OUT 		ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_modify_srq() is implemented by vendor to modify the attributes of a
*	srq. It is the pre-ioctl routine for ib_modify_srq().
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's srq Handle to the queue pair (in user-mode library)
*		whose state is to be modified.
*	p_srq_attr
*		[in] Specifies what attributes need to be modified in the srq.
*	srq_attr_mask
*		[in] Specifies which fields of ib_srq_attr_t are valid.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the requested operation.
*	IB_INVALID_SRQ_HANDLE
*		Invalid srq handle.
*	IB_UNSUPPORTED
*		Requested operation is not supported, for e.g. Atomic operations.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_pre_query_srq, uvp_post_query_srq_t,
*	uvp_post_modify_srq_t, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_modify_srq_t
* NAME
*	uvp_post_modify_srq_t -- Post-ioctl function to Modify attributes of
*						  the specified srq.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_modify_srq_t) (
	IN		const	ib_srq_handle_t				h_uvp_srq,
	IN				ib_api_status_t			ioctl_status,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_modify_srq_t() is implemented by vendor to modify the srq attributes.
*	It is the post-ioctl routine for ib_modify_srq().
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's srq Handle to the queue pair (in user-mode library)
*		whose state is modified.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_modify_srq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_pre_query_srq, uvp_post_query_srq_t,
*	uvp_pre_modify_srq, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_query_srq
* NAME
*	uvp_pre_query_srq -- Pre-ioctl function to Query the attributes of the srq
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_query_srq) (
	IN				ib_srq_handle_t				h_uvp_srq,
	IN	OUT 		ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_query_srq() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_query_srq().
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's handle to the srq (in user-mode library).
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INVALID_SRQ_HANDLE
*		srq handle is invalid
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_post_query_srq_t, uvp_pre_modify_srq,
*	uvp_post_modify_srq_t, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_query_srq_t
* NAME
*	uvp_post_query_srq_t -- Post-ioctl operation for user-mode ib_query_srq()
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_srq_t) (
	IN				ib_srq_handle_t				h_uvp_srq,
	IN				ib_api_status_t			ioctl_status,
	IN				ib_srq_attr_t				*p_query_attr,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_query_srq_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_query_srq().
*	UAL provides the results of the query to the vendor library in this
*	post-ioctl routine.
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's handle to the srq (in user-mode library).
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_query_attr
*		[in] srq attribute as returned by the ioctl.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_srq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_pre_query_srq, uvp_pre_modify_srq,
*	uvp_post_modify_srq_t, uvp_pre_destroy_srq, uvp_post_destroy_srq_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_destroy_srq
* NAME
*	uvp_pre_destroy_srq -- Pre-ioctl function to Destroy a Queue Pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_destroy_srq) (
	IN		const	ib_srq_handle_t				h_uvp_srq );

/*
* DESCRIPTION
*	uvp_pre_destroy_srq() is the pre-ioctl routine implemented by vendor
*	to destroy srq.
*	UAL invokes this pre-ioctl routine to destroy srq.
*	The vendor is expected to perform any preliminary steps in preparation
*	for destroying the srq and perform any book-keeping.
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's Handle to the srq (in user-mode library)
*		that needs to be destroyed.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_pre_query_srq, uvp_post_query_srq_t,
*	uvp_pre_modify_srq, uvp_post_modify_srq_t, uvp_post_destroy_srq_t
*
********/

/********/


/****f* user-mode Verbs/uvp_post_destroy_srq_t
* NAME
*	uvp_post_destroy_srq_t -- Post-ioctl function to Destroy a Queue Pair.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_destroy_srq_t) (
	IN		const	ib_srq_handle_t				h_uvp_srq,
	IN				ib_api_status_t			ioctl_status );

/*
* DESCRIPTION
*	uvp_post_destroy_srq_t() is implemented by vendor. It is the post-ioctl
*	routine for ib_destroy_srq().
*	UAL invokes this post-ioctl routine to destroy srq when it receives
*	asynchronous notification from the user-mode proxy in kernel.
*
* PARAMETERS
*	h_uvp_srq
*		[in] Vendor's Handle to the srq (in user-mode library)
*		that needs to be destroyed.
*
* RETURN VALUE
*	IB_SUCCESS
*		The post-ioctl call is successful.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_srq, uvp_post_create_srq_t, uvp_pre_query_srq, uvp_post_query_srq_t,
*	uvp_pre_modify_srq, uvp_post_modify_srq_t, uvp_pre_destroy_srq
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_create_qp
* NAME
*	uvp_pre_create_qp -- Pre-ioctl function to Create a Queue Pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_create_qp) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN		const	ib_qp_create_t				*p_create_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_qp_handle_t				*ph_uvp_qp);

/*
* DESCRIPTION
*	uvp_pre_create_qp() is implemented by vendor. It is the pre-ioctl routine
*	for ib_create_qp().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle in user-mode library.
*	p_create_attr
*		[in] Initial attributes with which the qp must be created.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_qp
*		[in out] (OPTIONAL) Vendor's Handle to the newly created QP.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INVALID_PD_HANDLE
*		The PD handle is invalid.
*	IB_INVALID_CQ_HANDLE
*		The send or receive completion queue to associate with the queue pair
*		is invalid.
*	IB_UNSUPPORTED
*		The specified queue pair type was not supported by the channel adapter.
*	IB_INVALID_MAX_WRS
*		The requested maximum send or receive work request depth could not be
*		supported.
*	IB_INVALID_MAX_SGE
*		The requested maximum number of scatter-gather entries for the send or
*		receive queue could not be supported.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*	IB_INVALID_PARAMETER
*		At least one parameter is invalid.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_post_create_qp_t, uvp_pre_query_qp, uvp_post_query_qp_t, uvp_pre_modify_qp,
*	uvp_post_modify_qp_t, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
********/

typedef struct _uvp_qp_create
{
	ib_qp_create_t			qp_create;

	void					*context;
	uint32_t				max_inline_send;
	uint32_t				initiator_depth;
	uint32_t				responder_resources;

}	uvp_qp_create_t;

typedef ib_api_status_t
(AL_API *uvp_wv_pre_create_qp) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN		const	uvp_qp_create_t				*p_create_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_qp_handle_t				*ph_uvp_qp);

/********/

/****f* user-mode Verbs/uvp_post_create_qp_t
* NAME
*	uvp_post_create_qp_t -- Post-ioctl function to Create a Queue Pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_create_qp_t) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_qp_handle_t				*ph_uvp_qp,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_create_qp_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_create_qp().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle in user-mode library.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_uvp_qp
*		[in out] Vendor's QP handle for the newly created QP.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_create_qp).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_pre_query_qp, uvp_post_query_qp_t, uvp_pre_modify_qp,
*	uvp_post_modify_qp_t, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_modify_qp
* NAME
*	uvp_pre_modify_qp -- Pre-ioctl function to Modify attributes of the
*						 specified QP.
*
* SYNOPSIS
*
*/

typedef ib_api_status_t
(AL_API *uvp_pre_modify_qp) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN		const	ib_qp_mod_t					*p_modify_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_modify_qp() is implemented by vendor to modify the attributes of a
*	QP. It is the pre-ioctl routine for ib_modify_qp().
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's qp Handle to the queue pair (in user-mode library)
*		whose state is to be modified.
*	p_modify_attr
*		[in] Specifies what attributes need to be modified in the qp.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the requested operation.
*	IB_INVALID_QP_HANDLE
*		Invalid QP handle.
*	IB_UNSUPPORTED
*		Requested operation is not supported, for e.g. Atomic operations.
*	IB_QP_INVALID_STATE
*		Invalid state transition request. Current QP state not in allowable
*		state.
*	IB_INVALID_PKEY
*		Pkey specified in modify request not valid entry in P_KEY table. Or
*		index is out of range.
*	IB_INVALID_PMIG_STATE
*		Invalid path migration state specified in the request.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_query_qp, uvp_post_query_qp_t,
*	uvp_post_modify_qp_t, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_modify_qp_t
* NAME
*	uvp_post_modify_qp_t -- Post-ioctl function to Modify attributes of
*						  the specified QP.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_modify_qp_t) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN				ib_api_status_t				ioctl_status,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_modify_qp_t() is implemented by vendor to modify the qp attributes.
*	It is the post-ioctl routine for ib_modify_qp().
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's qp Handle to the queue pair (in user-mode library)
*		whose state is modified.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_modify_qp).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_query_qp, uvp_post_query_qp_t,
*	uvp_pre_modify_qp, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_query_qp
* NAME
*	uvp_pre_query_qp -- Pre-ioctl function to Query the attributes of the QP
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_query_qp) (
	IN				ib_qp_handle_t				h_uvp_qp,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_query_qp() is implemented by vendor. It is the pre-ioctl routine
*	for the AL call ib_query_qp().
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's handle to the QP (in user-mode library).
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl function succeeded.
*	IB_INVALID_QP_HANDLE
*		QP handle is invalid
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_post_query_qp_t, uvp_pre_modify_qp,
*	uvp_post_modify_qp_t, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_post_query_qp_t
* NAME
*	uvp_post_query_qp_t -- Post-ioctl operation for user-mode ib_query_qp()
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_qp_t) (
	IN				ib_qp_handle_t				h_uvp_qp,
	IN				ib_api_status_t				ioctl_status,
	IN				ib_qp_attr_t				*p_query_attr,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_query_qp_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_query_qp().
*	UAL provides the results of the query to the vendor library in this
*	post-ioctl routine.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's handle to the QP (in user-mode library).
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_query_attr
*		[in] QP attribute as returned by the ioctl.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_qp).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User Mode
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_query_qp, uvp_pre_modify_qp,
*	uvp_post_modify_qp_t, uvp_pre_destroy_qp, uvp_post_destroy_qp_t
*
*********/

/********/

/****f* user-mode Verbs/uvp_pre_destroy_qp
* NAME
*	uvp_pre_destroy_qp -- Pre-ioctl function to Destroy a Queue Pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_destroy_qp) (
	IN		const	ib_qp_handle_t				h_uvp_qp );

/*
* DESCRIPTION
*	uvp_pre_destroy_qp() is the pre-ioctl routine implemented by vendor
*	to destroy QP.
*	UAL invokes this pre-ioctl routine to destroy QP.
*	The vendor is expected to perform any preliminary steps in preparation
*	for destroying the QP and perform any book-keeping.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Handle to the qp (in user-mode library)
*		that needs to be destroyed.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_query_qp, uvp_post_query_qp_t,
*	uvp_pre_modify_qp, uvp_post_modify_qp_t, uvp_post_destroy_qp_t
*
********/

/********/


/****f* user-mode Verbs/uvp_post_destroy_qp_t
* NAME
*	uvp_post_destroy_qp_t -- Post-ioctl function to Destroy a Queue Pair.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_destroy_qp_t) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_destroy_qp_t() is implemented by vendor. It is the post-ioctl
*	routine for ib_destroy_qp().
*	UAL invokes this post-ioctl routine to destroy QP when it receives
*	asynchronous notification from the user-mode proxy in kernel.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Handle to the qp (in user-mode library)
*		that needs to be destroyed.
*
* RETURN VALUE
*	IB_SUCCESS
*		The post-ioctl call is successful.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_query_qp, uvp_post_query_qp_t,
*	uvp_pre_modify_qp, uvp_post_modify_qp_t, uvp_pre_destroy_qp
*
********/

/****f* user-mode Verbs/uvp_nd_get_qp_state_t
* NAME
*	uvp_nd_get_qp_state_t -- Get QP state.
*
* SYNOPSIS
*/

typedef uint32_t
(AL_API *uvp_nd_get_qp_state_t) (
	IN		const	ib_qp_handle_t				h_uvp_qp
	);

/*
* DESCRIPTION
*	uvp_nd_get_qp_state_t() is implemented by vendor. 
*	There is no post uvp_nd_get_qp_state_t function.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Handle to the qp (in user-mode library)
*		that needs to be destroyed.
*
* RETURN VALUE
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*
********/

/****f* user-mode Verbs/uvp_nd_modify_qp_t
* NAME
*	uvp_nd_modify_qp_t -- Get QP state location and size.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_nd_modify_qp_t) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	OUT				void**						pp_outbuf,
	OUT				DWORD*					p_size
	);

/*
* DESCRIPTION
*	uvp_nd_modify_qp_t() is implemented by vendor. 
*	UAL invokes this pre-ioctl routine to enable KAL to change QP state
*	It is used after quick QP modify operation.
*	There is no post uvp_ndi_modify_qp_t function.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Handle to the qp (in user-mode library)
*		that needs to be destroyed.
*	pp_outbuf
*		[out] pointer to QP state internal variable;
*	p_size
*		[out] pointer to the size of QP state internal variable;
*
* RETURN VALUE
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_create_cq
* NAME
*	uvp_pre_create_cq -- Pre-ioctl function to Create a completion queue (CQ)
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_create_cq) (
	IN		const	ib_ca_handle_t				h_uvp_ca,
	IN	OUT			uint32_t*			const	p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_cq_handle_t				*ph_uvp_cq);
/*
* DESCRIPTION
*	uvp_pre_create_cq() is implemented by vendor. It is the pre-ioctl routine
*	for ib_create_cq().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's handle to an existing protection domain (in user-mode
*		library)
*	p_size
*		[in out] Points to a variable containing the number of CQ entries
*		requested by the consumer.
*		On return,  points to the size of the CQ that was created
*		by the provider.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_cq
*		[in out] Vendor's Handle to the newly created CQ.
*
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful.
*	IB_INVALID_PD_HANDLE
*		The h_uvp_pd passed is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_CQ_SIZE
*		Requested CQ Size is not supported.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_post_create_cq_t, uvp_pre_resize_cq, uvp_post_resize_cq_t,
*	uvp_pre_query_cq, uvp_post_query_cq_t, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_create_cq_t
* NAME
*	uvp_post_create_cq_t -- Post-ioctl function to Create a completion queue (CQ)
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_create_cq_t) (
	IN		const	ib_ca_handle_t				h_uvp_ca,
	IN				ib_api_status_t				ioctl_status,
	IN		const	uint32_t					size,
	IN	OUT			ib_cq_handle_t				*ph_uvp_cq,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_create_cq_t() is implemented by vendor to create CQ.
*	It is the post-ioctl routine for ib_create_cq().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's handle to an existing protection domain (in user-mode
*		library)
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	size
*		[in] size of the CQ that was created by the provider.
*		If VPD created the CQ in kernel, this is the value as set by
*		VPD. If UVP creates the CQ in user-mode, then uvp already knows
*		the size of the CQ in the pre-ioctl.
*	ph_uvp_cq
*		[in out] Vendor's Handle to the newly created CQ.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_create_cq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_pre_resize_cq, uvp_post_resize_cq_t,
*	uvp_pre_query_cq, uvp_post_query_cq_t, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_resize_cq
* NAME
*	uvp_pre_resize_cq -- Pre-ioctl function to resize a CQ.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_resize_cq) (
	IN		const	ib_cq_handle_t				h_uvp_cq,
	IN	OUT			uint32_t*			const	p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_resize_cq() is implemented by vendor to resize the CQ.
*	It is the pre-ioctl routine for ib_resize_cq().
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the already created CQ (in user-mode library).
*	p_size
*		[in out] On input, points to a variable containing the number
*		of CQ entries requested by the consumer.
*		On completion points to the size of the CQ that was resized by
*		the provider.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.

* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful.
*	IB_INVALID_CQ_HANDLE
*		The CQ handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_CQ_SIZE
*		Requested CQ Size is not supported.
*	IB_OVERFLOW
*		The CQ has more entries than the resize request. The CQ is not
*		modified, and old entries still exist.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_post_resize_cq_t,
*	uvp_pre_query_cq, uvp_post_query_cq_t, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_resize_cq_t
* NAME
*	uvp_post_resize_cq_t -- Post-ioctl function to resize a CQ.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_resize_cq_t) (
	IN		const	ib_cq_handle_t				h_uvp_cq,
	IN				ib_api_status_t				ioctl_status,
	IN		const	uint32_t					size,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_resize_cq_t() is implemented by vendor to resize the CQ.
*	It is the post-ioctl routine for ib_resize_cq().
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the already created CQ (in user-mode library).
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	size
*		[in] size of the CQ that was created by the provider.
*		If VPD resized the CQ in kernel, this is the value as set by
*		VPD. If UVP resizes the CQ in user-mode, then uvp already knows
*		the size of the CQ in the pre-ioctl.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_resize_cq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_pre_resize_cq,
*	uvp_pre_query_cq, uvp_post_query_cq_t, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_query_cq
* NAME
*	uvp_pre_query_cq -- Pre-ioctl to Query the number of entries
*						configured for the CQ.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_query_cq) (
	IN		const	ib_cq_handle_t				h_uvp_cq,
	IN	OUT			uint32_t* const				p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_query_cq() is implemented by vendor. It is the pre-ioctl routine
*	for ib_query_cq().
*	Can we always go to the kernel to query even if it is created
*	in vendor library in user-mode?
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the already created CQ (in user-mode library).
*
*	p_size
*		[out] Size of the CQ if processing ends in user-mode.
*
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful.
*	IB_INVALID_CQ_HANDLE
*		The CQ handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources in Vendor library to complete the call.
*	IB_VERBS_PROCESSING_DONE
*		The UVP fully processed the request.  The post_query_cq handler
*		will not be invoked.
*
* PORTABILITY
*	User mode.
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_pre_resize_cq,
*	uvp_post_resize_cq_t, uvp_post_query_cq_t, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_query_cq_t
* NAME
*	uvp_post_query_cq_t -- Post-ioctl to Query the number of entries
*						 configured for the CQ.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_cq_t) (
	IN		const	ib_cq_handle_t				h_uvp_cq,
	IN				ib_api_status_t				ioctl_status,
	IN		const	uint32_t					size,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_query_cq_t() is implemented by vendor to query CQ.
*	It is the post-ioctl routine for ib_query_cq().
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the already created CQ (in user-mode library).
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	size
*		[in] The size of the CQ retuned by the IOCTL.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_cq).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_pre_resize_cq,
*	uvp_post_resize_cq_t, uvp_pre_query_cq, uvp_pre_destroy_cq,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_destroy_cq
* NAME
*	uvp_pre_destroy_cq -- Pre-ioctl function to Destroy a CQ.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_destroy_cq) (
	IN		const	ib_cq_handle_t				h_uvp_cq );

/*
* DESCRIPTION
*	uvp_pre_destroy_cq() is implemented by vendor to destroy CQ.
*	It is the pre-ioctl routine for ib_destroy_cq().
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the cq (in user-mode library)
*		that needs to be destroyed.
*
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call is successful.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_pre_resize_cq,
*	uvp_post_resize_cq_t, uvp_pre_query_cq, uvp_post_query_cq_t,
*	uvp_post_destroy_cq_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_destroy_cq_t
* NAME
*	uvp_post_destroy_cq_t -- Post-ioctl function to Destroy a CQ.
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_destroy_cq_t) (
	IN		const	ib_cq_handle_t				h_uvp_cq,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_destroy_cq_t() is implemented by vendor to destroy CQ.
*	It is the post-ioctl routine for ib_destroy_cq().
*	UAL invokes this post-ioctl routine to destroy CQ when it receives
*	asynchronous notification from the user-mode proxy.
*
* PARAMETERS
*	h_uvp_cq
*		[in] Vendor's Handle to the cq (in user-mode library)
*		that needs to be destroyed.
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_pre_resize_cq,
*	uvp_post_resize_cq_t, uvp_pre_query_cq, uvp_post_query_cq_t,
*	uvp_pre_destroy_cq
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_create_mw
* NAME
*	uvp_pre_create_mw -- Pre-ioctl function to create a memory window
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_create_mw) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_mw_handle_t				*ph_uvp_mw);
/*
* DESCRIPTION
*	uvp_pre_create_mw() is implemented by vendor. It is the pre-ioctl routine
*	for ib_create_mw().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle (in user-mode library)
*		to use for this memory window
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_uvp_mw
*		[out] (OPTIONAL) Vendor's Handle to the newly created memory window.
*
* RETURN VALUE
*	IB_SUCCESS
*		The memory window allocation completed successfully.
*	IB_INSUFFICIENT_RESOURCES
*		Not enough resources to complete the request.
*	IB_INVALID_PD_HANDLE
*		pd_handle supplied is invalid.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_post_create_mw_t, uvp_pre_query_mw, uvp_post_query_mw_t,
*	uvp_bind_mw, uvp_pre_destroy_mw, uvp_post_destroy_mw_t
*
********/


/********/


/****f* user-mode Verbs/uvp_post_create_mw_t
* NAME
*	uvp_post_create_mw_t -- Post-ioctl function to create a memory window
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_create_mw_t) (
	IN		const	ib_pd_handle_t				h_uvp_pd,
	IN				ib_api_status_t				ioctl_status,
	IN				net32_t						rkey,
	IN	OUT			ib_mw_handle_t				*ph_uvp_mw,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_post_create_mw_t() is implemented by vendor. It is the post-ioctl routine
*	for ib_create_mw().
*
* PARAMETERS
*	h_uvp_pd
*		[in] Vendor's Protection domain handle (in user-mode library)
*		to use for this memory window
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	p_rkey
*		[in] Remote access key that can be exchanged with a remote node to
*		perform RDMA transactions on this memory window.
*	ph_uvp_mw
*		[in out] Vendor's Handle to the newly created memory window.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_create_mw).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return an error.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_mw, uvp_pre_query_mw, uvp_post_query_mw_t,
*	uvp_bind_mw, uvp_pre_destroy_mw, uvp_post_destroy_mw_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_query_mw
* NAME
*	uvp_pre_query_mw -- Pre-ioctl to Query a memory window
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_query_mw) (
	IN		const	ib_mw_handle_t				h_uvp_mw,
	IN	OUT			ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_pre_query_mw is implemented by vendor. It is the pre-ioctl routine
*	for ib_query_mw().
*
* PARAMETERS
*	h_uvp_mw
*		[in] Vendor's  Memory window handle (in user-mode library)
*		whose attributes are being retrieved.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
* RETURN VALUE
*	IB_SUCCESS
*		The pre-ioctl call completed successfully.
*	IB_INVALID_MW_HANDLE
*		mw_handle supplied is an invalid handle.
*	IB_INSUFFICIENT_RESOURCES
*		Not enough resources to complete the request.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_mw, uvp_post_create_mw_t, uvp_post_query_mw_t,
*	uvp_bind_mw, uvp_pre_destroy_mw, uvp_post_destroy_mw_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_query_mw_t
* NAME
*	uvp_post_query_mw_t -- Post-ioctl to Query a memory window
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_query_mw_t) (
	IN		const	ib_mw_handle_t				h_uvp_mw,
	IN				ib_api_status_t				ioctl_status,
	IN				net32_t						rkey,
		OUT			ib_pd_handle_t				*ph_pd,
	IN				ci_umv_buf_t				*p_umv_buf );
/*
* DESCRIPTION
*	uvp_post_query_mw_t is implemented by vendor. It is the post-ioctl routine
*	for ib_query_mw().
*
* PARAMETERS
*	h_uvp_mw
*		[in] Vendor's  Memory window handle (in user-mode library)
*		whose attributes are being retrieved.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	rkey
*		[in] Current R_KEY associated with this mw_handle
*	ph_pd
*		[in] Protection domain handle associated with this mw_handle
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_query_mw).
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	IB_SUCCESS
*		The query operation completed successfully.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_mw, uvp_post_create_mw_t, uvp_pre_query_mw,
*	uvp_bind_mw, uvp_pre_destroy_mw, uvp_post_destroy_mw_t
*
********/

/********/

/****f* user-mode Verbs/uvp_bind_mw
* NAME
*	uvp_bind_mw -- Bind a memory window to a memory region.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_bind_mw) (
	IN		const	ib_mw_handle_t				h_uvp_mw,
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN				ib_bind_wr_t				*p_mw_bind,
		OUT			net32_t* const				p_rkey );
/*
* DESCRIPTION
*	This routine posts a request to bind a memory window to a registered
*	memory region. If the queue pair was created with selectable signaling,
*	once the operation is completed successfully then a completion queue entry
*	is generated indicating the bind operation has completed. The IB_POST_FENCE
*	option could be specified to cause the requestor to wait until outstanding
*	RDMA operations can be completed.
*
* PARAMETERS
*	h_uvp_mw
*		[in] Vendor's Handle (in user-mode library) to memory window
*		that needs to be bound to a memory region.
*	h_uvp_qp
*		[in] Vendor's QP Handle (in user-mode library) to which
*		this bind request is to be posted.
*	p_mw_bind
*		[in] Input parameters for this bind request, consisting of virtual
*		addr range of bind request etc. On successful completion, the new R_KEY
*		is returned.
*	p_rkey
*		[out] Current R_KEY associated with this mw_handle
*
* RETURN VALUE
*	IB_SUCCESS
*		The memory bind operation was posted successfully.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the request.
*		No more WQE's to post this request
*		No more free WQE's to post this request
*	IB_INVALID_MW_HANDLE
*		memw_handle supplied is an invalid memory window handle.
*	IB_INVALID_PERMISSION
*		Invalid access rights specified in request
*	IB_INVALID_SERVICE_TYPE
*		Invalid service type for this qp_handle.
*	IB_INVALID_PARAMETER
*		Address or length parameter specified is invalid.
*	IB_INVALID_RKEY
*		R_KEY specified is invalid for the memory region being bound.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_mw, uvp_post_create_mw_t, uvp_pre_query_mw,
*	uvp_post_query_mw_t, uvp_pre_destroy_mw, uvp_post_destroy_mw_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_destroy_mw
* NAME
*	uvp_pre_destroy_mw -- Pre-ioctl function to destroy a memory window
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_destroy_mw) (
	IN		const	ib_mw_handle_t				h_uvp_mw );

/*
* DESCRIPTION
*	uvp_pre_destroy_mw() is implemented by vendor. It is the pre-ioctl routine
*	for ib_destroy_mw().
*
*
* PARAMETERS
*	h_uvp_mw
*		[in] Vendor's handle (in user-mode library) to the memory window
*
* RETURN VALUE
*	IB_SUCCESS
*		Pre-ioctl succeeded.
*
* PORTABILITY
*	User mode

* SEE ALSO
*	uvp_pre_create_mw, uvp_post_create_mw_t, uvp_pre_query_mw,
*	uvp_post_query_mw_t, uvp_bind_mw, uvp_post_destroy_mw_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_destroy_mw_t
* NAME
*	uvp_post_destroy_mw_t -- Post-ioctl function to destroy a memory window
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_destroy_mw_t) (
	IN		const	ib_mw_handle_t				h_uvp_mw,
	IN				ib_api_status_t				ioctl_status );
/*
* DESCRIPTION
*	uvp_post_destroy_mw_t() is implemented by vendor. It is the post-ioctl
*	routine to destroy a memory window.
*
*
* PARAMETERS
*	h_uvp_mw
*		[in] Vendor's handle to the memory window
*
* RETURN VALUE
*	IB_SUCCESS
*		Destroy operation successful.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_pre_create_mw, uvp_post_create_mw_t, uvp_pre_query_mw,
*	uvp_post_query_mw_t, uvp_bind_mw, uvp_pre_destroy_mw
*
********/

/********/

/****f* user-mode Verbs/uvp_post_send
* NAME
*	uvp_post_send -- Post a work request to the send side of a queue pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_send) (
	IN		const	void*						h_qp,
	IN				ib_send_wr_t*	const		p_send_wr,
		OUT			ib_send_wr_t**				pp_send_failure );
/*
* DESCRIPTION
*	This routine posts a work request to the send side of the queue pair.
*	The different types of work request that can be posted are explained in
*	the ib_wr_t structure. For exact details on ordering rules please consult
*	the Volume 1, of the InfiniBand Specifications. If there is more
*	outstanding requests posted that what the queue is configured for, an
*	immediate error is returned.
*
* PARAMETERS
*	h_qp
*		[in] Type-cast as appropriate for user/kernel mode, this is
*		the Queue pair handle to which the receive work request is being
*		posted.
*	p_send_wr
*		[in] List of work requests that needs to be send.
*	pp_send_failure
*		[out] The work requests that failed.
*
* RETURN VALUE
*	Any unsuccessful status indicates the status of the first failed request.
*
*	IB_SUCCESS
*		All the work requests are completed successfully
*	IB_INVALID_QP_HANDLE
*		The qp_handle supplied is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the request.
*		There are no more work elements in the channel interface to
*		process this request, and the total outstanding work request has
*		been exceeded.
*	IB_INVALID_WR_TYPE
*		The work request type was not valid.
*	IB_INVALID_QP_STATE
*		The queue pair is either in Reset, Init, RTR or Error state.
*	IB_INVALID_MAX_SGE
*		The work request has too many scatter gather elements than what the
*		QP is configured.
*	IB_UNSUPPORTED
*		Atomics or Reliable datagram request is not supported by this HCA.
*	IB_INVALID_ADDR_HANDLE
*		Address handle supplied in the work request is invalid.
*	IB_INVALID_PARAMETER
*		The size of the inline data is too much.
*
* PORTABILITY
*	Kernel & User mode.
*
* NOTES
*	Please refer to Table 81 and Table 82 for allowed operation types
*	on different types of queue pairs, and the different modifiers
*	acceptable for the work request for different QP service types.
*
* SEE ALSO
*	uvp_post_recv, uvp_poll_cq
*
********/

/********/

/****f* user-mode Verbs/uvp_post_recv
* NAME
*	uvp_post_recv -- Post a work request to the receive queue of a queue pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_recv) (
	IN		const	void* 						h_qp,
	IN				ib_recv_wr_t*	const		p_recv_wr,
		OUT			ib_recv_wr_t**				pp_recv_failure );

/*
* DESCRIPTION
*	This routine allows to queue a work request to the receive side of a
*	queue pair. The work_req holds necessary data to satisfy an incoming
*	receive message. If an attempt is made to queue more work requests than
*	what is available, an error is returned.
*
* PARAMETERS
*	h_qp
*		[in] Type-cast as appropriate for user/kernel mode, this is
*		the Queue pair handle to which the receive work request is being
*		posted.
*	p_recv_wr
*		[in] List of recv work requests that needs to be posted.
*	pp_recv_failure
*		[out] The work requests that failed.

* RETURN VALUE
*	Any unsuccessful status indicates the status of the first failed request.
*
*	IB_SUCCESS
*		The work request was successfully queued to the receive side of the QP.
*	IB_INVALID_QP_HANDLE
*		qp_handle supplied is not valid.
*	IB_INSUFFICIENT_RESOURCES
*		The qp has exceeded its receive queue depth than what is has been
*		configured.
*	IB_INVALID_WR_TYPE
*		Invalid work request type found in the request.
*	IB_INVALID_QP_STATE
*		QP was in reset or init state.
*		(TBD: there may be an errata that allows posting in init state)
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*	uvp_post_send, uvp_poll_cq
*
********/

/********/

/****f* user-mode Verbs/uvp_post_srq_recv
* NAME
*	uvp_post_srq_recv -- Post a work request to the shared receive queue of a queue pair.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_post_srq_recv) (
	IN		const	void* 						h_srq,
	IN				ib_recv_wr_t*	const		p_recv_wr,
		OUT			ib_recv_wr_t**				pp_recv_failure );

/*
* DESCRIPTION
*	This routine allows to queue a work request to the receive side of a shared
*	queue pair. The work_req holds necessary data to satisfy an incoming
*	receive message. If an attempt is made to queue more work requests than
*	what is available, an error is returned.
*
* PARAMETERS
*	h_srq
*		[in] Type-cast as appropriate for user/kernel mode, this is
*		the shared Queue pair handle to which the receive work request is being
*		posted.
*	p_recv_wr
*		[in] List of recv work requests that needs to be posted.
*	pp_recv_failure
*		[out] The work requests that failed.

* RETURN VALUE
*	Any unsuccessful status indicates the status of the first failed request.
*
*	IB_SUCCESS
*		The work request was successfully queued to the receive side of the QP.
*	IB_INVALID_SRQ_HANDLE
*		srq_handle supplied is not valid.
*	IB_INSUFFICIENT_RESOURCES
*		The qp has exceeded its receive queue depth than what is has been
*		configured.
*	IB_INVALID_WR_TYPE
*		Invalid work request type found in the request.
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*
********/

/********/

/****f* user-mode Verbs/uvp_peek_cq
* NAME
*	uvp_peek_cq
*
* DESCRIPTION
*	Returns the number of entries currently on the completion queue.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_peek_cq) (
	IN		const	void*						h_cq,
		OUT			uint32_t* const				p_n_cqes );

/*
* PARAMETERS
*	h_cq
*		[in] Type-cast as appropriate for user/kernel mode, this is the
*		CQ handle for the completion queue being peeked.
*
*	p_n_cqes
*		[out] Upon successful completion of this call, contains the number
*		of completion queue entries currently on the completion queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The peek operation completed successfully.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the completion queue entry count was not provided.
*
* PORTABILITY
*	Kernel and User mode
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_poll_cq, uvp_rearm_cq,
*	uvp_rearm_n_cq
*****/

/********/

/****f* user-mode Verbs/uvp_poll_cq
* NAME
*	uvp_poll_cq -- Retrieve a work completion record from a completion queue
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_poll_cq) (
	IN		const	void*						h_cq,
	IN	OUT			ib_wc_t**	const			pp_free_wclist,
		OUT			ib_wc_t**	const			pp_done_wclist );

/*
* DESCRIPTION
*	This routine retrieves a work completion entry from the specified
*	completion queue. The contents of the data returned in a work completion
*	is specified in ib_wc_t.
*
* PARAMETERS
*	h_cq
*		[in] Type-cast as appropriate for user/kernel mode, this is
*		the CQ handle for the completion queue being polled.
*	pp_free_wclist
*		[in out] A list of work request structures provided by the consumer
*		for the channel interface to return completed Completion Queue
*		entries.  If not all the entries are consumed, this list holds the
*		list of un-utilized completion entries provided back to the consumer.
*	pp_done_wclist
*		[out] A list of work completions retrieved from the completion queue
*		and successfully processed.
*
* RETURN VALUE
*	IB_SUCCESS
*		Poll completed successfully. If on completion the wc_free list is
*		empty, then there are potentially more entries and the consumer must
*		be ready to retrieve entries further.
*	IB_INVALID_CQ_HANDLE
*		The cq_handle supplied is not valid.
*	IB_NOT_FOUND
*		There are no more entries found in the specified CQ.
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_rearm_cq,
*	uvp_rearm_n_cq, uvp_post_send, uvp_post_recv
*
********/

/********/

/*
 * Define uvp_wc_t so that we can cast directly to ib_wc_t.
 */
typedef struct _uvp_wc
{
	void*					qp_context;
	/* If pointer size is 32-bits, then compiler will pad before uint64_t */
	uint64_t				wr_id;
	ib_wc_type_t			wc_type;

	uint32_t				length;
	uint64_t				vendor_specific;
	ib_wc_status_t			status;

	union _uvp_wc_recv
	{
		struct _uvp_wc_conn
		{
			ib_recv_opt_t	recv_opt;
			ib_net32_t		immediate_data;

		}	conn;

		struct _uvp_wc_ud
		{
			ib_recv_opt_t	recv_opt;
			ib_net32_t		immediate_data;
			ib_net32_t		remote_qp;
			uint16_t		pkey_index;
			ib_net16_t		remote_lid;
			uint8_t			remote_sl;
			uint8_t			path_bits;

		}	ud;
	}	recv;
}	uvp_wc_t;

typedef int
(AL_API *uvp_poll_cq_array) (
	IN		const	void*						h_cq,
	IN		const	int							num_entries,
	IN	OUT			uvp_wc_t*	const			wcs);

/********/

/****f* user-mode Verbs/uvp_rearm_cq
* NAME
*	uvp_rearm_cq -- Invoke the Completion handler, on next entry added.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_rearm_cq) (
	IN		const	void*						h_cq,
	IN		const	boolean_t					solicited );

/*
* DESCRIPTION
*	This routine instructs the channel interface to invoke the completion
*	handler when the next completion queue entry is added to this CQ.
*	Please refer to Volume 1, of the InfiniBand specification for a complete
*	description.
*
* PARAMETERS
*	h_cq
*		[in] Type-cast as appropriate for user/kernel mode, this is the
*		CQ handle for the completion queue being armed.
*	solicited
*		[in] A boolean flag indicating whether the request is to generate a
*		notification on the next entry or on the next solicited entry
*		being added to the completion queue.
*
* RETURN VALUE
*	IB_SUCCESS
*		The notification request was registered successfully.
*	IB_INVALID_CQ_HANDLE
*		cq_handle supplied is not a valid handle.
*
* PORTABILITY
*	Kernel and User mode
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_peek_cq, uvp_poll_cq,
*	uvp_rearm_n_cq
*
********/

/********/

/****f* user-mode Verbs/uvp_rearm_n_cq
* NAME
*	uvp_rearm_n_cq -- Invoke the Completion handler, when next
*	N completions have been added to this CQ.
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_rearm_n_cq) (
	IN		const	void*						h_cq,
	IN		const	uint32_t					n_cqes );

/*
* DESCRIPTION
*	This routine instructs the channel interface to invoke the completion
*	handler when the next N completions are added to this CQ.
*
* PARAMETERS
*	h_cq
*		[in] Type-cast as appropriate for user/kernel mode, this is the
*		CQ handle for the completion queue being armed.
*	n_cqes
*		[in] The number of completion queue entries to be added to the
*		completion queue before notifying the client.  This value must
*		greater than or equal to one and less than or equal to the size
*		of the completion queue.
*
* RETURN VALUE
*	IB_SUCCESS
*		The notification request was registered successfully.
*	IB_INVALID_CQ_HANDLE
*		cq_handle supplied is not a valid handle.
*	IB_INVALID_PARAMETER
*		The requested number of completion queue entries was invalid.
*
* PORTABILITY
*	Kernel and User mode
*
* SEE ALSO
*	uvp_pre_create_cq, uvp_post_create_cq_t, uvp_peek_cq, uvp_poll_cq,
*	uvp_rearm_cq
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_attach_mcast
* NAME
*	uvp_pre_attach_mcast -- Pre-ioctl function to Attach a queue pair
*							to a multicast group
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_attach_mcast) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN		const	ib_gid_t					*p_mcast_gid,
	IN		const	uint16_t					mcast_lid,
	IN	OUT			ci_umv_buf_t				*p_umv_buf,
		OUT			ib_mcast_handle_t			*ph_mcast);

/*
* DESCRIPTION
*	uvp_pre_attach_mcast() is the pre-ioctl routine implemented by vendor
*	to attach a queue pair to a multicast group.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Queue pair handle (in user-mode library)
*		which needs to be added to the multicast group on the adapter.
*	mcast_lid
*		[in] The multicast group LID value.
*	p_mcast_gid
*		[in] IPv6 address associated with this multicast group.
*	p_umv_buf
*		[in out] On input, UAL provides this buffer template.
*		On return from this function, p_umv_buf contains
*		any vendor-specific record to be exchanged with the vendor's
*		HCA driver.
*	ph_mcast
*		[out] (OPTIONAL) Vendor's Handle to the newly created Multicast handle
*		holding the association of this queue pair to the multicast group.
*
* RETURN VALUE
*	IB_SUCCESS
*		The queue pair handle was successfully added to the multicast
*		group.
*	IB_INVALID_QP_HANDLE
*		qp_handle supplied is invalid.
*	IB_INVALID_SERVICE_TYPE
*		Queue pair handle supplied is not of unreliable datagram type.
*	IB_INVALID_GID
*		The supplied addr is not a valid multicast ipv6 address.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the request.
*
* PORTABILITY
*	User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_post_attach_mcast_t,
*	uvp_pre_detach_mcast, uvp_post_detach_mcast_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_attach_mcast_t
* NAME
*	uvp_post_attach_mcast_t -- Post-ioctl function to Attach a queue pair
*							 to a multicast group
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_attach_mcast) (
	IN		const	ib_qp_handle_t				h_uvp_qp,
	IN				ib_api_status_t				ioctl_status,
	IN	OUT			ib_mcast_handle_t			*ph_mcast,
	IN				ci_umv_buf_t				*p_umv_buf );

/*
* DESCRIPTION
*	uvp_post_attach_mcast_t() is the post-ioctl routine implemented by vendor
*	to attach a queue pair to a multicast group.
*
* PARAMETERS
*	h_uvp_qp
*		[in] Vendor's Queue pair handle (in user-mode library)
*		which needs to be added to
*		the multicast group on the adapter.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*	ph_mcast
*		[in out] Vendor's Multicast handle,
*		holding the association of this queue pair to the multicast group.
*		If it specified in pre function than it must be the same value here.
*	p_umv_buf
*		[in out] On input, it contains any vendor-specific private information
*		exchanged with the vendor's Verbs Provider Driver (uvp_pre_attach_mcast)
*		Vendor is expected to check vendor-specific status in
*		umv_buf as appropriate.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_attach_mcast,
*	uvp_pre_detach_mcast, uvp_post_detach_mcast_t
*
********/

/********/

/****f* user-mode Verbs/uvp_pre_detach_mcast
* NAME
*	uvp_pre_detach_mcast -- Pre-ioctl function to detach a queue pair
*							to a multicast group
*
* SYNOPSIS
*/

typedef ib_api_status_t
(AL_API *uvp_pre_detach_mcast) (
	IN				ib_mcast_handle_t			h_uvp_mcast );

/*
* DESCRIPTION
*	uvp_pre_attach_mcast() is the pre-ioctl routine implemented by vendor
*	to attach a queue pair to a multicast group.
*	Upon return from the pre-ioctl function, UAL packages up the UMV buffer
*	in an IOCTL and passes it on to the user-mode proxy. UAL passes the
*	info to the user-mode proxy stating that it no longer wishes to receive
*	callback for mcast join for the caller.  Note that UAL takes care of
*	handling callbcak.
*
* PARAMETERS
*	h_uvp_mcast
*		[in] Vendor's Multicast handle (in user-mode library)
*		holding the association of this queue pair to the multicast group.
*
* RETURN VALUE
*	IB_SUCCESS
*		The queue pair handle was successfully added to the multicast
*		group.
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_attach_mcast,
*	uvp_post_attach_mcast_t, uvp_post_detach_mcast_t
*
********/

/********/

/****f* user-mode Verbs/uvp_post_detach_mcast_t
* NAME
*	uvp_post_detach_mcast_t -- Post-ioctl function to detach a queue pair
*							 from a multicast group
*
* SYNOPSIS
*/

typedef void
(AL_API *uvp_post_detach_mcast) (
	IN				ib_mcast_handle_t			h_uvp_mcast,
	IN				ib_api_status_t				ioctl_status );

/*
* DESCRIPTION
*	uvp_post_detach_mcast_t() is the post-ioctl routine implemented by vendor
*	to attach a queue pair to a multicast group.
*
* PARAMETERS
*	h_uvp_mcast
*		[out] Vendor's Multicast handle holding the association of this
*		queue pair to the multicast group.
*	ioctl_status
*		[in] The ioctl status of the AL API.
*
* RETURN VALUE
*	This function does not return a value.
*
* PORTABILITY
*	Kernel & User mode.
*
* SEE ALSO
*	uvp_pre_create_qp, uvp_post_create_qp_t, uvp_pre_attach_mcast,
*	uvp_post_attach_mcast_t, uvp_pre_detach_mcast
*
********/

/********/

/****s* user-mode Verbs/uvp_interface_t
* NAME
*	uvp_interface_t -- Interface holding supported Vendor APIs
*
* PURPOSE
*	The following structure is supplied by a Vendor library
*	providing verbs functionality.
*
* SOURCE
*/
typedef struct _uvp_interface
{
	uvp_pre_open_ca_t			pre_open_ca;
	uvp_post_open_ca_t			post_open_ca;
	uvp_pre_query_ca			pre_query_ca;
	uvp_post_query_ca_t			post_query_ca;
	uvp_pre_modify_ca			pre_modify_ca;
	uvp_post_modify_ca_t		post_modify_ca;
	uvp_pre_close_ca_t			pre_close_ca;
	uvp_post_close_ca_t			post_close_ca;

	uvp_pre_ci_call				pre_ci_call;
	uvp_post_ci_call			post_ci_call;

	uvp_pre_allocate_pd			pre_allocate_pd;
	uvp_post_allocate_pd_t		post_allocate_pd;
	uvp_pre_deallocate_pd		pre_deallocate_pd;
	uvp_post_deallocate_pd_t	post_deallocate_pd;

	uvp_pre_create_av			pre_create_av;
	uvp_post_create_av_t		post_create_av;
	uvp_pre_query_av			pre_query_av;
	uvp_post_query_av_t			post_query_av;
	uvp_pre_modify_av			pre_modify_av;
	uvp_post_modify_av_t		post_modify_av;
	uvp_pre_destroy_av			pre_destroy_av;
	uvp_post_destroy_av_t		post_destroy_av;

	uvp_pre_create_srq			pre_create_srq;
	uvp_post_create_srq_t		post_create_srq;
	uvp_pre_modify_srq			pre_modify_srq;
	uvp_post_modify_srq_t		post_modify_srq;
	uvp_pre_query_srq			pre_query_srq;
	uvp_post_query_srq_t		post_query_srq;
	uvp_pre_destroy_srq			pre_destroy_srq;
	uvp_post_destroy_srq_t		post_destroy_srq;

	uvp_pre_create_qp			pre_create_qp;
	uvp_post_create_qp_t		post_create_qp;
	uvp_pre_modify_qp			pre_modify_qp;
	uvp_post_modify_qp_t		post_modify_qp;
	uvp_pre_query_qp			pre_query_qp;
	uvp_post_query_qp_t			post_query_qp;
	uvp_pre_destroy_qp			pre_destroy_qp;
	uvp_post_destroy_qp_t		post_destroy_qp;

	uvp_pre_create_cq			pre_create_cq;
	uvp_post_create_cq_t		post_create_cq;
	uvp_pre_query_cq			pre_query_cq;
	uvp_post_query_cq_t			post_query_cq;
	uvp_pre_resize_cq			pre_resize_cq;
	uvp_post_resize_cq_t		post_resize_cq;
	uvp_pre_destroy_cq			pre_destroy_cq;
	uvp_post_destroy_cq_t		post_destroy_cq;

	uvp_pre_create_mw			pre_create_mw;
	uvp_post_create_mw_t		post_create_mw;
	uvp_pre_query_mw			pre_query_mw;
	uvp_post_query_mw_t			post_query_mw;
	uvp_pre_destroy_mw			pre_destroy_mw;
	uvp_post_destroy_mw_t		post_destroy_mw;

	uvp_bind_mw					bind_mw;
	uvp_post_send				post_send;
	uvp_post_recv				post_recv;
	uvp_post_srq_recv			post_srq_recv;

	uvp_peek_cq					peek_cq;
	uvp_poll_cq					poll_cq;
	uvp_rearm_cq				rearm_cq;
	uvp_rearm_n_cq				rearm_n_cq;

	uvp_pre_attach_mcast		pre_attach_mcast;
	uvp_post_attach_mcast		post_attach_mcast;
	uvp_pre_detach_mcast		pre_detach_mcast;
	uvp_post_detach_mcast		post_detach_mcast;

	uvp_nd_modify_qp_t			nd_modify_qp;
	uvp_nd_get_qp_state_t		nd_get_qp_state;
	uvp_wv_pre_create_qp		wv_pre_create_qp;
	uvp_poll_cq_array			poll_cq_array;

} uvp_interface_t;

/********/

// {A1F1EA66-4D17-4d04-B910-893F658241D0}
DEFINE_GUID(IID_UVP, 
0xa1f1ea66, 0x4d17, 0x4d04, 0xb9, 0x10, 0x89, 0x3f, 0x65, 0x82, 0x41, 0xd0);

/****f* user-mode Verbs/uvp_get_interface
* NAME
*	uvp_get_interface -- Get the Vendor's supported function calls
*
* SYNOPSIS
*/
typedef ib_api_status_t
(AL_API *uvp_get_interface_t)(
	IN			GUID				iid,
	IN	OUT		void*				pifc);
/*
* DESCRIPTION
*	This routine is called to get the functions supported by
*	a vendor's library.  It may be used to obtain the list of
*	verb calls supported by a vendor.
*
*	If the vendor does not support an interface, it should returns
*	IB_UNSUPPORTED.  The specific details of each interface returned
*	is dependent upon the requested interface.
*
* PARAMETERS
*	iid
*		[in] GUID of the requested interface.
*	pifc
*		[in out] Pointer to the structure that that will receive the
*		interface functions.
*
* RETURN VALUE
*	IB_SUCCESS
*		The interface was returned successfully.
*	IB_UNSUPPORTED
*		The requested interface isn't supported by the vendor's library.
*
* PORTABILITY
*	User mode
*
* SEE ALSO
*	uvp_interface_t
*
********/

#endif // __IB_UAL_UVP_H__

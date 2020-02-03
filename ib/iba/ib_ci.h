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
 * $Id: ib_ci.h 4395 2009-06-01 08:20:57Z leonid $
 */


#if !defined(__IB_CI_H__)
#define __IB_CI_H__

#include <iba/ib_types.h>


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


/****h* IB_API/Verbs
* NAME
*	Verbs -- Verbs implements the hardware and software glue to the OS layer.
* COPYRIGHT
*	Copyright© 2001 Intel Corporation - All Rights Reserved.
* DESCRIPTION
*	The Verbs API definition defines the interface mechanism between an IHV
*	supplied driver component. It implements verbs functionality as defined
*	Volume 1, of the InfiniBand(tm) specifications.
* AUTHOR
*	Intel Corporation
* CREATION DATE
*	XX.XX.XX
* NOTES
*	Evolving Spec!!
*	Invalid Handle checks are a mere signature checks in kernel mode. Hence
*	passing invalid pointer would lead to panics in the kernel. For user mode
*	These are verified for most verbs that need to take a kernel transition.
*	Verbs those are entirely done in user mode that would affect speed path
*	do not perform consistency checks. So invalid pointers would lead to
*	application crash with core dumps.
***********
*/


/*
 * Version that identifies this version of the header file for interface
 * definition.
 */
#define VERBS_MAJOR_VER			(0x0002)
#define VERBS_MINOR_VER			(0x0000)

#define VERBS_VERSION			(((VERBS_MAJOR_VER) << 16) | (VERBS_MINOR_VER))
#define MK_VERBS_VERSION(maj,min)	((((maj) & 0xFFFF) << 16) | \
									((min) & 0xFFFF))

/*
 * TODO: The in and out buffers should be separated (usage can still make
 * both point to the same actual memory region.
 */
/****s* Verbs/ci_umv_buf_t
* NAME
*	ci_umv_buf_t -- Vendor specific structure to facilitate user mode IO
* DESCRIPTION
*	This structure is provided to assist the vendor specific user mode
*	library to exchange information with its kernel mode driver. The
*	user mode InfiniBand(tm) Access Layer will call the vendor specific
*	module before a call is made to the kernel mode driver. The kernel mode
*	driver is expected to know the format and data in the p_inout_buf,
*	and copy any necessary data that must be handed to the user mode
*	vendor library.
* PURPOSE
*	command
*		A command code that is understood by the vendor specific kernel
*		mode driver.
*	p_inout_buf
*		The user mode component of the vendor specific library allocates
*		this memory and passes information in this buffer. vendor is expected
*		to set both the input and output buffer sizes appropriately.
*		This information is required since the kernel mode proxy that passes
*		this buffer to the kernel mode vendor specific library will copy the
*		content of this buffer to a kernel mode buffer. The kernel mode
*		vendor specific driver would copy the data that needs to be returned
*		to the user mode component, and set the output size appropriately
*		so that the proxy can now copy the data back to user mode buffer.
*
*		In the Infiniband Access Layer, it is important to know the
*		usage of umv_buf and whether the contents of the p_inout_buf
*		can have embedded user-mode pointers. When invoked from an
*		arbitrary thread context, Vendor driver can NOT access user-mode
*		pointers of a user-process.
*	input_size
*		Size of the input buffer, must be set by the user mode vendor
*		specific library.
*	output_size
*		Size of the output buffer. Must be set by the user mode component
*		to specify the maximum size of the data expected from its kernel
*		mode driver. The kernel mode driver would set the size to the exact
*		size that needs to be returned to its user mode counterpart.
*	status
*		Indicates the status of the operation from the kernel mode vendor
*		specific driver. The caller is supposed to initialize it appropriately
*		to identify if an operation succeded, or failed. For e.g. when
*		the user mode library is called after a resource creation, the user
*		mode vendor specific code must be able to identify if there is
*		post processing required, or if any resource allocation failed.
* SOURCE
*/
typedef struct _umv_buf
{
	uint32_t		command;
	uint32_t		status;
	uint32_t		input_size;
	uint32_t		output_size;
	uint64_t		p_inout_buf;
} ci_umv_buf_t;
/******/

/****f* Verbs/ci_completion_cb_t
* NAME
*	ci_completion_cb_t -- Completion Notification callback.
* SYNOPSIS
*/

typedef void
(*ci_completion_cb_t)(
	IN	void	*cq_context );

/*
* DESCRIPTION
*	This function prototype indicates the parameter passed to ci_open_ca()
*	to receive completion callbacks.
* PARAMETERS
*	cq_context
*		[in] Completion queue context passed during the ci_create_cq
* RETURN VALUE
*	None
* NOTES
*	The consumer only gets the cq_context. It is the client
*	responsibility to store the cq_handle in the context after the creation
*	time. So it can call ci_poll_cq() after the arrival of the notification.
* SEE ALSO
*	ci_create_cq
******
*/


/****f* Verbs/ci_async_event_cb_t
* NAME
*	ci_async_event_cb_t
*
* DESCRIPTION
*	Asynchronous event notification routine.
*
* SYNOPSIS
*/
typedef void
(*ci_async_event_cb_t)(
	IN		ib_event_rec_t*		p_event_record );
/*
* PARAMETERS
*	p_event_record
*		[in] Information describing the type of event that has occurred.
*
* NOTES
*	This routine is called when an asynchronous event is generated by a
*	channel adapter.  The event notification record passed has relevant
*	information on the type of the event, the source that caused the event,
*	and the context associated.
*
*   This routine is called at DISPATCH.
*****/


/****f* Verbs/ci_open_ca
* NAME
*	ci_open_ca -- open and possibly obtain a handle to access the HCA.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_open_ca) (
	IN		const	ib_net64_t					ca_guid,
	IN		const	ci_async_event_cb_t			pfn_async_event_cb,
	IN		const	void* const					ca_context,
		OUT			ib_ca_handle_t				*ph_ca );
/*
* DESCRIPTION
*	This routine returns a handle to an open instance of a HCA. Client can call
*	this routine to retrieve a new open instance. Only one instance of the
*	open call is active at any time. If a duplicate open is called by the
*	consumer or any other consumer, it IB_RESOURCE_BUSY error status is
*	returned.
* PARAMETERS
*	ca_guid
*		[in] The HCA adapter's EUI64 identifier. Clients would use other
*		enumeration API's to locate all available adapters and their
*		guids in a system, e.g. GetCaGuids(), maintained by the IB
*		Access Layer. User mode consumers also have the same mechanism
*		to retrieve this information.
*	pfn_async_event_cb
*		[in] Asynchronous event handler, one per open instance.
*	ca_context
*		[in] Verbs consumer supplied value, which is returned on calls to
*		handlers which in turn is used by clients to identify the
*		open instance.
*	ph_ca
*		[out] Pointer to a handle to the newly open instance of the CA returned
*		by the Verbs Provider.
*
* RETURN VALUE
*	IB_SUCCESS
*		The HCA is successfully opened and returned handle is valid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		Callback routine are not provided, GUID value is zero, or ph_ca is NULL
*	IB_RESOURCE_BUSY
*		The interface is already open by another consumer.
*	IB_NOT_FOUND
*		ca_guid passed is not valid
*
* SEE ALSO
*	ci_query_ca, ci_modify_ca, ci_close_ca
*********/


/*
 * Structure is provided by caller, and must be valid while registered.
 * Callback context for CA events references structure.
 */
typedef struct _ci_event_handler
{
	LIST_ENTRY				entry;
	ci_async_event_cb_t		pfn_async_event_cb;

}	ci_event_handler_t;

typedef void
(*ci_register_event_handler_t) (
	IN		const	ib_ca_handle_t				h_ca,
	IN				ci_event_handler_t*			p_reg);

typedef void
(*ci_unregister_event_handler_t) (
	IN		const	ib_ca_handle_t				h_ca,
	IN				ci_event_handler_t*			p_reg);


/****f* Verbs/ci_um_open_ca
* NAME
*	ci_um_open_ca -- Create a CA context for use by user-mode processes.
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_um_open_ca_t) (
	IN		const	ib_ca_handle_t				h_ca,
	IN	OUT			ci_umv_buf_t* const			p_umv_buf,
		OUT			ib_ca_handle_t* const		ph_um_ca );
/*
* DESCRIPTION
*	This routine is used called on behalf of a user-mode application to
*	establish a per-CA context in user-mode.
*
* PARAMETERS
*	h_ca
*		[in] Handle returned by an earlier call to ci_open_ca()
*	p_umv_buf
*		[in/out] Vendor specific parameter to support user mode IO.
*	ph_um_ca
*		[out] Handle to pass into ci_um_close_ca call to free any kernel
*		resources allocated for the user-mode appliation.
*
* RETURN VALUE
*	IB_SUCCESS
*		The user-mode context information is returned successfully.
*	IB_INSUFFICIENT_MEMORY
*		The size of the p_ca_attr buffer, specified through p_size, is
*		insufficient to store all of the CA attribute information.
*	IB_INVALID_CA_HANDLE
*		h_ca is invalid
*	IB_INVALID_PARAMETER
*		The p_umv_buf parameters are insufficient to complete the request.
*
* SEE ALSO
*	ci_query_ca, ci_modify_ca, ci_close_ca
*********/


/****f* Verbs/ci_query_ca
* NAME
*	ci_query_ca -- Query the attributes of the HCA
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_ca) (
	IN		const	ib_ca_handle_t				h_ca,
		OUT			ib_ca_attr_t				*p_ca_attr OPTIONAL,
	IN	OUT			uint32_t					*p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine retrieves vital information about this hca. It returns
*	necessary information about HCA guid, port guids, LID's assigned by
*	the master SM. Clients can use this information to communicate with the
*	Master SM node to perform path queries etc.
* PARAMETERS
*	h_ca
*		[in] Handle returned by an earlier call to ci_open_ca()
*	p_ca_attr
*		[out] CA attribute of this Host Channel adapter
*	p_size
*		[in/out] On input, this references the size of the data buffer
*		referenced by the p_ca_attr parameter.
*		On output, the number of bytes used or needed to copy all CA
*		attribute information.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The attribute structure is returned completely.
*	IB_INSUFFICIENT_MEMORY
*		The size of the p_ca_attr buffer, specified through p_size, is
*		insufficient to store all of the CA attribute information.
*	IB_INVALID_CA_HANDLE
*		h_ca is invalid
*	IB_INVALID_PARAMETER
*		p_size is NULL.
* NOTES
*	Users may obtain the size of the data buffer required to obtain the
*	CA attributes by calling this function with p_ca_attr set to NULL.
*	The channel interface will then return the necessary size in the
*	variable referenced by the p_size parameter.  The caller can then allocate
*	exact size and call this routine again. No partial information is returned
*	if the size is not sufficient.
* SEE ALSO
*	ci_open_ca, ci_modify_ca
********/

/****f* Verbs/ci_modify_ca
* NAME
*	ci_modify_ca -- Modify port attributes and error counters
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_ca) (
	IN	const ib_ca_handle_t		h_ca,
	IN	const uint8_t				port_num,
	IN	const ib_ca_mod_t			ca_mod,
	IN	const ib_port_attr_mod_t	*p_port_attr_mod );
/*
* DESCRIPTION
*	Modifies either the P_KEY/Q_KEY violation counters, or sets the capability
*	mask in the port attributes. This is effectively translated to PORTINFO
*	values responded later when a MAD from SM or another node arrives to
*	retrieve port related attributes.
*
* PARAMETERS
*	h_ca
*		[in] Handle returned by previous call to ci_open_ca()
*	port_num
*		[in] Port number, which needs to be modified.
*	ca_mod
*		[in] Command mask to perform operations on.
*	p_port_attr_mod
*		[in] port attribute which needs this change to be performed.
*		if the capability bit is set, then that corresponding
*		port capability is turned on.
* RETURN VALUE
*	IB_SUCCESS
*		Modify port attributes was performed
*	IB_INVALID_PORT
*		Invalid port number supplied in port_att.
*	IB_INVALID_PARAMETER
*		Unknown Command supplied in port_attr.
*	IB_UNSUPPORTED
*		Optional Q_KEY and P_KEY violation counters are not supported.
*	IB_INVALID_CA_HANDLE
*		h_ca is invalid
* NOTES
* No ownership checks are performed in the Verbs Provider Driver.
* All such permission checks are to be performed by the IB access layer
* before passing requests down to the HCA driver. These operations can be
* performed only by the special QP owner. Either the QP0 or QP1. Since port
* attributes is really maintained by the QP0 for SMA to respond with correct
* values, but the agent capability is really a QP1 functionality.
* SEE ALSO
*	ci_open_ca, ci_query_ca, ci_close_ca
*/
/********/

/****f* Verbs/ci_close_ca
* NAME
*	ci_close_ca -- Close access to adapter via this h_ca
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_close_ca) (
	IN		ib_ca_handle_t	h_ca );
/*
* DESCRIPTION
*	This routine is called when the client no longer wishes to use HCA
*	resources obtained via this h_ca. All resources allocated as part
*	this handle during the ci_open_ca are destroyed.
* PARAMETERS
*	h_ca
*		[in] CA handle returned via the ci_open_ca() call.
* RETURN VALUE
*	IB_SUCCESS
*		The intend to destroy is registered. No further calls for
*		completion or async event will be sent to this instance. When it is
*		appropriate to destroy this instance, the event h_kevent is signaled.
*	IB_RESOURCE_BUSY
*		Some resource allocated via this handle is not freed.
*	IB_INVALID_CA_HANDLE
*		h_ca is invalid
* NOTES
*	This call cannot be called from any of the notification functions invoked
*	by the Verbs driver. For e.g. the completion handler or the async error
*	callback provided during the ci_open_ca() call. The call will block until
*	all references to this adapter object is closed which includes all the
*	pending callbacks returning back to the verbs provider driver.
*
*	Resources allocated during the ci_open_ca() is deallocated. Other resource
*	cleanup are responsibility of the consumer .
* SEE ALSO
*	ci_open_ca
********/

/****f* Verbs/ci_um_close_ca_t
* NAME
*	ci_um_close_ca_t -- Close user-mode access to adapter via this h_ca
* SYNOPSIS
*/
typedef void
(*ci_um_close_ca_t) (
	IN				ib_ca_handle_t				h_ca,
	IN				ib_ca_handle_t				h_um_ca );
/*
* DESCRIPTION
*	This routine is called when the client no longer wishes to use HCA
*	resources obtained via this h_ca. All resources allocated as part
*	this handle during the ci_um_open_ca are destroyed.
* PARAMETERS
*	h_ca
*		[in] CA handle returned via the ci_open_ca() call.
*	h_um_ca
*		[in] CA handle returned via the ci_um_open_ca() call.
*
* RETURN VALUE
*	This funtion does not return a value.
* NOTES
*	This call is invoked from the context of a UM application when such an
*	appliation closes the HCA in user-mode.
*
*	Resources allocated during the ci_um_open_ca() are deallocated.
*
* SEE ALSO
*	ci_um_open_ca
********/

/****f* Verbs/ci_allocate_pd
* NAME
*	ci_allocate_pd -- Allocate a protection domain for this adapter.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_allocate_pd) (
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_pd_type_t				type,
		OUT			ib_pd_handle_t				*ph_pd,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine allocates a protection domain handle, which is later
*	used to create QP's, Register Memory Regions, Bind Memory Windows
*	and address vectors. Protection domain has no InfiniBand architectural
*	attributes but the OS implements policy on its usage and allocation.
* PARAMETERS
*	h_ca
*		[in] Handle returned by ci_open_ca()
*
*	type
*		[in] Type of the protection domain.  CA vendors may use this
*		information to optimize how the PD is allocated.
*
*	ph_pd
*		[out] The handle to the newly created protection domain
*
*	p_umv_buf
*		[in/out] Vendor specific parameter to support user mode IO.
*
* RETURN VALUE
*	IB_SUCCESS
*		PD is successfully allocated and the ph_pd is valid.
*
*	IB_INSUFFICIENT_RESOURCES
*		No more PD's available for this adapter.
*
*	IB_INVALID_CA_HANDLE
*		HCA handle is not valid
*
* SEE ALSO
*	ci_deallocate_pd
*******/

/****f* Verbs/ci_deallocate_pd
* NAME
*	ci_deallocate_pd -- Deallocate a protection domain object.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_deallocate_pd) (
	IN		ib_pd_handle_t		h_pd );

/*
* DESCRIPTION
*	This routine deallocates a pd that is allocated via the ci_allocate_pd()
*	call. The PD cannot be deallocated if it is still bound to a QP, any memory
*	region, memory window or address vector.
* PARAMETERS
*	h_pd
*		[in] Handle allocated via the ci_allocate_pd()
* RETURN VALUE
*	IB_SUCCESS
*		PD is freed successfully
*	IB_INVALID_PD_HANDLE
*		pd_handle is invalid
*	IB_RESOURCE_BUSY
*		PD is probably still bound to some resource
* SEE ALSO
*	ci_allocate_pd
*******
*/


/****f* Verbs/ci_create_av
* NAME
*	ci_create_av -- Create an address vector for use in UD.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_av) (
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_av_attr_t				*p_av_attr,
		OUT			ib_av_handle_t				*ph_av,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );

/*
* DESCRIPTION
*	This routine creates an address vector for use in unreliable datagram
*	queue pairs. The information necessary to create the address vector
*	handle is supplied in the ib_av_attr_t parameter.
* PARAMETERS
*	h_pd
*		[in] Protection domain to which this av is associated.
*	p_av_attr
*		[in] Parameters to create the address vector handle
*	ph_av
*		[out] Handle to use for datagram sends.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The create operation was successful
*	IB_INSUFFICIENT_RESOURCES
*		No more address handles are available
*	IB_INVALID_PD_HANDLE
*		The specified protection domain handle is invalid
*	IB_INVALID_PORT
*		Invalid port number supplied.
*	IB_INVALID_PARAMETER
*		One of the p_av_attr or p_av_attr was NULL.
* NOTES
*	The values in the p_av_attr is not validated for correctness. The values
*	in the attribute such as port number, protection domain etc are also
*	validated during processing by the channel adapter. If the attribute
*	validation fails a processing error IB_WCS_LOCAL_OP_ERR.
* SEE ALSO
*	ci_allocate_pd
********/

/****f* Verbs/ci_query_av
* NAME
*	ci_query_av -- Obtain the address vector associated with the handle
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_av) (
	IN		const	ib_av_handle_t				h_av,
		OUT			ib_av_attr_t				*p_av_attr,
		OUT			ib_pd_handle_t				*ph_pd,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );

/*
* DESCRIPTION
*	This routine returns the address vector and pd_handle associated with the
*	av_handle.
* PARAMETERS
*	h_av
*		[in] Handle to the address vector
*	p_av_attr
*		[out] address vector data referred by the av_handle
*	ph_pd
*		[out] pd handle associated with the av_handle
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		returned values are valid
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid
*	IB_INVALID_PARAMETER
*		One of the p_av_attr or ph_pd parameters was NULL.
*	IB_INVALID_PORT
*		Invalid port number passed in the Address Vector.
* SEE ALSO
*	ci_create_av, ci_modify_av
*******
*/

/****f* Verbs/ci_modify_av
* NAME
*	ci_modify_av -- Change the address vector referred by the av_handle
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_av) (
	IN		const	ib_av_handle_t				h_av,
	IN		const	ib_av_attr_t				*p_av_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine allows a consumer to modify the address information
*	passed.
* PARAMETERS
*	h_av
*		[in] Address handle that needs to be updated with new info.
*	p_av_attr
*		[in] New address vector to associate with the addr_handle.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		Operation was successful
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid
*	IB_INVALID_PORT
*		Invalid port number passed in the Address Vector.
*	IB_INVALID_PARAMETER
*		The parameter p_av_attr is not valid.
* NOTES
*	The values in the p_av_attr is not validated for correctness. The values
*	in the attribute such as port number, protection domain etc are validated
*	during processing by the channel adapter. If the attribute validation fails
*	a processing error IB_WCS_LOCAL_OP_ERR.
* SEE ALSO
*	ci_create_av, ci_query_av
*********
*/

/****f* Verbs/ci_destroy_av
* NAME
*	ci_destroy_av -- Destroy the address vector
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_destroy_av) (
	IN	const	ib_av_handle_t		h_av );
/*
* DESCRIPTION
*	This routine destroys the specified address handle. After the routine
*	returns, this address handle cannot be used to reference the destination.
* PARAMETERS
*	h_av
*		[in] Handle that needs to be destroyed.
* RETURN VALUE
*	IB_SUCCESS
*		Operation was successful.
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid
* SEE ALSO
*	ci_create_av
*********
*/

/****f* Verbs/ci_create_srq
* NAME
*	ci_create_srq -- Create a Shared Queue Pair for the specified HCA
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_srq) (
	IN		const	ib_pd_handle_t			h_pd,
	IN		const	void						*srq_context,
	IN		const	ci_async_event_cb_t			pfn_async_event_cb,
	IN		const	ib_srq_attr_t * const		p_srq_attr,
		OUT			ib_srq_handle_t			*ph_srq,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	A new shared queue pair is created on the specified HCA. The initial set of
*	parameters is provided by the srq_attr_mask/p_srq_attr parameters. The newly created
*	queue pair  with its attributes is returned in the srq_query_attr structure.
* PARAMETERS
*	h_pd
*		[in] Handle to Protection Domain
*	srq_context
*		[in] A user specified context passed in a asynchronous error callback.
*	pfn_async_event_cb
*		[in] Asynchronous event handler.
*	p_srq_attr
*		[in out] Initial attributes with which the srq must be created.
*	ph_srq
*		[out] Handle to the queue pair newly created.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The queue pair is successfully created with the provided initial
*		attributes.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_PD_HANDLE
*		pd_handle supplied in the qp_create_attr is invalid
*	IB_INVALID_SERVICE_TYPE
*		Invalid service type.
*	IB_INVALID_MAX_WRS
*		Max WRS capacity exceeded
*	IB_INVALID_MAX_SGE
*		Max Scatter gather element request exceeds HCA capability
*	IB_UNSUPPORTED
*		Unreliable datagram not supported
*	IB_INVALID_PARAMETER
*		The parameter p_create_attr is invalid.
* NOTES
*	If any of the initial parameters is not valid, the queue pair is not
*	created. If the routine call is not successful then the contents of
*	qp_query_attr and qp_handle is undefined.
* SEE ALSO
*	ci_query_qp, ci_modify_qp, ci_destroy_qp
******
*/


/****f* Verbs/ci_modify_srq
* NAME
*	ci_modify_srq -- Modify attributes of the specified SRQ.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_srq) (
	IN		const	ib_srq_handle_t				h_srq,
	IN		const	ib_srq_attr_t* const			p_srq_attr,
	IN		const	ib_srq_attr_mask_t			srq_attr_mask,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine is used to modify the srq states or other attributes of the
*	srq. On successful completion, the requested state transition is performed
*	and the srq is transitioned to the required state.
* PARAMETERS
*	h_srq
*		[in] Handle to the queue pair whose state is to be modified.
*	p_srq_attr
*		[in] Initial attributes with which the srq must be created.
*	srq_attr_mask
*		[in] Flags, specifying valid fields in ib_srq_attr_t structure.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful and the QP attributes are modified
*		to the requested state.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the requested operation.
*	IB_INVALID_QP_HANDLE
*		Invalid QP handle was passed.
*	IB_UNSUPPORTED
*		Requested operation is not supported, for e.g. Atomic operations.
*	IB_QP_INVALID_STATE
*		Invalid state transition request. Current QP state not in allowable
*		state.
*	IB_INVALID_PARAMETER
*		The parameter p_modify_attr is not valid.
* SEE ALSO
*	ci_create_qp, ci_destroy_qp, ci_query_qp
******
*/


/****f* Verbs/ci_query_srq
* NAME
*	ci_query_srq -- Query the current SRQ attributes
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_srq) (
	IN		const	ib_srq_handle_t				h_srq,
		OUT			ib_srq_attr_t* const			p_srq_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine queries the current attributes for the srq
*	corresponding to h_srq. The attributes are returned in p_query_attr.
*	Depending on the current state of the srq, some of the fields in the
*	attribute structure may not be valid.
* PARAMETERS
*	h_srq
*		[in] Handle to the srq for which the attributes are being retrieved
*	p_srq_attr
*		[out] Pointer to the ib_srq_query_t structure where the current
*		attributes of the srq is returned.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The values returned in p_qp_attr are valid.
*	IB_INVALID_QP_HANDLE
*		The h_qp supplied is not a valid handle.
*	IB_INVALID_PARAMETER
*		Parameter p_qp_attr is not valid.
* SEE ALSO
*	ci_create_qp, ci_destroy_qp, ci_modify_srq
*****
*/


/****f* Verbs/ci_destroy_srq
* NAME
*	ci_destroy_srq -- Destroy the specified Shared Queue Pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_destroy_srq) (
	IN	const	ib_srq_handle_t		h_srq );
/*
* DESCRIPTION
*	Destroys the associated srq. The srq could have outstanding work requests
*	when this call is made. Any outstanding work requests *SHALL NOT* be
*	completed after this routine returns.
* PARAMETERS
*	h_srq
*		[in] Handle to the srq that needs to be destroyed.
* RETURN VALUE
*	IB_SUCCESS
*		The intend to destroy this queue pair is registered and no further
*		work requests will be processed. When no pending callbacks are in
*		progress, the destroy_callback function is invoked which marks the
*		destruction of the resource. The consumer can be guaranteed that
*		no future callbacks will be propagated on behalf of this resource.
*	IB_INVALID_QP_HANDLE
*		The handle passed is invalid.
*	IB_RESOURCE_BUSY
*		If the queue pair is a unreliable datagram service type, and
*		is still bound to a multicast group.
* NOTES
*	This call cannot be called from any of the notification functions invoked
*	by the Verbs driver. For e.g. the completion handler or the async error
*	callback provided during the ci_open_ca() call. The call will block until
*	all references to this adapter object is closed which includes all the
*	pending callbacks returning back to the verbs provider driver.
* SEE ALSO
*	ci_cquery_qp, ci_destroy_qp, ci_modify_srq
******
*/


/****f* Verbs/ci_create_qp
* NAME
*	ci_create_qp -- Create a Queue Pair for the specified HCA
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_qp) (
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	void						*qp_context,
	IN		const	ci_async_event_cb_t			pfn_async_event_cb,
	IN		const	ib_qp_create_t				*p_create_attr,
		OUT			ib_qp_attr_t				*p_qp_attr,
		OUT			ib_qp_handle_t				*ph_qp,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	A new queue pair is created on the specified HCA. The initial set of
*	parameters is provided by the qp_create_attr parameter. The newly created
*	queue pair, with its attributes such as the qp number is returned
*	in the qp_query_attr structure.
* PARAMETERS
*	h_pd
*		[in] Handle to Protection Domain
*	qp_context
*		[in] A user specified context passed in a asynchronous error callback.
*	pfn_async_event_cb
*		[in] Asynchronous event handler.
*	p_create_attr
*		[in] Initial attributes with which the qp must be created.
*	p_qp_attr
*		[out] Attributes of the newly created queue pair.
*	ph_qp
*		[out] Handle to the queue pair newly created.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The queue pair is successfully created with the provided initial
*		attributes.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_PD_HANDLE
*		pd_handle supplied in the qp_create_attr is invalid
*	IB_INVALID_CQ_HANDLE
*		cq_handle supplied for send/receive is invalid.
*	IB_INVALID_SERVICE_TYPE
*		Invalid service type.
*	IB_INVALID_MAX_WRS
*		Max WRS capacity exceeded
*	IB_INVALID_MAX_SGE
*		Max Scatter gather element request exceeds HCA capability
*	IB_UNSUPPORTED
*		Unreliable datagram not supported
*	IB_INVALID_PARAMETER
*		The parameter p_create_attr is invalid.
* NOTES
*	If any of the initial parameters is not valid, the queue pair is not
*	created. If the routine call is not successful then the contents of
*	qp_query_attr and qp_handle is undefined.
* SEE ALSO
*	ci_create_spl_qp, ci_query_qp, ci_modify_qp, ci_destroy_qp
******
*/

/****f* Verbs/ci_create_spl_qp
* NAME
*	ci_create_spl_qp -- Create a special queue pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_spl_qp) (
	IN		const	ib_pd_handle_t		h_pd,
	IN		const	uint8_t				port_num,
	IN		const	void				*qp_context,
	IN		const	ci_async_event_cb_t			pfn_async_event_cb,
	IN		const	ib_qp_create_t		*p_create_attr,
		OUT			ib_qp_attr_t		*p_qp_attr,
		OUT			ib_qp_handle_t		*ph_qp );
/*
* DESCRIPTION
*	Create and return a handle to for the indicated service type on the
*	specified port. QP service types can be one of SMI, GSI, Raw IPv6 or
*	Raw ether type as specified in qp_type_t.
* PARAMETERS
*	h_pd
*		[in] Handle to the PD on which the special queue pair is to be created.
*	port_num
*		[in] Port number for which this special queue pair is created.
*	qp_context
*		[in] User specified context passed during the async error callback
*		routine.
*	pfn_async_event_cb
*		[in] Asynchronous event handler.
*	p_create_attr
*		[in] Initial set of attributes with which the queue pair is to be
*		created.
*	p_qp_attr
*		[out] QP attributes after the qp is successfully created.
*
*	ph_qp
*		[out] Handle to the special qp after its creation.
* RETURN VALUE
*	IB_SUCCESS
*		The special queue pair of the requested service type is created.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy the request.
*	IB_NOT_FOUND
*		Indicated port guid is not found on this HCA.
*	IB_INVALID_CQ_HANDLE
*		Invalid cq handle passed to send/receive side.
*	IB_INVALID_MAX_WRS
*		Max WRS capacity exceeded
*	IB_INVALID_MAX_SGE
*		Max Scatter gather element request exceeds HCA capability
*	IB_RESOURCE_BUSY
*		Applicable to SMI/GSI qp's. This return code means that the SMI/GSI
*		QP is already allocated.
*	IB_INVALID_PD
*		Invalid protection domain supplied.
*	IB_INVALID_PORT
*		Invalid port number supplied.
*	IB_UNSUPPORTED
*		Raw datagram unsupported.
*	IB_INVALID_PARAMETER
*		The parameter p_create_attr is not valid.
* NOTES
*	This verb is privileged and only available in kernel mode. The User mode
*	clients that need access to SMI/GSI qp's is recommended to do this via
*	a higher level of abstraction.
* SEE ALSO
*	ci_create_qp, ci_query_qp, ci_modify_qp, ci_destroy_qp
******
*/

/****f* Verbs/ci_modify_qp
* NAME
*	ci_modify_qp -- Modify attributes of the specified QP.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_qp) (
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_qp_mod_t					*p_modify_attr,
		OUT			ib_qp_attr_t				*p_qp_attr OPTIONAL,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine is used to modify the qp states or other attributes of the
*	QP. On successful completion, the requested state transition is performed
*	and the QP is transitioned to the required state.
* PARAMETERS
*	h_qp
*		[in] Handle to the queue pair whose state is to be modified.
*	p_modify_attr
*		[in] Specifies what attributes need to be modified in the qp.
*	p_qp_attr
*		[out] QP attributes after the qp is successfully created.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful and the QP attributes are modified
*		to the requested state.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the requested operation.
*	IB_INVALID_QP_HANDLE
*		Invalid QP handle was passed.
*	IB_UNSUPPORTED
*		Requested operation is not supported, for e.g. Atomic operations.
*	IB_QP_INVALID_STATE
*		Invalid state transition request. Current QP state not in allowable
*		state.
*	IB_INVALID_PKEY
*		Pkey specified in modify request not valid entry in P_KEY table. Or
*		index is out of range.
*	IB_INVALID_APM_STATE
*		Invalid automatic path migration state specified in the request.
*	IB_INVALID_PARAMETER
*		The parameter p_modify_attr is not valid.
* NOTES
*	Refer to Table 79 in chapter 11, Volume 1 of the InfiniBand Specifications.
* SEE ALSO
*	ci_create_qp, ci_create_spl_qp, ci_query_qp
******
*/

/****f* Verbs/ci_ndi_modify_qp
* NAME
*	ci_ndi_modify_qp -- Modify attributes of the specified QP.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_ndi_modify_qp) (
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_qp_mod_t					*p_modify_attr,
		OUT			ib_qp_attr_t				*p_qp_attr OPTIONAL,
	IN		const	uint32_t					buf_size,
	IN	OUT			uint8_t* const				p_outbuf);
/*
* DESCRIPTION
*	This routine is used to modify the qp states or other attributes of the
*	QP. On successful completion, the requested state transition is performed
*	and the QP is transitioned to the required state.
* PARAMETERS
*	h_qp
*		[in] Handle to the queue pair whose state is to be modified.
*	p_modify_attr
*		[in] Specifies what attributes need to be modified in the qp.
*	p_qp_attr
*		[out] QP attributes after the qp is successfully created.
*	buf_size
*		[in] Size of the output buffer
*	p_outbuf
*		[in out] Output buffer
*
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful and the QP attributes are modified
*		to the requested state.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the requested operation.
*	IB_INVALID_QP_HANDLE
*		Invalid QP handle was passed.
*	IB_UNSUPPORTED
*		Requested operation is not supported, for e.g. Atomic operations.
*	IB_QP_INVALID_STATE
*		Invalid state transition request. Current QP state not in allowable
*		state.
*	IB_INVALID_PKEY
*		Pkey specified in modify request not valid entry in P_KEY table. Or
*		index is out of range.
*	IB_INVALID_APM_STATE
*		Invalid automatic path migration state specified in the request.
*	IB_INVALID_PARAMETER
*		The parameter p_modify_attr is not valid.
* NOTES
*	Refer to Table 79 in chapter 11, Volume 1 of the InfiniBand Specifications.
* SEE ALSO
*	ci_create_qp, ci_create_spl_qp, ci_query_qp
******
*/

/****f* Verbs/ci_query_qp
* NAME
*	ci_query_qp -- Query the current QP attributes
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_qp) (
	IN		const	ib_qp_handle_t				h_qp,
		OUT			ib_qp_attr_t* const			p_qp_attr,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine queries the current attributes for the QP
*	corresponding to h_qp. The attributes are returned in p_query_attr.
*	Depending on the current state of the QP, some of the fields in the
*	attribute structure may not be valid.
* PARAMETERS
*	h_qp
*		[in] Handle to the QP for which the attributes are being retrieved
*	p_qp_attr
*		[out] Pointer to the ib_qp_query_t structure where the current
*		attributes of the QP is returned.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The values returned in p_qp_attr are valid.
*	IB_INVALID_QP_HANDLE
*		The h_qp supplied is not a valid handle.
*	IB_INVALID_PARAMETER
*		Parameter p_qp_attr is not valid.
* SEE ALSO
*	ci_create_qp
*****
*/

/****f* Verbs/ci_destroy_qp
* NAME
*	ci_destroy_qp -- Destroy the specified Queue Pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_destroy_qp) (
	IN	const	ib_qp_handle_t		h_qp,
	IN	const	uint64_t			timewait );
/*
* DESCRIPTION
*	Destroys the associated QP. The QP could have outstanding work requests
*	when this call is made. Any outstanding work requests *SHALL NOT* be
*	completed after this routine returns.
* PARAMETERS
*	h_qp
*		[in] Handle to the qp that needs to be destroyed.
*	timewait
*		[in] Time (in microseconds) at which the QP should leave
*		the timewait state and can be reused.
*
* RETURN VALUE
*	IB_SUCCESS
*		The intend to destroy this queue pair is registered and no further
*		work requests will be processed. When no pending callbacks are in
*		progress, the destroy_callback function is invoked which marks the
*		destruction of the resource. The consumer can be guaranteed that
*		no future callbacks will be propagated on behalf of this resource.
*	IB_INVALID_QP_HANDLE
*		The handle passed is invalid.
*	IB_RESOURCE_BUSY
*		If the queue pair is a unreliable datagram service type, and
*		is still bound to a multicast group.
* NOTES
*	This call cannot be called from any of the notification functions invoked
*	by the Verbs driver. For e.g. the completion handler or the async error
*	callback provided during the ci_open_ca() call. The call will block until
*	all references to this adapter object is closed which includes all the
*	pending callbacks returning back to the verbs provider driver.
*
*	If the CQ associated with this QP is still not destroyed, the completions
*	on behalf of this QP can still be pulled via the ci_poll_cq() call. Any
*	resources allocated by the Channel Interface must be deallocated as part
*	of this call.
* SEE ALSO
*	ci_create_qp, ci_create_spl_qp
******
*/

/****f* Verbs/ci_create_cq
* NAME
*	ci_create_cq -- Create a completion queue (CQ) on the specified HCA.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_cq) (
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	void						*cq_context,
	IN		const	ci_async_event_cb_t			pfn_async_event_cb,
	IN				ci_completion_cb_t			completion_cb,
	IN	OUT			uint32_t* const				p_size,
		OUT			ib_cq_handle_t				*ph_cq,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	The consumer must specify the minimum number of entries in the CQ. The
*	exact number of entries the Channel Interface created is returned to the
*	client. If the requested number of entries is larger than what this
*	HCA can support, an error is returned.
* PARAMETERS
*	h_ca
*		[in] A handle to the open HCA
*	cq_context
*		[in] The context that is passed during the completion callbacks.
*	pfn_async_event_cb
*		[in] Asynchronous event handler.
*	completion_cb
*		[in] Callback for completion events
*	p_size
*		[in out] Points to a variable containing the number of CQ entries
*		requested by the consumer. On completion points to the size of the
*		CQ that was created by the provider.
*	ph_cq
*		[out] Handle to the newly created CQ on successful creation.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The operation was successful.
*	IB_INVALID_CA_HANDLE
*		The h_ca passed is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_CQ_SIZE
*		Requested CQ Size is not supported.
*	IB_INVALID_PARAMETER
*		one of the parameters was NULL.
* NOTES
*	The consumer would need a way to retrieve the cq_handle associated with
*	context being returned, so it can perform ci_poll_cq() to retrieve
*	completion queue entries. The handle as such is not being passed, since
*	there is no information in the handle that is visible to the consumer.
*	Passing a context directly would help avoid any reverse lookup that the
*	consumer would need to perform in order to identify it's own internal
*	data-structures	needed to process this completion completely.
* SEE ALSO
*	ci_destroy_cq, ci_query_cq, ci_resize_cq
******
*/

/****f* Verbs/ci_resize_cq
* NAME
*	ci_resize_cq -- Modify the maximum number of entries the CQ could hold.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_resize_cq) (
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			uint32_t* const				p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine allows the caller to modify the maximum number of entries
*	the CQ could hold. It is possible to resize the CQ while there are
*	entries in the CQ, and with outstanding work requests that will generate
*	completions. If the entries in the CQ are more than the new size which is
*	being created, an error is returned.
* PARAMETERS
*	h_cq
*		[in] Completion Queue handle
*	p_size
*		[in out] This parameter indicates the requested size of the CQ. On
*		successful completion, the current size allocated is returned in
*		this same parameter.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The resize operation was successful.
*	IB_INVALID_CQ_HANDLE
*		The CQ handle is invalid.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_PARAMETER
*		one of the parameters was NULL.
*	IB_INVALID_CQ_SIZE
*		Requested CQ Size is not supported.
*	IB_OVERFLOW
*		The CQ has more entries than the resize request. The CQ is not
*		modified, and old entries still exist.
* NOTES
*	If the consumer wishes to resize the CQ smaller than originally created,
*	it is recommended to retrieve all entries before performing a CQ resize
*	operation. It is left to the verb implementer on resize operations, to
*	actually reduce the entries, or leave it as it. The consumer must make no
*	assumptions on the successful completion, and should only rely on the
*	size returned in p_size.
* SEE ALSO
*	ci_create_cq
******
*/

/****f* Verbs/ci_query_cq
* NAME
*	ci_query_cq -- Query the number of entries configured for the CQ.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_cq) (
	IN		const	ib_cq_handle_t				h_cq,
		OUT			uint32_t					*p_size,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine returns the maximum number of entries this completion
*	queue is configured.
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue
*	p_size
*		[out] The number of entries the completion queue is configured to hold
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The call completed successfully, and the returned values are valid
*	IB_INVALID_CQ_HANDLE
*		The cq_handle passed is invalid.
*	IB_INVALID_PARAMETER
*		one of the parameters was NULL.
* SEE ALSO
*	ci_create_cq, ci_resize_cq
******
*/

/****f* Verbs/ci_destroy_cq
* NAME
*	ci_destroy_cq -- Destroy a completion queue.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_destroy_cq) (
	IN	const	ib_cq_handle_t		h_cq );
/*
* DESCRIPTION
*	Destroys a completion queue. If any queue pairs are still bound
*	to this CQ, the attempt to destroy will fail, and the CQ and associated
*	resources are *NOT* destroyed.
* PARAMETERS
*	cq_handle
*		[in] Handle to the cq that is to be destroyed.
* RETURN VALUE
*	IB_SUCCESS
*		The intend to destroy the completion queue is registered successfully.
*		The destroy_callback function will be invoked when it is safe and
*		guarantees that no more completion callbacks will be invoked for
*		this CQ. Any pending CQ notifications are discarded.
*	IB_INVALID_CQ_HANDLE
*		The CQ handle is invalid.
*	IB_RESOURCE_BUSY
*		Queue pairs may still be bound to this completion queue.
*	IB_INVALID_PARAMETER
*		one of the parameters was NULL.
* SEE ALSO
*	ci_create_cq
* NOTES
*	This call cannot be called from any of the notification functions invoked
*	by the Verbs driver. For e.g. the completion handler or the async error
*	callback provided during the ci_open_ca() call. The call will block until
*	all references to this adapter object is closed which includes all the
*	pending callbacks returning back to the verbs provider driver.
******
*/



/****f* Verbs/ci_register_mr
* NAME
*	ci_register_mr -- Register a memory region with the HCA.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_register_mr) (
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_mr_create_t* const		p_mr_create,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t *	const		ph_mr,
	IN				boolean_t					um_call );
/*
* DESCRIPTION
*	This routine registers a virtually contiguous region of memory with the
*	HCA. All memory regions that need to be used by the HCA must be registered
*	prior to use in data transfer operations. On successful completion
*	the region handle, lkey are returned. If remote access rights are specified
*	then the rkey is also returned.
* PARAMETERS
*	h_pd
*		[in] Handle to the PD on which memory is being registered
*	p_mr_create
*		[in] Holds attributes for the region being registered. Look at
*		ib_mr_create_t for more details.
*	p_lkey
*		[out] Local Key Attributes of the registered memory region
*	p_rkey
*		[out] Remote key of the registered memory region. The verbs provider
*		is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	ph_mr
*		[out] Handle to the registered memory region. This handle is used when
*		submitting work requests to refer to this region of memory.
*	um_call
*		[in] Boolean indicating whether the registration originated in user-mode.
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
*	IB_INVALID_PD_HANDLE
*		Invalid mr_pdhandle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
* NOTES
*	In addition to registration, the routine also pins memory, so that the
*	physical page associated with the virtual address does not get swapped
*	out during the time the HCA is attempting to transfer data to this
*	address. If the memory is not pinned, this could lead to data-corruption
*	and unpredictable behavior by the operating environment.
*
* SEE ALSO
*	ci_deregister_mr, ci_query_mr, ci_register_pmr, ci_modify_mr,
*	ci_register_smr
******
*/

/****f* Verbs/ci_register_pmr
* NAME
*	ci_register_pmr -- Register a physical memory region with the HCA.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_register_pmr) (
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_phys_create_t*const		p_pmr_create,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr,
	IN				boolean_t					um_call );
/*
* DESCRIPTION
*	This routine registers an array of physical pages as a single virtually
*	contiguous region with the HCA. All memory regions that need to be used by
*	the HCA must be registered prior to use in data transfer operations.
*	On successful completion the region handle, lkey and rkey used for
*	local and remote access authentication are returned.
* PARAMETERS
*	h_pd
*		[in] Handle to the PD on which memory is being registered
*	p_pmr_create
*		[in] Holds attributes for the region being registered.
*	p_vaddr
*		[in/out] On input, references the requested virtual address for the
*		start of the physical region.  On output, references the actual
*		virtual address assigned to the registered region.
*	p_lkey
*		[out] Local key of the registered memory region
*	p_rkey
*		[out] Remote key of the registered memory region.The verbs provider
*		is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	ph_mr
*		[out] Handle to the registered memory region. This handle is used when
*		submitting work requests to refer to this region of memory.
*	um_call
*		[in] Boolean indicating whether the registration originated in user-mode.
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		Invalid length or address in p_mr_create. Also returned if the page_size
*		passed is not one of supported page sizes by the HCA.
*	IB_INVALID_PD_HANDLE
*		Invalid mr_pdhandle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
* PORTABILITY
*	Kernel Mode only
* NOTES
*	Remote and Atomic access settings in ib_access_ctrl_t, requires local
*	write access to be enabled, otherwise IB_INVALID_PERMISSION is returned.
*	The p_vaddr returned could be different from the vaddr specified in
*	p_pmr_create.  If the requested virtual addr offset in a page does not
*	match, the channel interface is free to pick and assign a pseudo virtual
*	address. The address constructed is not a system virtual address, and only
*	meaningful to the adapter to which this registration is targeted.
* SEE ALSO
*	ci_deregister_mr, ci_query_mr, ci_register_mr, ci_modify_mr,
*	ci_register_smr
******
*/

/****f* Verbs/ci_query_mr
* NAME
*	ci_query_mr -- Query attributes of a registered memory region
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_mr) (
	IN		const	ib_mr_handle_t			h_mr,
		OUT			ib_mr_attr_t*	const	p_mr_query );
/*
* DESCRIPTION
*	This routine retrieves the memory region attributed of a
*	registered memory region. The memory handle could have been
*	obtained via ci_register_mr or ci_register_pmr.
* PARAMETERS
*	h_mr
*		[in] Memory handle for which the attributes need to be retrieved.
*	p_mr_query
*		[out] Attributes of the memory region associated with memory handle.
* RETURN VALUE
*	IB_SUCCESS
*		The routine completed successfully and attributes returned
*		are valid.
*	IB_INVALID_MR_HANDLE
*		The memory handle is not valid.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
* NOTES
*	Invalid handle checks are a mere signature checks in kernel mode.
*	Drivers in kernel are expected to be good corporate citizens.
*	In user mode, proper ownership is determined before passing handles
*	down to kernel to protect from rogue applications.
* SEE ALSO
*	ci_register_mr, ci_register_pmr
******
*/

/****f* Verbs/ci_modify_mr
* NAME
*	ci_modify_mr -- Modify some or all parameters of a memory region.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_mr) (
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_modify_mask,
	IN		const	ib_mr_create_t* const		p_mr_create OPTIONAL,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL,
	IN				boolean_t					um_call );
/*
* DESCRIPTION
*	This routine modifies attributes of the specified memory region
*	irrespective of whether the handle was obtained via ci_register_mr
*	or ci_register_pmr. This verb conceptually performs a de-registration
*	followed by a ci_register_mr.
* PARAMETERS
*	h_mr
*		[in] Handle to the memory region whose attributes are to be modified.
*	mr_modify_mask
*		[in] Command specifying which parts of the mem_region is valid. The
*		command is specified as a bit mask.
*	p_mr_create
*		[in] Desired attributes that need to be modified for mem_handle.
*		This is an optional parameter which can be NULL if mr_modify_mask
*		is set to IB_MR_MOD_PD.
*	p_lkey
*		[out] The new l_key for this newly registered memory region.
*	p_rkey
*		[out] The new r_key for this newly registered memory region.
*		The verbs provider is required to give this in the expected ordering
*		on the wire. When rkey's are exchanged between remote nodes, no
*		swapping of this data will be performed.
*	h_pd
*		[in] This parameter is valid only if the IB_MR_MOD_PD flag is set
*		in the mr_modify_req parameter. This field supplies the new
*		protection domain to which the modified region should be
*		associated with.
*	um_call
*		[in] Boolean indicating whether the registration originated in user-mode.
* RETURN VALUE
*	IB_SUCCESS
*		The modify memory region request completed successfully.
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the request.
*	IB_INVALID_MR_HANDLE
*		The memory handle supplied is not a valid memory region handle.
*	IB_INVALID_PERMISSION
*		Invalid access rights specified.
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*	IB_INVALID_PD_HANDLE
*		Protection domain handle supplied is not valid.
* NOTES
*	Remote and Atomic access settings in ib_access_ctrl_t, requires local
*	write access to be enabled.
*	TBD: How to handle shared memory region being passed to modify_mem?
* SEE ALSO
*	ci_register_mr, ci_register_pmr, ci_register_smr
*******
*/

/****f* Verbs/ci_modify_pmr
* NAME
*	ci_modify_pmr -- Modify some or all parameters of a memory region.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_modify_pmr) (
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_modify_mask,
	IN		const	ib_phys_create_t* const		p_pmr_create,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL,
	IN				boolean_t					um_call );
/*
* DESCRIPTION
*	This routine modifies attributes of the specified memory region
*	irrespective of whether the handle was obtained via ci_register_mr
*	or ci_register_pmr. This verb conceptually performs a de-registration
*	followed by a ci_register_pmr.
* PARAMETERS
*	h_mr
*		[in] Handle to the memory region whose attributes are to be modified.
*	mr_modify_mask
*		[in] Command specifying which parts of the mem_region is valid. The
*		command is specified as a bit mask.
*	p_pmr_create
*		[in] Desired attributes that need to be modified for mem_handle.
*	p_vaddr
*		[in/out] On input, references the requested virtual address for the
*		start of the physical region.  On output, references the actual
*		virtual address assigned to the registered region.
*	p_lkey
*		[out] The new l_key for this newly registered physical memory region.
*	p_rkey
*		[out] The new r_key for this newly registered physical memory region.
*		VPD is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	h_pd
*		[in] This parameter is valid only if the IB_MR_MOD_PD flag is set
*		in the mr_modify_req parameter. This field supplies the new
*		protection domain to which the modified region should be
*		associated with.
*	um_call
*		[in] Boolean indicating whether the registration originated in user-mode.
* RETURN VALUE
*	IB_SUCCESS
*		The modify memory region request completed successfully.
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete the request.
*	IB_INVALID_MR_HANDLE
*		The memory handle supplied is not a valid memory region handle.
*	IB_INVALID_PERMISSION
*		Invalid access rights specified.
*	IB_INVALID_PARAMETER
*		A reference to the virtual address, lkey, rkey was not provided or
*		the specified modify mask is invalid.
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
* PORTABILITY
*	Kernel Mode only
* NOTES
*	Remote and Atomic access settings in ib_access_ctrl_t, requires local
*	write access to be enabled.
* SEE ALSO
*	ci_register_mr, ci_register_pmr, ci_register_smr
*********
*/

/****f* Verbs/ci_register_smr
* NAME
*	ci_register_smr -- Register a memory region using same physical pages as
*                      an existing memory region.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_register_smr) (
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_access_t					access_ctrl,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr,
	IN				boolean_t					um_call );
/*
* DESCRIPTION
*	This routine registers a new memory region but shares the same set of
*	physical pages associated with memory handle. For user mode applications
*	the process *must* be owning memory handle for this call to be successful.
* PARAMETERS
*	h_mr
*		[in] Handle to memory region whose physical pages are being registered
*		by this shared registration.
*	h_pd
*		[in] Handle to the PD on which memory is being registered
*	access_ctrl
*		[in] Memory access restrictions on the registered memory.
*	p_vaddr
*		[in/out] On input, references the requested virtual address for the
*		start of the physical region.  On output, references the actual
*		virtual address assigned to the registered region.
*	p_lkey
*		[out] L_KEY for this memory region.
*	p_rkey
*		[out] R_KEY for this memory region. This is valid only when remote
*		access is enabled for this region. The verbs provider
*		is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	ph_mr
*		[out] Handle to the registered memory region. This handle is used when
*		submitting work requests to refer to this region of memory.
*	um_call
*		[in] Boolean indicating whether the registration originated in user-mode.
* RETURN VALUE
*	IB_SUCCESS
*		The call is successful and a new region handle returned is valid.
*	IB_INVALID_MR_HANDLE
*		mr_handle is invalid.
*	IB_INVALID_PD_HANDLE
*		mr_pdhandle supplied is invalid.
*	IB_INVALID_PERMISSION
*		Invalid access rights passed in mr_access.
* NOTES
*	ISSUE: how to deal with ci_deregister_mr, ci_modify_mr, ci_modify_pmr
*	should we treat them as memory windows and fail those if a shared region
*	was registered?
* SEE ALSO
*	ci_register_mr, ci_register_pmr, ci_modify_mr, ci_modify_pmr
******
*/

/****f* Verbs/ci_deregister_mr
* NAME
*	ci_deregister_mr -- Deregister a memory region
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_deregister_mr) (
	IN	const	ib_mr_handle_t		h_mr );
/*
* DESCRIPTION
*	This routine deregisters  a memory region from the HCA. The region can
*	de-registered only if there are no existing memory windows bound to
*	this region, and if no existing shared memory regions were registered
*	that refers to the same set of physical pages associated with the memory
*	handle.  If there are outstanding work requests referring to this memory
*	region, then after this call is successful, those work requests will
*	complete with WRS_LOCAL_PROTECTION_ERR.
* PARAMETERS
*	h_mr
*		[in] Memory handle that is being de-registered.
* RETURN VALUE
*	IB_SUCCESS
*		The memory de-registration was successful
*	IB_INVALID_MR_HANDLE
*		The memory handle supplied is not a valid memory handle.
*	IB_RESOURCE_BUSY
*		The memory region has active windows bound.
* NOTES
* SEE ALSO
*	ci_register_mr, ci_register_pmr, ci_register_smr
******
*/


#ifdef CL_KERNEL
/****f* Verbs/ci_alloc_mlnx_fmr
* NAME
*	ci_alloc_mlnx_fmr -- Allocate a Mellanox fast memory region with the HCA.
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_alloc_mlnx_fmr) (
	IN		const	ib_pd_handle_t				h_pd,
	IN				mlnx_fmr_create_t const		*p_fmr_ctreate,
		OUT			mlnx_fmr_handle_t* const	ph_fmr);
/*
* DESCRIPTION
*	//TODO 
* PARAMETERS
*	h_pd
*		[in] Handle to the PD on which fast memory is being registered
*	mr_access_flags
*		[in] mask of the access rights to the memory region
*	p_fmr_attr
*		[in] attribute of this fmr
*	ph_fmr
*		[out] Handle to the fast memory region. This handle is used when
*		mapin/unmaping fmr
* 
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
*	IB_INVALID_PD_HANDLE
*		Invalid pd handle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
*
* NOTES
*	The Alloc operation does not map nor pinned any memory.
*	In order to use the FMR the user need to call map
*
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	ci_dealloc_mlnx_fmr, ci_map_phys_mlnx_fmr, ci_unmap_mlnx_fmr
******
*/


/****f* Verbs/ci_map_phys_mlnx_fmr
* NAME
*	ci_map_phys_mlnx_fmr -- Map a Mellanox fast memory region with a
*	given page list.
*
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_map_phys_mlnx_fmr) (
	IN		const	mlnx_fmr_handle_t			h_fmr,
	IN		const	uint64_t* const				paddr_list,
	IN		const	int							list_len,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey);
/*
* DESCRIPTION
*	//TODO
*
* PARAMETERS
*	h_fmr
*		[in] Handle to the fast memory region that  these pages map to 
*	page_list
*		[in] array of phys address
*	list_len
*		[in] number of pages in the list
*	p_vaddr
*		[in/out] On input, references the requested virtual address for the
*		start of the FMR.  On output, references the actual
*		virtual address assigned to the FMR.
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing
*		RDMA or atomic operations to this registered memory region.
*
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
*	IB_INVALID_PD_HANDLE
*		Invalid pd handle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
*
* NOTES
*	The Alloc operation does not map nor pinned any memory.
*	In order to use the FMR the user need to call map 
*
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	ci_dealloc_mlnx_fmr, ci_alloc_mlnx_fmr, ci_unmap_mlnx_fmr
******
*/


/****f* Verbs/ci_unmap_mlnx_fmr
* NAME
*	ci_unmap_mlnx_fmr -- UnMap a Mellanox fast memory region.
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_unmap_mlnx_fmr) (
	IN		const	mlnx_fmr_handle_t			*ph_fmr);
/*
* DESCRIPTION
*	//TODO
*
* PARAMETERS
*	h_fmr
*		[in] Handle to the fast memory region that  these pages map to 
*
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
*	IB_INVALID_PD_HANDLE
*		Invalid pd handle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
*
* NOTES
*	The Alloc operation does not map nor pinned any memory.
*	In order to use the FMR the user need to call map 
*
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	ci_dealloc_mlnx_fmr, ci_alloc_mlnx_fmr, ci_map_phy_mlnx_fmr
******
*/


/****f* Verbs/ci_dealloc_mlnx_fmr
* NAME
*	ci_dealloc_mlnx_fmr -- Deallocate a Mellanox fast memory region.
*
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_dealloc_mlnx_fmr) (
	IN			mlnx_fmr_handle_t	const		h_fmr);
/*
* DESCRIPTION
*	//TODO
*
* PARAMETERS
*	h_fmr
*		[in] Handle to the fast memory region. This handle is used when
*		mapin/unmaping fmr
*
* RETURN VALUE
*	IB_SUCCESS
*		Registration with the adapter was successful.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to satisfy request.
*	IB_INVALID_PARAMETER
*		One of the input pointers was NULL.
*	IB_INVALID_PD_HANDLE
*		Invalid pd handle
*	IB_INVALID_PERMISSION
*		Invalid access rights.
*
* NOTES
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	ci_dealloc_mlnx_fmr, ci_map_phys_mlnx_fmr, ci_unmap_mlnx_fmr
******
*/
#endif


/****f* Verbs/ci_create_mw
* NAME
*	ci_create_mw -- Create a memory window entry for later use
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_create_mw) (
	IN		const	ib_pd_handle_t				h_pd,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mw_handle_t				*ph_mw,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine allocates a memory window. This window entry cannot be used
*	for remote access unless this window is bound to a memory region
*	via the ci_bind_mw call.
* PARAMETERS
*	h_pd
*		[in] Protection domain handle to use for this memory window
*	p_rkey
*		[out] Remote access key that can be exchanged with a remote node to
*		perform RDMA transactions on this memory window. This R_KEY is still not
*		bound to any memory regions, until a successful call to ci_bind_mw.
*		VPD is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	ph_mw
*		[out] Handle to the newly created memory window.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The memory window allocation completed successfully.
*	IB_INSUFFICIENT_RESOURCES
*		Not enough resources to complete the request.
*	IB_INVALID_PD_HANDLE
*		pd_handle supplied is invalid.
*	IB_INVALID_PARAMETER
*		One of the pointers was not valid.
* SEE ALSO
*	ci_destroy_mw, ci_query_mw, ci_bind_mw
******
*/

/****f* Verbs/ci_query_mw
* NAME
*	ci_query_mw -- Query memory window attributes for memory window handle
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_query_mw) (
	IN		const	ib_mw_handle_t				h_mw,
		OUT			ib_pd_handle_t				*ph_pd,
		OUT			net32_t* const				p_rkey,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine retrieves the current R_KEY and protection domain
*	handle associated with this mw_handle.
* PARAMETERS
*	h_mw
*		[in] Memory window handle whose attributes are being retrieved.
*	ph_pd
*		[out] Protection domain handle associated with this mw_handle
*	p_rkey
*		[out] Current R_KEY associated with this mw_handle.The verbs provider
*		is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
* RETURN VALUE
*	IB_SUCCESS
*		The query operation completed successfully.
*	IB_INVALID_MW_HANDLE
*		mw_handle supplied is an invalid handle
*	IB_INVALID_PARAMETER
*		One of the pointers was not valid.
* SEE ALSO
*	ci_create_mw, ci_bind_mw
******
*/

/****f* Verbs/ci_bind_mw
* NAME
*	ci_bind_mw -- Bind a memory window to a memory region.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_bind_mw) (
	IN		const	ib_mw_handle_t				h_mw,
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_bind_wr_t* const			p_mw_bind,
		OUT			net32_t* const				p_rkey );
/*
* DESCRIPTION
*	This routine posts a request to bind a memory window to a registered
*	memory region. If the queue pair was created with selectable signaling,
*	once the operation is completed successfully then a completion queue entry
*	is generated indicating the bind operation has completed. The IB_POST_FENCE
*	option could be specified to cause the requestor to wait until outstanding
*	RDMA operations can be completed.
* PARAMETERS
*	h_mw
*		[in] Handle to memory window that needs to be bound to a memory region.
*	h_qp
*		[in] Queue Pair to which this bind request is to be posted.
*	p_mw_bind
*		[in] Input parameters for this bind request, consisting of virtual
*		addr range of bind request etc.
*	p_rkey
*		[out] On successful completion, the new R_KEY is returned.
*		VPD is required to give this in the expected ordering on the wire. When
*		rkey's are exchanged between remote nodes, no swapping of this data
*		will be performed.
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
*		One of the pointers was not valid.
*	IB_INVALID_RKEY
*		R_KEY specified is invalid for the memory region being bound.
*	IB_INVALID_QP_HANDLE
*		h_qp supplied was an invalid QP handle.
* NOTES
*	- A previously bound memory window can be bound to the same or different
*	memory region.
*
*	- A bind operation with length of 0, invalidates any previous binding
*	and returns an R_KEY in the unbound state.
* SEE ALSO
*	ci_create_mw
******
*/

/****f* Verbs/ci_destroy_mw
* NAME
*	ci_destroy_mw -- Destroy a memory window.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_destroy_mw) (
	IN	const	ib_mw_handle_t		h_mw );
/*
* DESCRIPTION
*	This routine deallocates a window entry created via a ci_create_mw.
*	Once this operation is complete, the channel interface guarantees that
*	no future remote accesses will be permitted to this window entry.
* PARAMETERS
*	h_mw
*		[in] Handle to the memory window that is being destroyed.
* RETURN VALUE
*	IB_SUCCESS
*		The destroy window request completed successfully.
*	IB_INVALID_MW_HANDLE
*		memw_handle supplied is invalid.
* NOTES
*	Deallocate memory window implicitly means the window is also unbound
*	once the call completes successfully. Any future remote access with
*	the same R_KEY should fail with protection violation.
* SEE ALSO
*	ci_create_mw
******
*/

/****f* Verbs/ci_post_send
* NAME
*	ci_post_send -- Post a work request to the send side of a queue pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_post_send) (
	IN		const	ib_qp_handle_t			h_qp,
	IN				ib_send_wr_t*	const	p_send_wr,
		OUT			ib_send_wr_t			**pp_failed );
/*
* DESCRIPTION
*	This routine posts a work request to the send side of the queue pair.
*	The different types of work request that can be posted are explained in
*	the ib_wr_t structure. For exact details on ordering rules please consult
*	the Volume 1, of the InfiniBand Specifications. If there is more
*	outstanding requests posted that what the queue is configured for, an
*	immediate error is returned.
* PARAMETERS
*	h_qp
*		[in] The queue pair to which this work request is being submitted.
*	p_send_wr
*		[in] A pointer to the head of the list that must be posted to the
*		Send Queue.
*	pp_failed
*		[out] A pointer to the head of the list that holds the failed WRs.
*		If all the entries provided are posted with the CI, then this parameter
*		would be set to NULL.
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
* NOTES
*	Please refer to Table 81 and Table 82 for allowed operation types
*	on different types of queue pairs, and the different modifiers
*	acceptable for the work request for different QP service types.
* SEE ALSO
*	ci_post_recv, ci_poll_cq
******
*/

/****f* Verbs/ci_post_srq_recv
* NAME
*	ci_post_srq_recv -- Post a work request to the receive queue of a queue pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_post_srq_recv) (
	IN		const	ib_srq_handle_t			h_srq,
	IN				ib_recv_wr_t*	const	p_recv_wr,
		OUT			ib_recv_wr_t			**pp_failed );
/*
* DESCRIPTION
*	This routine allows to queue a work request to the receive side of a
*	shared queue pair. The work_req holds necessary data to satisfy an incoming
*	receive message. If an attempt is made to queue more work requests than
*	what is available, an error is returned.
* PARAMETERS
*	h_srq
*		[in] Handle to the queue pair to which the receive work request is being
*		posted.
*	p_recv_wr
*		[in] Holds the WRs to be posted to the receive queue.
*	pp_failed
*		[out] If any entry could not be posted with the CI, then this points
*		to the first WR that completed unsuccessfully. If all entries are
*		posted, then this field is set to NULL on successful exit.
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
* SEE ALSO
******
*/



/****f* Verbs/ci_post_recv
* NAME
*	ci_post_recv -- Post a work request to the receive queue of a queue pair.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_post_recv) (
	IN		const	ib_qp_handle_t			h_qp,
	IN				ib_recv_wr_t*	const	p_recv_wr,
		OUT			ib_recv_wr_t			**pp_failed );
/*
* DESCRIPTION
*	This routine allows to queue a work request to the receive side of a
*	queue pair. The work_req holds necessary data to satisfy an incoming
*	receive message. If an attempt is made to queue more work requests than
*	what is available, an error is returned.
* PARAMETERS
*	h_qp
*		[in] Handle to the queue pair to which the receive work request is being
*		posted.
*	p_recv_wr
*		[in] Holds the WRs to be posted to the receive queue.
*	pp_failed
*		[out] If any entry could not be posted with the CI, then this points
*		to the first WR that completed unsuccessfully. If all entries are
*		posted, then this field is set to NULL on successful exit.
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
* SEE ALSO
*	ci_post_send, ci_poll_cq.
******
*/

/****f* Verbs/ci_peek_cq
* NAME
*	ci_peek_cq
*
* DESCRIPTION
*	Returns the number of entries currently on the completion queue.
*
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_peek_cq) (
	IN		const	ib_cq_handle_t				h_cq,
	OUT				uint32_t* const				p_n_cqes );
/*
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue to peek.
*
*	p_n_cqes
*		[out] The number of completion entries on the CQ.
*
* RETURN VALUES
*	IB_SUCCESS
*		The peek operation completed successfully.
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*	IB_INVALID_PARAMETER
*		A reference to the completion queue entry count was not provided.
*	IB_UNSUPPORTED
*		This operation is not supported by the channel adapter.
*
* NOTES
*	The value returned is a snapshot of the number of compleiton queue
*	entries curently on the completion queue.  Support for this operation
*	is optional by a channel adapter vendor.
*
* SEE ALSO
*	ci_create_cq, ci_poll_cq, ci_enable_cq_notify, ci_enable_ncomp_cq_notify
*****/

/****f* Verbs/ci_poll_cq
* NAME
*	ci_poll_cq -- Retrieve a work completion record from a completion queue
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_poll_cq) (
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			ib_wc_t** const				pp_free_wclist,
		OUT			ib_wc_t** const				pp_done_wclist );
/*
* DESCRIPTION
*	This routine retrieves a work completion entry from the specified
*	completion queue. The contents of the data returned in a work completion
*	is specified in ib_wc_t.
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue being polled.
*	pp_free_wclist
*		[in out] A list of work request structures provided by the consumer
*		for the channel interface to return completed Completion Queue
*		entries.  If not all the entries are consumed, this list holds the
*		list of un-utilized completion entries provided back to the consumer.
*	pp_done_wclist
*		[out] A list of work completions retrieved from the completion queue
*		and successfully processed.
* RETURN VALUE
*	IB_SUCCESS
*		Poll completed successfully and found one or more entries. If on
*		completion the pp_free_wclist is empty, then there are potentially more
*		entries and the consumer must continue to retrieve entries.
*	IB_INVALID_CQ_HANDLE
*		The cq_handle supplied is not valid.
*	IB_NOT_FOUND
*		There were no completion entries found in the specified CQ.
* SEE ALSO
*	ci_create_cq, ci_post_send, ci_post_recv, ci_bind_mw
******
*/

/****f* Verbs/ci_enable_cq_notify
* NAME
*	ci_enable_cq_notify -- Invoke the Completion handler, on next entry added.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_enable_cq_notify) (
	IN	const	ib_cq_handle_t		h_cq,
	IN	const	boolean_t			solicited );
/*
* DESCRIPTION
*	This routine instructs the channel interface to invoke the completion
*	handler when the next completion queue entry is added to this CQ.
*	Please refer to Volume 1, of the InfiniBand specification for a complete
*	description.
* PARAMETERS
*	h_cq
*		[in] Handle to the CQ on which the notification is being enabled.
*	solicited
*		[in] A boolean flag indicating whether the request is to generate a
*		notification on the next entry or on the next solicited entry
*		being added to the completion queue.
* RETURN VALUE
*	IB_SUCCESS
*		The notification request was registered successfully.
*	IB_INVALID_CQ_HANDLE
*		cq_handle supplied is not a valid handle.
* NOTES
*	The consumer cannot call a request for notification without emptying
*	entries from the CQ. i.e if a consumer registers for a notification
*	request in the completion callback before pulling entries from the
*	CQ via ci_poll_cq, the notification is not generated for completions
*	already in the CQ. For e.g. in the example below, if there are no calls
*   to ci_poll_cq()	after the ci_enable_cq_notify(). For any CQ entries added
*	before calling this ci_enable_cq_notify() call, the consumer does not
*	get a completion notification callback. In order to comply with the verb
*	spec, consumer is supposed to perform a ci_poll_cq() after the
*	ci_enable_cq_notify() is made to retrive any entries that might have
*	been added to the CQ before the CI registers the notification enable.
*
*			while ((ret_val = ci_poll_cq(cq_handle, &free_list, &done_list)
*				== FSUCCESS))
*			{
*				process entries;
*			}
*			if (ret_val == IB_NOT_FOUND)
*				ci_enable_cq_notify(cq_handle);
*			// Need to perform a ci_poll_cq()
*           // after the enable.
* SEE ALSO
*	ci_create_cq, ci_peek_cq, ci_poll_cq, ci_enable_ncomp_cq_notify
******
*/

/****f* Verbs/ci_enable_ncomp_cq_notify
* NAME
*	ci_enable_ncomp_cq_notify -- Invoke the Completion handler when the next
*	N completions are added.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_enable_ncomp_cq_notify) (
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	uint32_t					n_cqes );
/*
* DESCRIPTION
*	This routine instructs the channel interface to invoke the completion
*	handler when the next N completions have been added to this CQ.
* PARAMETERS
*	h_cq
*		[in] Handle to the CQ on which the notification is being enabled.
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
*	IB_UNSUPPORTED
*		This operation is not supported by the channel adapter.
*
* NOTES
*	This routine instructs the channel interface to invoke the completion
*	handler when the next N completions have been added to this CQ regardless
*	of the completion type (solicited or unsolicited).  Any CQ entries that
*	existed before the rearm is enabled will not result in a call to the
*	handler.  Support for this operation is optional by a channel adapter
*	vendor.
*
* SEE ALSO
*	ci_create_cq, ci_peek_cq, ci_poll_cq, ci_enable_cq_notify
******
*/

/****f* Verbs/ci_attach_mcast
* NAME
*	ci_attach_mcast -- Attach a queue pair to a multicast group
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_attach_mcast) (
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_gid_t					*p_mcast_gid,
	IN		const	ib_net16_t					mcast_lid,
		OUT			ib_mcast_handle_t			*ph_mcast,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* DESCRIPTION
*	This routine attaches the given qp_handle to a multicast gid as specified
*	by mcast_gid parameter.
* PARAMETERS
*	h_qp
*		[in] Queue pair handle which needs to be added to the multicast group
*		on the adapter.
*	mcast_lid
*		[in] The multicast group LID value.
*	p_mcast_gid
*		[in] IPv6 address associated with this multicast group.
*	ph_mcast
*		[out] Multicast handle holding the association of this queue pair
*		to the multicast group.
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
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
*	IB_INVALID_LID
*		The supplied lid is not a valid multicast lid.
*	IB_UNSUPPORTED
*		Multicast is not supported by this HCA.
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient resources to complete request.
*	IB_INVALID_PARAMETER
*		One of the parameters was NULL.
* SEE ALSO
*	ci_create_qp, ci_detach_mcast
******
*/


/****f* Verbs/ci_detach_mcast
* NAME
*	ci_detach_mcast -- Detach a queue pair from a multicast group
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_detach_mcast) (
	IN	const	ib_mcast_handle_t		h_mcast );
/*
* DESCRIPTION
*	This routine detaches a queue pair from its previously associated multicast
*	group.
* PARAMETERS
*	h_mcast
*		[in] The multicast handle passed back to consumer after the
*		ci_mcast_attach call.
* RETURN VALUE
*	IB_SUCCESS
*		The qp was successfully detached from the multicast group.
*	IB_INVALID_MCAST_HANDLE
*		mcast_handle supplied is an invalid handle
*	IB_INVALID_PARAMETER
*		One of the parameters was NULL.
* SEE ALSO
*	ci_attach_mcast
******
*/

/****f* Verbs/ci_local_mad
* NAME
*	ci_local_mad -- Request a mad to be processed by the local adapter.
* SYNOPSIS
*/

typedef ib_api_status_t
(*ci_local_mad) (
	IN		const	ib_ca_handle_t		h_ca,
	IN		const	uint8_t				port_num,
	IN		const	ib_av_attr_t			*p_src_av_attr,
	IN		const	ib_mad_t			*p_mad_in,
	OUT			ib_mad_t			*p_mad_out );
/*
* DESCRIPTION
*	This routine is OPTIONAL for the channel interface. This is required
*	for adapters which do not have the agents such as Subnet Management
*	agent (SMA) Or the GSA in the Verbs Provider driver.
*	hardware, for all such adapters the exact queue pair management of
*	special queue pairs happen above the channel interface. This routine
*	is used to perform local operations, since there is no agent below the
*	channel interface. For e.g: If a Subnet Management packet (SMP) to
*	set PORT_STATE is received, this reception is processed above the channel
*	interface, then this call is done to set the port state on the local
*	adapter. On successful return, the response is generated and sent to the
*	Subnet Manager.
* PARAMETERS
*	h_ca
*		[in] A handle to the channel adapter that should process the MAD.
*		This must be the same adapter that the MAD was received on.
*	port_num
*		[in] port number to which this request is directed is to be sent.
*	p_mad_in
*		[in] pointer to a management datagram (MAD) structure containing
*		the command to be processed.
*	p_mad_out
*		[out] Response packet after processing the command. The storage to this
*		must be allocated by the consumer.
* RETURN VALUE
*	IB_SUCCESS
*		Command processed successfully.
*	IB_INVALID_CA_HANDLE
*		The HCA handle supplied is not valid.
*	IB_INVALID_PORT
*		The port number supplied is invalid.
* PORTABILITY
*	Kernel Mode only
* NOTES
*	This call is provided to aid adapters that don't have a agent functionality
*	built in the channel interface. Some adapters do have a local processor
*	to process these packets, hence even for local port management, we can
*	use the same mechanism we use to configure external nodes by using a
*	hop counter = 1 in the MAD packets. If the SMA indicates it has a local
*	sma in the ib_ca_attr_t, then the packets are posted to the adapter
*	instead of making a private call to the adapter.
* SEE ALSO
*	ci_query_ca, ci_ca_attr_t
******
*/


/****f* Verbs/ci_vendor_call
* NAME
*	ci_vendor_call
*
* DESCRIPTION
*	Performs a vendor specific CA interface function call.
*
* SYNOPSIS
*/
typedef ib_api_status_t
(*ci_vendor_call)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	void** const				handle_array	OPTIONAL,
	IN				uint32_t					num_handles,
	IN				ib_ci_op_t* const			p_ci_op,
	IN	OUT			ci_umv_buf_t				*p_umv_buf OPTIONAL );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an opened CA.
*
*	handle_array
*		[in] This parameter references an array containing handles of
*		existing CA resources.  This array should contain all of the
*		handles specified in the vendor specific data provided with this
*		call.  All handles specified through this array are validated by
*		the verbs provider driver to ensure that the number and type of
*		handles are correct for the requested operation.
*
*	num_handles
*		[in] The number of the handles in handle array.  This count is
*		verified by the access layer.
*
*	p_ci_op
*		[in] A reference to the vendor specific CA interface data
*		structure containing the operation parameters.
*
*	p_umv_buf
*		[in out] Vendor specific parameter to support user mode IO.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_CA_HANDLE
*		The CA handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the vendor specific data was not provided.
*
*	IB_INVALID_HANDLE
*		A handle specified in the handle array was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_ERROR
*		An error occurred while processing the command.  Additional
*		error information is provided in the p_ci_op status field.
*
* NOTES
*	This routine performs a vendor specific CA interface function call.
*	The p_ci_op structure provides a means to pass vendor specific data to
*	the verbs provider driver.  If the vendor specific data contains handles,
*	the client should provide the optional handle array that lists all of
*	 all of the handles specified in the vendor specific data.  The handles
*	in the handle array are restricted to the following types:  ib_ca_handle_t,
*	ib_pd_handle_t, ib_cq_handle_t, ib_av_handle_t, ib_qp_handle_t,
*	ib_mr_handle_t, or ib_mw_handle_t
*	The contents of the handle array are verified by the
*	access layer and the verbs provider driver.
*
* SEE ALSO
*	ci_open_ca, ci_allocate_pd, ci_create_av, ci_create_cq,
*	ci_create_qp, ci_register_mr, ci_register_pmr,
*	ci_register_smr, ci_create_mw, ib_ci_op_t
*****/


#define	MAX_LIB_NAME		32

#ifdef CL_KERNEL

/****s* Verbs/ci_interface_t
* NAME
*	ci_interface_t -- Interface holding Channel Interface API's
* PURPOSE
*	The following structure is supplied by a Channel Interface
*	providing verbs functionality.
* SOURCE
*/

typedef struct _ci_interface
{
	net64_t				guid;

	/*
	 * Handle, returned by open_ca function of HCA upper interface (ci_interface_t)
	 * It has ib_ca_handle_t type
	 */
	void				*p_hca_obj;

	/*
	 * Vendor ID, Device ID, Device Revision of the HCA
	 * libname refers to the user mode library to load
	 * to support direct user mode IO. If vendor does not support one
	 * then the fields must be initialized to all zero's.
	 */
	uint32_t			vend_id;
	uint16_t			dev_id;
	uint16_t			dev_revision;
	char				libname[MAX_LIB_NAME];
	/*
	 * Version of the header file this interface export can handle
	 */
	uint32_t			version;

	/*
	 * HCA Access Verbs
	 */
	ci_open_ca			open_ca;
	ci_um_open_ca_t		um_open_ca;
	ci_query_ca			query_ca;
	ci_modify_ca		modify_ca;
	ci_close_ca			close_ca;
	ci_um_close_ca_t	um_close_ca;
	ci_register_event_handler_t		register_event_handler;
	ci_unregister_event_handler_t	unregister_event_handler;


	ci_vendor_call		vendor_call;

	/*
	 * Protection Domain
	 */
	ci_allocate_pd		allocate_pd;
	ci_deallocate_pd	deallocate_pd;

	/*
	 * Address Vector Management Verbs
	 */

	ci_create_av		create_av;
	ci_query_av			query_av;
	ci_modify_av		modify_av;
	ci_destroy_av		destroy_av;

	/*
	 * SRQ Management Verbs
	 */
	ci_create_srq		create_srq;
	ci_modify_srq		modify_srq;
	ci_query_srq		query_srq;
	ci_destroy_srq	destroy_srq;

	/*
	 * QP Management Verbs
	 */
	ci_create_qp		create_qp;
	ci_create_spl_qp	create_spl_qp;
	ci_modify_qp		modify_qp;
	ci_query_qp			query_qp;
	ci_destroy_qp		destroy_qp;
	ci_ndi_modify_qp	ndi_modify_qp;

	/*
	 * Completion Queue Management Verbs
	 */
	ci_create_cq		create_cq;
	ci_resize_cq		resize_cq;
	ci_query_cq			query_cq;
	ci_destroy_cq		destroy_cq;

	/*
	 * Memory Management Verbs
	 */
	ci_register_mr		register_mr;
	ci_register_pmr		register_pmr;
	ci_query_mr			query_mr;
	ci_modify_mr		modify_mr;
	ci_modify_pmr		modify_pmr;
	ci_register_smr		register_smr;
	ci_deregister_mr	deregister_mr;

	/*
	 * Mellanox Fast Memory Management Verbs
	 */
	ci_alloc_mlnx_fmr		alloc_mlnx_fmr;
	ci_map_phys_mlnx_fmr	map_phys_mlnx_fmr;
	ci_unmap_mlnx_fmr		unmap_mlnx_fmr;
	ci_dealloc_mlnx_fmr		dealloc_mlnx_fmr;


	/*
	 * Memory Window Verbs
	 */
	ci_create_mw		create_mw;
	ci_query_mw			query_mw;
	ci_bind_mw			bind_mw;
	ci_destroy_mw		destroy_mw;

	/*
	 * Work Request Processing Verbs
	 */
	ci_post_send		post_send;
	ci_post_recv		post_recv;
	ci_post_srq_recv	post_srq_recv;

	/*
	 * Completion Processing and
	 * Completion Notification Request Verbs.
	 */
	ci_peek_cq					peek_cq;				/* Optional */
	ci_poll_cq					poll_cq;
	ci_enable_cq_notify			enable_cq_notify;
	ci_enable_ncomp_cq_notify	enable_ncomp_cq_notify;	/* Optional */

	/*
	 * Multicast Support Verbs
	 */
	ci_attach_mcast		attach_mcast;
	ci_detach_mcast		detach_mcast;

	/*
	 * Local MAD support, for HCA's that do not support
	 * Agents in the HW.
	 */
	ci_local_mad		local_mad;

} ci_interface_t;
/********/


/****f* Verbs/ib_register_ca
* NAME
*	ib_register_ca -- Inform the IB Access Layer about a new HCA
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t
ib_register_ca (
	IN		const	ci_interface_t*				p_ci,
	IN		const	PDEVICE_OBJECT				p_hca_dev,
	IN		const	PDEVICE_OBJECT				p_fdo
	);
/*
* DESCRIPTION
*	This routine is called by a HCA kernel mode driver to inform the
*	IB Access Layer about a new HCA that is ready for use. It is expected
*	that the Access Layer could immediatly turn around and call for services
*	even before the call returns back the HCA driver code. The HCA driver
*	must initialize all resources and be ready to service any calls before adding
*	its services to the IB Access Layer.
*
* PARAMETERS
*	p_ci
*		[in] Pointer to the ci_interface_t structure that has the function
*		vector to support verbs functionality.
*	p_hca_dev
*		PDO of HCA device
*	p_fdo
*		FDO of IBBUS device
*
* RETURN VALUE
*	IB_SUCCESS
*		The registration is successful.
*
*	IB_INVALID_PARAMETER
*		A reference to the CI interface structure was not provided.
*
*	IB_INSUFFICIENT_RESOURCES
*		Insufficient memory to satisfy resource requirements.
*
*	IB_DUPLICATE_CA
*		HCA GUID is already registered with the IB Access Layer
*
* PORTABILITY
*	Kernel Mode only
*
* SEE ALSO
*	ib_deregister_ca, ci_interface_t
*******/

/****f* Verbs/ib_deregister_ca
* NAME
*	ib_deregister_ca -- Inform the IB Access Layer that this HCA is no longer available
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t
ib_deregister_ca (
	IN		const	net64_t						ca_guid );
/*
* DESCRIPTION
*	This routine is called by the HCA driver when this HCA would no longer be
*	available for services. The access layer is expected to return all resources
*	back to the HCA driver, and perform a ci_close_ca on this interface.
*
* PARAMETERS
*	ca_guid
*		[in] GUID of the HCA that is being removed.
*
* RETURN VALUE
*	IB_SUCCESS
*		The deregistration is successful.
*
*	IB_NOT_FOUND
*		No HCA with the specified GUID is registered.
*
*	IB_BUSY
*		The HCA is still in use and cannot be released.
*
* PORTABILITY
*	Kernel Mode only
*
* SEE ALSO
*	ib_register_ca, ci_interface_t
*******/

#endif /* CL_KERNEL */


#ifdef __cplusplus
}	/* extern "C" */
#endif	/* __cplusplus */

#endif // __IB_CI_H__

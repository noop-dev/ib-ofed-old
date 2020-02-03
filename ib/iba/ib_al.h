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
 * $Id: ib_al.h 7145 2011-01-03 08:37:20Z irena $
 */


#if !defined(__IB_AL_H__)
#define __IB_AL_H__

#include <iba/ib_types.h>
#include <complib/cl_waitobj.h>
#include <complib/cl_qlist.h>


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

/****h* IB Access Layer API/Access Layer
* NAME
*	InfiniBand Access Layer
* COPYRIGHT
*	Copyright (c) 2003 Intel Corporation - All Rights Reserved.
* DESCRIPTION
*	The access layer provides transport level access to an InfiniBand fabric.
*	It supplies a foundation upon which a channel driver may be built.  The
*	access layer exposes the capabilities of the InfiniBand architecture and
*	adds support for higher-level functionality required by most users of an
*	InfiniBand fabric.  Users define the protocols and policies used by the
*	access layer, and the access layer implements them under the direction
*	of a user.
****/

typedef struct _ib_al*					ib_al_handle_t;
typedef struct _al_pnp*					ib_pnp_handle_t;
typedef struct _al_reg_svc*				ib_reg_svc_handle_t;
typedef struct _al_mad_send*			ib_mad_send_handle_t;
typedef struct _al_mad_svc*				ib_mad_svc_handle_t;
typedef struct _al_query*				ib_query_handle_t;
typedef struct _al_sub*					ib_sub_handle_t;
typedef struct _al_listen*				ib_listen_handle_t;
typedef struct _al_ioc*					ib_ioc_handle_t;
typedef struct _al_svc_entry*			ib_svc_handle_t;
typedef struct _al_pool_key*			ib_pool_key_t;
typedef struct _al_pool*				ib_pool_handle_t;
typedef struct _mlnx_fmr_pool_element*	mlnx_fmr_pool_el_t;


typedef struct _ib_cm_handle
{
	ib_al_handle_t		h_al;
	ib_qp_handle_t		h_qp;
	net32_t				cid;

}	ib_cm_handle_t;


/****s* Access Layer/ib_shmid_t
* NAME
*	ib_shmid_t
*
* DESCRIPTION
*	Shared Memory Identifier, used to uniquely identify a shared memory region.
*
* SYNOPSIS
*/
typedef uint8_t		ib_shmid_t[64];
/*
* SEE ALSO
*	ib_reg_shmid
*********/


/****d* Access Layer/ATS
* NAME
*	DAPL Address Translation Service
*
* DESCRIPTION
*	ATS service ID, service name, and IPv4 offset for DAPL-compliant
*	ATS service records.
*/
#define ATS_SERVICE_ID		CL_NTOH64( 0x10000CE100415453 )
#define ATS_NAME			"DAPL Address Translation Service"
#define ATS_IPV4_OFFSET		12
/**********/

#define	LIST_SAVE_SIZE 48


/****s* Access Layer/ib_mad_element_t
* NAME
*	ib_mad_element_t
*
* DESCRIPTION
*	Information used to submit a work request to a management datagram (MAD)
*	queue pair.
*
* SYNOPSIS
*/
typedef struct _ib_mad_element
{
	TO_LONG_PTR(struct _ib_mad_element*, p_next);
	TO_LONG_PTR(const void*,	context1);
	TO_LONG_PTR(const void*,	context2);

	/* Request/completion data. */
	TO_LONG_PTR(ib_mad_t*,		p_mad_buf);
	uint32_t					size;
	uint32_t					immediate_data;
	ib_net32_t					remote_qp;

	/* Send request information. */
	TO_LONG_PTR(ib_av_handle_t,	h_av);
	ib_send_opt_t				send_opt;
	ib_net32_t					remote_qkey;
	boolean_t					resp_expected;
	uint32_t					timeout_ms;
	uint32_t					retry_cnt;
	uint8_t						rmpp_version;

	/* Completion information. */
	ib_wc_status_t				status;
	boolean_t					grh_valid;
	TO_LONG_PTR(ib_grh_t*,		p_grh);

	/* Completed receive data or send request information if h_av is NULL. */
	uint32_t					recv_opt;
	ib_net16_t					remote_lid;
	uint8_t						remote_sl;
	uint16_t					pkey_index;
	uint8_t						path_bits;

	/* Transaction completion data. */
	TO_LONG_PTR(void*,			send_context1);
	TO_LONG_PTR(void*,			send_context2);

	/* MAD debug */
	uint32_t				mad_index;
	char					last_list[LIST_SAVE_SIZE];

}	ib_mad_element_t;
/*
* FIELDS
*	p_next
*		A pointer used to chain MAD elements together.  This value is
*		set to NULL to mark the end of the chain.
*
*	context1
*		User-defined context information associated with the datagram.
*
*	context2
*		User-defined context information associated with the datagram.
*
*	p_buffer
*		The local data buffer contain the MAD.
*
*	size
*		The size of the MAD referenced by p_buffer.
*
*	immediate_data
*		32-bit field sent or received as part of a datagram message.
*		This field is valid for send operations if the send_opt
*		IB_SEND_OPT_IMMEDIATE flag has been set.  This field is valid
*		on received datagram completions if the recv_opt
*		IB_RECV_OPT_IMMEDIATE flag is set.
*
*	remote_qp
*		Identifies the destination queue pair of a datagram send operation or
*		the source queue pair of a received datagram.
*
*	h_av
*		An address vector that specifies the path information used to route
*		the outbound datagram to the destination queue pair.  This handle may
*		be NULL when sending a directed route SMP or if the access layer
*		should create the address vector for the user.
*
*	send_opt
*		Optional send control parameters.  The following options are valid:
*		IB_SEND_OPT_IMMEDIATE and IB_SEND_OPT_SOLICITED.  IB_SEND_OPT_FENCE
*		is only valid on MAD QPs.
*
*	remote_qkey
*		The qkey for the destination queue pair.
*
*	resp_expected
*		This field is used to indicate that the submitted operation expects
*		a response.  When set, the access layer will retry this send operation
*		until the corresponding response is successfully received, or the
*		request times out.  Send operations for which a response is expected
*		will always be completed by the access layer before the corresponding
*		received response.
*
*	timeout_ms
*		Specifies the number of milliseconds to wait for a response to
*		a request until retrying or timing out the request.  This field is
*		ignored if resp_expected is set to FALSE.
*
*	retry_cnt
*		Specifies the number of times that the request will be retried
*		before failing the request.  This field is ignored if resp_expected
*		is set to FALSE.
*
*	rmpp_version
*		Indicates the version of the RMPP protocol to use when sending this
*		MAD.  For MADs posted to MAD services of type IB_MAD_SVC_DEFAULT,
*		setting this field to 0 disables RMPP on user-defined management
*		classes or invokes the default RMPP version for well-defined management
*		classes, if appropriate.  For MADs posted to MAD services of type
*		IB_MAD_SVC_RMPP, setting this field to 0 disables RMPP on the sent
*		MAD.  Note that if the RMPP header exists, but the RMPP protocol is
*		not activated for this MAD, the user must ensure that the RMPP header
*		has been zeroed.  This field is intended to help support backwards
*		compatibility.
*
*	status
*		The result of the MAD work request.
*
*	grh_valid
*		A flag indicating whether the p_grh reference is valid.
*
*	p_grh
*		A reference to the global route header information.
*
*	recv_opt
*		Indicates optional fields valid as part of a work request that
*		completed on an unreliable datagram queue pair.
*
*	remote_lid
*		The source LID of the received datagram.
*
*	remote_sl
*		The service level used by the source of the received datagram.
*
*	pkey_index
*		This is valid only for IB_QPT_QP1 and IB_QPT_QP1_ALIAS QP types.
*		For received datagrams, this field contains the pkey index for
*		the source queue pair.  For send operations, this field contains
*		the pkey index to use when posting the send work request. 
*
*	path_bits
*		The portion of the remote_lid that may be changed to vary the path
*		through the subnet to the remote port.
*
*	send_context1
*		If this datagram was received as a response to a sent datagram, this
*		field contains the context1 value of the send operation.  If this is
*		an unsolicited receive, this field will be 0.
*
*	send_context2
*		If this datagram was received as a response to a sent datagram, this
*		field contains the context2 value of the send operation.  If this is
*		an unsolicited receive, this field will be 0.
*
*	remote_qp
*		Identifies the source queue pair of a received datagram.
*
* NOTES
*	The format of data sent over the fabric is expected to be in the form
*	of a MAD.  MADs are expected to match the format defined by the
*	Infiniband specification and must be in network-byte order when posted
*	to a MAD service.
*
*	This structure is received to notify a user that a datagram has been
*	received for a registered management class.  Information of the source
*	of the data is provided, along with the data buffer.
*
*	The MAD element structure is defined such that a received MAD element
*	may be re-used as a sent response.  In such cases, the h_av field may be
*	NULL.  The address vector will be created and destroyed by the access
*	layer.
*
* SEE ALSO
*	ib_get_mad, ib_put_mad, ib_send_mad, ib_local_ds_t, ib_send_opt_t,
*	ib_pfn_mad_recv_cb_t, ib_get_mad_buf
*****/


/****f* Access Layer/ib_get_mad_buf
* NAME
*	ib_get_mad_buf
*
* DESCRIPTION
*	Returns a pointer to the MAD buffer associated with a MAD element.
*
* SYNOPSIS
*/
#pragma warning(push)
#pragma warning(disable: 4244 ) 
AL_INLINE void* AL_API
ib_get_mad_buf(
	IN		const	ib_mad_element_t* const		p_mad_element )
{
	CL_ASSERT( p_mad_element );
	return( p_mad_element->p_mad_buf );
}
#pragma warning (pop)
/*
* PARAMETERS
*	p_mad_element
*		[in] A pointer to a MAD element.
*
* NOTES
*	Returns a pointer to the MAD buffer associated with a MAD element.
*
* SEE ALSO
*	ib_mad_element_t
*****/


/****f* Access Layer/ib_pfn_comp_cb_t
* NAME
*	ib_pfn_comp_cb_t
*
* DESCRIPTION
*	Completion callback provided by a client.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_comp_cb_t)(
	IN		const	ib_cq_handle_t				h_cq,
	IN				void						*cq_context );
/*
* PARAMETERS
*	h_cq
*		[in] Handle for the completion queue on which the completion occurred.
*
*	cq_context
*		[in] User-specified context for the completion queue on which the
*		completion occurred.
*
* NOTES
*	This function is invoked upon completion of a work request on a queue pair
*	associated with the completion queue.  The context associated with the
*	completion queue on which the completion occurred is return to the client
*	through the callback.
*
*	In the kernel, this callback is usually invoked using a tasklet, dependent
*	on the implementation of the underlying verbs provider driver.
*****/


/****d* Access Layer/ib_al_flags_t
* NAME
*	ib_al_flags_t
*
* DESCRIPTION
*	Access layer flags used to direct the operation of various calls.
*
* SYNOPSIS
*/
typedef uint32_t							ib_al_flags_t;
#define IB_FLAGS_SYNC						0x00000001
/*
* VALUES
*	IB_FLAGS_SYNC
*		Indicates that the given operation should be performed synchronously.
*		The call will block until it completes.  Callbacks will still be
*		invoked.
*
* SEE ALSO
*	ib_cm_req_t, ib_cm_rep_t, ib_cm_dreq_t, ib_cm_lap_t,
*	ib_reg_svc_req_t, ib_mcast_req_t, ib_query_req_t, ib_sub_req_t
*****/


/****f* Access Layer/ib_pfn_destroy_cb_t
* NAME
*	ib_pfn_destroy_cb_t
*
* DESCRIPTION
*	Asynchronous callback invoked after a resource has been successfully
*	destroyed.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_destroy_cb_t)(
	IN				void						*context );
/*
* PARAMETERS
*	context
*		[in] User-specified context associated with the resource being
*		destroyed.  The context for the resource is usually set during the
*		object's creation.
*
* NOTES
*	This callback notifies a client that a resource has been successfully
*	destroyed.  It is used to indicate that all pending callbacks associated
*	with the resource have completed, and no additional events will be
*	generated for that resource.
*
*	This callback is invoked within a system thread context in the kernel.
*
*	If the user specifies ib_sync_destroy as the asynchronous callback, then
*	the object being destroyed will be destroyed synchronously.  This may 
*	result in the calling thread blocking while outstanding callbacks complete.
*
* SEE ALSO
*	ib_sync_destroy
*****/



/****f* Access Layer/ib_sync_destroy
* NAME
*	ib_sync_destroy
*
* DESCRIPTION
*	Access layer routine used to indicate synchronous destruction of an
*	object.
*
* SYNOPSIS
*/
static const ib_pfn_destroy_cb_t ib_sync_destroy = (ib_pfn_destroy_cb_t)(LONG_PTR)-1;
/*
* PARAMETERS
*	Not Applicable.
*
* NOTES
*	Users specify ib_sync_destroy as the ib_pfn_destroy_cb_t callback in order
*	to force synchronous object destruction.  This may result in the calling
*	thread blocking while outstanding callbacks complete.
*
* SEE ALSO
*	ib_pfn_destroy_cb_t
*****/


/****s* Access Layer/ib_async_event_rec_t
* NAME
*	ib_async_event_rec_t
*
* DESCRIPTION
*	Information returned when an asynchronous event occurs on an allocated
*	resource.
*
* SYNOPSIS
*/
typedef struct _ib_async_event_rec
{
	ib_async_event_t							code;
	uint64_t									vendor_specific;

	TO_LONG_PTR(void*,							context);
	union _handle_t
	{
		TO_LONG_PTR(ib_ca_handle_t,				h_ca);
		TO_LONG_PTR(ib_cq_handle_t,				h_cq);
		TO_LONG_PTR(ib_qp_handle_t,				h_qp);
		TO_LONG_PTR(struct _ib_srq*,			h_srq);

	} handle;
	
	uint8_t										port_number;

}	ib_async_event_rec_t;
/*
* FIELDS
*	code
*		A code that identifies the type of event being reported.
*
*	vendor_specific
*		A field containing optional vendor specific information.
*
*	context
*		User-defined context information associated with the resource on
*		which the error occurred.
*
*	handle
*		A handle to the resource for which this event record was generated.
*		This handle will match the handle returned during the creation of
*		resource.  It is provided in case an event occurs before a client's
*		call to create a resource can return.
*
* SEE ALSO
*	ib_async_event_t, ib_pfn_event_cb_t
*****/


/****f* Access Layer/ib_pfn_event_cb_t
* NAME
*	ib_pfn_event_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after an asynchronous event
*	has occurred on an allocated resource.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_event_cb_t)(
	IN				ib_async_event_rec_t		*p_event_rec );
/*
* PARAMETERS
*	p_event_rec
*		[in] Information returned to the user, indicating the type of
*		event and the associated user context.
*
* NOTES
*	This callback is invoked within a system thread context in the kernel.
*
* SEE ALSO
*	ib_async_event_rec_t
*****/


/****f* Access Layer/ib_open_ca
* NAME
*	ib_open_ca
*
* DESCRIPTION
*	Opens a channel adapter for additional access.  A channel adapter must
*	be opened before consuming resources on that adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_open_ca(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_net64_t					ca_guid,
	IN		const	ib_pfn_event_cb_t			pfn_ca_event_cb OPTIONAL,
	IN		const	void* const					ca_context,
		OUT			ib_ca_handle_t* const		ph_ca );
/*
* PARAMETERS
*	h_al
*		[in] The handle to an open instance of AL.
*
*	ca_guid
*		[in] The GUID of the channel adapter to open.
*
*	pfn_ca_event_cb
*		[in] A user-specified callback that is invoked after an
*		asynchronous event has occurred on the channel adapter.
*
*	ca_context
*		[in] A client-specified context to associate with this opened instance
*		of the channel adapter.  This context is returned to the user when
*		invoking asynchronous callbacks referencing this channel adapter.
*
*	ph_ca
*		[out] Upon successful completion of this call, this references a
*		handle to the opened channel adapter.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_GUID
*		No channel adapter in the system was found for the specified ca_guid.
*
*	IB_INVALID_PARAMETER
*		A reference to the CA handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to open the channel adapter.
*
* NOTES
*	When successful, this routine returns a handle to an open instance of a CA.
*
* SEE ALSO
*	ib_query_ca, ib_modify_ca, ib_close_ca, ib_pfn_event_cb_t
*****/


/****f* Access Layer/ib_query_ca
* NAME
*	ib_query_ca
*
* DESCRIPTION
*	Queries the attributes of an opened channel adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_ca(
	IN		const	ib_ca_handle_t				h_ca,
		OUT			ib_ca_attr_t* const			p_ca_attr OPTIONAL,
	IN	OUT			uint32_t* const				p_size );
/*
* PARAMETERS
*	h_ca
*		[in] The handle to an open channel adapter.
*
*	p_ca_attr
*		[out] A reference to a buffer where the channel adapter attributes,
*		including port attribute information will be copied.  If this parameter
*		is NULL, then the required buffer size needed to return all of the CA
*		attribute information is returned through the p_size parameter.  The
*		ib_ca_attr_t structure for the specified channel adapter is stored
*		at the top of the buffer.
*
*	p_size
*		[in/out] On input, this references the size of the data buffer
*		referenced by the p_ca_attr parameter.
*
*		On output, the number of bytes used or needed to copy all CA
*		attribute information.
*
* RETURN VALUES
*	IB_SUCCESS
*		The attributes were returned successfully.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		The size of the p_ca_attr buffer, specified through p_size, is
*		insufficient to store all of the CA attribute information.
*
*	IB_INVALID_PARAMETER
*		A reference to the size was not provided.
*
* NOTES
*	This routine returns information about the specified channel adapter,
*	including port attributes.  The amount of information returned through
*	this call is variable sized.  Users may obtain the size of the data
*	buffer required to obtain the CA attributes by calling this function
*	with p_ca_attr set to NULL.  The access layer will then return the
*	necessary size in the variable referenced by the p_size parameter.
*
* SEE ALSO
*	ib_open_ca, ib_query_ca_by_guid, ib_modify_ca, ib_close_ca, ib_ca_attr_t
*****/


/****f* Access Layer/ib_query_ca_by_guid
* NAME
*	ib_query_ca_by_guid
*
* DESCRIPTION
*	Queries the attributes of an opened channel adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_ca_by_guid(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_net64_t					ca_guid,
		OUT			ib_ca_attr_t* const			p_ca_attr OPTIONAL,
	IN	OUT			uint32_t* const				p_size );
/*
* PARAMETERS
*	h_al
*		[in] The handle to an open instance of AL.
*
*	ca_guid
*		[in] The GUID of the channel adapter to query.
*
*	p_ca_attr
*		[out] A reference to a buffer where the channel adapter attributes,
*		including port attribute information will be copied.  If this parameter
*		is NULL, then the required buffer size needed to return all of the CA
*		attribute information is returned through the p_size parameter.  The
*		ib_ca_attr_t structure for the specified channel adapter is stored
*		at the top of the buffer.
*
*	p_size
*		[in/out] On input, this references the size of the data buffer
*		referenced by the p_ca_attr parameter.
*
*		On output, the number of bytes used or needed to copy all CA
*		attribute information.
*
* RETURN VALUES
*	IB_SUCCESS
*		The attributes were returned successfully.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_GUID
*		No channel adapter in the system was found for the specified ca_guid.
*
*	IB_INSUFFICIENT_MEMORY
*		The size of the p_ca_attr buffer, specified through p_size, is
*		insufficient to store all of the CA attribute information.
*
*	IB_INVALID_PARAMETER
*		A reference to the size was not provided.
*
* NOTES
*	This routine returns information about the specified channel adapter,
*	including port attributes.  The amount of information returned through
*	this call is variable sized.  Users may obtain the size of the data
*	buffer required to obtain the CA attributes by calling this function
*	with p_ca_attr set to NULL.  The access layer will then return the
*	necessary size in the variable referenced by the p_size parameter.
*
* SEE ALSO
*	ib_open_ca, ib_query_ca, ib_modify_ca, ib_close_ca, ib_ca_attr_t
*****/


/****f* Access Layer/ib_modify_ca
* NAME
*	ib_modify_ca
*
* DESCRIPTION
*	Modifies the attributes and violation counters associated with a port.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_modify_ca(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	uint8_t						port_num,
	IN		const	ib_ca_mod_t					ca_mod,
	IN		const	ib_port_attr_mod_t* const	p_port_attr_mod );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an opened channel adapter.
*
*	port_num
*		[in] An index to the port that is being modified.  The port_num matches
*		the index of the port as returned through the ib_query_ca call.
*
*	ca_mod
*		[in] A mask of the attributes and counters to modify.
*
*	p_port_attr_mod
*		[in] A list of the specific port attribute information to modify.  For
*		the access layer to modify an attribute, its corresponding bit must be
*		set in the ca_mod parameter.
*
* RETURN VALUES
*	IB_SUCCESS
*		The attributes were successfully modified.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INVALID_PORT
*		The port number supplied was invalid for the given channel adapter.
*
*	IB_INVALID_PARAMETER
*		The supplied ca_mod mask is invalid or a reference to the port
*		attribute information was not provided.
*
*	IB_UNSUPPORTED
*		The optional qkey and pkey violation counters are not supported by
*		this channel adapter, but an attempt was made to modify them.
*
* NOTES
*	This call sets the attributes for a port in its associated PORT_INFO
*	structure.  It will also reset pkey and qkey violation counters.
*
* SEE ALSO
*	ib_open_ca, ib_query_ca, ib_close_ca, ib_ca_mod_t, ib_port_attr_mod_t
*****/


/****f* Access Layer/ib_close_ca
* NAME
*	ib_close_ca
*
* DESCRIPTION
*	Closes an opened channel adapter.  Once closed, no further access to this
*	channel adapter is possible.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_close_ca(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an opened channel adapter.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the channel
*		adapter has been successfully destroyed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The close request was registered.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
* NOTES
*	This call closes the opened channel adapter and frees all associated
*	resources, such as queue pairs, protection domains, and completion
*	queues.  Since callbacks may be outstanding against the channel adapter
*	or one of its resources at the time the close operation is invoked, this
*	call operates asynchronously.  The user will be notified through a callback
*	once the close operation completes, indicating that no additional callbacks
*	will be invoked for the specified channel adapter or a related resource.
*
* SEE ALSO
*	ib_open_ca
*****/


/****f* Access Layer/ib_alloc_pd
* NAME
*	ib_alloc_pd
*
* DESCRIPTION
*	Allocates a protection domain on the specified channel adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_alloc_pd(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_pd_type_t				pd_type,
	IN		const	void* const					pd_context,
		OUT			ib_pd_handle_t* const		ph_pd );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an opened channel adapter.
*
*	pd_type
*		[in] Indicates the type of protection domain being created.
*
*	pd_context
*		[in] A client-specified context to associate with this allocated
*		protection domain.  This context is returned to the user when
*		invoking asynchronous callbacks referencing this protection domain.
*
*	ph_pd
*		[out] Upon successful completion of this call, this references a
*		handle to the allocated protection domain.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INVALID_PARAMETER
*		The supplied pd_type value is invalid or a reference to the protection
*		domain handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to allocate the protection domain.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the protection domain.
*
* NOTES
*	When successful, this routine returns a handle to a newly allocated
*	protection domain.
*
* SEE ALSO
*	ib_dealloc_pd, ib_pd_type_t
*****/


/****f* Access Layer/ib_dealloc_pd
* NAME
*	ib_dealloc_pd
*
* DESCRIPTION
*	Deallocates a protection domain.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_dealloc_pd(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an allocated protection domain.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the protection
*		domain has been successfully destroyed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
* NOTES
*	This call deallocates a protection domain and releases all associated
*	resources, including queue pairs and registered memory regions.  Since
*	callbacks may be outstanding against one of protection domain's related
*	resources at the time the deallocation call is invoked, this call operates
*	asynchronously.  The user will be notified through a callback once the
*	deallocation call completes, indicating that no additional callbacks
*	will be invoked for a related resource.
*
* SEE ALSO
*	ib_alloc_pd
*****/


/****f* Access Layer/ib_create_av
* NAME
*	ib_create_av
*
* DESCRIPTION
*	Creates an address vector.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_av(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_av_attr_t* const			p_av_attr,
		OUT			ib_av_handle_t* const		ph_av );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an allocated protection domain that the address
*		vector will be associated with.
*
*	p_av_attr
*		[in] Attributes for the newly created address vector.
*
*	ph_av
*		[out] Upon successful completion of this call, this references a
*		handle to the newly created address vector.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the address vector attributes or handle was not
*		provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the address vector.
*
*	IB_INVALID_PORT
*		The port number supplied, through the address vector attributes,
*		was invalid for the given channel adapter.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the address vector.
*
* NOTES
*	This routine creates an address vector.  Clients specify the attributes
*	of the address vector through the p_av_attr parameter.
*
* SEE ALSO
*	ib_query_av, ib_modify_av, ib_destroy_av
*****/


/****f* Access Layer/ib_query_av
* NAME
*	ib_query_av
*
* DESCRIPTION
*	Returns the attributes of an address vector.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_av(
	IN		const	ib_av_handle_t				h_av,
		OUT			ib_av_attr_t* const			p_av_attr,
		OUT			ib_pd_handle_t* const		ph_pd );
/*
* PARAMETERS
*	h_av
*		[in] A handle to an existing address vector.
*
*	p_av_attr
*		[out] Upon successful completion, the structure referenced by this
*		parameter contains the attributes of the specified address vector.
*
*	ph_pd
*		[out] Upon successful completion, this references a handle to the
*		protection domain associated with the address vector.
*
* RETURN VALUES
*	IB_SUCCESS
*		The attributes were returned successfully.
*
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the address vector attributes structure or protection
*		domain handle was not provided.
*
* SEE ALSO
*	ib_create_av, ib_modify_av, ib_destroy_av, ib_av_attr_t
*****/


/****f* Access Layer/ib_modify_av
* NAME
*	ib_modify_av
*
* DESCRIPTION
*	Modifies the attributes of an existing address vector.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_modify_av(
	IN		const	ib_av_handle_t				h_av,
	IN		const	ib_av_attr_t* const			p_av_attr );
/*
* PARAMETERS
*	h_av
*		[in] A handle to an existing address vector.
*
*	p_av_attr
*		[in] The new attributes to use when modifying the address vector.
*
* RETURN VALUES
*	IB_SUCCESS
*		The address vector was successfully modified.
*
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the address vector attributes structure was not
*		provided.
*
*	IB_INVALID_PORT
*		The port number supplied, through the address vector attributes,
*		was invalid for the given channel adapter.
*
* NOTES
*	This routine modifies the attributes of an existing address vector.
*	The new attributes are specified through the p_av_attr parameter.
*
* SEE ALSO
*	ib_create_av, ib_destroy_av
*****/


/****f* Access Layer/ib_destroy_av
* NAME
*	ib_destroy_av
*
* DESCRIPTION
*	Destroys an existing address vector.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_av(
	IN		const	ib_av_handle_t				h_av );
/*
* PARAMETERS
*	h_av
*		[in] A handle to an existing address vector.
*
* RETURN VALUES
*	IB_SUCCESS
*		The address vector was successfully destroyed.
*
*	IB_INVALID_AV_HANDLE
*		The address vector handle was invalid.
*
* NOTES
*	This routine destroys an existing address vector.
*
* SEE ALSO
*	ib_create_av
*****/


/****f* Access Layer/ib_create_srq
* NAME
*	ib_create_srq
*
* DESCRIPTION
*	Creates a shared receive queue and returns its handle to the user.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_srq(
	IN		const	ib_pd_handle_t			h_pd,
	IN		const	ib_srq_attr_t* const		p_srq_attr,
	IN		const	void* const				srq_context,
	IN		const	ib_pfn_event_cb_t			pfn_srq_event_cb OPTIONAL,
		OUT		ib_srq_handle_t* const		ph_srq );
/*
* PARAMETERS
*	h_pd
*		[in] This is a handle to a protection domain associated with the shared queue
*		pair.
*
*	p_srq_attr
*		[in] Attributes necessary to allocate and initialize a shared receive queue.
*
*	srq_context
*		[in] A user-specified context information associated with the shared
*		receive queue.
*
*	pfn_qp_event_cb
*		[in] User-specified error callback routine invoked after an
*		asynchronous event has occurred on the shared receive queue.
*
*	ph_srq
*		[out] Upon successful completion of this call, this references a
*		handle to the newly created shared receive queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The receive queue was successfully created.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain to associate with the shared receive queue was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the shared receive queue attributes or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the shared receive queue.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the shared receive queue.
*
*	IB_INVALID_SETTING
*		The specified shared receive queue creation attributes are invalid.
*
*	IB_INVALID_MAX_WRS
*		The requested maximum send or receive work request depth could not be
*		supported.
*
*	IB_INVALID_MAX_SGE
*		The requested maximum number of scatter-gather entries for the send or
*		receive queue could not be supported.
*
* NOTES
*	This routine allocates a shared receive queue with the specified attributes.  If
*	the shared receive queue cannot be allocated, an error is returned.  When creating
*	the shared receive queue, users associate a context with the shared receive queue.  This
*	context is returned to the user through the asynchronous event callback
*	if an event occurs.
*
*	This routine is used to create receive queues, which work with QPs of type:
*
*	IB_QPT_RELIABLE_CONN
*	IB_QPT_UNRELIABLE_CONN
*	IB_QPT_UNRELIABLE_DGRM
*
* SEE ALSO
*	ib_query_srq, ib_modify_srq, ib_destroy_srq, ib_srq_attr_t,
*	ib_srq_attr_mask_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_query_srq
* NAME
*	ib_query_srq
*
* DESCRIPTION
*	Query the current attributes of the shared receive queue.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_srq(
	IN		const	ib_srq_handle_t				h_srq,
		OUT			ib_srq_attr_t* const			p_srq_attr );
/*
* PARAMETERS
*	h_srq
*		[in] A handle to an existing shared receive queue.
*
*	p_srq_attr
*		[out] Upon successful completion of this call, the structure
*		referenced by this parameter contains the attributes of the specified
*		quere pair.
*
* RETURN VALUES
*	IB_SUCCESS
*		The shared receive queue attributes were returned successfully.
*
*	IB_INVALID_SRQ_HANDLE
*		The shared receive queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the shared receive queue attributes structure was not provided.
*
* NOTES
*	This routine returns information about the specified shared receive queue.
*
* SEE ALSO
*	ib_query_srq, ib_modify_srq, ib_destroy_srq, ib_srq_attr_t,
*	ib_srq_attr_mask_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_modify_srq
* NAME
*	ib_modify_srq
*
* DESCRIPTION
*	Modifies the attributes of an existing shared receive queue.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_modify_srq(
	IN		const	ib_srq_handle_t			h_srq,
	IN		const	ib_srq_attr_t* const		p_srq_attr,
	IN		const	ib_srq_attr_mask_t			srq_attr_mask );
/*
* PARAMETERS
*	h_srq
*		[in] A handle to an existing shared receive queue.
*
*	p_srq_attr
*		[in] Attributes necessary to allocate and initialize a shared receive queue.
*
*	srq_attr_mask
*		[in] Flags, indicating which fields in the previous structure are valid.
*
* RETURN VALUES
*	IB_SUCCESS
*		The shared receive queue was successfully modified.
*
*	IB_INVALID_SRQ_HANDLE
*		The shared receive queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the shared receive queue attributes was not provided.
*
*	IB_INVALID_SETTING
*		The specified shared receive queue attributes were invalid.
*
*	IB_UNSUPPORTED
*		The required action is not supported yet.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the modify the shared receive queue.
*
* NOTES
*	This routine modifies the attributes of an existing shared receive queue and
*	transitions it to a new state.  The new state and attributes are
*	specified through the p_qp_mod parameter.  Upon successful completion,
*	the shared receive queue is in the requested state.
*
* SEE ALSO
*	ib_query_srq, ib_modify_srq, ib_destroy_srq, ib_srq_attr_t,
*	ib_srq_attr_mask_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_destroy_srq
* NAME
*	ib_destroy_srq
*
* DESCRIPTION
*	Release a shared receive queue.  Once destroyed, no further access to this
*	shared receive queue is possible.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_srq(
	IN		const	ib_srq_handle_t				h_srq,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_srq
*		[in] A handle to an existing shared shared receive queue.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the shared receive queue
*		has been successfully destroyed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The destroy request was registered.
*
*	IB_INVALID_SRQ_HANDLE
*		The shared receive queue handle was invalid.
*
*	IB_RESOURCE_BUSY
*		There are QPs, bound to the shared receive queue
*
* NOTES
*	This call destroys an existing shared receive queue.  Since callbacks may be
*	outstanding against the shared receive queue at the time the destroy operation is
*	invoked, then this call operates asynchronously.  The user will be notified
*	through a callback once the destroy operation completes, indicating that
*	no additional callbacks will be invoked for the specified shared receive queue.
*
* SEE ALSO
*	ib_query_srq, ib_modify_srq, ib_destroy_srq, ib_srq_attr_t,
*	ib_srq_attr_mask_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_post_srq_recv
* NAME
*	ib_post_srq_recv
*
* DESCRIPTION
*	This routine posts a work request to the shared receive queue of a shared receive queue.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_post_srq_recv(
	IN		const	ib_srq_handle_t				h_srq,
	IN				ib_recv_wr_t* const			p_recv_wr,
		OUT			ib_recv_wr_t				**pp_recv_failure OPTIONAL );
/*
* PARAMETERS
*	h_srq
*		[in] The shared receive queue to which this work request is being submitted.
*
*	p_recv_wr
*		[in] A reference to the head of the work request list.
*
*	pp_recv_failure
*		[out] If the post receive operation failed, this references the work
*		request in the p_recv_wr list where the first failure occurred.
*		This parameter may be NULL if only a single work request is being
*		posted to the QP.
*
* RETURN VALUES
*	IB_SUCCESS
*		All work requests were successfully posted.
*
*	IB_INVALID_QP_HANDLE
*		The shared receive queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the receive work request list was not provided.
*
*	IB_INSUFFICIENT_RESOURCES
*		The number of posted work requests exceed the current depth available
*		on the receive queue.
*
*	IB_INVALID_WR_TYPE
*		The work request type was invalid.
*
*	IB_INVALID_QP_STATE
*		The current shared receive queue state does not allow posting receives.
*
* NOTES
*	This routine posts a work request to the shared receive queue.
*	The type of work to perform is defined by the p_recv_wr parameter.  This
*	call is used to post data buffers to receive incoming message sends.
*
* SEE ALSO
*	ib_recv_wr_t
*****/


/****f* Access Layer/ib_create_qp
* NAME
*	ib_create_qp
*
* DESCRIPTION
*	Creates a queue pair and returns its handle to the user.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_qp(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_qp_create_t* const		p_qp_create,
	IN		const	void* const					qp_context,
	IN		const	ib_pfn_event_cb_t			pfn_qp_event_cb OPTIONAL,
		OUT			ib_qp_handle_t* const		ph_qp );
/*
* PARAMETERS
*	h_pd
*		[in] This is a handle to a protection domain associated with the queue
*		pair.
*
*	p_qp_create
*		[in] Attributes necessary to allocate and initialize the queue pair.
*
*	qp_context
*		[in] A user-specified context information associated with the
*		queue pair.
*
*	pfn_qp_event_cb
*		[in] User-specified error callback routine invoked after an
*		asynchronous event has occurred on the queue pair.
*
*	ph_qp
*		[out] Upon successful completion of this call, this references a
*		handle to the newly created queue pair.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair was successfully created.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain to associate with the queue pair was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the queue pair attributes or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the queue pair.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the queue pair.
*
*	IB_INVALID_CQ_HANDLE
*		The send or receive completion queue to associate with the queue pair
*		was invalid.
*
*	IB_INVALID_SRQ_HANDLE
*		The shared receive queue to be associated with the queue pair
*		was invalid.
*
*	IB_INVALID_SETTING
*		The specified queue pair creation attributes are invalid.
*
*	IB_UNSUPPORTED
*		The specified queue pair type was not supported by the channel adapter.
*
*	IB_INVALID_MAX_WRS
*		The requested maximum send or receive work request depth could not be
*		supported.
*
*	IB_INVALID_MAX_SGE
*		The requested maximum number of scatter-gather entries for the send or
*		receive queue could not be supported.
*
* NOTES
*	1. This routine allocates a queue pair with the specified attributes.  If
*	the queue pair cannot be allocated, an error is returned.  When creating
*	the queue pair, users associate a context with the queue pair.  This
*	context is returned to the user through the asynchronous event callback
*	if an event occurs.
*
*	2. For QPs that are associated with an SRQ, the Consumer should take
*	the QP through the Error State before invoking a Destroy QP or a Modify
*	QP to the Reset State. The Consumer may invoke the Destroy QP without
*	first performing a Modify QP to the Error State and waiting for the Affiliated 
*	Asynchronous Last WQE Reached Event. However, if the Consumer
*	does not wait for the Affiliated Asynchronous Last WQE Reached Event,
*	then WQE and Data Segment leakage may occur.
*
*	3. This routine is used to create queue pairs of type:
*		IB_QPT_RELIABLE_CONN
*		IB_QPT_UNRELIABLE_CONN
*		IB_QPT_UNRELIABLE_DGRM
*		IB_QPT_MAD
*
*	4. Callers of ib_create_qp should call ib_init_dgrm_svc if the queue pair
*	is of type IB_QPT_UNRELIABLE_DGRM or IB_QPT_MAD before sending or
*	receiving data.  IB_QPT_RELIABLE_CONN, IB_QPT_UNRELIABLE_CONN type
*	queue pairs should be used by the connection establishment process
*	before data may be sent or received on the QP.
*
*	This call does not return the QP attributes as MAD QPs do not support
*	such an operation.  This is a minor specification deviation.
*
* SEE ALSO
*	ib_query_qp, ib_modify_qp, ib_destroy_qp, ib_cm_req, ib_cm_rep, ib_cm_rtu
*	ib_init_dgrm_svc, ib_qp_create_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_get_spl_qp
* NAME
*	ib_get_spl_qp
*
* DESCRIPTION
*	Create a special QP or QP alias.  This call provides access to queue
*	pairs 0 and 1, and the raw queue pair types.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_spl_qp(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_net64_t					port_guid,
	IN		const	ib_qp_create_t* const		p_qp_create,
	IN		const	void* const					qp_context,
	IN		const	ib_pfn_event_cb_t			pfn_qp_event_cb OPTIONAL,
		OUT			ib_pool_key_t* const		p_pool_key OPTIONAL,
		OUT			ib_qp_handle_t* const		ph_qp );
/*
* PARAMETERS
*	h_pd
*		[in] This is a handle to a protection domain associated with the queue
*		pair.  This must be a protection domain alias for aliased QP types.
*
*	port_guid
*		[in] The port GUID that the special QP will be associated with.
*
*	p_qp_create
*		[in] Attributes necessary to allocate and initialize the queue pair.
*
*	qp_context
*		[in] A user-specified context information associated with the
*		queue pair.
*
*	pfn_qp_ervent_cb
*		[in] User-specified error callback routine invoked after an
*		asynchronous event has occurred on the queue pair.
*
*	p_pool_key
*		[in] A key to a pool of MAD elements that are used to send MADs.
*		This key is only valid for aliased QP types.
*
*	ph_qp
*		[out] Upon successful completion of this call, this references a
*		handle to the newly created queue pair.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair was successfully created.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain to associate with the queue pair was invalid.
*
*	IB_INVALID_PORT
*		The port number supplied was invalid for the given channel adapter.
*
*	IB_INVALID_PARAMETER
*		A reference to the queue pair attributes or handle was not provided.
*
*	IB_INVALID_PERMISSION
*		The calling process does not have sufficient privilege to create the
*		requested queue pair type.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the queue pair.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the queue pair.
*
*	IB_INVALID_CQ_HANDLE
*		The send or receive completion queue to associate with the queue pair
*		was invalid.
*
*	IB_INVALID_SETTING
*		The specified queue pair type was invalid.
*
*	IB_UNSUPPORTED
*		The specified queue pair type was not supported by the channel adapter.
*
*	IB_INVALID_MAX_WRS
*		The requested maximum send or receive work request depth could not be
*		supported.
*
*	IB_INVALID_MAX_SGE
*		The requested maximum number of scatter-gather entries for the send or
*		receive queue could not be supported.
*
* NOTES
*	This routine allocates a queue pair with the specified attributes.  If
*	the queue pair cannot be allocated, an error is returned.  When creating
*	the queue pair, users associate a context with the queue pair.  This
*	context is returned to the user through the asynchronous event callback
*	if an event occurs.
*
*	This routine is used to create queue pairs of type:
*
*	IB_QPT_QP0
*	IB_QPT_QP1
*	IB_QPT_RAW_IPV6
*	IB_QPT_RAW_ETHER
*	IB_QPT_QP0_ALIAS
*	IB_QPT_QP1_ALIAS
*
*	Callers of ib_get_spl_qp should call ib_init_dgrm_svc if the queue pair is
*	of type IB_QPT_QP0, IB_QPT_QP1, IB_QPT_RAW_IPV6, IB_QPT_RAW_ETHER before
*	sending or receiving data.  MADs may be sent on aliased QPs on the
*	successful return of this routine.
*
* SEE ALSO
*	ib_query_qp, ib_modify_qp, ib_destroy_qp, ib_get_mad
*	ib_init_dgrm_svc, ib_qp_create_t, ib_pfn_event_cb_t, ib_qp_attr_t
*****/


/****f* Access Layer/ib_query_qp
* NAME
*	ib_query_qp
*
* DESCRIPTION
*	Query the current attributes of the queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_qp(
	IN		const	ib_qp_handle_t				h_qp,
		OUT			ib_qp_attr_t* const			p_qp_attr );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an existing queue pair.
*
*	p_qp_attr
*		[out] Upon successful completion of this call, the structure
*		referenced by this parameter contains the attributes of the specified
*		quere pair.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair attributes were returned successfully.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the queue pair attributes structure was not provided.
*
* NOTES
*	This routine returns information about the specified queue pair.
*
* SEE ALSO
*	ib_create_qp, ib_modify_qp, ib_qp_attr_t
*****/


/****f* Access Layer/ib_modify_qp
* NAME
*	ib_modify_qp
*
* DESCRIPTION
*	Modifies the attributes of an existing queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_modify_qp(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_qp_mod_t* const			p_qp_mod );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an existing queue pair.
*
*	p_qp_mod
*		[in] The new attributes to use when modifying the queue pair.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair was successfully modified.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the queue pair attributes was not provided.
*
*	IB_INVALID_SETTING
*		The specified queue pair attributes were invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the modify the queue pair.
*
*	IB_UNSUPPORTED
*		The requested modification was not supported.
*
*	IB_INVALID_QP_STATE
*		The queue pair was in an invalid state for the requested operation.
*
*	IB_INVALID_PKEY
*		The specified pkey was not valid.
*
*	IB_INVALID_APM_STATE
*		The specified automatic path migration state was not valid.
*
* NOTES
*	This routine modifies the attributes of an existing queue pair and
*	transitions it to a new state.  The new state and attributes are
*	specified through the p_qp_mod parameter.  Upon successful completion,
*	the queue pair is in the requested state.
*
* SEE ALSO
*	ib_create_qp, ib_destroy_qp, ib_qp_mod_t
*****/


/****f* Access Layer/ib_destroy_qp
* NAME
*	ib_destroy_qp
*
* DESCRIPTION
*	Release a queue pair.  Once destroyed, no further access to this
*	queue pair is possible.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_qp(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an existing queue pair.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the queue pair
*		has been successfully destroyed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The destroy request was registered.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
* NOTES
*	This call destroys an existing queue pair.  Since callbacks may be
*	outstanding against the queue pair at the time the destroy operation is
*	invoked, the this call operates asynchronously.  The user will be notified
*	through a callback once the destroy operation completes, indicating that
*	no additional callbacks will be invoked for the specified queue pair.
*
* SEE ALSO
*	ib_create_qp
*****/


/****s* Access Layer/ib_cq_create_t
* NAME
*	ib_cq_create_t
*
* DESCRIPTION
*	Attributes used to initialize a completion queue at creation time.
*
* SYNOPSIS
*/
typedef struct _ib_cq_create
{
	uint32_t								size;
	ib_pfn_comp_cb_t						pfn_comp_cb;
	cl_waitobj_handle_t						h_wait_obj;

}	ib_cq_create_t;
/*
* FIELDS
*	size
*		Specifies the maximum number of work completions that may be on the
*		completion queue.  If the creation call is successful, the actual
*		size of the completion queue will be returned.  The actual size of
*		the CQ will be greater than or equal to the requested size.
*
*	pfn_comp_cb
*		A callback that is invoked whenever a signaled completion occurs on
*		the completion queue.  This field is mutually exclusive with the
*		p_event field.
*
*	h_wait_obj
*		A wait object that is triggered whenever a signaled completion occurs
*		on the completion queue.  This field is mutually exclusive with the
*		pfn_comp_cb field and is only valid for user-mode clients.  The wait
*		object must be ready for use when the call to ib_create_cq is invoked.
*
* NOTES
*	Clients must specify either an event or a callback when creating a
*	completion queue.  When a signaled completion occurs on the completion
*	queue, the client will be notified through the callback or by
*	signaling the specified event.
*
* SEE ALSO
*	ib_create_cq, ib_pfn_comp_cb_t
*****/


/****f* Access Layer/ib_create_cq
* NAME
*	ib_create_cq
*
* DESCRIPTION
*	Creates a completion queue and returns its handle to the user.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_cq(
	IN		const	ib_ca_handle_t				h_ca,
	IN	OUT			ib_cq_create_t* const		p_cq_create,
	IN		const	void* const					cq_context,
	IN		const	ib_pfn_event_cb_t			pfn_cq_event_cb OPTIONAL,
		OUT			ib_cq_handle_t* const		ph_cq );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an open channel adapter.
*
*	p_cq_create
*		[in] Attributes necessary to allocate and initialize the
*		completion queue.
*
*	cq_context
*		[in] A user-specified context associated with the completion queue.
*
*	pfn_cq_event_cb
*		[in] User-specified error callback routine invoked after an
*		asynchronous event has occurred on the completion queue.
*
*	ph_cq
*		[out] Upon successful completion of this call, this references a
*		handle to the newly created completion queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The completion queue was successfully created.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the completion queue attributes or handle was not
*		provided.
*
*	IB_INVALID_SETTING
*		The specified attributes that should be used to create the completion
*		queue are invalid.  Both completion callback and wait object
*		information were supplied or are missing.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the completion queue.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the completion queue.
*
*	IB_INVALID_CQ_SIZE
*		The requested size of the completion queue was larger than the
*		maximum supported by the associated channel adapter.
*
* NOTES
*	This routine allocates a completion queue on the specified channel
*	adapter.  If the completion queue cannot be allocated, an error is
*	returned.  When creating the completion queue, users associate a context
*	with the completion queue.  This context is returned to the user through
*	the completion and asynchronous event callbacks.
*
* SEE ALSO
*	ib_query_cq, ib_modify_cq, ib_destroy_cq, ib_cq_create_t, ib_pfn_event_cb_t
*****/


/****f* Access Layer/ib_modify_cq
* NAME
*	ib_modify_cq
*
* DESCRIPTION
*	Modifies the attributes associated with a completion queue, allowing the
*	completion queue to be resized.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_modify_cq(
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			uint32_t* const				p_size );
/*
* PARAMETERS
*	h_cq
*		[in] A handle to an existing completion queue.
*
*	p_size
*		[in/out] Specifies the new size of the completion queue.  If the
*		modify call is successful, the actual size of the completion queue
*		will be returned.  The actual size of the CQ will be greater than or
*		equal to the requested size.
*
* RETURN VALUES
*	IB_SUCCESS
*		The completion queue was successfully modified.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the completion queue size was not provided.
*
*	IB_INVALID_CQ_SIZE
*		The requested size of the completion queue was larger than the
*		maximum supported by the associated channel adapter.
*
*	IB_OVERFLOW
*		The specified size of the completion queue is smaller than the number
*		of work completions currently on the completion queue.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to modify the completion queue.
*
* NOTES
*	This routine allows a client to modify the size of a completion queue.
*	If the new size is larger than what the associated channel adapter can
*	support, an error is returned.  If the completion queue has valid
*	completion entries on it and the requested size is smaller than the
*	number of entries, an overflow error is returned and the modify
*	operation is aborted.
*
* SEE ALSO
*	ib_create_cq
*****/


/****f* Access Layer/ib_query_cq
* NAME
*	ib_query_cq
*
* DESCRIPTION
*	Returns information about the specified completion queue.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_cq(
	IN		const	ib_cq_handle_t		h_cq,
	OUT		uint32_t* const				p_size );
/*
* PARAMETERS
*	h_cq
*		[in] A handle to an existing completion queue.
*
*	p_size
*		[out] Upon successful completion of this call, contains the actual
*		size of the completion queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The completion queue was successfully queried.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the completion queue size was not provided.
*
* SEE ALSO
*	ib_create_cq
*****/


/****f* Access Layer/ib_destroy_cq
* NAME
*	ib_destroy_cq
*
* DESCRIPTION
*	Destroys a completion queue.  Once destroyed, no further access to the
*	completion queue is possible.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_cq(
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an existing completion queue.
*
*	pfn_destroy_cb
*		[in] A user-provided callback that is invoked after the
*		completion queue has been successfully destroyed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The destroy request was registered.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
* NOTES
*	This call destroys an existing completion queue.  Since callbacks may be
*	outstanding against the completion queue at the time the destroy operation
*	is invoked, the this call operates asynchronously.  The user will be
*	notified through a callback once the destroy operation completes,
*	indicating that no additional callbacks will be invoked for the specified
*	completion queue.
*
*	If there are still queue pairs associated with the completion queue when
*	this function is invoked, the destroy operation will fail with status
*	IB_RESOURCE_BUSY.
*
* SEE ALSO
*	ib_create_cq, ib_pfn_destroy_cb_t
*****/


/****f* Access Layer/ib_reg_mem
* NAME
*	ib_reg_mem
*
* DESCRIPTION
*	Registers a virtual memory region with a channel adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_mem(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_mr_create_t* const		p_mr_create,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an existing protection domain that the memory
*		should be registered with.
*
*	p_mr_create
*		[in] Information describing the memory region to register.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing
*		RDMA or atomic operations to this registered memory region.
*
*	ph_mr
*		[out] Upon successful completion of this call, this references a
*		handle to the registered memory region.  This handle is used when
*		performing data transfers and to deregister the memory.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region was successfully registered.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain to associate with the memory region was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the memory region information, lkey, rkey, or handle
*		was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the memory region.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
* NOTES
*	This routine registers a virtual memory region with a channel adapter.
*	Memory must be registered before being used in a data transfer operation.
*
* SEE ALSO
*	ib_dereg_mr, ib_reg_phys, ib_reg_shared, ib_mr_create_t
*****/


/****f* Access Layer/ib_reg_phys
* NAME
*	ib_reg_phys
*
* DESCRIPTION
*	Registers a physical memory region with a channel adapter.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_phys(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_phys_create_t* const		p_phys_create,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an existing protection domain that the memory
*		should be registered with.
*
*	p_phys_create
*		[in] Information describing the memory region to register.
*
*	p_vaddr
*		[in/out] On input, references the requested virtual address for the
*		start of the physical region.  On output, references the actual
*		virtual address assigned to the registered region.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing
*		RDMA or atomic operations to this registered memory region.
*
*	ph_mr
*		[out] Upon successful completion of this call, this references a
*		handle to the registered memory region.  This handle is used when
*		performing data transfers and to deregister the memory.
*
* RETURN VALUES
*	IB_SUCCESS
*		The physical memory region was successfully registered.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain to associate with the physical memory region
*		was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the physical memory region information, virtual address,
*		lkey, rkey, or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the physical memory region.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the physical memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
* NOTES
*	This routine registers an array of physical pages as a single virtually
*	contiguous region with a channel adapter.  Memory must be registered
*	before being used in a data transfer operation.
*
* SEE ALSO
*	ib_dereg_mr, ib_reg_mem, ib_reg_shared, ib_phys_create_t
*****/


/****f* Access Layer/ib_query_mr
* NAME
*	ib_query_mr
*
* DESCRIPTION
*	Query the current attributes of a memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_mr(
	IN		const	ib_mr_handle_t				h_mr,
		OUT			ib_mr_attr_t* const			p_mr_attr );
/*
* PARAMETERS
*	h_mr
*		[in] A handle to a registered memory region.
*
*	p_mr_attr
*		[out] A reference to a structure where the registered memory attributes
*		will be copied.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were returned successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the memory region attributes was not provided.
*
* NOTES
*	This routine returns information about the specified registered memory
*	region.
*
* SEE ALSO
*	ib_dereg_mr, ib_reg_mem, ib_reg_shared, ib_mr_attr_t
*****/


/****f* Access Layer/ib_rereg_mem
* NAME
*	ib_rereg_mem
*
* DESCRIPTION
*	Modifies the attributes of an existing memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_rereg_mem(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_mod_mask,
	IN		const	ib_mr_create_t* const		p_mr_create OPTIONAL,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL );
/*
* PARAMETERS
*	h_mr
*		[in] A handle to the registered memory region being modified.
*
*	mr_mod_mask
*		[in] A mask used to specify which attributes of the memory region are
*		being modified.
*
*	p_mr_create
*		[in] This references information needed to perform the modification on
*		the registered memory region.  This parameter may be NULL if only the
*		protection domain will be modified.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing RDMA
*		or atomic operations to this registered memory region.
*
*	h_pd
*		[in] An optionally provided parameter used to modify the protection
*		domain of a registered region.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This routine modifies the attributes of the specified memory region.
*	The memory being modified may have been registered using either virtual
*	or physical registration.  Conceptually, this routine is equivalent to
*	to calling ib_dereg_mr, followed by ib_reg_mem, but may be higher
*	performing.
*
* SEE ALSO
*	ib_reg_mem, ib_reg_phys, ib_dereg_mr, ib_mr_mod_t, ib_mr_create_t
*****/


/****f* Access Layer/ib_rereg_phys
* NAME
*	ib_rereg_phys
*
* DESCRIPTION
*	Modifies the attributes of an existing memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_rereg_phys(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_mod_mask,
	IN		const	ib_phys_create_t* const		p_phys_create OPTIONAL,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL );
/*
* PARAMETERS
*	h_mr
*		[in] A handle to the registered memory region being modified.
*
*	mr_mod_mask
*		[in] A mask used to specify which attributes of the memory region are
*		being modified.
*
*	p_phys_create
*		[in] This references information needed to perform the modification on
*		the registered memory region.  This parameter may be NULL if
*		only the protection domain will be modified.
*
*	p_vaddr
*		[in/out] On input, this specifies the requested virtual address for the
*		start of the physical region.  On output, this references the actual
*		virtual address assigned to the registered region.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing RDMA
*		or atomic operations to this registered memory region.
*
*	h_pd
*		[in] An optionally provided parameter used to modify the protection
*		domain of a registered region.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the virtual address, lkey, rkey was not provided or
*		the specified modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This routine modifies the attributes of the specified memory region.
*	The memory being modified may have been registered using either virtual
*	or physical registration.  Conceptually, this routine is equivalent to
*	to calling ib_dereg_mr, followed by ib_reg_phys, but may be higher
*	performing.
*
* SEE ALSO
*	ib_reg_mem, ib_reg_phys, ib_dereg_mr, ib_mr_mod_t, ib_mr_create_t
*****/


/****f* Access Layer/ib_reg_shared
* NAME
*	ib_reg_shared
*
* DESCRIPTION
*	Registers a memory region that has the same physical pages as an
*	existing registered memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_shared(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_access_t					access_ctrl,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );
/*
* PARAMETERS
*	h_mr
*		[in] A handle to an existing registered memory region that this
*		registration should share physical pages with.
*
*	h_pd
*		[in] Handle to the PD on which memory is being registered
*
*	access_ctrl
*		[in] Access rights of the registered region.
*
*	p_vaddr
*		[in/out] On input, this specifies the requested virtual address for the
*		start of the physical region.  On output, this references the actual
*		virtual address assigned to the registered region.  This is always a
*		64-bit quantity to support registering more than 4GB of memory on
*		32-bit systems with PAE.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing RDMA
*		or atomic operations to this registered memory region.
*
*	ph_mr
*		[out] Upon successful completion of this call, this references a handle
*		to the registered memory region.  This handle is used when performing
*		data transfers and to deregister the memory.
*
* RETURN VALUES
*	IB_SUCCESS
*		The shared memory region was successfully registered.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the virtual address, lkey, rkey, or handle was not
*		provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the shared memory region.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the shared memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
* NOTES
*	This routine registers a memory region that shares the same set of
*	physical pages associated with an existing registered memory region.
*
* SEE ALSO
*	ib_dereg_mr, ib_reg_mem, ib_reg_phys, ib_reg_shared, ib_mr_create_t
*****/


/****f* Access Layer/ib_reg_shmid
* NAME
*	ib_reg_shmid
*
* DESCRIPTION
*	Registers a memory region to be shared across multiple processes.
*	The memory is referenced by a shared memory identifier.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_shmid(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_shmid_t					shmid,
	IN		const	ib_mr_create_t* const		p_mr_create,
		OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an existing protection domain that the memory
*		should be registered with.
*
*	shmid
*		[in] An identifier for the shared memory region.
*
*	p_mr_create
*		[in] Information describing the attributes of the memory region to
*		register.
*
*	p_vaddr,
*		[out] The HCA assigned, HCA relative virtual address for the
*		memory region.
*
*	p_lkey
*		[out] The local access key associated with this registered memory
*		region.
*
*	p_rkey
*		[out] A key that may be used by a remote end-point when performing RDMA
*		or atomic operations to this registered memory region.
*
*	ph_mr
*		[out] Upon successful completion of this call, this references a handle
*		to the registered memory region.  This handle is used when performing
*		data transfers and to deregister the memory.
*
* RETURN VALUES
*	IB_SUCCESS
*		The shared memory region was successfully registered.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the memory region information, lkey, rkey, or handle
*		was not provided.
*
*	IB_INVALID_SETTING
*		The length and page mapping for the memory region do not match those
*		of the region identified by the provided SHMID.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the shared memory region.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the shared memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
* NOTES
*	This routine registers a memory region that is shared between processes.
*	The region being registered is identified through a shared memory
*	identifier.  The registered region shares hardware resources as much
*	as possible.
*
* SEE ALSO
*	ib_dereg_mr, ib_reg_mem, ib_reg_shared, ib_mr_create_t
*****/


/****f* Access Layer/ib_dereg_mr
* NAME
*	ib_dereg_mr
*
* DESCRIPTION
*	Deregisters a registered memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_dereg_mr(
	IN		const	ib_mr_handle_t				h_mr );
/*
* PARAMETERS
*	h_mr
*		[in] A handle to a registered memory region that will be unregistered.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region was successfully deregistered.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has memory windows bound to it.
*
* NOTES
*	This routine deregisters a memory region with a channel adapter.  The
*	region may be deregistered only if there are no memory  windows or
*	existing shared memory regions currently bound to the region.  Work
*	requests referencing this region when it is deregistered will fail
*	with a WRS_LOCAL_PROTECTION_ERR error.
*
* SEE ALSO
*	ib_reg_mem, ib_reg_phys, ib_reg_shared
*****/


#ifdef CL_KERNEL

/****f* Access Layer/mlnx_create_fmr
* NAME
*	mlnx_create_fmr
*
* DESCRIPTION
*	Creates a Mellanox fast memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_create_fmr(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	mlnx_fmr_create_t* const	p_fmr_create,
		OUT			mlnx_fmr_handle_t* const	ph_fmr );
/*
* PARAMETERS
*	h_pd
*		[in] An optionally provided parameter used to modify the protection
*		domain of a registered region.
*	p_fmr_create
*		[in] This references information needed to perform the modification on
*		the registered memory region.  This parameter may be NULL if only the
*		protection domain will be modified.
*	ph_fmr
*		[out] A handle to the registered memory region being modified.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	mlnx_destroy_fmr, mlnx_fmr_create_t
*****/


/****f* Access Layer/mlnx_map_fmr
* NAME
*	mlnx_map_fmr
*
* DESCRIPTION
*	//TODO
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_map_phys_fmr(
	IN		const	mlnx_fmr_handle_t			h_fmr,
	IN		const	uint64_t* const				paddr_list,
	IN		const	int							list_len,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey );
/*
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
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	mlnx_destroy_fmr, mlnx_fmr_create_t
*****/


/****f* Access Layer/mlnx_unmap_fmr
* NAME
*	mlnx_unmap_fmr
*
* DESCRIPTION
*	//TODO
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_unmap_fmr(
	IN		const	mlnx_fmr_handle_t			h_fmr );
/*
* PARAMETERS
*	h_fmr
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	mlnx_destroy_fmr, mlnx_fmr_create_t
*****/


/****f* Access Layer/mlnx_destroy_fmr
* NAME
*	mlnx_destroy_fmr
*
* DESCRIPTION
*	Destroys an existing Mellanox fast memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_destroy_fmr(
	IN		const	mlnx_fmr_handle_t			h_fmr );
/*
* PARAMETERS
*	h_fmr
*		[in] A handle to the registered memory region being modified.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory region attributes were modified successfully.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the lkey or rkey was not provided or the specified
*		modify mask is invalid.
*
*	IB_INVALID_SETTING
*		The specified memory region attributes are invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to modify the memory region.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_RESOURCE_BUSY
*		The memory region has windows bound to it.
*
* NOTES
*	This is a Mellanox specific extension to verbs.
*
* SEE ALSO
*	mlnx_destroy_fmr, mlnx_fmr_create_t
*****/


/****f* Access Layer/ib_create_fmr_pool
* NAME
*	ib_create_fmr_pool
*
* DESCRIPTION
*	Creates a pool of FMR elements for use 
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_create_fmr_pool(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	mlnx_fmr_pool_create_t		*p_fmr_pool_attr,
	OUT		mlnx_fmr_pool_handle_t* const		ph_pool );
/*
* PARAMETERS
TODO
*
* RETURN VALUES
TODO
*
* NOTES
TODO
*
* SEE ALSO
TOD
*****/


/****f* Access Layer/ib_destroy_fmr_pool
* NAME
*	ib_destroy_fmr_pool
*
* DESCRIPTION
*	Destroys a MAD pool and all associated resources.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_destroy_fmr_pool(
	IN		const	mlnx_fmr_pool_handle_t			h_pool );
/*
* PARAMETERS
TODO
*
* RETURN VALUES
TODO
*
* NOTES
TODO
*
* SEE ALSO
TODO
*****/







/****f* Access Layer/ib_fmr_pool_map_phys
* NAME
*	ib_destroy_fmr_pool
*
* DESCRIPTION
*	Destroys a MAD pool and all associated resources.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_map_phys_fmr_pool(
	IN		const	mlnx_fmr_pool_handle_t		h_pool ,
	IN		const	uint64_t* const				paddr_list,
	IN		const	int							list_len,
	IN	OUT			uint64_t* const				p_vaddr,
	OUT		net32_t* const					p_lkey,
	OUT		net32_t* const					p_rkey,
	OUT				mlnx_fmr_pool_el_t		*pp_fmr_el);
/*
* PARAMETERS
TODO
*
* RETURN VALUES
TODO
*
* NOTES
TODO
*
* SEE ALSO
TODO
*****/





/****f* Access Layer/ib_destroy_fmr_pool
* NAME
*	ib_destroy_fmr_pool
*
* DESCRIPTION
*	Destroys a MAD pool and all associated resources.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
mlnx_unmap_fmr_pool(
	IN			mlnx_fmr_pool_el_t			p_fmr_el );
/*
* PARAMETERS
TODO
*
* RETURN VALUES
TODO
*
* NOTES
TODO
*
* SEE ALSO
TODO
*****/


/****f* Access Layer/ib_flush_fmr_pool
* NAME
*	ib_flush_fmr_pool
*
* DESCRIPTION
*	Destroys a MAD pool and all associated resources.
*
* SYNOPSIS
*/
ib_api_status_t
mlnx_flush_fmr_pool(mlnx_fmr_pool_handle_t  h_pool); 
/*
* PARAMETERS
TODO
*
* RETURN VALUES
TODO
*
* NOTES
TODO
*
* SEE ALSO
TODO
*****/
#endif	/* CL_KERNEL */

/****f* Access Layer/ib_create_mw
* NAME
*	ib_create_mw
*
* DESCRIPTION
*	Creates a memory window associated with the specified protection domain.
*	Newly created windows are not bound to any specific memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_mw(
	IN		const	ib_pd_handle_t				h_pd,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mw_handle_t* const		ph_mw );
/*
* PARAMETERS
*	h_pd
*		[in] A handle to an existing protection domain that the memory window
*		should be created within.
*
*	p_rkey
*		[out] The current rkey associated with the memory window.  This key is
*		used to bind the window to a registered memory region.
*
*	ph_mw
*		[out] Upon successful completion of this call, this references a handle
*		to the memory window.  This handle is used to bind and destroy
*		the window.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory window was successfully created.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the memory window rkey or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the memory window.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to create the memory window.
*
* NOTES
*	This routine creates an unbound memory window associated with a specified
*	protection domain.  The memory window cannot be used for data transfer
*	operations until being bound to a registered memory region.
*
* SEE ALSO
*	ib_destroy_mw, ib_query_mw, ib_bind_mw
*****/


/****f* Access Layer/ib_query_mw
* NAME
*	ib_query_mw
*
* DESCRIPTION
*	Query the current attributes of a memory window.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query_mw(
	IN		const	ib_mw_handle_t				h_mw,
		OUT			ib_pd_handle_t* const		ph_pd,
		OUT			net32_t* const				p_rkey );
/*
* PARAMETERS
*	h_mw
*		[in] A handle to an existing memory window.
*
*	ph_pd
*		[out] Upon successful completion of this call, this will reference
*		the protection domain associated with this memory window.
*
*	p_rkey
*		[out] Upon successful completion of this call, this will reference
*		the current rkey associated with this memory window.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory window attributes were returned successfully.
*
*	IB_INVALID_MW_HANDLE
*		The memory window handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the protection domain handle or rkey was not provided.
*
* NOTES
*	This routine returns information about the specified memory window.
*
* SEE ALSO
*	ib_create_mw
*****/


/****f* Access Layer/ib_bind_mw
* NAME
*	ib_bind_mw
*
* DESCRIPTION
*	Binds a memory window to a registered memory region.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_bind_mw(
	IN		const	ib_mw_handle_t				h_mw,
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_bind_wr_t* const			p_mw_bind,
		OUT			net32_t* const				p_rkey );
/*
* PARAMETERS
*	h_mw
*		[in] A handle to an existing memory window.
*
*	h_qp
*		[in] A handle to a queue pair that the bind request will be posted to.
*
*	p_mw_bind
*		[in] Describes the memory window bind request.
*
*	p_rkey
*		[out] The new rkey for the memory window that may be used by a remote
*		end-point when performing RDMA or atomic operations to this memory
*		region.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory window bind operation was successfully posted.
*
*	IB_INVALID_MW_HANDLE
*		The memory window handle was invalid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the memory window bind work request or rkey was not
*		provided.
*
*	IB_INVALID_SERVICE_TYPE
*		The queue pair configuration does not support this type of service.
*
*	IB_INVALID_MR_HANDLE
*		The memory region handle was invalid.
*
*	IB_INVALID_RKEY
*		The rkey is invalid for the memory region being bound.
*
*	IB_UNSUPPORTED
*		The requested access rights are not supported by the channel adapter.
*
*	IB_INVALID_PERMISSION
*		The requested access rights are invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to bind the memory window.
*
* NOTES
*	This routine posts a request to bind a memory window to a registered
*	memory region.  The bind operation occurs on the specified queue pair,
*	but the bound region is usable across all queue pairs within the same
*	protection domain.
*
* SEE ALSO
*	ib_create_mw, ib_bind_wr_t
*****/


/****f* Access Layer/ib_destroy_mw
* NAME
*	ib_destroy_mw
*
* DESCRIPTION
*	Destroys a memory window.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_mw(
	IN		const	ib_mw_handle_t				h_mw );
/*
* PARAMETERS
*	h_mw
*		[in] A handle to an existing memory window.
*
* RETURN VALUES
*	IB_SUCCESS
*		The memory window was successfully destroyed.
*
*	IB_INVALID_MW_HANDLE
*		The memory window handle was invalid.
*
* NOTES
*	This routine deallocates a window entry created via a ib_create_mw.
*	Once this operation is complete, future accesses to the window will fail.
*
* SEE ALSO
*	ib_create_mw
*****/


/****f* Access Layer/ib_post_send
* NAME
*	ib_post_send
*
* DESCRIPTION
*	This routine posts a work request to the send queue of a queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_post_send(
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_send_wr_t* const			p_send_wr,
		OUT			ib_send_wr_t				**pp_send_failure OPTIONAL );
/*
* PARAMETERS
*	h_qp
*		[in] The queue pair to which this work request is being submitted.
*
*	p_send_wr
*		[in] A reference to the head of the work request list.
*
*	pp_send_failure
*		[out] If the post send operation failed, this references the work
*		request in the p_send_wr list where the first failure occurred.
*		This parameter may be NULL if only a single work request is being
*		posted to the QP.
*
* RETURN VALUES
*	IB_SUCCESS
*		All work requests were successfully posted.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the send work request list was not provided.
*
*	IB_INSUFFICIENT_RESOURCES
*		The number of posted work requests exceed the current depth available
*		on the send queue.
*
*	IB_INVALID_WR_TYPE
*		The work request type was invalid.
*
*	IB_INVALID_QP_STATE
*		The current queue pair state does not allow posting sends.
*
*	IB_INVALID_MAX_SGE
*		The number of work request scatter gather elements exceed the queue
*		pair configuration.
*
*	IB_UNSUPPORTED
*		The requested operation is not supported by the channel adapter.
*
* NOTES
*	This routine posts a work request to the send queue of a queue pair.
*	The type of work to perform is defined by the p_send_wr parameter.
*
* SEE ALSO
*	ib_send_wr_t
*****/


/****f* Access Layer/ib_post_recv
* NAME
*	ib_post_recv
*
* DESCRIPTION
*	This routine posts a work request to the receive queue of a queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_post_recv(
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_recv_wr_t* const			p_recv_wr,
		OUT			ib_recv_wr_t				**pp_recv_failure OPTIONAL );
/*
* PARAMETERS
*	h_qp
*		[in] The queue pair to which this work request is being submitted.
*
*	p_recv_wr
*		[in] A reference to the head of the work request list.
*
*	pp_recv_failure
*		[out] If the post receive operation failed, this references the work
*		request in the p_recv_wr list where the first failure occurred.
*		This parameter may be NULL if only a single work request is being
*		posted to the QP.
*
* RETURN VALUES
*	IB_SUCCESS
*		All work requests were successfully posted.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the receive work request list was not provided.
*
*	IB_INSUFFICIENT_RESOURCES
*		The number of posted work requests exceed the current depth available
*		on the receive queue.
*
*	IB_INVALID_WR_TYPE
*		The work request type was invalid.
*
*	IB_INVALID_QP_STATE
*		The current queue pair state does not allow posting receives.
*
* NOTES
*	This routine posts a work request to the receive queue of a queue pair.
*	The type of work to perform is defined by the p_recv_wr parameter.  This
*	call is used to post data buffers to receive incoming message sends.
*
* SEE ALSO
*	ib_recv_wr_t
*****/


/****f* Access Layer/ib_send_mad
* NAME
*	ib_send_mad
*
* DESCRIPTION
*	This routine posts a work request to the send queue of a queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_send_mad(
	IN		const	ib_mad_svc_handle_t			h_mad_svc,
	IN				ib_mad_element_t* const		p_mad_element_list,
		OUT			ib_mad_element_t			**pp_mad_failure OPTIONAL );
/*
* PARAMETERS
*	h_mad_svc
*		[in] The MAD service to which this work request is being submitted.
*
*	p_mad_element_list
*		[in] A list of MAD elements that will be posted to the send queue.
*
*	pp_mad_failure
*		[out] If the send MAD operation failed, this references the MAD
*		element in the p_mad_element_list where the first failure occurred.
*		This parameter is optional if p_mad_element_list contains a single
*		MAD.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD element list was successfully posted.
*
*	IB_INVALID_HANDLE
*		The MAD service handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the MAD element list was not provided.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available to complete
*		the request.
*
*	IB_INVALID_SETTING
*		The MAD element RMPP version is not supported by the access layer.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to complete the request.
*
* NOTES
*	This routine posts a work request to send a MAD on a MAD service.  All
*	MAD elements successfully posted by this call are under the control of
*	the access layer and should not be accessed until the send operation
*	completes.
*
*	In order to guarantee that MADs sent by separate clients do not use the
*	same transaction ID, the access layer reserves the upper 32-bits of the
*	TID on all unsolicited MADs.  MADs sent with the response bit set will
*	not have their transaction ID's modified.  Unsolicited MADs will have the
*	upper 32-bits of their TID set to an access layer generated client ID.
*
* SEE ALSO
*	ib_mad_element_t, ib_cancel_mad
*****/


/****f* Access Layer/ib_cancel_mad
* NAME
*	ib_cancel_mad
*
* DESCRIPTION
*	This routine cancels a pending send transaction to a MAD service.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cancel_mad(
	IN		const	ib_mad_svc_handle_t			h_mad_svc,
	IN				ib_mad_element_t* const		p_mad_element );
/*
* PARAMETERS
*	h_mad_svc
*		[in] The MAD service to which the send operation was directed.
*
*	p_mad_element
*		[in] A handle to a sent MAD element.
*
* RETURN VALUES
*	IB_SUCCESS
*		The requested MAD transaction was located and canceled.
*
*	IB_INVALID_PARAMETER
*		A reference to the MAD element list was not provided.
*
*	IB_NOT_FOUND
*		The requested transaction was not located or had already completed.
*
* NOTES
*	This routine cancels a pending send transaction to a MAD service.  If
*	the request is successfully located and has not yet completed, it will
*	be completed with its status set to IB_CANCELED.  The canceled operation
*	will be returned to the user through the normal MAD completion callback.
*	If the send transaction has already completed, this call will return
*	IB_NOT_FOUND.
*
* SEE ALSO
*	ib_send_mad
*****/


/****f* Access Layer/ib_peek_cq
* NAME
*	ib_peek_cq
*
* DESCRIPTION
*	Returns the number of entries currently on the completion queue.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_peek_cq(
	IN		const	ib_cq_handle_t				h_cq,
	OUT				uint32_t* const				p_n_cqes );
/*
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue to peek.
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
*	IB_UNSUPPORTED
*		This operation is not supported by the channel adapter.
*
* NOTES
*	The value returned is a snapshot of the number of compleiton queue
*	entries curently on the completion queue.  Support for this operation
*	is optional by a channel adapter vendor.
*
* SEE ALSO
*	ib_create_cq, ib_poll_cq, ib_rearm_cq, ib_rearm_n_cq
*****/


/****f* Access Layer/ib_poll_cq
* NAME
*	ib_poll_cq
*
* DESCRIPTION
*	Checks a completion queue for completed work requests.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_poll_cq(
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			ib_wc_t** const				pp_free_wclist,
		OUT			ib_wc_t** const				pp_done_wclist );
/*
* PARAMETERS
*	h_cq
*		[in] A handle to a completion queue to check for completions on.
*
*	pp_free_wclist
*		[in/out] On input, a list of work completion structures provided by
*		the client.  These are used to report completed work requests through
*		the pp_done_wclist.
*
*		On output, this contains the list of work completions structures for
*		which no work completion was found.
*
*	pp_done_wclist
*		[out] A list of work completions retrieved from the completion queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The poll operation completed successfully.  If the work completion
*		structures referenced by the pp_free_wclist list is empty there are
*		potentially more completions available to retrieve.
*
*	IB_INVALID_PARAMETER
*		A reference to the free or done work completion list was not provided.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
*	IB_NOT_FOUND
*		No completed work requests were removed from the completion queue.
*
* NOTES
*	This routine retrieves completed work requests from the specified
*	completion queue.  This call will retrieve all completed requests,
*	up to to the number of work completion structures referenced by the
*	pp_free_wclist.  Completed requests will be returned through the
*	pp_done_wclist parameter.
*
* SEE ALSO
*	ib_create_cq, ib_post_send, ib_post_recv, ib_bind_mw, ib_wc_t
*****/


/****f* Access Layer/ib_rearm_cq
* NAME
*	ib_rearm_cq
*
* DESCRIPTION
*	This indicates that the completion queue should notify the client when
*	the next completion is added.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_rearm_cq(
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	boolean_t					solicited );
/*
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue to rearm.
*
*	solicited
*		[in] A flag indicating whether the request is to generate a
*		notification on the next entry, if set to FALSE, or on the next
*		solicited entry being added to the completion queue, if set to TRUE.
*
* RETURN VALUES
*	IB_SUCCESS
*		The completion queue rearm request was registered successfully.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
* NOTES
*	This routine instructs the channel interface to invoke the completion
*	handler when the next completion queue entry is added to this CQ.
*
* SEE ALSO
*	ib_create_cq, ib_peek_cq, ib_poll_cq, ib_rearm_n_cq
*****/


/****f* Access Layer/ib_rearm_n_cq
* NAME
*	ib_rearm_n_cq
*
* DESCRIPTION
*	This indicates that the completion queue should notify the client when
*	the next N completions have been added to this CQ.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_rearm_n_cq(
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	uint32_t					n_cqes );
/*
* PARAMETERS
*	h_cq
*		[in] Handle to the completion queue to rearm.
*
*	n_cqes
*		[in] The number of completion queue entries to be added to the
*		completion queue before notifying the client.  This value must
*		greater than or equal to one and less than or equal to the size
*		of the completion queue.
*
* RETURN VALUES
*	IB_SUCCESS
*		The completion queue rearm request was registered successfully.
*
*	IB_INVALID_CQ_HANDLE
*		The completion queue handle was invalid.
*
*	IB_INVALID_PARAMETER
*		The requested number of completion queue entries was invalid.
*
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
*	ib_create_cq, ib_peek_cq, ib_poll_cq, ib_rearm_cq
*****/


/****s* Access Layer/ib_mcast_rec_t
* NAME
*	ib_mcast_rec_t
*
* DESCRIPTION
*	Information returned as a result of joining a multicast group.
*
* SYNOPSIS
*/
typedef struct _ib_mcast_rec
{
	TO_LONG_PTR(const void*,	mcast_context);
	ib_api_status_t				status;
	ib_net16_t					error_status;

	TO_LONG_PTR(ib_mcast_handle_t, h_mcast);
	TO_LONG_PTR(ib_member_rec_t*, p_member_rec);

}	ib_mcast_rec_t;
/*
* FIELDS
*	mcast_context
*		User-defined context information associated with the multicast join
*		request.
*
*	status
*		Indicates the success of the multicast group join operation.
*
*	error_status
*		Provide additional error information that was provided by the SA.
*		This field is only valid if status is set to IB_REMOTE_ERROR.
*
*	h_mcast
*		Upon successful completion of a multicast join, this references a
*		handle to the multicast group.  This handle is used to leave the
*		multicast group.
*
*	p_member_rec
*		References a member record that provides information about the
*		multicast group.
*
* NOTES
*	This structure is returned to a client through a callback to notify them
*	of the result of a multicast join operation.
*
* SEE ALSO
*	ib_join_mcast, ib_pfn_mcast_cb_t, ib_leave_mcast
*****/


/****f* Access Layer/ib_pfn_mcast_cb_t
* NAME
*	ib_pfn_mcast_cb_t
*
* DESCRIPTION
*	User-defined callback invoked on completion of a multicast join request.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_mcast_cb_t)(
	IN				ib_mcast_rec_t				*p_mcast_rec );
/*
* PARAMETERS
*	p_mcast_rec
*		[in] References the result of the join operation.
*
* NOTES
*	The callback is used to notify a client of the result of a multicast
*	join request.
*
*	This callback is invoked within a system thread context in the kernel.
*
* SEE ALSO
*	ib_join_mcast, ib_mcast_rec_t
*****/


/****s* Access Layer/ib_mcast_req_t
* NAME
*	ib_mcast_req_t
*
* DESCRIPTION
*	Information used to join a multicast group.
*
* SYNOPSIS
*/
typedef struct _ib_mcast_req
{
	boolean_t					create;
	ib_member_rec_t				member_rec;

	const void*					mcast_context;
	ib_pfn_mcast_cb_t			pfn_mcast_cb;

	uint32_t					timeout_ms;
	uint32_t					retry_cnt;
	ib_al_flags_t				flags;

	ib_net64_t					port_guid;
	uint16_t					pkey_index;

}	ib_mcast_req_t;
/*
* FIELDS
*	create
*		Indicates that the multicast group should be created if it does not
*		already exist.
*
*	member_rec
*		Specifies the membership information of the multicast group to join
*		or create.  The mgid and join state (scope_state) fields of the
*		member record must be set.  In addition, if create is set to TRUE,
*		the following fields must also be set: qkey, tclass, service level
*		and flow label (sl_flow_hop), and pkey.  All other fields are ignored
*		by the access layer.
*
*	mcast_context
*		User-defined context information associated with the join request.
*		This context is returned to the user through the function specified
*		by the pfn_mcast_cb field.
*
*	pfn_mcast_cb
*		A user-defined callback that is invoked upon completion of the
*		join request.
*
*	timeout_ms
*		Specifies the number of milliseconds to wait for a response for
*		the join request until retrying or timing out the request.
*
*	retry_cnt
*		Specifies the number of times that the join request will be retried
*		before failing the request.
*
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	port_guid
*		Indicates the port that will join the multicast group.  The QP
*		specified as part of the ib_join_mast call will bind to this port.
*
*	pkey_index
*		Specifies the pkey associated with this queue pair.
*
* NOTES
*	This structure is used when joining an existing multicast group or
*	creating a new multicast group.
*
* SEE ALSO
*	ib_join_mcast, ib_pfn_mcast_cb_t, ib_gid_t
*****/


/****f* Access Layer/ib_join_mcast
* NAME
*	ib_join_mcast
*
* DESCRIPTION
*	Attaches a queue pair to a multicast group.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_join_mcast(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_mcast_req_t* const		p_mcast_req );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an unreliable datagram queue pair that will join the
*		multicast group.
*
*	p_mcast_req
*		[in] Specifies the multicast group to join.
*
* RETURN VALUES
*	IB_SUCCESS
*		The join multicast group request has been initiated.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the multicast group request information was not
*		provided.
*
*	IB_INVALID_SERVICE_TYPE
*		The queue pair configuration does not support this type of service.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to join the multicast group.
*
*	IB_INVALID_GUID
*		No port was found for the port_guid specified in the request.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
*	IB_INVALID_PKEY
*		The pkey specified in the multicast join request does not match the
*		pkey of the queue pair.
*
*	IB_INVALID_PORT
*		The port GUID specified in the multicast join request does not match
*		the port of the queue pair.
*
*	IB_ERROR
*		An error occurred while performing the multicast group join operation.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available to complete
*		the request.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to complete the request.
*
* NOTES
*	This routine results in the specified queue pair joining a multicast
*	group.  If the multicast group does not already exist, it will be created
*	at the user's option.  Information about the multicast group is returned
*	to the user through a callback specified through the p_mcast_req
*	parameter.
*
*	If the specified queue pair is already a member of a multicast group when
*	this call is invoked, an error will occur if there are conflicting
*	membership requirements.  The QP is restricted to being bound to a single
*	port_guid and using a single pkey.
*
* SEE ALSO
*	ib_leave_mcast, ib_mcast_req_t, ib_create_qp, ib_init_dgrm_svc
*****/


/****f* Access Layer/ib_leave_mcast
* NAME
*	ib_leave_mcast
*
* DESCRIPTION
*	Removes a queue pair from a multicast group.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_leave_mcast(
	IN		const	ib_mcast_handle_t			h_mcast,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_mcast
*		[in] A handle to a joined multicast group.
*
*	pfn_destroy_cb
*		[in] An optional user-specified callback that is invoked after the
*		leave request has completed.
*
* RETURN VALUES
*	IB_SUCCESS
*		The leave multicast group request has been initiated.
*
*	IB_INVALID_MCAST_HANDLE
*		The multicast group handle was invalid.
*
*	IB_ERROR
*		An error occurred while performing the multicast group leave operation.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine detaches a queue pair from a multicast group and removes
*	it as a member of the group with the subnet administrator.
*
* SEE ALSO
*	ib_join_mcast, ib_pfn_destroy_cb_t
*****/


/****f* Access Layer/ib_local_mad
* NAME
*	ib_local_mad
*
* DESCRIPTION
*	Request that a locally received MAD be processed by the channel adapter
*	on which it was received.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_local_mad(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	uint8_t						port_num,
	IN		const	void* const					p_mad_in,
		OUT			void*						p_mad_out );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to the channel adapter that should process the MAD.
*		This must be the same adapter that the MAD was received on.
*
*	port_num
*		[in] The port number to which this request is directed.
*
*	p_mad_in
*		[in] Pointer to a management datagram (MAD) structure containing
*		the command to be processed.
*
*	p_mad_out
*		[out] References a MAD that should contain the response to the
*		received input MAD specified through the p_mad_in parameter.
*
* RETURN VALUES
*	IB_SUCCESS
*		The local MAD was processed successfully.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INVALID_PORT
*		The port number was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the MAD input or MAD output buffer was not provided.
*
* NOTES
*	This call is provided to support SMA and GSA implementations above the
*	verbs interface on ports that the access layer has disabled.  This routine
*	is used to perform local operations by the channel adapter.  On successful
*	return, the provide output MAD should be used when sending a response.
*
* SEE ALSO
*	ib_query_ca, ib_ca_attr_t
*****/


/****s* Access Layer/ib_req_pdata_t
* NAME
*	ib_req_pdata_t
*
* DESCRIPTION
*	User data sent as part of a request for communication.
*
* SYNOPSIS
*/
typedef union _ib_req_pdata
{
	uint8_t						data[IB_REQ_PDATA_SIZE];

}	ib_req_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_rep_pdata_t
* NAME
*	ib_rep_pdata_t
*
* DESCRIPTION
*	User data sent as part of a reply to a request for communication.
*
* SYNOPSIS
*/
typedef union _ib_rep_pdata
{
	uint8_t						data[IB_REP_PDATA_SIZE];

}	ib_rep_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_rtu_pdata_t
* NAME
*	ib_rtu_pdata_t
*
* DESCRIPTION
*	User data sent as part of a ready to use message.
*
* SYNOPSIS
*/
typedef union _ib_rtu_pdata
{
	uint8_t						data[IB_RTU_PDATA_SIZE];

}	ib_rtu_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_rej_pdata_t
* NAME
*	ib_rej_pdata_t
*
* DESCRIPTION
*	User data sent as part of a connection reject message.
*
* SYNOPSIS
*/
typedef union _ib_rej_pdata
{
	uint8_t						data[IB_REJ_PDATA_SIZE];

}	ib_rej_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_mra_pdata_t
* NAME
*	ib_mra_pdata_t
*
* DESCRIPTION
*	User data sent as part of a message receipt acknowledgement.
*
* SYNOPSIS
*/
typedef union _ib_mra_pdata
{
	uint8_t						data[IB_MRA_PDATA_SIZE];

}	ib_mra_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_lap_pdata_t
* NAME
*	ib_lap_pdata_t
*
* DESCRIPTION
*	User data sent as part of a load alternate path message.
*
* SYNOPSIS
*/
typedef union _ib_lap_pdata
{
	uint8_t						data[IB_LAP_PDATA_SIZE];

}	ib_lap_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_apr_pdata_t
* NAME
*	ib_apr_pdata_t
*
* DESCRIPTION
*	User data sent as part of an alternate path response.
*
* SYNOPSIS
*/
typedef union _ib_apr_pdata
{
	uint8_t						data[IB_APR_PDATA_SIZE];

}	ib_apr_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_dreq_pdata_t
* NAME
*	ib_dreq_pdata_t
*
* DESCRIPTION
*	User data sent as part of a disconnection request.
*
* SYNOPSIS
*/
typedef union _ib_dreq_pdata
{
	uint8_t						data[IB_DREQ_PDATA_SIZE];

}	ib_dreq_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_drep_pdata_t
* NAME
*	ib_drep_pdata_t
*
* DESCRIPTION
*	User data sent as part of a reply to a disconnection request.
*
* SYNOPSIS
*/
typedef union _ib_drep_pdata
{
	uint8_t						data[IB_DREP_PDATA_SIZE];

}	ib_drep_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_sidr_req_pdata_t
* NAME
*	ib_sidr_req_pdata_t
*
* DESCRIPTION
*	User data sent as part of a service ID resolution request.
*
* SYNOPSIS
*/
typedef union _ib_sidr_req_pdata
{
	uint8_t						data[IB_SIDR_REQ_PDATA_SIZE];

}	ib_sidr_req_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_sidr_rep_pdata_t
* NAME
*	ib_sidr_rep_pdata_t
*
* DESCRIPTION
*	User data sent as part of a service ID resolution reply.
*
* SYNOPSIS
*/
typedef union _ib_sidr_rep_pdata
{
	uint8_t						data[IB_SIDR_REP_PDATA_SIZE];

}	ib_sidr_rep_pdata_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_ari_t
* NAME
*	ib_ari_t
*
* DESCRIPTION
*	Infiniband-defined additional rejection information.
*
* SYNOPSIS
*/
typedef struct _ib_ari
{
	uint8_t						data[IB_ARI_SIZE];

}	ib_ari_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_apr_info_t
* NAME
*	ib_apr_info_t
*
* DESCRIPTION
*	Infiniband-defined additional rejection information.
*
* SYNOPSIS
*/
typedef struct _ib_apr_info
{
	uint8_t						data[IB_APR_INFO_SIZE];

}	ib_apr_info_t;
/*
* SEE ALSO
*	ib_cm_data_sizes_t
*****/


/****s* Access Layer/ib_cm_req_rec_t
* NAME
*	ib_cm_req_rec_t
*
* DESCRIPTION
*	Connection request information returned to the user through their
*	connection request callback.
*
* SYNOPSIS
*/

#pragma warning(disable:4324)
typedef struct _ib_cm_req_rec
{
	const void*					context;
	ib_cm_handle_t				h_cm_req;
	ib_listen_handle_t			h_cm_listen;

	const uint8_t*				p_req_pdata;

	ib_qp_type_t				qp_type;

	/* valid for rc, uc & rd qp_type only */
	uint8_t						resp_res;
	boolean_t					flow_ctrl;
	uint8_t						rnr_retry_cnt;
	ib_path_rec_t				primary_path;
	ib_path_rec_t				alt_path;

	/* valid for ud qp_type only */
	ib_net16_t					pkey;
	const void*					sidr_context;

}	ib_cm_req_rec_t;
#pragma warning(default:4324)
/*
* FIELDS
*	context
*		For peer-to-peer connections, this is the queue pair context associated
*		with a connection request.  For listens, this is the listen context
*		specified through the ib_cm_listen routine.
*
*	h_cm_req
*		The handle to the communication manager request.  This handle is used
*		to reply to the or reject the connection.
*
*	h_cm_listen
*		For connection request callbacks initiated in response to an
*		ib_cm_listen call, this is a handle to the listen request.  This
*		handle is provided to the user to avoid a race condition between
*		the return of the ib_cm_listen routine and the notification of a
*		connection request.
*
*	p_req_pdata
*		A reference to user-defined private data sent as part of the connection
*		request.
*
*	qp_type
*		Indicates the CM service type.
*
*	resp_res
*		The maximum number of RDMA read/atomic operations from the recipient
*		that the requestor supports on the connection.  The init_depth
*		specified in the call to ib_cm_rep must be less than or equal to
*		this value.
*
*	flow_ctrl
*		Indicates if the remote CA implements hardware end-to-end flow control.
*
*	rnr_retry_cnt
*		Requested number of RNR NAK retries to perform before generating a
*		local error.
*
*	primary_path
*		The path record to use for the primary connection.
*
*	alt_path
*		The path record to use for the alternate connection.
*
*	pkey
*		The pkey used in the user's request.
*
*	sidr_context
*		The sidr_context used in ib_cm_listen.
*
* SEE ALSO
*	ib_cm_req, ib_cm_listen, ib_pfn_cm_req_cb_t,
*	ib_access_t, ib_path_rec_t, ib_req_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_rep_rec_t
* NAME
*	ib_cm_rep_rec_t
*
* DESCRIPTION
*	Connection request reply information returned to the user through their
*	connection reply callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rep_rec
{
	const uint8_t*				p_rep_pdata;

	ib_qp_type_t				qp_type;

	ib_cm_handle_t				h_cm_rep;
	/* valid for rc, uc & rd qp_type only */
	const void*					qp_context;
	uint8_t						resp_res;
	boolean_t					flow_ctrl;
	ib_apr_status_t				apr_status;

	/* valid for ud qp_type only */
	const void*					sidr_context;
	ib_sidr_status_t			status;
	ib_net32_t					remote_qp;
	ib_net32_t					remote_qkey;
	ib_class_port_info_t		class_info;

}	ib_cm_rep_rec_t;
/*
* FIELDS
*	p_rep_pdata
*		A reference to user-defined private data sent as part of the connection
*		request reply.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_cm_rep
*		The handle to the communication manager reply.  This handle is used
*		to issue a ready to use message or to reject the connection.
*
*	h_qp
*		The handle to the queue pair associated with a connection request.
*
*	qp_context
*		The queue pair context associated with a connection request.
*
*	resp_res
*		The maximum number of RDMA read/atomic operations from the recipient
*		that the requestor supports on the connection.  This may be less than
*		the init_depth specified in the call to ib_cm_req.  The local queue
*		pair will be configured with this value unless the connection is
*		rejected.
*
*	flow_ctrl
*		Indicates if the remote CA implements hardware end-to-end flow control.
*
*	apr_status
*		Indicates whether the alternate path information was accepted.
*
*	h_al
*		The AL handle on which the SIDR request was issued.
*
*	sidr_context
*		The sidr_context used in ib_cm_req.
*
*	status
*		Status of the request made previously using ib_cm_req.
*
*	remote_qp
*		Identifies the destination queue pair number.
*
*	remote_qkey
*		Identifies the destination qkey.
*
*	class_info
*		Identifies the class_port_info returned if status was not successful.
*		This field has no value if status is successful.
*
* SEE ALSO
*	ib_cm_req, ib_cm_rep, ib_pfn_cm_rep_cb_t, ib_cm_status_t, ib_rep_pdata_t
*	ib_qp_type_t, ib_sidr_status_t
*****/


/****s* Access Layer/ib_cm_rtu_rec_t
* NAME
*	ib_cm_rtu_rec_t
*
* DESCRIPTION
*	Connection ready to use message information returned to the user through
*	their ready to use callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rtu_rec
{
	const uint8_t*				p_rtu_pdata;

	ib_qp_handle_t				h_qp;
	const void*					qp_context;

}	ib_cm_rtu_rec_t;
/*
* FIELDS
*	p_rtu_pdata
*		A reference to user-defined private data sent as part of the ready
*		to use message.
*
*	h_qp
*		The queue pair handle associated with the connection request.
*
*	qp_context
*		The queue pair context associated with the connection request.
*
* SEE ALSO
*	ib_cm_rtu, ib_pfn_cm_rtu_cb_t, ib_cm_status_t, ib_rtu_pdata_t,
*	ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_rej_rec_t
* NAME
*	ib_cm_rej_rec_t
*
* DESCRIPTION
*	Connection rejection information returned to the user through their
*	rejection callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rej_rec
{
	ib_rej_status_t				rej_status;
	const uint8_t*				p_ari;
	uint8_t						ari_length;

	const uint8_t*				p_rej_pdata;

	ib_qp_handle_t				h_qp;
	const void*					qp_context;

}	ib_cm_rej_rec_t;
/*
* FIELDS
*	rej_status
*		The reason for the connection rejection.
*
*	p_ari
*		Additional rejection information.  The data referenced by this field
*		is dependent on the rej_status and is defined by the Infiniband
*		specification.
*
*	ari_length
*		Length of valid data provided in the p_ari buffer.
*
*	p_rej_pdata
*		A reference to user-defined private data sent as part of the connection
*		request reply.
*
*	h_qp
*		The queue pair handle associated with a connection request.
*
*	qp_context
*		The queue pair context associated with a connection request.
*
* SEE ALSO
*	ib_cm_rej, ib_pfn_cm_rej_cb_t, ib_rej_status_t, ib_ari_t, ib_rej_pdata_t,
*	ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_mra_rec_t
* NAME
*	ib_cm_mra_rec_t
*
* DESCRIPTION
*	Message received acknowledgement information returned to the user through
*	a callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_mra_rec
{
	const uint8_t*				p_mra_pdata;

	ib_qp_handle_t				h_qp;
	const void*					qp_context;

}	ib_cm_mra_rec_t;
/*
* FIELDS
*	p_mra_pdata
*		A reference to user-defined private data sent as part of the MRA.
*
*	h_qp
*		The queue pair handle associated with a connection request.
*
*	qp_context
*		The queue pair context associated with a connection request.
*
* SEE ALSO
*	ib_cm_req, ib_cm_mra, ib_pfn_cm_mra_cb_t, ib_mra_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_lap_rec_t
* NAME
*	ib_cm_lap_rec_t
*
* DESCRIPTION
*	Load alternate path request information returned to the user through
*	a callback.
*
* SYNOPSIS
*/
#pragma warning(disable:4324)
typedef struct _ib_cm_lap_rec
{
	ib_cm_handle_t				h_cm_lap;
	ib_path_rec_t				alt_path;

	const uint8_t*				p_lap_pdata;

	const void*					qp_context;

}	ib_cm_lap_rec_t;
#pragma warning(default:4324)
/*
* FIELDS
*	p_lap_pdata
*		A reference to user-defined private data sent as part of the load
*		alternate path request.
*
*	qp_context
*		The queue pair context associated with a connection request.
*
*	h_cm_lap
*		A handle to the load alternate path request.  This handle is used
*		to reply to the load request.
*
*	alt_path
*		Requested alternate path.  Users must accept or reject the path by
*		calling ib_cm_apr.
*
* SEE ALSO
*	ib_cm_lap, ib_pfn_cm_lap_cb_t, ib_lap_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_apr_rec_t
* NAME
*	ib_cm_apr_rec_t
*
* DESCRIPTION
*	Load alternate path response information returned to the user through
*	a callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_apr_rec
{
	ib_api_status_t				cm_status;
	ib_apr_status_t				apr_status;

	const uint8_t*				p_info;
	uint8_t						info_length;

	const uint8_t*				p_apr_pdata;

	ib_qp_handle_t				h_qp;
	const void*					qp_context;

}	ib_cm_apr_rec_t;
/*
* FIELDS
*	cm_status
*		The status of the alternate path response.  IB_SUCCESS indicates that
*		the alternate path was loaded successfully.  IB_TIMEOUT indicates that
*		a reply was not received within the specified timeout and retry count.
*		Other error values indicates that the alternate path was not loaded.
*		if the apr_status is IB_AP_SUCCESS, the QP failed to load the path.
*		Other apr_status values indicate that the request was rejected for some
*		reason.
*
*	apr_status
*		The alternate path response status.  This indicates additional failure
*		information to a load alternate path request and is defined by the
*		InfiniBand specification.
*
*	info_length
*		Length of valid data in the APR additional information buffer.
*
*	p_info
*		APR additional information.
*
*	p_apr_pdata
*		A reference to user-defined private data sent as part of the alternate
*		path response.
*
*	h_qp
*		The queue pair handle associated with the alternate path response.
*
*	qp_context
*		The queue pair context associated with the alternate path response.
*
* SEE ALSO
*	ib_cm_lap, ib_pfn_cm_apr_cb_t, ib_apr_status_t, ib_apr_info_t
*	ib_apr_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_dreq_rec_t
* NAME
*	ib_cm_dreq_rec_t
*
* DESCRIPTION
*	Disconnection request information returned to the user through their
*	disconnection callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_dreq_rec
{
	ib_cm_handle_t				h_cm_dreq;

	const uint8_t*				p_dreq_pdata;

	const void*					qp_context;

}	ib_cm_dreq_rec_t;
/*
* FIELDS
*	h_cm_dreq
*		A handle to the disconnection request.  This handle is used to reply
*		to the disconnection request.
*
*	p_dreq_pdata
*		A reference to user-defined private data sent as part of the
*		disconnect request.
*
*	qp_context
*		The queue pair context associated with the disconnect request.
*
* SEE ALSO
*	ib_cm_dreq, ib_pfn_cm_dreq_cb_t, ib_dreq_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_drep_rec_t
* NAME
*	ib_cm_drep_rec_t
*
* DESCRIPTION
*	Disconnection reply information returned to the user through their
*	disconnect reply callback.
*
* SYNOPSIS
*/
typedef struct _ib_cm_drep_rec
{
	ib_api_status_t				cm_status;

	const uint8_t*				p_drep_pdata;

	ib_qp_handle_t				h_qp;
	const void*					qp_context;

}	ib_cm_drep_rec_t;
/*
* FIELDS
*	cm_status
*		The status of the disconnect request.  Valid values are IB_SUCCESS
*		and IB_TIMEOUT.  IB_TIMEOUT indicates that a reply was not received
*		within the specified timeout and retry count.
*
*	p_drep_pdata
*		A reference to user-defined private data sent as part of the
*		disconnect reply.
*
*	h_qp
*		The queue pair handle associated with the disconnect reply.
*
*	qp_context
*		The queue pair context associated with the disconnect reply.
*
* SEE ALSO
*	ib_cm_drep, ib_pfn_cm_drep_cb_t, ib_drep_pdata_t, ib_qp_type_t
*****/


/****f* Access Layer/ib_pfn_cm_req_cb_t
* NAME
*	ib_pfn_cm_req_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a connection
*	request message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_req_cb_t)(
	IN				ib_cm_req_rec_t				*p_cm_req_rec );
/*
* PARAMETERS
*	p_cm_req_rec
*		[in] Connection request information returned to the user, indicating
*		the parameters for the connection.
*
* NOTES
*	This callback is invoked to notify the user of a connection request.  This
*	routine is invoked for peer to peer connection request calls to ib_cm_req
*	and for calls to ib_cm_listen.  Users must call ib_cm_rep to accept the
*	connection or ib_cm_rej to reject the connection from the callback.
*
*	Users may also call ib_cm_mra to acknowledge the connection request and
*	prevent the remote side from timing out the connection request.  The
*	ib_cm_mra routine should be invoked if the user requires substantial
*	processing time to process the connection request.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_req, ib_cm_listen, ib_cm_rep, ib_cm_mra, ib_cm_rej, ib_cm_req_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_rep_cb_t
* NAME
*	ib_pfn_cm_rep_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a connection
*	request reply message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_rep_cb_t)(
	IN				ib_cm_rep_rec_t				*p_cm_rep_rec );
/*
* PARAMETERS
*	p_cm_rep_rec
*		[in] Connection request reply information returned to the user,
*		indicating the remote connection data.
*
* NOTES
*	This callback is invoked to notify the user of a connection request reply.
*	This routine is invoked after calling ib_cm_req.  Users must call
*	ib_cm_rtu to accept the connection or ib_cm_rej to reject the connection
*	from the callback.
*
*	Users may also call ib_cm_mra to acknowledge the connection request reply
*	and prevent the remote side from timing out the connection request.  The
*	ib_cm_mra routine should be invoked if the user requires substantial
*	processing time to process the connection request reply.
*
*	If a reply is not received within the specified timeout period,
*	this callback will be invoked with the status set to IB_CM_TIMEOUT.  Users
*	may call ib_cm_rej to notify the remote side that the connection request
*	is being rejected due to a timeout.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_req, ib_cm_listen, ib_cm_rep, ib_cm_rej, ib_cm_mra, ib_cm_rej,
*	ib_cm_rep_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_rtu_cb_t
* NAME
*	ib_pfn_cm_rtu_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a connection
*	ready to use message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_rtu_cb_t)(
	IN				ib_cm_rtu_rec_t				*p_cm_rtu_rec );
/*
* PARAMETERS
*	p_cm_rtu_rec
*		[in] Connection ready to use information returned to the user.
*
* NOTES
*	This callback is invoked to notify the user that a connection is ready
*	to use.  This routine is invoked after calling ib_cm_rep.  If a ready to
*	use message is not received within the specified timeout period, this
*	callback will be invoked with the status set to IB_CM_TIMEOUT.
*
*	This callback will be invoked before a user is notified of any completions
*	that has occurred on the associated queue pair.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_rep, ib_cm_rtu_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_rej_cb_t
* NAME
*	ib_pfn_cm_rej_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a connection
*	rejection message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_rej_cb_t)(
	IN				ib_cm_rej_rec_t				*p_cm_rej_rec );
/*
* PARAMETERS
*	p_cm_rej_rec
*		[in] Connection rejection information returned to the user.
*
* NOTES
*	This callback is invoked to notify the user that a connection has been
*	rejected.  This routine may be invoked after calling ib_cm_req or
*	ib_cm_rep.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_req, ib_cm_rep, ib_cm_rtu, ib_cm_rej, ib_cm_rej_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_mra_cb_t
* NAME
*	ib_pfn_cm_mra_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a message
*	received acknowledgement.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_mra_cb_t)(
	IN				ib_cm_mra_rec_t				*p_cm_mra_rec );

/*
* PARAMETERS
*	p_cm_mra_rec
*		[in] Message received acknowledgement information received from the
*		remote side.
*
* NOTES
*	This callback is invoked to notify the user that their request was
*	successfully received, but additional processing is required.  This
*	callback may be invoked after calling ib_cm_req or ib_cm_rep
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_req, ib_cm_rep, ib_cm_mra_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_lap_cb_t
* NAME
*	ib_pfn_cm_lap_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a load
*	alternate path message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_lap_cb_t)(
	IN				ib_cm_lap_rec_t				*p_cm_lap_rec );
/*
* PARAMETERS
*	p_cm_lap_rec
*		[in] Load alternate path information sent by the remote side.
*
* NOTES
*	This callback is invoked to notify the user of a load alternate path
*	request.  Users must call ib_cm_apr to respond to the load alternate
*	path request from within this callback.  The ib_cm_apr call is used
*	to accept or reject the load alternate path request.
*
*	In the kernel, this callback is typically invoked from within a
*	tasklet, depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_lap, ib_cm_apr, ib_cm_lap_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_apr_cb_t
* NAME
*	ib_pfn_cm_apr_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a load
*	alternate path response message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_apr_cb_t)(
	IN				ib_cm_apr_rec_t				*p_cm_apr_rec );
/*
* PARAMETERS
*	p_cm_apr_rec
*		[in] Load alternate path response information sent by the remote side.
*
* NOTES
*	This callback is invoked to notify the user of a load alternate path
*	response.  If a response is not received within the specified timeout
*	period, this callback will be invoked with the status set to IB_CM_TIMEOUT.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_lap, ib_cm_apr, ib_cm_apr_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_dreq_cb_t
* NAME
*	ib_pfn_cm_dreq_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a disconnect
*	request message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_dreq_cb_t)(
	IN				ib_cm_dreq_rec_t			*p_cm_dreq_rec );
/*
* PARAMETERS
*	p_cm_dreq_rec
*		[in] Disconnect request information returned to the user.
*
* NOTES
*	This callback is invoked to notify the user of a disconnect request.
*	Users must call ib_cm_drep to respond to the disconnect request.  After
*	this callback returns, the queue pair associated with the connection is
*	transitioned to the time-wait state and is no longer usable for sending
*	and receiving data.
*
*	In the kernel, this callback is typically invoked from within a tasklet,
*	depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_req, ib_cm_listen, ib_cm_drep, ib_cm_dreq_rec_t
*****/


/****f* Access Layer/ib_pfn_cm_drep_cb_t
* NAME
*	ib_pfn_cm_drep_cb_t
*
* DESCRIPTION
*	A user-specified callback that is invoked after receiving a disconnect
*	reply message.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_cm_drep_cb_t)(
	IN				ib_cm_drep_rec_t			*p_cm_drep_rec );
/*
* PARAMETERS
*	p_cm_drep_rec
*		[in] Disconnect reply information returned to the user.
*
* NOTES
*	This callback is invoked to notify the user of a disconnect reply.  If
*	no reply was received within the specified timeout period, this callback
*	will be invoked with the status set to IB_CM_TIMEOUT.
*
*	In the kernel, this callback is typically invoked from within a
*	tasklet, depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_cm_dreq, ib_cm_drep, ib_cm_drep_rec_t
*****/


/****d* Access Layer/ib_listen_info_t
* NAME
*	ib_listen_info_t
*
* DESCRIPTION
*	Constants used to specify directed listen requests.
*
* SYNOPSIS
*/
#define IB_ALL_CAS						0
#define IB_ALL_PORTS					0
#define IB_ALL_LIDS						0
#define IB_ALL_PKEYS					0
/*
* SEE ALSO
*	ib_cm_listen, ib_cm_listen_t
*****/


/****s* Access Layer/ib_cm_listen_t
* NAME
*	ib_cm_listen_t
*
* DESCRIPTION
*	Request to listen for incoming connection attempts.
*
* SYNOPSIS
*/
typedef struct _ib_cm_listen
{
	ib_net64_t					svc_id;

	ib_net64_t					ca_guid;
	ib_net64_t					port_guid;
	ib_net16_t					lid;
	ib_net16_t					pkey;

	uint8_t*					p_compare_buffer;
	uint8_t						compare_offset;
	uint8_t						compare_length;

	ib_pfn_cm_req_cb_t			pfn_cm_req_cb;

	ib_qp_type_t				qp_type;

	/* valid for ud qp_type only */
	const void*					sidr_context;

}	ib_cm_listen_t;
/*
* FIELDS
*	svc_id
*		The identifier of the service to register for incoming connection
*		requests.
*
*	ca_guid
*		Directs the communication manager to register the listen only
*		with the specified channel adapter.  This should be set to IB_ALL_CAS
*		if the listen is not directed to a particular channel adapter.
*
*	port_guid
*		Directs the communication manager to register the listen only
*		with the specified port.  This should be set to IB_ALL_PORTS
*		if the listen is not directed to a particular port.
*
*	lid
*		Directs the communication manager to register the listen only
*		with the specified LID.  This should be set to IB_ALL_LIDS
*		if the listen is not directed to a particular LID.
*
*	pkey
*		Directs the communication manager to register the listen only with
*		the specified pkey value.  This should be set to IB_ALL_PKEYS
*		iv the listen is not directed to a particular partition.
*
*	p_compare_buffer
*		An optionally provided buffer that will be used to match incoming
*		connection requests with a registered service.  Use of this buffer
*		permits multiple services to listen on the same service ID as long as
*		they provide different compare buffers.  Incoming requests will
*		be matched against the compare buffer.
*
*	compare_offset
*		An offset into the user-defined data area of a connection request
*		which contains the start of the data that will be compared against.
*		The offset must be the same for all requests using the same service ID.
*
*	compare_length
*		Specifies the size of the compare buffer in bytes.  The length must
*		be the same for all requests using the same service ID.
*
*	pfn_cm_req_cb
*		References a user-provided callback that will be invoked whenever a
*		connection request is received.
*
*	qp_type
*		Indicates the CM service type.
*
*	pfn_cm_mra_cb
*		References a user-provided callback that will be invoked when
*		a message received acknowledgement is received.
*
*	pfn_cm_rej_cb
*		References a user-provided callback that will be invoked if the
*		connection is rejected by the remote end-point.
*
*	sidr_context
*		sidr specific context for listens. This context is passed back in
*		the ib_pfn_cm_req_cb_t callback.
*
* NOTES
*	Users fill out this structure when listening on a service ID with the
*	local communication manager.  The communication manager will use the given
*	service ID and compare buffer to route connection requests to the
*	appropriate client.  Users may direct listens requests on a particular
*	channel adapter, port, or LID.
*
*	Message received acknowledgement (MRA) callbacks will not be invoked
*	until a connection request has been replied to.
*
* SEE ALSO
*	ib_listen_info_t, ib_pfn_cm_req_cb_t, ib_pfn_cm_mra_cb_t,
*	ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_req_t
* NAME
*	ib_cm_req_t
*
* DESCRIPTION
*	Connection request information used to establish a new connection.
*
* SYNOPSIS
*/
typedef struct _ib_cm_req
{
	ib_net64_t					svc_id;

	ib_al_flags_t				flags;
	uint8_t						max_cm_retries;
	TO_LONG_PTR(ib_path_rec_t*,	p_primary_path);

	TO_LONG_PTR(ib_pfn_cm_rep_cb_t, pfn_cm_rep_cb);

	TO_LONG_PTR(const uint8_t*,	p_req_pdata);
	uint8_t						req_length;

	ib_qp_type_t				qp_type;

	/* valid for rc, uc & rd qp_type only */
	TO_LONG_PTR(ib_qp_handle_t,	h_qp);

	TO_LONG_PTR(uint8_t*,		p_compare_buffer);
	uint8_t						compare_offset;
	uint8_t						compare_length;

	uint8_t						resp_res;
	uint8_t						init_depth;
	uint8_t						remote_resp_timeout;
	boolean_t					flow_ctrl;
	uint8_t						local_resp_timeout;
	uint8_t						rnr_nak_timeout;
	uint8_t						rnr_retry_cnt;
	uint8_t						retry_cnt;

	TO_LONG_PTR(ib_path_rec_t*,	p_alt_path OPTIONAL);

	TO_LONG_PTR(ib_pfn_cm_req_cb_t, pfn_cm_req_cb);
	TO_LONG_PTR(ib_pfn_cm_mra_cb_t, pfn_cm_mra_cb);
	TO_LONG_PTR(ib_pfn_cm_rej_cb_t, pfn_cm_rej_cb);

	/* valid for ud qp_type only */
	TO_LONG_PTR(ib_al_handle_t,	h_al);
	TO_LONG_PTR(const void*,	sidr_context);
	uint32_t					timeout_ms;
	ib_net16_t					pkey;

}	ib_cm_req_t;
/*
* FIELDS
*	svc_id
*		The ID of the remote service to which the connection request is
*		being made.
*
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	max_cm_retries
*		The maximum number of times that either CM should resend a connection
*		establishment message.
*
*	p_primary_path
*		Path information over which to establish the primary connection.
*
*	pfn_cm_rep_cb
*		References a user-provided callback that will be invoked when
*		a reply to the connection request is received.
*
*	p_req_pdata
*		Optional user-defined private data sent as part of the connection
*		request.
*
*	req_length
*		Defines the size of the user-defined private data.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_qp
*		A handle to the queue pair to use in the connection.
*
*	p_compare_buffer
*		An optionally provided buffer that will be used to match incoming
*		connection requests with a registered service.  Use of this buffer
*		permits multiple services to connect using the same service ID as
*		long as they provide different compare buffers.  Incoming requests
*		will be matched against the compare buffer.  Valid for peer-to-peer
*		connection requests only.
*
*	compare_offset
*		An offset into the user-defined data area of a connection request
*		which contains the start of the data that will be compared against.
*		The offset must be the same for all requests using the same service ID.
*		Valid for peer-to-peer connection requests only.
*
*	compare_length
*		Specifies the size of the compare buffer in bytes.  The length must
*		be the same for all requests using the same service ID.  Valid for
*		peer-to-peer connection requests only.
*
*	resp_res
*		The maximum number of outstanding RDMA read/atomic operations the
*		requestor supports from the remote QP.
*
*	init_depth
*		The maximum number of outstanding RDMA read/atomic operations the
*		requestor will have outstanding to the remote QP.
*
*	remote_resp_timeout
*		The time within which the remote CM should transmit a response to
*		the sender.  This value is expressed as
*		4.096 * (2 ^ local_resp_timeout) microseconds.
*
*	flow_ctrl
*		Indicates whether the local CA supports end-to-end flow control.
*
*	local_resp_timeout
*		The time that the remote CM should wait to receive a response from
*		the local CM.  This value is expressed as
*		4.096 * (2 ^ local_resp_timeout) microseconds.
*
*	rnr_nak_timeout
*		The time to wait before retrying a packet after receiving a RNR NAK.
*		This value is defined in section 9.7.5.2.8 of the IB Spec, table 45.
*
*	rnr_retry_cnt
*		The number of times that the local QP should retry a send operation
*		after receiving an RNR NACK before reporting an error.
*
*	retry_cnt
*		The number of times that a QP should retry a send operation before
*		reporting an error.
*
*	p_alt_path
*		Optional path information that will be used as the alternate
*		connection path	in the case of failure.
*
*	pfn_cm_req_cb
*		References a user-provided callback that will be invoked when
*		a request for a connection is received.  This is required for peer-to
*		peer connection requests, and must be NULL for client/server
*		connection requests.
*
*	pfn_cm_mra_cb
*		References a user-provided callback that will be invoked when
*		a message received acknowledgement is received.
*
*	pfn_cm_rej_cb
*		References a user-provided callback that will be invoked if the
*		connection is rejected by the remote end-point.
*
*	sidr_context
*		The user-defined sidr context information that will be passed back in a
*		ib_cm_req callback.
*
*	timeout_ms
*		Timeout value in milli-seconds for the REQ to expire.  The CM will add
*		twice packet lifetime to this value to determine the actual timeout
*		value used.
*
*	pkey
*		pkey to be used as part of the request. This field is only valid for
*		IB_MCLASS_CM_VER_2 clients.
*
* SEE ALSO
*	ib_cm_req, ib_pfn_cm_req_cb_t, ib_pfn_cm_rep_cb_t, ib_pfn_cm_mra_cb_t,
*	ib_pfn_cm_rej_cb_t, ib_path_rec_t, ib_req_pdata_t, ib_qp_type_t
*****/


/****d* Access Layer/ib_cm_failover_t
* NAME
*	ib_cm_failover_t
*
* DESCRIPTION
*	Fail over acceptance status returned as part of a connection reply.
*
* SYNOPSIS
*/
typedef uint8_t								ib_cm_failover_t;
#define IB_FAILOVER_ACCEPT_SUCCESS			0
#define IB_FAILOVER_ACCEPT_UNSUPPORTED		1
#define IB_FAILOVER_ACCEPT_ERROR			2
/*
* NOTES
*	These values and their use are defined the Infiniband specification.
*
* SEE ALSO
*	ib_cm_rep, ib_cm_rep_t
*****/


/****s* Access Layer/ib_cm_rep_t
* NAME
*	ib_cm_rep_t
*
* DESCRIPTION
*	Connection reply information used when establishing a connection.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rep
{
	ib_al_flags_t				flags;

	TO_LONG_PTR(const uint8_t*,	p_rep_pdata);
	uint8_t						rep_length;

	TO_LONG_PTR(ib_qp_handle_t,	h_qp);
	ib_qp_type_t				qp_type;

	/* valid for rc, uc & rd qp_type only */
	ib_access_t					access_ctrl;
	uint32_t					sq_depth;
	uint32_t					rq_depth;

	uint8_t						init_depth;
	uint8_t						target_ack_delay;
	ib_cm_failover_t			failover_accepted;
	boolean_t					flow_ctrl;
	uint8_t						rnr_nak_timeout;
	uint8_t						rnr_retry_cnt;

	TO_LONG_PTR(ib_pfn_cm_rej_cb_t, pfn_cm_rej_cb);
	TO_LONG_PTR(ib_pfn_cm_mra_cb_t, pfn_cm_mra_cb);
	TO_LONG_PTR(ib_pfn_cm_rtu_cb_t, pfn_cm_rtu_cb);
	TO_LONG_PTR(ib_pfn_cm_lap_cb_t, pfn_cm_lap_cb);
	TO_LONG_PTR(ib_pfn_cm_dreq_cb_t, pfn_cm_dreq_cb);

	TO_LONG_PTR(ib_recv_wr_t*,	p_recv_wr);
	TO_LONG_PTR(ib_recv_wr_t**,	pp_recv_failure);

	/*valid for ud qp_type only */
	ib_sidr_status_t			status;
	ib_class_port_info_t		class_info;

}	ib_cm_rep_t;
/*
* FIELDS
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	p_rep_pdata
*		Optional user-defined private data sent as part of the connection
*		reply.
*
*	rep_length
*		Defines the size of the user-defined private data.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_qp
*		A handle to the queue pair to use in the connection. For SIDR, h_qp
*		is valid only if sidr status is IB_SIDR_SUCCESS.
*
*	access_ctrl
*		Indicates the type of access permitted on the local QP.
*
*	sq_depth
*		The maximum number of outstanding send operations that the local
*		QP needs to support.
*
*	rq_depth
*		The maximum number of outstanding receive operations that the local
*		QP needs to support.
*
*	init_depth
*		The maximum number of outstanding RDMA read/atomic operations the
*		sender of the reply will have outstanding to the remote QP.
*
*	target_ack_delay
*		The time that the remote QP should wait to receive an ACK from the
*		local QP.
*
*	failover_accepted
*		Status indicating if the fail over path was accepted by the sender
*		of the reply.
*
*	flow_ctrl
*		Indicates whether the local CA supports end-to-end flow control.
*
*	rnr_nak_timeout
*		The time to wait before retrying a packet after receiving a RNR NAK.
*
*	rnr_retry_cnt
*		The number of times that the local QP should retry a send operation
*		after receiving an RNR NACK before reporting an error.
*
*	pfn_cm_rtu_cb
*		References a user-defined callback that will be invoked when
*		a connection is ready to use for send operations.
*
*	pfn_cm_lap_cb
*		References a user-defined callback that will be invoked when
*		a load alternate path request is received for the connecting
*		queue pair or end-to-end context.
*
*	pfn_cm_dreq_cb
*		References a user-defined callback that will be invoked when
*		a disconnect request is received is for the connecting
*		queue pair or end-to-end context.
*
*	p_recv_wr
*		A reference to the head of the work request list to be initially
*		posted to the receive queue.  Providing this list closes a potential
*		race condition between sending a CM REP message and posting receives.
*		Use of this field is optional.
*
*	pp_recv_failure
*		If the post receive operation failed, this references the work
*		request in the p_recv_wr list where the first failure occurred.
*		This field is required only if p_recv_wr is used.
*
*	status
*		sidr status value returned back to a previously received REQ.
*
*	class_info
*		The contents of this field are valid only if status is IB_SIDR_REDIRECT.
*
* SEE ALSO
*	ib_cm_rep, ib_access_t, ib_cm_failover_t, ib_rep_pdata_t
*	ib_pfn_cm_rtu_cb_t, ib_pfn_cm_lap_cb_t, ib_pfn_cm_dreq_cb_t,
*	ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_rtu_t
* NAME
*	ib_cm_rtu_t
*
* DESCRIPTION
*	Connection ready to use information used when establishing a connection.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rtu
{
	ib_access_t					access_ctrl;
	uint32_t					sq_depth;
	uint32_t					rq_depth;

	const uint8_t*				p_rtu_pdata;
	uint8_t						rtu_length;

	ib_pfn_cm_apr_cb_t			pfn_cm_apr_cb;
	ib_pfn_cm_dreq_cb_t			pfn_cm_dreq_cb;

}	ib_cm_rtu_t;
/*
* FIELDS
*	access_ctrl
*		Indicates the type of access permitted on the local QP.
*
*	sq_depth
*		The maximum number of outstanding send operations that the local
*		QP needs to support.  This field should be set to zero if the CA
*		does not support changing the work request depth after the QP is
*		created.
*
*	rq_depth
*		The maximum number of outstanding receive operations that the local
*		QP needs to support.  This field should be set to zero if the CA
*		does not support changing the work request depth after the QP is
*		created.
*
*	p_rtu_pdata
*		Optional user-defined private data sent as part of the connection
*		ready to use message.
*
*	rtu_length
*		Defines the size of the user-defined private data.
*
*	pfn_cm_apr_cb
*		References a user-defined callback that will be invoked when an
*		alternate path response is received for the connecting queue pair
*		or end-to-end context.
*
*	pfn_cm_dreq_cb
*		References a user-defined callback that will be invoked when a
*		disconnect request is received is for the connecting queue pair
*		or end-to-end context.
*
* SEE ALSO
*	ib_cm_rtu, ib_access_t, ib_rtu_pdata_t
*****/


/****s* Access Layer/ib_cm_rej_t
* NAME
*	ib_cm_rej_t
*
* DESCRIPTION
*	Information used to reject a connection request.
*
* SYNOPSIS
*/
typedef struct _ib_cm_rej
{
	ib_rej_status_t				rej_status;

	ib_ari_t*					p_ari;
	uint8_t						ari_length;
	const uint8_t*				p_rej_pdata;
	uint8_t						rej_length;

}	ib_cm_rej_t;
/*
* FIELDS
*	rej_status
*		The reason for the connection rejection.
*
*	p_ari
*		Additional rejection information.  The data referenced by this field
*		is dependent on the rej_status and is defined by the Infiniband
*		specification.
*
*	ari_length
*		Length of valid data provided in the p_ari buffer.
*
*	p_rej_pdata
*		A reference to user-defined private data sent as part of the
*		reject message.
*
*	rej_length
*		Defines the size of the user-defined private data.
*
* SEE ALSO
*	ib_cm_rej, ib_pfn_cm_rej_cb_t, ib_rej_status_t, ib_ari_t, ib_rej_pdata_t
*****/


/****s* Access Layer/ib_cm_mra_t
* NAME
*	ib_cm_mra_t
*
* DESCRIPTION
*	Connection message received acknowledgement information used to
*	indicate that a connection request, reply, or load alternate path
*	has been received.
*
* SYNOPSIS
*/
typedef struct _ib_cm_mra
{
	uint8_t						svc_timeout;

	TO_LONG_PTR(const uint8_t*,	p_mra_pdata);
	uint8_t						mra_length;

}	ib_cm_mra_t;
/*
* FIELDS
*	svc_timeout
*		Indicates the amount of time that the local service requires to
*		complete processing of the previously received message.
*
*	p_mra_pdata
*		Optional user-defined private data sent as part of the message
*		received acknowledgement.
*
*	mra_length
*		Defines the size of the user-defined private data.
*
* SEE ALSO
*	ib_cm_mra, ib_pfn_cm_req_cb_t, ib_pfn_cm_rep_cb_t, ib_pfn_cm_lap_cb_t,
*	ib_mra_pdata_t
*****/


/****s* Access Layer/ib_cm_lap_t
* NAME
*	ib_cm_lap_t
*
* DESCRIPTION
*	Load alternate path information used to configure a queue pair with an
*	alternate path.
*
* SYNOPSIS
*/
typedef struct _ib_cm_lap
{
	ib_al_flags_t				flags;

	TO_LONG_PTR(const uint8_t*,	p_lap_pdata);
	uint8_t						lap_length;

	ib_qp_type_t				qp_type;

	/* valid for rc, uc & rd qp_type only */
	TO_LONG_PTR(ib_qp_handle_t,	h_qp);

	uint8_t						remote_resp_timeout;
	TO_LONG_PTR(ib_path_rec_t*,	p_alt_path);
	ib_pfn_cm_apr_cb_t			pfn_cm_apr_cb;

}	ib_cm_lap_t;
/*
* FIELDS
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	p_lap_pdata
*		Optional user-defined private data sent as part of the load alternate
*		path message.
*
*	lap_length
*		Defines the size of the user-defined private data.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_qp
*		A handle to the queue pair that should receive the alternate path.
*
*	remote_resp_timeout
*		The time within which the remote CM should transmit a response to
*		the sender.  This value is expressed as
*		4.096 * (2 ^ local_resp_timeout) microseconds.
*
*	p_alt_path
*		The path record to use for the alternate connection.
*
*	pfn_cm_apr_cb
*		References a user-defined callback that will be invoked when the
*		response to the load request is received.
*
* SEE ALSO
*	ib_cm_lap, ib_pfn_cm_lap_cb_t, ib_pfn_cm_apr_cb_t, ib_path_rec_t,
*	ib_pfn_lap_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_apr_t
* NAME
*	ib_cm_apr_t
*
* DESCRIPTION
*	Load alternate path information used to configure a queue pair with an
*	alternate path.
*
* SYNOPSIS
*/
typedef struct _ib_cm_apr
{
	TO_LONG_PTR(const uint8_t*,		p_apr_pdata);
	uint8_t							apr_length;

	ib_qp_type_t					qp_type;

	/* valid for rc, uc & rd qp_type only */
	TO_LONG_PTR(ib_qp_handle_t,		h_qp); 

	ib_apr_status_t					apr_status;
	uint8_t							info_length;
	TO_LONG_PTR(const ib_apr_info_t*, p_info);

}	ib_cm_apr_t;
/*
* FIELDS
*	p_apr_pdata
*		Optional user-defined private data sent as part of the alternate
*		path response message.
*
*	apr_length
*		Defines the size of the user-defined private data.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_qp
*		A handle to the queue pair that should receive the alternate path.
*
*	apr_status
*		The alternate path response status.  This indicates additional failure
*		information to a load alternate path request and is defined by the
*		Infiniband specification.
*
*	info_length
*		Length of valid data in the APR additional information buffer.
*
*	p_info
*		APR additional information.
*
* SEE ALSO
*	ib_cm_apr, ib_pfn_cm_apr_cb_t, ib_lap_pdata_t, ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_dreq_t
* NAME
*	ib_cm_dreq_t
*
* DESCRIPTION
*	Disconnection request information used to tear down a connection.
*
* SYNOPSIS
*/
typedef struct _ib_cm_dreq
{
	ib_al_flags_t				flags;

	uint8_t*					p_dreq_pdata;
	uint8_t						dreq_length;

	ib_qp_type_t				qp_type;

	/* valid for rc, uc & rd qp_type only */
	ib_qp_handle_t				h_qp;
	ib_pfn_cm_drep_cb_t			pfn_cm_drep_cb;

}	ib_cm_dreq_t;
/*
* FIELDS
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	p_dreq_pdata
*		A reference to user-defined private data sent as part of the
*		disconnection request.
*
*	dreq_length
*		Defines the size of the user-defined private data.
*
*	qp_type
*		Indicates the CM service type.
*
*	h_qp
*		A handle to the queue pair to disconnect.
*
*	pfn_cm_drep_cb
*		References a user-defined callback that will be invoked when
*		the reply to the disconnect is received.
*
* NOTES
*	Users submit this structure to disconnect a queue pair or end-to-end
*	context.  A single disconnect call disconnects either a queue pair or
*	an end-to-end context, but not both.
*
* SEE ALSO
*	ib_cm_dreq, ib_cm_drep, ib_dreq_pdata_t, ib_al_flags_t,
*	ib_qp_type_t
*****/


/****s* Access Layer/ib_cm_drep_t
* NAME
*	ib_cm_drep_t
*
* DESCRIPTION
*	Disconnection reply information used when tearing down a connection.
*
* SYNOPSIS
*/
typedef struct _ib_cm_drep
{
	uint8_t*					p_drep_pdata;
	uint8_t						drep_length;

}	ib_cm_drep_t;
/*
* FIELDS
*	p_drep_pdata
*		A reference to user-defined private data sent as part of the
*		disconnection reply.
*
*	drep_length
*		Defines the size of the user-defined private data.
*
* SEE ALSO
*	ib_cm_drep, ib_drep_pdata_t
*****/


/****f* Access Layer/ib_cm_listen
* NAME
*	ib_cm_listen
*
* DESCRIPTION
*	Issues a request to the local communication manager to listen for
*	incoming connection requests.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_listen(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_cm_listen_t* const		p_cm_listen,
	IN		const	void* const					listen_context,
		OUT			ib_listen_handle_t* const	ph_cm_listen );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an opened instance of the access layer.
*
*	p_cm_listen
*		[in] Information used to direct the listen request to match incoming
*		connection requests.
*
*	listen_context
*		User-specified context information that is returned as a part of all
*		connection requests through the pfn_cm_req_cb routine.  The context is
*		also returned through the error and destroy callbacks.
*
*	ph_cm_listen
*		[out] Upon successful completion of this call, this references a handle
*		to the listen request.  This handle may be used to cancel the listen
*		operation.
*
* RETURN VALUES
*	IB_SUCCESS
*		The listen request was successfully registered with the connection
*		manager.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the listen request information, error callback function,
*		or listen handle was not provided.
*
*	IB_INVALID_SETTING
*		The class version specified in the listen request is not supported by
*		connection manager or the listen request is not unique.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the listen request.
*
*	IB_INVALID_GUID
*		A channel adapter or port GUID is not wildcarded and no channel adapter
*		or port in the system was found for the specified GUID.
*
*	IB_INVALID_LID
*		The lid is not wildcarded and is not within the lid range for the port
*		specified in the listen request information.
*
*	IB_INVALID_PKEY
*		The pkey is not wildcarded and is not a valid pkey for the port
*		specified in the listen request information.
*
* NOTES
*	This routine directs the access layer to route connection requests
*	matching the specified connection parameters to the client.  Clients
*	listen for connections matching a particular service ID, and may optionally
*	direct their listen request towards a specific channel adapter, port, or
*	LID.
*
*	If local configuration changes occur that invalidate a listen request, the
*	specified error callback will be invoked.  Invalidated listen requests
*	should be canceled by the user.  An example of a configuration change that
*	invalidates listen requests is a LID change for directed listens.  The
*	listen error callback will be invoked within the context of a system
*	thread.
*
* SEE ALSO
*	ib_cm_listen_t
*****/


/****f* Access Layer/ib_cm_cancel
* NAME
*	ib_cm_cancel
*
* DESCRIPTION
*	Routine used to cancel listening for connection requests.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_cancel(
	IN		const	ib_listen_handle_t			h_cm_listen,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_cm_listen
*		[in] A handle to an existing listen request.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the listen
*		request has been successfully canceled.
*
* RETURN VALUES
*	IB_SUCCESS
*		The cancel listen operation was initiated.
*
*	IB_INVALID_HANDLE
*		The connection manager handle was invalid.
*
* NOTES
*	This routine cancels a listen request.  To avoid a race condition
*	canceling a request at the same time a connection callback is in
*	progress, the cancel operation operates asynchronously.  For
*	additional details see ib_pfn_destroy_cb_t.
*
* SEE ALSO
*	ib_cm_listen, ib_pfn_destroy_cb_t
*****/


/****f* Access Layer/ib_cm_req
* NAME
*	ib_cm_req
*
* DESCRIPTION
*	Issues a connection request to a specified end-point.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_req(
	IN		const	ib_cm_req_t* const			p_cm_req );
/*
* PARAMETERS
*	p_cm_req
*		[in] Information describing the type of connection and the remote
*		endpoint for the connection.
*
* RETURN VALUES
*	IB_SUCCESS
*		The connection request was initiated.
*
*	IB_INVALID_PARAMETER
*		A reference to the connect request information was not provided.
*
*	IB_INVALID_SETTING
*		The connect request information contains one or more of the following
*		errors:
*		  - The class version, queue pair type, or path is not supported by
*			connection manager.
*		  - The private data length exceeds the value allowed by the specified
*			connection class version.
*		  - The primary path is not on the same channel adapter as the queue
*			pair.
*		  - The primary and alternate paths are on different channel adapters.
*		  - The primary and alternate paths specify different MTUs.
*		  - A primary or alternate path record packet lifetime is out of range.
*		  - A primary or alternate path record pkey is out of range.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle specified in the connect request was invalid.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_STATE
*		The queue pair or end-to-end context is already connected.
*
*	IB_INVALID_QP_STATE
*		The queue pair was in an invalid state to perform the operation.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to initiate the connect request.
*
* NOTES
*	This routine issues a connection request through the communication
*	manager to a specified end-point.  The p_cm_req parameter contains
*	details needed to form the connection.  The connection request will
*	match with a remote ib_cm_listen or ib_cm_req connection request.
*
* SEE ALSO
*	ib_cm_req_t, ib_cm_listen, ib_pfn_cm_req_cb_t
*****/


/****f* Access Layer/ib_cm_rep
* NAME
*	ib_cm_rep
*
* DESCRIPTION
*	Sends a reply to a connection request, indicating that the connection
*	has been accepted.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_rep(
	IN		const	ib_cm_handle_t				h_cm_req,
	IN		const	ib_cm_rep_t* const			p_cm_rep );
/*
* PARAMETERS
*	h_cm_req
*		[in] A handle to the connection request being replied to.  This handle
*		is provided by the access layer through the ib_pfn_cm_req_cb_t
*		callback.
*
*	p_cm_rep
*		[in] Contains reply information to return to the initiator of the
*		connection request.
*
* RETURN VALUES
*	IB_SUCCESS
*		The connection reply was initiated.
*
*	IB_INVALID_HANDLE
*		The connection manager request handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the reply information was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The connect reply information contains one or more of the following
*		errors:
*		  - The class version, queue pair type, or path is not supported by
*			connection manager.
*		  - The private data length exceeds the value allowed by the connection
*			class version.
*		  - The primary path is not on the same channel adapter as the queue
*			pair.
*		  - The primary and alternate paths are on different channel adapters.
*		  - The primary and alternate paths specify different MTUs.
*		  - A primary or alternate path record packet lifetime is out of range.
*		  - A primary or alternate path record pkey is out of range.
*		  - The specified private data length is invalid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle specified in the reply was invalid.
*
*	IB_INVALID_QP_STATE
*		The queue pair was in an invalid state to perform the operation.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the connect reply.
*
* NOTES
*	This routine results in the access layer replying to a connection
*	request from a remote node.  This call results in sending a response
*	to the requesting node that the request has been accepted.
*
* SEE ALSO
*	ib_cm_rep_t, ib_pfn_cm_req_cb_t, ib_pfn_cm_rep_cb_t
*****/


/****f* Access Layer/ib_cm_rtu
* NAME
*	ib_cm_rtu
*
* DESCRIPTION
*	Sends a ready to use message for a connection request, indicating that
*	the connection has been accepted and is ready for use.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_rtu(
	IN		const	ib_cm_handle_t				h_cm_rep,
	IN		const	ib_cm_rtu_t* const			p_cm_rtu );
/*
* PARAMETERS
*	h_cm_rep
*		[in] A handle to the connection reply being responded to.  This handle
*		is provided by the access layer through the ib_pfn_cm_rep_cb_t
*		callback.
*
*	p_cm_rtu
*		[in] Contains ready to use information to return to the sender of the
*		connection reply.
*
* RETURN VALUES
*	IB_SUCCESS
*		The connection ready to use was initiated.
*
*	IB_INVALID_HANDLE
*		The connection manager reply handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the ready to use information was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The specified queue pair attributes were invalid or the private data
*		length exceeds the value allowed by the specified connection class
*		version.
*
*	IB_UNSUPPORTED
*		The specified queue pair access control was not supported.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the ready to use response.
*
* NOTES
*	This routine results in the access layer marking a connection as ready
*	to use and notifying the remote end-point.
*
* SEE ALSO
*	ib_cm_rep_t, ib_pfn_cm_rep_cb_t, ib_cm_rtu_t
*****/


/****f* Access Layer/ib_cm_rej
* NAME
*	ib_cm_rej
*
* DESCRIPTION
*	Rejects a connection request from a remote end-point.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_rej(
	IN		const	ib_cm_handle_t				h_cm,
	IN		const	ib_cm_rej_t* const			p_cm_rej );
/*
* PARAMETERS
*	h_cm
*		[in] A handle to the connection request or reply being rejected.
*		This is the h_cm_req or h_cm_rep handle provided through the
*		ib_pfn_cm_req_cb_t or ib_pfn_cm_rep_cb_t callback, respectively.
*
*	p_cm_rej
*		[in] Contains the connection rejection information to return to the
*		connecting end-point.
*
* RETURN VALUES
*	IB_SUCCESS
*		The connection reject was initiated.
*
*	IB_INVALID_HANDLE
*		The connection manager handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the reject information was not provided.
*
* NOTES
*	This routine results in the access layer rejecting a connection
*	and notifying the remote end-point.
*
* SEE ALSO
*	ib_pfn_cm_req_cb_t, ib_pfn_cm_rep_cb_t, ib_cm_rej_t
*****/


/****f* Access Layer/ib_cm_mra
* NAME
*	ib_cm_mra
*
* DESCRIPTION
*	Notifies the remote end-point of a connection or load alternate path
*	request that the request message has been received, but additional
*	processing is required.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_mra(
	IN		const	ib_cm_handle_t				h_cm,
	IN		const	ib_cm_mra_t* const			p_cm_mra );
/*
* PARAMETERS
*	h_cm
*		[in] A handle to the connection request, connection reply, or load
*		alternate path request that should receive the message received
*		acknowledgement message.  This is the h_cm_req, h_cm_rep, or
*		h_cm_lap handle provided through the ib_pfn_cm_req_cb_t,
*		ib_pfn_cm_rep_cb_t, or ib_pfn_cm_lap_cb_t callback, respectively.
*
*	p_cm_mra
*		[in] Contains the message received acknowledgement data to return to
*		the requesting end-point.
*
* RETURN VALUES
*	IB_SUCCESS
*		The message receive acknowledge was sent successfully.
*
*	IB_INVALID_HANDLE
*		The connection manager reply handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the message receive acknowledge information was not
*		provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The class version is not supported by connection manager or the
*		specified private data length is invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the message receive acknowledge.
*
* NOTES
*	This routine results in the access layer acknowledging a connection or
*	load alternate path message.  It should be invoked by a client if the
*	client is unable to respond to a request within a specified timeout,
*	in order to prevent the remote end-point from timing out.
*
* SEE ALSO
*	ib_pfn_cm_req_cb_t, ib_pfn_cm_rep_cb_t, ib_pfn_cm_lap_cb_t, ib_cm_mra_t
*****/


/****f* Access Layer/ib_cm_lap
* NAME
*	ib_cm_lap
*
* DESCRIPTION
*	Issues a load alternate path request to a specified end-point.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_lap(
	IN		const	ib_cm_lap_t* const			p_cm_lap );
/*
* PARAMETERS
*	p_cm_lap
*		[in] Information describing the alternate path to load and the remote
*		endpoint for the connection.
*
* RETURN VALUES
*	IB_SUCCESS
*		The load alternate path request was sent successfully.
*
*	IB_INVALID_PARAMETER
*		A reference to the load alternate path information was not provided.
*
*	IB_UNSUPPORTED
*		The passive side of the connection attempted to load an alternate path.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The load alternate path information contains one or more of the
*		following errors:
*		  - The class version, queue pair type, or path is not supported by
*			connection manager.
*		  - The primary path is not on the same channel adapter as the queue
*			pair.
*		  - The primary and alternate paths are on different channel adapters.
*		  - The primary and alternate paths specify different MTUs.
*		  - The alternate path record packet lifetime is out of range.
*		  - The alternate path record pkey is out of range.
*		  - The specified private data length is invalid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle specified in the load alternate path information
*		was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the load alternate path request.
*
* NOTES
*	This routine issues initiates loading an alternate path on an existing
*	connected queue pair or end-to-end context.  If the request is successful,
*	the alternate path will be loaded and armed for path migration.
*
*	The p_cm_lap parameter describes the alternate path to load and indicates
*	the remote endpoint of an existing connection that will receive the load
*	request.
*
* SEE ALSO
*	ib_cm_apr, ib_cm_lap_t, ib_pfn_cm_lap_cb_t, ib_pfn_cm_apr_cb_t
*****/


/****f* Access Layer/ib_cm_apr
* NAME
*	ib_cm_apr
*
* DESCRIPTION
*	Responds to a load alternate path request, to accept or reject the
*	proposed alternate path.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_apr(
	IN		const	ib_cm_handle_t				h_cm_lap,
	IN		const	ib_cm_apr_t* const			p_cm_apr );
/*
* PARAMETERS
*	h_cm_lap
*		[in] A handle to a load alternate path request corresponding to the
*		response.  This handle is provided through the ib_pfn_cm_lap_cb_t.
*
*	p_cm_apr
*		[in] Information describing the alternate path response.  The response
*		will accept or reject the load request.  If the request is rejected
*		this parameter will reference additional rejection information.
*
* RETURN VALUES
*	IB_SUCCESS
*		The load alternate path response was sent successfully.
*
*	IB_INVALID_HANDLE
*		The connection manager load alternate path handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the alternate path information was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The private data length specified in alternate path information is
*		invalid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle specified in the alternate path information
*		was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the alternate path response.
*
* NOTES
*	This routine responds to a load alternate path request.
*
* SEE ALSO
*	ib_cm_lap, ib_cm_apr_t, ib_pfn_cm_lap_cb_t, ib_pfn_cm_apr_cb_t
*****/


/****f* Access Layer/ib_force_apm
* NAME
*	ib_force_apm
*
* DESCRIPTION
*	This routine indicates that a queue pair should immediately migrate to its
*	alternate path.  All future data transfers will occur over the new path.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_force_apm(
	IN		const	ib_qp_handle_t				h_qp );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to the queue pair to migrate.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair or end-to-end context was successfully modified.
*
*	IB_INVALID_PARAMETER
*		Neither or both of the queue pair or the end-to-end context handles
*		were valid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the modify the queue pair or end-to-end context.
*
*	IB_UNSUPPORTED
*		The requested modification was not supported.
*
*	IB_INVALID_QP_STATE
*		The queue pair was in an invalid state for the requested operation.
*
* NOTES
*	For this routine to operate correctly, the specified queue pair must have
*	an existing alternate path loaded.  If an alternate path is not loaded, or
*	has not yet been armed, this call will fail.
*
*	Use of this call results in additional data transfers that occur on the
*	given queue pair using the alternate path.  Once this call completes, a
*	new alternate path may be loaded using the ib_cm_lap call.
*
* SEE ALSO
*	ib_cm_lap
*****/


/****f* Access Layer/ib_cm_dreq
* NAME
*	ib_cm_dreq
*
* DESCRIPTION
*	This routine disconnects a queue pair or end-to-end context.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_dreq(
	IN		const	ib_cm_dreq_t* const			p_cm_dreq );
/*
* PARAMETERS
*	p_cm_dreq
*		[in] Information that describes the connection being disconnected.
*
* RETURN VALUES
*	IB_SUCCESS
*		The disconnect request was sent successfully.
*
*	IB_INVALID_PARAMETER
*		A reference to the disconnect request information was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The private data length specified in disconnect request information is
*		invalid.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle specified in the disconnect request information
*		was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the disconnect request.
*
* NOTES
*	This function will disconnect a queue pair or end-to-end context.
*	It results in sending a disconnection request message to the remote
*	end-point.  After calling this routine, data transfers on the specified
*	queue pair or end-to-end context will fail.
*
* SEE ALSO
*	ib_cm_drep, ib_pfn_cm_dreq_cb_t, ib_cm_dreq_t
*****/


/****f* Access Layer/ib_cm_drep
* NAME
*	ib_cm_drep
*
* DESCRIPTION
*	This routine replies to a disconnection request and disconnects
*	a queue pair or end-to-end context.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_drep(
	IN		const	ib_cm_handle_t				h_cm_dreq,
	IN		const	ib_cm_drep_t* const			p_cm_drep );
/*
* PARAMETERS
*	h_cm_dreq
*		[in] A handle to a disconnection request being replied to.  This
*		handle is provided through the ib_pfn_cm_dreq_cb_t callback.
*
*	p_cm_drep
*		[in] Reply information used to respond to the disconnection request.
*
* RETURN VALUES
*	IB_SUCCESS
*		The disconnect request was sent successfully.
*
*	IB_INVALID_HANDLE
*		The connection manager disconnect request handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the disconnect repy information was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow sending this message.
*
*	IB_INVALID_SETTING
*		The private data length specified in disconnect reply information is
*		invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to send the disconnect reply.
*
* NOTES
*	This function will disconnect a queue pair or end-to-end context.  It
*	results in sending a disconnection reply message to the remote end-point.
*	After calling this routine, data transfers on the specified queue pair or
*	end-to-end context will fail.
*
* SEE ALSO
*	ib_cm_dreq, ib_pfn_cm_dreq_cb_t, ib_cm_drep_t
*****/


/****f* Access Layer/ib_cm_handoff
* NAME
*	ib_cm_handoff
*
* DESCRIPTION
*	Hands off the received REQ information to svc_id.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_cm_handoff(
	IN		const	ib_cm_handle_t				h_cm_req,
	IN		const	ib_net64_t					svc_id );
/*
* PARAMETERS
*	h_cm_req
*		[in] A handle to the connection request being handed off.
*		This is the h_cm_req handle provided through the ib_pfn_cm_req_cb_t
*		callback.
*
*	svc_id
*		[in] The service id to which this connection request is handed off.
*
* RETURN VALUES
*	IB_SUCCESS
*		The handoff was initiated.
*
*	IB_INVALID_HANDLE
*		The connection manager handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A valid service id was not provided.
*
*	IB_INVALID_STATE
*		The current connection state does not allow this transfer.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to complete the request.
*
* NOTES
*	This routine results in the access layer handing off the connection
*	to the service id as a new incoming connection.
*
* SEE ALSO
*	ib_pfn_cm_req_cb_t, ib_cm_rej_t, ib_cm_listen
*****/


/****s* Access Layer/ib_cep_listen_t
* NAME
*	ib_cep_listen_t
*
* DESCRIPTION
*	Request to listen for incoming connection attempts.
*
* SYNOPSIS
*/
typedef struct _ib_cep_listen
{
	net64_t						svc_id;

	net64_t						port_guid;

	TO_LONG_PTR(uint8_t*,		p_cmp_buf);
	uint8_t						cmp_len;
	uint8_t						cmp_offset;

}	ib_cep_listen_t;
/*
* FIELDS
*	svc_id
*		The identifier of the service to register for incoming connection
*		requests.
*
*	port_guid
*		Directs the communication manager to register the listen only
*		with the specified port.  This should be set to IB_ALL_PORTS
*		if the listen is not directed to a particular port.
*
*	p_cmp_buf
*		An optionally provided buffer that will be used to match incoming
*		connection requests with a registered service.  Use of this buffer
*		permits multiple services to listen on the same service ID as long as
*		they provide different compare buffers.  Incoming requests will
*		be matched against the compare buffer.
*
*	cmp_len
*		Specifies the size of the compare buffer in bytes.  The length must
*		be the same for all requests using the same service ID.
*
*	cmp_offset
*		An offset into the user-defined data area of a connection request
*		which contains the start of the data that will be compared against.
*		The offset must be the same for all requests using the same service ID.
*
* NOTES
*	Users fill out this structure when listening on a service ID with the
*	local communication manager.  The communication manager will use the given
*	service ID and compare buffer to route connection requests to the
*	appropriate client.  Users may direct listens requests on a particular
*	channel adapter, port, or LID.
*****/


/****f* Access Layer/ib_create_ioc
* NAME
*	ib_create_ioc
*
* DESCRIPTION
*	Creates an instance of an I/O controller.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_ioc(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_ioc_profile_t* const		p_ioc_profile,
		OUT			ib_ioc_handle_t* const		ph_ioc );
/*
* PARAMETERS
*	h_ca
*		[in] A handle to an opened channel adapter.  The controller will be
*		created to be exposed through the given adapter.
*
*	p_ioc_profile
*		[in] I/O controller profile information.
*
*	ph_ioc
*		[out] Upon successful completion of this call, this references a
*		handle to the created I/O controller.  This handle may be used to
*		add service entries to the controller and register it.
*
* RETURN VALUES
*	IB_SUCCESS
*		The I/O controller was successfully created.
*
*	IB_INVALID_CA_HANDLE
*		The channel adapter handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the I/O controller profile information or handle
*		was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the I/O controller.
*
* NOTES
*	This routine creates an I/O controller.  Once created, services may be
*	added to the controller before being registered with the local device
*	manager.
*
* SEE ALSO
*	ib_destroy_ioc, ib_add_svc_entry, ib_reg_ioc, ib_ioc_profile_t
*****/


/****f* Access Layer/ib_destroy_ioc
* NAME
*	ib_destroy_ioc
*
* DESCRIPTION
*	Destroys an instance of an I/O controller.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_ioc(
	IN		const	ib_ioc_handle_t				h_ioc );
/*
* PARAMETERS
*	h_ioc
*		[in] A handle to an existing I/O controller.
*
* RETURN VALUES
*	IB_SUCCESS
*		The I/O controller was successfully destroyed.
*
*	IB_INVALID_HANDLE
*		The I/O controller handle was invalid.
*
* NOTES
*	Once an I/O controller is destroyed, it is no longer reported by the
*	local device manager as an exported device.  This routine automatically
*	removes all services associated with the controller.
*
* SEE ALSO
*	ib_create_ioc
*****/


/****f* Access Layer/ib_reg_ioc
* NAME
*	ib_reg_ioc
*
* DESCRIPTION
*	Registers an I/O controller with the local device manager, which will
*	export the controller to the fabric.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_ioc(
	IN		const	ib_ioc_handle_t				h_ioc );
/*
* PARAMETERS
*	h_ioc
*		[in] A handle to the controller being registered.
*
* RETURN VALUES
*	IB_SUCCESS
*		The I/O controller was successfully registered.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the I/O controller.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the I/O
*		unit to register the I/O controller.
*
*	IB_INVALID_HANDLE
*		The I/O controller handle was invalid.
*
* NOTES
*	This routine registers an I/O controller with the local device manager.
*	The device manager exports the controller to the fabric as part of an
*	I/O unit.  Typically, clients will call ib_add_svc_entry to add services
*	to the controller before registering it with the device manager.
*
* SEE ALSO
*	ib_create_ioc, ib_destroy_ioc, ib_add_svc_entry
*****/


/****f* Access Layer/ib_add_svc_entry
* NAME
*	ib_add_svc_entry
*
* DESCRIPTION
*	Adds a new service entry to an existing I/O controller.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_add_svc_entry(
	IN		const	ib_ioc_handle_t				h_ioc,
	IN		const	ib_svc_entry_t* const		p_svc_entry,
		OUT			ib_svc_handle_t* const		ph_svc );
/*
* PARAMETERS
*	h_ioc
*		[in] A handle to an existing I/O controller that will support the
*		added service.
*
*	p_svc_entry
*		[in] Service entry information that will be reported as part of the
*		controller's service profile.
*
*	ph_svc
*		[out] Upon successful completion of this call, this references a handle
*		to the added service.  This handle may be used to remove the service
*		entry.
*
* RETURN VALUES
*	IB_SUCCESS
*		The service entry was successfully added.
*
*	IB_INVALID_HANDLE
*		The I/O controller handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the service entry information or handle was not
*		provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the service entry.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the I/O
*		controller to register the service entry.
*
* NOTES
*	This routine adds a new service to an I/O controller.  Once added, the
*	service will be reported with the controller profile, provided that the
*	controller is registered with the local device manager.
*
* SEE ALSO
*	ib_create_ioc, ib_remove_svc_entry, ib_reg_ioc, ib_svc_entry_t
*****/


/****f* Access Layer/ib_remove_svc_entry
* NAME
*	ib_remove_svc_entry
*
* DESCRIPTION
*	This removes a service entry from an I/O controller.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_remove_svc_entry(
	IN		const	ib_svc_handle_t				h_svc );
/*
* PARAMETERS
*	h_svc
*		[in] A handle to an existing service entry.
*
* RETURN VALUES
*	IB_SUCCESS
*		The service entry was successfully removed.
*
*	IB_INVALID_HANDLE
*		The service entry handle was invalid.
*
* NOTES
*	This routine removes the specified service from its associated I/O
*	controller.  Once removed, the service information will no longer be
*	exported along with the controller.
*
* SEE ALSO
*	ib_add_svc_entry
*****/


/****f* Access Layer/ib_get_ca_guids
* NAME
*	ib_get_ca_guids
*
* DESCRIPTION
*	Returns a list of GUIDS for all channel adapter currently available in
*	the system.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_ca_guids(
	IN				ib_al_handle_t				h_al,
		OUT			ib_net64_t* const			p_guid_array OPTIONAL,
	IN	OUT			size_t* const				p_guid_cnt );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an opened instance of the access layer.
*
*	p_guid_array
*		[out] An array of GUIDs provided by the user and filled out by the
*		access layer.  If this parameter is NULL, the access layer will return
*		the number of entries in the array necessary to retrieve the GUID list.
*
*	p_guid_cnt
*		[in/out] On input, this specifies the number of entries in the
*		GUID array.
*
*		On output, the access layer will set this to the number of valid
*		entries in the p_guid_array or the minimum number of entries needed
*		in the GUID array in order to return all channel adapter GUIDs.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the GUID count was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
* NOTES
*	This routine returns a list of GUIDs for all available channel adapters.
*	When called, the access layer will examine p_guid_cnt to determine the
*	number of entries available in the p_guid_array.  If the count is too
*	small, the function will return IB_INSUFFICIENT_MEMORY, and set p_guid_cnt
*	to the number of needed entries.
*
* SEE ALSO
*	ib_open_al, ib_open_ca
*****/


/****f* Access Layer/ib_get_ca_by_gid
* NAME
*	ib_get_ca_by_gid
*
* DESCRIPTION
*	Returns the GUID of a channel adapter contain the given port GID.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_ca_by_gid(
	IN				ib_al_handle_t				h_al,
	IN		const	ib_gid_t* const				p_gid,
		OUT			ib_net64_t* const			p_ca_guid );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an opened instance of the access layer.
*
*	p_gid
*		[in] A port GID.
*
*	p_ca_guid
*		[out] A GUID to the CA that contains the port matching the user-
*		specified GID.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the port GID or CA GUID was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_NOT_FOUND
*		No channel adapters in the system contain the specifed port GID.
*
* NOTES
*	This routine returns a CA GUID that contains the user-specified port GID.
*	If no channel adapters in the system contain the port GID, the call will
*	return IB_NOT_FOUND.
*
* SEE ALSO
*	ib_open_al, ib_open_ca, ib_get_ca_guids
*****/


/****f* Access Layer/ib_get_port_by_gid
* NAME
*	ib_get_port_by_gid
*
* DESCRIPTION
*	Returns the GUID of a port that contains the given port GID.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_port_by_gid(
	IN				ib_al_handle_t				h_al,
	IN		const	ib_gid_t* const				p_gid,
		OUT			ib_net64_t* const			p_port_guid );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an opened instance of the access layer.
*
*	p_gid
*		[in] A port GID.
*
*	p_port_guid
*		[out] A GUID to the port that contains the matching user-
*		specified GID.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the port GID or port GUID was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_NOT_FOUND
*		No channel adapters in the system contain the specifed port GID.
*
* NOTES
*	This routine returns a port GUID that contains the user-specified port GID.
*	If no channel adapters in the system contain the port GID, the call will
*	return IB_NOT_FOUND.
*
* SEE ALSO
*	ib_open_al, ib_open_ca, ib_get_ca_guids
*****/


/****f* Access Layer/ib_pfn_mad_comp_cb_t
* NAME
*	ib_pfn_mad_comp_cb_t
*
* DESCRIPTION
*	User-defined callback used to notify the user of a completion for a
*	sent or received datagram.
*
* SYNOPSIS
*/
typedef	void
(AL_API *ib_pfn_mad_comp_cb_t)(
	IN		const	ib_mad_svc_handle_t			h_mad_svc,
	IN				void						*mad_svc_context,
	IN				ib_mad_element_t			*p_mad_element );
/*
* PARAMETERS
*	h_mad_svc
*		[in] Handle to the MAD service on which the completion occured.
*
*	mad_svc_context
*		[in] User-defined context information associated with the MAD service
*		on which the completion occurred.
*
*	p_mad_element
*		[in] References information on the completed MAD request.
*
* NOTES
*	This function is invoked upon completion of a sent or receive MAD.
*	It is separate from the normal completion callbacks in order to allow
*	the access layer to perform post processing on the MAD, such as
*	segmentation and reassembly, and retransmissions if a response was
*	expected.
*
*	The mad element returned through this call should be returned to its MAD
*	pool after completion processing on the MAD has concluded.  Completed
*	receive MAD elements should not be reposted to the receive queue of a
*	MAD QP.
*
*	In the kernel, this callback is typically invoked from within a
*	tasklet, depending on the implementation of the verbs provider driver.
*
* SEE ALSO
*	ib_send_mad, ib_reg_mad_svc
*****/


/****f* Access Layer/ib_create_mad_pool
* NAME
*	ib_create_mad_pool
*
* DESCRIPTION
*	Creates a pool of MAD elements for use sending and receive management
*	datagrams.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_create_mad_pool(
	IN		const	ib_al_handle_t				h_al,
	IN		const	size_t						min,
	IN		const	size_t						max,
	IN		const	size_t						grow_size,
		OUT			ib_pool_handle_t* const		ph_pool );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	min
*		[in] The minimum number of MAD elements to create in the pool.
*
*	max
*		[in] The maximum number of MAD elements that will be created by the
*		pool.  If max is set to 0, the pool will continue to grow as long
*		as system resources are available.
*
*	grow_size
*		[in] The number of MAD elements to add to the pool when growing it.
*		If set to 0, the pool will not grow beyond the number specified
*		at creation.  This value must be greater than 0, if min is set to 0.
*
*	ph_pool
*		[out] On successful completion of this call, this returns a handle to
*		the newly created pool.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD pool was created successfully.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the pool handle was not provided.
*
*	IB_INVALID_SETTING
*		The maximum number of MAD elements was non-zero and less than the
*		minimum number of MAD elements.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to create the MAD pool.
*
* NOTES
*	This routine creates a pool of MAD elements.  The elements may be used
*	to send and receive MADs on alias and MAD type QPs.
*
* SEE ALSO
*	ib_destroy_mad_pool, ib_get_mad, ib_put_mad, ib_reg_mad_pool,
*	ib_dereg_mad_pool
*****/


/****f* Access Layer/ib_destroy_mad_pool
* NAME
*	ib_destroy_mad_pool
*
* DESCRIPTION
*	Destroys a MAD pool and all associated resources.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_destroy_mad_pool(
	IN		const	ib_pool_handle_t			h_pool );
/*
* PARAMETERS
*	h_pool
*		[in] A handle to a MAD pool allocated through the ib_create_mad_pool
*		routine.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD pool was successfully destroyed.
*
*	IB_INVALID_HANDLE
*		The MAD pool handle was invalid.
*
*	IB_RESOURCE_BUSY
*		One or more MAD elements have not been returned to the MAD pool.
*
* NOTES
*	This call destroys a MAD pool and all resources allocated by the pool.
*
* SEE ALSO
*	ib_create_mad_pool, ib_get_mad, ib_put_mad
*****/


/****f* Access Layer/ib_reg_mad_pool
* NAME
*	ib_reg_mad_pool
*
* DESCRIPTION
*	Registers a MAD pool for use with a protection domain.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_mad_pool(
	IN		const	ib_pool_handle_t			h_pool,
	IN		const	ib_pd_handle_t				h_pd,
		OUT			ib_pool_key_t* const		p_pool_key );
/*
* PARAMETERS
*	h_pool
*		[in] A handle to a MAD pool.
*
*	h_pd
*		[in] A handle to a protection domain.
*
*	p_pool_key
*		[out] A key associated with registering the MAD pool with the
*		protection domain.  This key is returned to the user and is used
*		when retrieving MADs from the pool.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD pool was successfully registered with the protection domain.
*
*	IB_INVALID_HANDLE
*		The MAD pool handle was invalid.
*
*	IB_INVALID_PD_HANDLE
*		The protection domain handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the pool key was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the MAD pool.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to register the MAD pool.
*
* NOTES
*	This function registers a MAD pool with a protection domain.  After
*	successful completion of this call, the MAD elements of the associated
*	pool are usable on any queue pairs associated with the given protection
*	domain.
*
* SEE ALSO
*	ib_create_mad_pool, ib_destroy_mad_pool, ib_dereg_mad_pool, ib_get_mad
*****/


/****f* Access Layer/ib_dereg_mad_pool
* NAME
*	ib_dereg_mad_pool
*
* DESCRIPTION
*	Deregisters a MAD pool from a protection domain.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_dereg_mad_pool(
	IN		const	ib_pool_key_t				pool_key );
/*
* PARAMETERS
*	pool_key
*		[in] Key to the MAD pool to deregister.  The specified pool must
*		have been registered with a protection domain through a call to
*		ib_reg_mad_pool.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD pool was successfully deregistered from the protection domain.
*
*	IB_INVALID_PARAMETER
*		The MAD pool key was invalid.
*
*	IB_RESOURCE_BUSY
*		One or more MAD elements were removed from the MAD pool using the
*		specified pool key, and were not returned.
*
* NOTES
*	This function deregisters a MAD pool with a protection domain.  After
*	successful completion of this call, the MAD elements of the associated
*	pool are no longer usable on the protection domain.
*
* SEE ALSO
*	ib_create_mad_pool, ib_destroy_mad_pool, ib_reg_mad_pool
*****/


/****f* Access Layer/ib_get_mad
* NAME
*	ib_get_mad
*
* DESCRIPTION
*	Obtains a MAD element from the pool.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_mad(
	IN		const	ib_pool_key_t				pool_key,
	IN		const	size_t						buf_size,
		OUT			ib_mad_element_t			**pp_mad_element );
/*
* PARAMETERS
*	pool_key
*		[in] Key for the pool to obtain a MAD element for the desired
*		protection domain.
*
*	buf_size
*		[in] The size of the buffer referenced by the MAD element.
*
*	pp_mad_element
*		[out] Upon successful completion of this call, this references
*		the returned MAD element.
*
* RETURN VALUES
*	IB_SUCCESS
*		The MAD element was successfully retrieved from the MAD pool.
*
*	IB_INVALID_PARAMETER
*		The MAD pool key was invalid or a reference to the MAD element
*		pointer was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to obtain the MAD element.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to grow and register the MAD pool.
*
* NOTES
*	This function obtains a MAD element containing a data segment
*	that references a data buffer for the given pool key.  The data buffer
*	referenced by the MAD element is zeroed before being returned to the
*	user.
*
*	It is recommended that elements retrieved from a MAD pool for use on
*	the receive queue of a MAD QP have a buffer size of 256 bytes.
*
*	For MADs being sent, buf_size should be set to the size of the relevant
*	data sent as part of the MAD, and should not include any padding needed
*	to make the MAD size a multiple of 256 bytes.  For most MADs, buf_size
*	may be set equal to the size of the MAD header plus the amount of user
*	data transfered as part of the MAD.
*
* SEE ALSO
*	ib_put_mad, ib_send_mad, ib_mad_element_t
*****/


/****f* Access Layer/ib_put_mad
* NAME
*	ib_put_mad
*
* DESCRIPTION
*	Returns a list of MAD elements to the pool.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_put_mad(
	IN		const	ib_mad_element_t*			p_mad_element_list );
/*
* PARAMETERS
*	p_mad_element_list
*		[in] A pointer to a list of MAD elements.
*
* RETURN VALUES
*	IB_SUCCESS
*		The list of MAD elements was successfully returned to the MAD pool.
*
*	IB_INVALID_PARAMETER
*		A reference to the MAD element list was not provided.
*
* NOTES
*	This function returns a list of MAD elements to the pool.
*
* SEE ALSO
*	ib_get_mad, ib_mad_element_t
*****/

AL_EXPORT ib_api_status_t AL_API
ib_put_mad_locked(
	IN		const	ib_mad_element_t*			p_mad_element_list );


/****s* Access Layer/ib_dgrm_info_t
* NAME
*	ib_dgrm_info_t
*
* DESCRIPTION
*	Information specified when initializing a datagram queue pair before its
*	first use.
*
* SYNOPSIS
*/
typedef struct _ib_dgrm_info
{
	ib_net64_t					port_guid;
	uint32_t					qkey;
	uint16_t					pkey_index;

}	ib_dgrm_info_t;
/*
* FIELDS
*	port_guid
*		Specifies the port that the datagram service will use.  This field
*		applies only to IB_QPT_UNRELIABLE_DGRM and IB_QPT_MAD QP types.
*
*	qkey
*		Specifies the qkey that the queue pair will use.  Incoming messages
*		must have a matching qkey for the message to be accepted by the
*		receiving QP.
*
*	pkey_index
*		Specifies the pkey associated with this queue pair.
*
* SEE ALSO
*	ib_init_dgrm_svc
*****/


/****f* Access Layer/ib_init_dgrm_svc
* NAME
*	ib_init_dgrm_svc
*
* DESCRIPTION
*	Initializes a datagram queue pair for use.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_init_dgrm_svc(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_dgrm_info_t* const		p_dgrm_info OPTIONAL );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to an existing queue pair.
*
*	p_dgrm_info
*		[in] References information needed to configure the queue pair for
*		use sending and receiving datagrams.  This field is optional for
*		IB_QPT_QP0, IB_QPT_QP1 queue pair types and is not used for
*		IB_QPT_RAW_IPV6, and IB_QPT_RAW_ETHER queue pair types.
*
* RETURN VALUES
*	IB_SUCCESS
*		The datagram queue pair was initialized successfully.
*
*	IB_INVALID_QP_HANDLE
*		The datagram queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		The queue pair handle was not created as a datagram queue pair type
*		or a reference to the datagram service information was not provided.
*
*	IB_INVALID_QP_STATE
*		The queue pair was in an invalid state for the requested operation.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to initialize the datagram queue pair.
*
* NOTES
*	This call binds the queue pair to a given port and transitions its state
*	to ready to send and receive data.  A queue pair must be initialized
*	before it can be used to send and receive datagrams.
*
*	This routine is used to initialize queue pairs of type:
*
*	IB_QPT_QP0
*	IB_QPT_QP1
*	IB_QPT_MAD
*	IB_QPT_RAW_IPV6
*	IB_QPT_RAW_ETHER
*	IB_QPT_UNRELIABLE_DGRM
*
*	For IB_QPT_MAD type queue pairs, receive buffers are automatically posted
*	by the access layer, however, users must call ib_reg_mad_svc to receive
*	MADs.  Received MAD elements must be returned to the access layer through
*	the ib_put_mad() call.
*
* SEE ALSO
*	ib_create_qp, ib_get_spl_qp, ib_dgrm_info_t, ib_reg_mad_svc
*****/


/****d* Access Layer/ib_mad_svc_type_t
* NAME
*	ib_mad_svc_type_t
*
* DESCRIPTION
*	Indicates the type of services provided by a MAD service.
*
* SYNOPSIS
*/
typedef enum _ib_mad_svc_type
{
	IB_MAD_SVC_DEFAULT = 0,
	IB_MAD_SVC_RMPP,
	IB_MAD_SVC_RAW

}	ib_mad_svc_type_t;
/*
* VALUES
*	IB_MAD_SVC_DEFAULT
*		Indicates that the access layer will provide all necessary services,
*		including retransmissions and RMPP for well-defined management classes.
*
*	IB_MAD_SVC_RMPP
*		Indicates that the MAD service requires retransmissions and the RMPP
*		header is available on all MADs.  (The RMPP protocol will be activated
*		on a per send basis.)  This service type should be used for
*		user-defined management classes requiring RMPP.
*
*	IB_MAD_SVC_RAW
*		Specifies that the MAD service will not perform retransmissions or
*		perform RMPP.  All MADs received or sent on a MAD service of this type
*
* NOTES
*	This enum is used to define the types of MAD services available to users.
*
* SEE ALSO
*	ib_mad_svc_t, ib_reg_mad_svc
*****/



/****s* Access Layer/ib_mad_svc_t
* NAME
*	ib_mad_svc_t
*
* DESCRIPTION
*	Information used to request management datagram support with a queue pair.
*
* SYNOPSIS
*/
typedef struct _ib_mad_svc
{
	TO_LONG_PTR(void,			*mad_svc_context);
	TO_LONG_PTR(ib_pfn_mad_comp_cb_t, pfn_mad_send_cb);
	TO_LONG_PTR(ib_pfn_mad_comp_cb_t, pfn_mad_recv_cb);

	boolean_t					support_unsol;
	uint8_t						mgmt_class;
	uint8_t						mgmt_version;
	boolean_t					method_array[IB_MAX_METHODS];

    ib_mad_svc_type_t			svc_type;

}	ib_mad_svc_t;
/*
* FIELDS
*	mad_svc_context
*		User-defined context that is returned by the access layer through
*		the pfn_mad_send_cb and pfn_mad_recv_cb.
*
*	pfn_mad_send_cb
*		A send callback that is invoked to notify the user that a send
*		operation has completed for a sent MAD.
*
*	pfn_mad_recv_cb
*		A receive callback that is invoked to notify the user that a MAD
*		has been received.
*
*	support_unsol
*		If set to TRUE, this field indicates that the registering client
*		supports processing unsolicited MADs.  Unsolicited MADs are
*		received MADs that do not have the response bit set.  If set to TRUE,
*		the following fields are required (must be non-zero): mgmt_class,
*		mgmt_version, and method_array.
*
*	mgmt_version
*		Indicates which version of a management class the client requires
*		support for.  The access layer distinguishes between clients
*		requiring different versions of the same management class.
*		This field is ignored if the support_unsol field is set to FALSE.
*
*	mgmt_class
*		Indicates the management class that should be supported by the
*		access layer.  This field is ignored if the support_unsol field is
*		set to FALSE.
*
*	method_array
*		An array of 127 entries specifying which methods are supported by
*		a client when receiving unsolicited MADs.  Each index corresponds to
*		a single method, and each entry in the array indicates if the method
*		is supported by the client.  This field is ignored if the
*		support_unsol field is set to FALSE.
*
*	svc_type
*		Indicates the type of services that should be provided by the MAD
*		service.
*
* NOTES
*	Clients use this structure to define which management datagram methods
*	they support, and the type of support required for each.  A received MAD
*	is distinguished by the access layer based on the following three fields:
*	management class, management version, and method.
*
*	Specific combinations of class, version, and method may be registered
*	for unsolicited MADs only once.  The access layer supports multiple
*	clients registering for unsolicited MADs as long as they do not share the
*	same methods, class, or version.
*
*	The svc_type field can be set by a client to indicate that the access
*	layer should invoke RMPP for the specified management class of MADs.  If
*	set to IB_MAD_SVC_DEFAULT, the access layer will automatically invoke RMPP
*	for well known MAD classes (those defined by the 1.1 version of the
*	InfiniBand specification).  The svc_type field is intended to be used by
*	clients sending and receiving vendor specific management class requiring
*	RMPP and clients providing their own MAD services.
*
* SEE ALSO
*	ib_reg_mad_svc, ib_pfn_mad_send_cb_t, ib_pfn_mad_recv_cb_t,
*	ib_mad_svc_type_t
*****/


/****f* Access Layer/ib_reg_mad_svc
* NAME
*	ib_reg_mad_svc
*
* DESCRIPTION
*	Requests management datagram support for a specified class with a
*	queue pair.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_mad_svc(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_mad_svc_t* const			p_mad_svc,
		OUT			ib_mad_svc_handle_t* const	ph_mad_svc );
/*
* PARAMETERS
*	h_qp
*		[in] A handle to queue pair.  The queue pair must have been created
*		as one of the following types: IB_QPT_QP0, IB_QPT_QP0_ALIAS,
*		IB_QPT_QP1, IB_QPT_QP1_ALIAS, or IB_QPT_MAD.
*
*	p_mad_svc
*		[in] A reference to the management class and methods supported by
*		this queue pair.
*
*	ph_mad_svc
*		[out] On successful completion of this call, this references a
*		handle to the newly created MAD service.
*
* RETURN VALUES
*	IB_SUCCESS
*		The queue pair was registered successfully.
*
*	IB_INVALID_QP_HANDLE
*		The queue pair handle was invalid.
*
*	IB_INVALID_PARAMETER
*		The queue pair handle was not created with the proper queue pair
*		type or a reference to the MAD service information or handle was
*		not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register the queue pair.
*
* NOTES
*	This routine registers a queue pair as using a particular management
*	class.  This indicates that the access layer should perform additional
*	processing on MADs sent and received by this queue pair.  Queue pairs
*	registered for MAD support receive access layer SAR and retransmissions
*	services.  A queue pair may be registered for multiple management classes.
*
* SEE ALSO
*	ib_create_qp, ib_mad_svc_t
*****/

/****s* Access Layer/ib_reg_svc_rec_t
* NAME
*	_ib_reg_svc_rec_t
*
* DESCRIPTION
*	Information returned as a result of registering a service with the subnet
*	administrator.  This includes name service registration.
*
* SYNOPSIS
*/
typedef struct _ib_reg_svc_rec
{
	const void*					svc_context;
	ib_reg_svc_handle_t			h_reg_svc;
	ib_api_status_t				req_status;
	ib_net16_t					resp_status;
	ib_service_record_t			svc_rec;

}	ib_reg_svc_rec_t;
/*
* FIELDS
*	svc_context
*		User-defined context information associated with the registration
*		through the ib_reg_svc call.
*
*	req_status
*		Indicates the success of the registration operation.
*
*	resp_status
*		Indicates the status of the response from the SA
*
*	h_reg_svc
*		For successful queries, this references the first record of
*		information returned by the subnet administrator.  If multiple
*		records of information were returned, the ib_reg_svc_rec_t will
*		be chained together.
*
*	svc_rec
*		The service record returned by the SA for the registered service.
*
* NOTES
*	A query result structure is returned to a client through their
*	ib_pfn_query_cb_t routine to notify them of the results of a subnet
*	administration query.
*
* SEE ALSO
*	ib_reg_svc, ib_pfn_reg_svc_cb_t, ib_reg_svc_status_t
*****/


/****f* Access Layer/ib_pfn_reg_svc_cb_t
* NAME
*	ib_pfn_reg_svc_cb_t
*
* DESCRIPTION
*	User-defined callback that is invoked to notify a client of the result
*	of a service registration attempt.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_reg_svc_cb_t)(
	IN				ib_reg_svc_rec_t			*p_reg_svc_rec );
/*
* PARAMETERS
*	p_reg_svc_rec
*		[in] References the result of the service registration attempt.
*
* NOTES
*	The callback is used to notify a client of the result of a service
*	registration attempt with the subnet administrator.
*
*	In the kernel, this callback is usually invoked using a tasklet, dependent
*	on the implementation of the underlying verbs provider driver.
*
* SEE ALSO
*	ib_reg_svc, ib_reg_svc_rec_t
*****/


/****s* Access Layer/ib_reg_svc_req_t
* NAME
*	ib_reg_svc_req_t
*
* DESCRIPTION
*	Information used to request that a service be registered with the subnet
*	administrator.
*
* SYNOPSIS
*/
typedef struct _ib_reg_svc_req
{
	ib_service_record_t			svc_rec;
	ib_net64_t					port_guid;

	uint32_t					timeout_ms;
	uint32_t					retry_cnt;
	ib_al_flags_t				flags;

	const void					*svc_context;
	ib_net64_t					svc_data_mask;

	ib_pfn_reg_svc_cb_t			pfn_reg_svc_cb;

}	ib_reg_svc_req_t;
/*
* FIELDS
*	svc_rec
*		Service record that describes the service being registered.
*
*	port_guid
*		Directs the registration to use the specified port.  The request will
*		contact the management entity reachable through the given port.
*
*	timeout_ms
*		Specifies the number of milliseconds to wait for a response for
*		the registration until retrying or timing out the request.
*
*	retry_cnt
*		Specifies the number of times that the registration will be retried
*		before failing the request.
*
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	svc_context
*		User-defined context information associated with this registration
*		request.  This context is returned to the user through the function
*		specified by the pfn_reg_svc_cb field.
*
*	svc_data_mask
*		User-defined component mask indicating which parts of the private
*		data is populated. This is used as an extension to the svc_id
*		for data compare. Also used as a cheap way to communicate data
*		to all clients for this service.
*
*	pfn_reg_svc_cb
*		A user-defined callback that is invoked upon completion of the
*		registration request.
*
* NOTES
*	This structure is used to register a service with the subnet administrator.
*	The registration call operates asynchronously unless the flags field is
*	set to IB_FLAGS_SYNC.  If synchronous operation is indicated, the client
*	will receive a callback with the results of the registration attempt
*	before the ib_reg_svc call returns.  Synchronous operation results in
*	the calling thread blocking.
*
* SEE ALSO
*	ib_reg_svc, ib_svc_rec_t, ib_pfn_reg_svc_cb_t
*****/


/****f* Access Layer/ib_reg_svc
* NAME
*	ib_reg_svc
*
* DESCRIPTION
*	Routine used to register for a service with the subnet administrator.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_svc(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_reg_svc_req_t* const		p_reg_svc_req,
		OUT			ib_reg_svc_handle_t* const	ph_reg_svc );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	p_reg_svc_req
*		[in] Describes the service to register with the subnet administrator.
*
*	ph_reg_svc
*		[out] Pointer to a service registration handle, used to deregister
*		the service.  Set upon successful completion of the function.
*
* RETURN VALUES
*	IB_SUCCESS
*		The service registration was initiated.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the service registration request was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_NOT_FOUND
*		No channel adapters in the system contain the GID specified in the
*		service record.
*
*	IB_INVALID_GID
*		No port was found matching the GID specified in the service record.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine registers a service with the subnet administrator.  Registered
*	services are reported by the subnet administrator to clients querying the
*	subnet administrator for service information.
*
*	Once registered, a client will receive notification, via a callback,
*	that a service has been successfully registered.
*
* SEE ALSO
*	ib_dereg_svc, ib_reg_svc_req_t
*****/


/****f* Access Layer/ib_dereg_svc
* NAME
*	ib_dereg_svc
*
* DESCRIPTION
*	Remove a service as being registered with the subnet administrator.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_dereg_svc(
	IN		const	ib_reg_svc_handle_t			h_reg_svc,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_reg_svc
*		[in] A handle to a registered service.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the service
*		has been deregistered.
*
* RETURN VALUES
*	IB_SUCCESS
*		The service deregistration was initiated.
*
*	IB_INVALID_HANDLE
*		The registered service handle was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine deregisters a service with the subnet administrator.
*	To avoid a race condition deregistering a service at the same time
*	the registration completion callback is in progress, the deregister
*	operation operates asynchronously.  For additional details see
*	ib_pfn_destroy_cb_t.
*
* SEE ALSO
*	ib_reg_svc, ib_pfn_destroy_cb_t
*****/


/****d* Access Layer/ib_query_type_t
* NAME
*	ib_query_type_t
*
* DESCRIPTION
*	Abstracted queries supported by the access layer.
*
* SYNOPSIS
*/
typedef enum _ib_query_type
{
	IB_QUERY_USER_DEFINED,

	IB_QUERY_ALL_SVC_RECS,
	IB_QUERY_SVC_REC_BY_NAME,
	IB_QUERY_SVC_REC_BY_ID,

	IB_QUERY_CLASS_PORT_INFO,

	IB_QUERY_NODE_REC_BY_NODE_GUID,
	IB_QUERY_PORT_REC_BY_LID,

	IB_QUERY_VLARB_BY_LID_PORT_BLOCK,
	IB_QUERY_SLVL_BY_LID_AND_PORTS,

	IB_QUERY_PATH_REC_BY_PORT_GUIDS,
	IB_QUERY_PATH_REC_BY_GIDS,
	IB_QUERY_PATH_REC_BY_LIDS,

}	ib_query_type_t;
/*
* VALUES
*	IB_QUERY_USER_DEFINED
*		Query the SA based on user-defined input.  Queries of this type
*		should reference an ib_user_query_t structure as input into the
*		query.
*
*	IB_QUERY_SVC_REC_BY_NAME
*		Query for service records based on the service name.  Queries of
*		this type should reference an ib_svc_name_t structure as input
*		into the query.
*
*	IB_QUERY_SVC_REC_BY_ID
*		Query for service records based on the service ID.  Queries of
*		this type should reference an ib_net64_t value that indicates the
*		ID of the service being requested.
*
*	IB_QUERY_NODE_REC_BY_NODE_GUID
*		Query for node information based on the node's GUID.  Queries of
*		this type should reference an ib_net64_t value that indicates the
*		GUID of the node being requested.
*
*	IB_QUERY_PORT_REC_BY_LID
*		Query for port information based on the port's base LID.  Queries of
*		this type should reference an ib_net16_t value that indicates the
*		base LID of the port being requested.
*
*	IB_QUERY_PATH_REC_BY_PORT_GUIDS
*		Query for path records between the specified pair of port GUIDs.
*		Queries of this type should reference an ib_guid_pair_t structure
*		that indicates the GUIDs of the path being requested.
*
*	IB_QUERY_PATH_REC_BY_GIDS
*		Query for path records between the specified pair of port GIDs.
*		Queries of this type should reference an ib_gid_pair_t structure
*		that indicates the GIDs of the path being requested.
*
*	IB_QUERY_PATH_REC_BY_LIDS
*		Query for path records between the specified pair of port LIDs.
*		Queries of this type should reference an ib_lid_pair_t structure
*		that indicates the LIDs of the path being requested.
*
* NOTES
*	This enum is used to define abstracted queries provided by the access
*	layer.  Users may issue queries not listed here by sending MADs directly
*	to the subnet administrator or a class manager.  These queries are
*	intended to represent those most often used by clients.
*
* SEE ALSO
*	ib_query, ib_query_req_t, ib_user_query_t, ib_gid_pair_t, ib_lid_pair_t
*	ib_guid_pair_t
*****/


/****s* Access Layer/ib_user_query_t
* NAME
*	ib_user_query_t
*
* DESCRIPTION
*	User-defined query information.
*
* SYNOPSIS
*/
typedef struct _ib_user_query
{
	uint8_t					method;
	ib_net16_t				attr_id;
	uint32_t				attr_size;
	ib_net64_t				comp_mask;
	TO_LONG_PTR(void*,		p_attr);

}	ib_user_query_t;
/*
* FIELDS
*
*	method
*		Method to be run
*
*	attr_id
*		Attribute identifier of query data.
*
*	attr_size
*		Size of the query attribute in bytes.  This is translated into the
*		attr_offset field of the SA MAD by the ib_query call.
*
*	comp_mask
*		Indicates the attribute components that are specified for the query.
*
*	p_attr
*		References the attribute structure used as input into the query.
*		This field is ignored if comp_mask is set to 0.
*
* NOTES
*	This structure is used to describe a user-defined query.  The attribute
*	ID, attribute offset, component mask, and attribute structure must match
*	those defined by the IBA specification.  Users should refer to chapter 15
*	of the IBA specification for additional details.
*
* SEE ALSO
*	ib_query_type_t, ib_query, ib_get_attr_offset, ib_get_attr_size
*****/


/****s* Access Layer/ib_gid_pair_t
* NAME
*	ib_gid_pair_t
*
* DESCRIPTION
*	Source and destination GIDs.
*
* SYNOPSIS
*/
typedef struct _ib_gid_pair
{
	ib_gid_t					src_gid;
	ib_gid_t					dest_gid;

}	ib_gid_pair_t;
/*
* FIELDS
*	src_gid
*		Source GID of a path.
*
*	dest_gid
*		Destination GID of a path.
*
* NOTES
*	This structure is used to describe the endpoints of a path.
*
* SEE ALSO
*	ib_gid_t
*****/


/****s* Access Layer/ib_lid_pair_t
* NAME
*	ib_lid_pair_t
*
* DESCRIPTION
*	Source and destination LIDs.
*
* SYNOPSIS
*/
typedef struct _ib_lid_pair
{
	ib_net16_t					src_lid;
	ib_net16_t					dest_lid;

}	ib_lid_pair_t;
/*
* FIELDS
*	src_lid
*		Source LID of a path.
*
*	dest_lid
*		Destination LID of a path.
*
* NOTES
*	This structure is used to describe the endpoints of a path.
*****/


/****s* Access Layer/ib_guid_pair_t
* NAME
*	ib_guid_pair_t
*
* DESCRIPTION
*	Source and destination GUIDs.  These may be port or channel adapter
*	GUIDs, depending on the context in which this structure is used.
*
* SYNOPSIS
*/
typedef struct _ib_guid_pair
{
	ib_net64_t					src_guid;
	ib_net64_t					dest_guid;

}	ib_guid_pair_t;
/*
* FIELDS
*	src_guid
*		Source GUID of a path.
*
*	dest_guid
*		Destination GUID of a path.
*
* NOTES
*	This structure is used to describe the endpoints of a path.  The given
*	GUID pair may belong to either ports or channel adapters.
*
* SEE ALSO
*	ib_guid_t
*****/


/****s* Access Layer/ib_query_rec_t
* NAME
*	ib_query_rec_t
*
* DESCRIPTION
*	Contains the results of a subnet administration query.
*
* SYNOPSIS
*/
typedef struct _ib_query_rec
{
	const void*					query_context;
	ib_api_status_t				status;

	ib_query_type_t				query_type;
	uint32_t					result_cnt;
	ib_mad_element_t*			p_result_mad;

}	ib_query_rec_t;
/*
* FIELDS
*	query_context
*		User-defined context information associated with the query through
*		the ib_reg_query call.
*
*	status
*		Indicates the success of the query operation.
*
*	query_type
*		Indicates the type of query for which the results are being returned.
*		This matches the query_type specified through the ib_reg_query call.
*
*	result_cnt
*		The number of result structures that were returned by the query.
*
*	p_result_mad
*		For queries returning IB_SUCCESS or IB_REMOTE_ERROR, this references
*		the MAD returned by the subnet administrator containing the list
*		of results or the returned error code.
*
* NOTES
*	A query result structure is returned to a client through their
*	ib_pfn_query_cb_t routine to notify them of the results of a subnet
*	administration query.  If the query was successful or received an error
*	from the subnet administrator, p_result_mad will reference a MAD element
*	containing the results.  The MAD referenced by p_result_mad is owned by
*	the user and remains available even after their callback returns.  Users
*	must call ib_put_mad() to return the MAD element back to the access layer
*	when they are done accessing the results.
*
*	To retrieve individual result structures from the p_result_mad, users
*	may call ib_get_query_result().
*
* SEE ALSO
*	ib_query, ib_pfn_query_cb_t, ib_api_status_t, ib_put_mad, ib_mad_element_t
*	ib_query_status_t, ib_query_type_t, ib_get_query_result
*****/


/****f* Access Layer/ib_get_query_result
* NAME
*	ib_get_query_result
*
* DESCRIPTION
*	Retrieves a result structure from a MAD returned by a call to ib_query().
*
* SYNOPSIS
*/
AL_INLINE void* AL_API
ib_get_query_result(
	IN				ib_mad_element_t			*p_result_mad,
	IN				uint32_t					result_index )
{
	ib_sa_mad_t		*p_sa_mad;

	CL_ASSERT( p_result_mad );
	p_sa_mad = (ib_sa_mad_t*)ib_get_mad_buf( p_result_mad );
	CL_ASSERT( p_sa_mad );
	CL_ASSERT( ib_get_attr_size( p_sa_mad->attr_offset ) * (result_index + 1) +
		IB_SA_MAD_HDR_SIZE <= p_result_mad->size );

	return( p_sa_mad->data +
		(ib_get_attr_size( p_sa_mad->attr_offset ) * result_index) );
}
/*
* PARAMETERS
*	p_result_mad
*		[in] This is a reference to the MAD returned as a result of the
*		query.
*
*	result_index
*		[in] A zero-based index indicating which result to return.
*
* NOTES
*	This call returns a pointer to the start of a result structure from a call
*	to ib_query().  The type of result structure must be known to the user
*	either through the user's context or the query_type returned as part of
*	the ib_query_rec_t structure.
*
* SEE ALSO
*	ib_query_rec_t, ib_mad_element_t
*****/


/****f* Access Layer/ib_get_query_path_rec
* NAME
*	ib_get_query_path_rec
*
* DESCRIPTION
*	Retrieves a path record result from a MAD returned by a call to
*	ib_query().
*
* SYNOPSIS
*/
AL_INLINE ib_path_rec_t* AL_API
ib_get_query_path_rec(
	IN				ib_mad_element_t			*p_result_mad,
	IN				uint32_t					result_index )
{
	ib_sa_mad_t		*p_sa_mad;

	CL_ASSERT( p_result_mad );
	p_sa_mad = (ib_sa_mad_t*)ib_get_mad_buf( p_result_mad );
	CL_ASSERT( p_sa_mad && p_sa_mad->attr_id == IB_MAD_ATTR_PATH_RECORD );

	return( (ib_path_rec_t*)ib_get_query_result( p_result_mad, result_index ) );
}
/*
* PARAMETERS
*	p_result_mad
*		[in] This is a reference to the MAD returned as a result of the
*		query.
*
*	result_index
*		[in] A zero-based index indicating which result to return.
*
* NOTES
*	This call returns a pointer to the start of a path record result from
*	a call to ib_query().
*
* SEE ALSO
*	ib_query_rec_t, ib_mad_element_t, ib_get_query_result, ib_path_rec_t
*****/


/****f* Access Layer/ib_get_query_portinfo_rec
* NAME
*	ib_get_query_portinfo_rec
*
* DESCRIPTION
*	Retrieves a port info record result from a MAD returned by a call to
*	ib_query().
*
* SYNOPSIS
*/
AL_INLINE ib_portinfo_record_t* AL_API
ib_get_query_portinfo_rec(
	IN				ib_mad_element_t			*p_result_mad,
	IN				uint32_t					result_index )
{
	ib_sa_mad_t		*p_sa_mad;

	CL_ASSERT( p_result_mad );
	p_sa_mad = (ib_sa_mad_t*)ib_get_mad_buf( p_result_mad );
	CL_ASSERT( p_sa_mad && p_sa_mad->attr_id == IB_MAD_ATTR_PORTINFO_RECORD );

	return( (ib_portinfo_record_t*)ib_get_query_result( p_result_mad,
		result_index ) );
}
/*
* PARAMETERS
*	p_result_mad
*		[in] This is a reference to the MAD returned as a result of the
*		query.
*
*	result_index
*		[in] A zero-based index indicating which result to return.
*
* NOTES
*	This call returns a pointer to the start of a port info record result from
*	a call to ib_query().
*
* SEE ALSO
*	ib_query_rec_t, ib_mad_element_t, ib_get_query_result, ib_portinfo_record_t
*****/


/****f* Access Layer/ib_get_query_node_rec
* NAME
*	ib_get_query_node_rec
*
* DESCRIPTION
*	Retrieves a node record result from a MAD returned by a call to
*	ib_query().
*
* SYNOPSIS
*/
AL_INLINE ib_node_record_t* AL_API
ib_get_query_node_rec(
	IN				ib_mad_element_t			*p_result_mad,
	IN				uint32_t					result_index )
{
	ib_sa_mad_t		*p_sa_mad;

	CL_ASSERT( p_result_mad );
	p_sa_mad = (ib_sa_mad_t*)ib_get_mad_buf( p_result_mad );
	CL_ASSERT( p_sa_mad && p_sa_mad->attr_id == IB_MAD_ATTR_NODE_RECORD );

	return( (ib_node_record_t*)ib_get_query_result( p_result_mad,
		result_index ) );
}
/*
* PARAMETERS
*	p_result_mad
*		[in] This is a reference to the MAD returned as a result of the
*		query.
*
*	result_index
*		[in] A zero-based index indicating which result to return.
*
* NOTES
*	This call returns a pointer to the start of a node record result from
*	a call to ib_query().
*
* SEE ALSO
*	ib_query_rec_t, ib_mad_element_t, ib_get_query_result, ib_node_record_t
*****/


/****f* Access Layer/ib_get_query_svc_rec
* NAME
*	ib_get_query_svc_rec
*
* DESCRIPTION
*	Retrieves a service record result from a MAD returned by a call to
*	ib_query().
*
* SYNOPSIS
*/
AL_INLINE ib_service_record_t* AL_API
ib_get_query_svc_rec(
	IN				ib_mad_element_t			*p_result_mad,
	IN				uint32_t					result_index )
{
	ib_sa_mad_t		*p_sa_mad;

	CL_ASSERT( p_result_mad );
	p_sa_mad = (ib_sa_mad_t*)ib_get_mad_buf( p_result_mad );
	CL_ASSERT( p_sa_mad && p_sa_mad->attr_id == IB_MAD_ATTR_SERVICE_RECORD );

	return( (ib_service_record_t*)ib_get_query_result( p_result_mad,
		result_index ) );
}
/*
* PARAMETERS
*	p_result_mad
*		[in] This is a reference to the MAD returned as a result of the
*		query.
*
*	result_index
*		[in] A zero-based index indicating which result to return.
*
* NOTES
*	This call returns a pointer to the start of a service record result from
*	a call to ib_query().
*
* SEE ALSO
*	ib_query_rec_t, ib_mad_element_t, ib_get_query_result, ib_service_record_t
*****/


/****f* Access Layer/ib_pfn_query_cb_t
* NAME
*	ib_pfn_query_cb_t
*
* DESCRIPTION
*	User-defined callback invoked on completion of a subnet administrator
*	query.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_query_cb_t)(
	IN				ib_query_rec_t				*p_query_rec );
/*
* PARAMETERS
*	p_query_rec
*		[in] This is a reference to a structure containing the result of the
*		query.
*
* NOTES
*	This routine is invoked to notify a client of the result of a subnet
*	administration query.  The p_query_rec parameter references the result
*	of the query and, in the case of a successful query, any information
*	returned by the subnet administrator.
*
*	In the kernel, this callback is usually invoked using a tasklet, dependent
*	on the implementation of the underlying verbs provider driver.
*
* SEE ALSO
*	ib_query_rec_t
*****/


/****s* Access Layer/ib_query_req_t
* NAME
*	ib_query_req_t
*
* DESCRIPTION
*	Information used to request an access layer provided query of the subnet
*	administrator.
*
* SYNOPSIS
*/
typedef struct _ib_query_req
{
	ib_query_type_t				query_type;
	const void*					p_query_input;
	ib_net64_t					port_guid;

	uint32_t					timeout_ms;
	uint32_t					retry_cnt;
	ib_al_flags_t				flags;

	const void*					query_context;
	ib_pfn_query_cb_t			pfn_query_cb;

}	ib_query_req_t;
/*
* FIELDS
*	query_type
*		Indicates the type of query that the access layer should perform.
*
*	p_query_input
*		A pointer to the input for the query.  The data referenced by this
*		structure is dependent on the type of query being requested and is
*		determined by the specified query_type.
*
*	port_guid
*		Directs the query to use the specified port.  The request will
*		contact the management entity reachable through the given port.
*
*	timeout_ms
*		Specifies the number of milliseconds to wait for a response for
*		this query until retrying or timing out the request.
*
*	retry_cnt
*		Specifies the number of times that the query will be retried before
*		failing the request.
*
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	query_context
*		User-defined context information associated with this query.  The
*		context data is returned to the user as a part of their query
*		callback.
*
*	pfn_query_cb
*		A user-defined callback that is invoked upon completion of the query.
*
* NOTES
*	This structure is used when requesting an access layer provided query
*	of the subnet administrator.  Clients specify the type of query through
*	the query_type field.  Based on the type of query, the p_query_input
*	field is set to reference the appropriate data structure.
*
*	The information referenced by the p_query_input field is one of the
*	following:
*
*		-- a NULL terminated service name
*		-- a service id
*		-- a single GUID
*		-- a pair of GUIDs specified through an ib_guid_pair_t structure
*		-- a pair of GIDs specified through an ib_gid_pair_t structure
*
* SEE ALSO
*	ib_query_type_t, ib_pfn_query_cb_t, ib_guid_pair_t,
*	ib_gid_pair_t
*****/


/****f* Access Layer/ib_query
* NAME
*	ib_query
*
* DESCRIPTION
*	Routine used to request an access layer provided query of the subnet
*	administrator.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_query(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_query_req_t* const		p_query_req,
		OUT			ib_query_handle_t* const	ph_query OPTIONAL );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	p_query_req
*		[in] Specifies the type of query that the access layer should perform,
*		along with information needed to process the completed query.
*
*	ph_query
*		[out] Pointer to a query handle that can be used to cancel the query.
*
* RETURN VALUES
*	IB_SUCCESS
*		The subnet administrator query was initiated.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the query request was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_INVALID_GUID
*		No port was found for the port_guid specified in the request.
*
*	IB_ERROR
*		An invalid query_type was specified in the request.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine directs the access layer to initiate a query to the subnet
*	administrator for desired information.  The access layer will issue the
*	query, collect the results, and report them to the client through a user-
*	specified callback.  The access layer is responsible for retrying the
*	operation as directed by the client.
*
* SEE ALSO
*	ib_cancel_query, ib_query_req_t
*****/


/****f* Access Layer/ib_cancel_query
* NAME
*	ib_cancel_query
*
* DESCRIPTION
*	Routine used to cancel a query of the subnet administrator.
*
* SYNOPSIS
*/
AL_EXPORT void AL_API
ib_cancel_query(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_query_handle_t			h_query );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	h_query
*		[in] Query handle returned by a previous call to ib_query().
*
* RETURN VALUES
*	This function does not return a value.
*
* NOTES
*	This routine directs the access layer to cancel a query to the subnet
*	administrator.  The access layer will issue notify the user with the
*	final status of the query through the query callback specified in the
*	call to ib_query().
*
* SEE ALSO
*	ib_query
*****/


/****d* Access Layer/ib_pnp_class_t
* NAME
*	ib_pnp_class_t
*
* DESCRIPTION
*	Specifies the class of plug and play events that are being subscribed for.
*
* SYNOPSIS
*/
#define IB_PNP_CA						0x00000001
#define IB_PNP_PORT						0x00000002
#define IB_PNP_IOU						0x00000004
#define IB_PNP_IOC						0x00000008

#define IB_PNP_FLAG_REG_SYNC			0x40000000
#define IB_PNP_FLAG_REG_COMPLETE		0x80000000
#define IB_PNP_FLAG_MASK				0xF0000000
#define IB_PNP_CLASS_MASK				0x000000FF

typedef uint32_t	ib_pnp_class_t;
/*
* VALUES
*	IB_PNP_CA
*		Value used to register for local channel adapter events.  These
*		 events include the addition or removal of a local channel adapter.
*
*	IB_PNP_PORT
*		Value used to register for local port events.  These events include
*		local port up or down events and port LID or Pkey changes.
*
*	IB_PNP_IOU
*		Value used to register for I/O unit PnP events.  I/O unit events
*		include notifications of I/O unit assignment to and dissociation from
*		the local host.
*
*	IB_PNP_IOC
*		Value used to register for an I/O controller PnP event.  I/O controller
*		events include notification of an I/O controller assignment to a local
*		port and indication that an I/O controller dissociation has occurred.
*
*	IB_PNP_FLAG_REG_SYNC
*		Flag that is ORed with the PnP Class to control behavior of the
*		ib_pnp_reg call.  When set, ib_pnp_reg returns after client has
*		received all events for the current state of the system.
*
*	IB_PNP_FLAG_REG_COMPLETE
*		Flag that is ORed with the PnP Class to control whether an event
*		is generated to indicate that a client has received all events for the
*		current state of the system.
*
* NOTES
*	When registering for PnP notification, a client specifies the class of
*	local events that the client wishes to be notified of.  For example to
*	request notification of events on a port, a client would use IB_PNP_PORT.
*	To be notified of the assignment of an I/O controller, a client would use
*	IB_PNP_IOC.
*
*	The PnP APIs do not support registration for multiple event classes at
*	a time.
*
* SEE ALSO
*	ib_pfn_pnp_cb_t, ib_pfn_report_cb_t, ib_pnp_rec_t, ib_pnp_event_t
*****/


/****d* Access Layer/ib_pnp_event_t
* NAME
*	ib_pnp_event_t
*
* DESCRIPTION
*	Indicates the type of plug and play event that has occurred.
*
* SYNOPSIS
*/
#define IB_PNP_EVENT_PATH				0x00000800
#define IB_PNP_EVENT_ADD				0x00001000
#define IB_PNP_EVENT_REMOVE				0x00002000
#define IB_PNP_EVENT_CHANGE				0x00004000
#define IB_PNP_EVENT_INIT				0x00008000
#define IB_PNP_EVENT_ARMED				0x00010000
#define IB_PNP_EVENT_ACTIVE				0x00020000
#define IB_PNP_EVENT_DOWN				0x00040000
#define IB_PNP_EVENT_PKEY				0x00080000
#define IB_PNP_EVENT_SM					0x00100000
#define IB_PNP_EVENT_GID				0x00200000
#define IB_PNP_EVENT_LID				0x00400000
#define IB_PNP_EVENT_SUBNET				0x00800000

#define IB_PNP_CA_ADD					(IB_PNP_CA | IB_PNP_EVENT_ADD)
#define IB_PNP_CA_REMOVE				(IB_PNP_CA | IB_PNP_EVENT_REMOVE)

#define IB_PNP_PORT_ADD					(IB_PNP_PORT | IB_PNP_EVENT_ADD)
#define IB_PNP_PORT_REMOVE				(IB_PNP_PORT | IB_PNP_EVENT_REMOVE)
#define IB_PNP_PORT_INIT				(IB_PNP_PORT | IB_PNP_EVENT_INIT)
#define IB_PNP_PORT_ARMED				(IB_PNP_PORT | IB_PNP_EVENT_ARMED)
#define IB_PNP_PORT_ACTIVE				(IB_PNP_PORT | IB_PNP_EVENT_ACTIVE)
#define IB_PNP_PORT_DOWN				(IB_PNP_PORT | IB_PNP_EVENT_DOWN)
#define IB_PNP_PKEY_CHANGE				(IB_PNP_PORT | IB_PNP_EVENT_PKEY)
#define IB_PNP_SM_CHANGE				(IB_PNP_PORT | IB_PNP_EVENT_SM)
#define IB_PNP_GID_CHANGE				(IB_PNP_PORT | IB_PNP_EVENT_GID)
#define IB_PNP_LID_CHANGE				(IB_PNP_PORT | IB_PNP_EVENT_LID)
#define IB_PNP_SUBNET_TIMEOUT_CHANGE	(IB_PNP_PORT | IB_PNP_EVENT_SUBNET)

#define IB_PNP_IOU_ADD					(IB_PNP_IOU | IB_PNP_EVENT_ADD)
#define IB_PNP_IOU_REMOVE				(IB_PNP_IOU | IB_PNP_EVENT_REMOVE)
#define IB_PNP_IOC_ADD					(IB_PNP_IOC | IB_PNP_EVENT_ADD)
#define IB_PNP_IOC_REMOVE				(IB_PNP_IOC | IB_PNP_EVENT_REMOVE)
#define IB_PNP_IOC_PATH_ADD				(IB_PNP_IOC | IB_PNP_EVENT_PATH | \
										IB_PNP_EVENT_ADD)
#define IB_PNP_IOC_PATH_REMOVE			(IB_PNP_IOC | IB_PNP_EVENT_PATH | \
										IB_PNP_EVENT_REMOVE)

#define IB_PNP_REG_COMPLETE				IB_PNP_FLAG_REG_COMPLETE

typedef uint32_t	ib_pnp_event_t;
/*
* VALUES
*	IB_PNP_CA_ADD
*		Indicates that a new channel adapter has been added.
*
*	IB_PNP_CA_REMOVE
*		Indicates that a channel adapter has been removed.
*
*	IB_PNP_PORT_ADD
*		Indicates that a new port has been added.  This callback will always
*		be followed by a callback to indicate the actual port state to allow
*		clients to use the PnP callbacks to drive their state machine.
*
*	IB_PNP_PORT_REMOVE
*		Indicates that a port has been removed.
*		A CA remove event will trigger this event first.
*
*	IB_PNP_PORT_INIT
*		Indicates that a port is in the IB_LINK_INIT state.
*
*	IB_PNP_PORT_ARMED
*		Indicates that a port is in the IB_LINK_ARMED state.
*
*	IB_PNP_PORT_ACTIVE
*		Indicates that a port is in the IB_LINK_ACTIVE state.
*
*	IB_PNP_PORT_DOWN
*		Indicates that a port down event has occurred.
*
*	IB_PNP_PKEY_CHANGE
*		Indicates that port Pkey change has ocurred.
*
*	IB_PNP_SM_CHANGE
*		Indicates that the SM assignment for a port has changed.
*
*	IB_PNP_GID_CHANGE
*		Indicates that the GID assignment for a port has changed.
*
*	IB_PNP_LID_CHANGE
*		Indicates that the LID or LMC assignment for a port has changed.
*
*	IB_PNP_SUBNET_TIMEOUT_CHANGE
*		Indicates that the subnet timeout assignment for a port has changed.
*
*	IB_PNP_IOU_ADD
*		Indicates that an I/O unit assignment has occured.
*
*	IB_PNP_IOU_REMOVE
*		Indicates that an I/O unit disassociation has occured.
*
*	IB_PNP_IOC_ADD
*		Indicates that an I/O controller assignment has occurred.
*
*	IB_PNP_IOC_REMOVE
*		Indicates that an I/O controller dissociation has occurred.
*		A port down event will trigger this event first.
*
*	IB_PNP_IOC_PATH_ADD
*		Indicates that a new path to an I/O controller is available.
*
*	IB_PNP_IOC_PATH_REMOVE
*		Indiactes that a path to an I/O controller is no longer avaialble.
*
*	IB_PNP_REG_COMPLETE
*		Indicates that all events associated with a ib_reg_pnp call have been
*		reported to the user.  The user's state of the system is now in
*		sync with that of the access layer.
*
* NOTES
*		The Access Layer maintains a queue of client PnP registrations.
*		Using this queue, PnP events are reported to clients in a specific
*		order.  CA add, port add, and IOC add events are reported from the
*		head of the queue, while CA remove, port remove, and IOC remove events
*		are reported from the tail.  Clients are responsible for performing
*		registrations in the proper sequence to ensure that PnP event
*		notifiations occur in the desired order.
*
* SEE ALSO
*	ib_pfn_pnp_cb_t, ib_pfn_report_cb_t, ib_pnp_rec_t, ib_pnp_class_t
*****/


AL_INLINE const char* AL_API
ib_get_pnp_event_str(
	IN				ib_pnp_event_t				event )
{
	switch( event )
	{
	case IB_PNP_CA_ADD					: return "IB_PNP_CA_ADD";
	case IB_PNP_CA_REMOVE				: return "IB_PNP_CA_REMOVE";
	case IB_PNP_PORT_ADD				: return "IB_PNP_PORT_ADD";
	case IB_PNP_PORT_REMOVE				: return "IB_PNP_PORT_REMOVE";
	case IB_PNP_PORT_INIT				: return "IB_PNP_PORT_INIT";
	case IB_PNP_PORT_ARMED				: return "IB_PNP_PORT_ARMED";
	case IB_PNP_PORT_ACTIVE				: return "IB_PNP_PORT_ACTIVE";
	case IB_PNP_PORT_DOWN				: return "IB_PNP_PORT_DOWN";
	case IB_PNP_PKEY_CHANGE				: return "IB_PNP_PKEY_CHANGE";
	case IB_PNP_SM_CHANGE				: return "IB_PNP_SM_CHANGE";
	case IB_PNP_GID_CHANGE				: return "IB_PNP_GID_CHANGE";
	case IB_PNP_LID_CHANGE				: return "IB_PNP_LID_CHANGE";
	case IB_PNP_SUBNET_TIMEOUT_CHANGE	: return "IB_PNP_SUBNET_TIMEOUT_CHANGE";
	case IB_PNP_IOU_ADD					: return "IB_PNP_IOU_ADD";
	case IB_PNP_IOU_REMOVE				: return "IB_PNP_IOU_REMOVE";
	case IB_PNP_IOC_ADD					: return "IB_PNP_IOC_ADD";
	case IB_PNP_IOC_REMOVE				: return "IB_PNP_IOC_REMOVE";
	case IB_PNP_IOC_PATH_ADD			: return "IB_PNP_IOC_PATH_ADD";
	case IB_PNP_IOC_PATH_REMOVE			: return "IB_PNP_IOC_PATH_REMOVE";
	case IB_PNP_REG_COMPLETE			: return "IB_PNP_REG_COMPLETE";
	}
	return "Unknown";
}


/****s* Access Layer/ib_pnp_rec_t
* NAME
*	ib_pnp_rec_t
*
* DESCRIPTION
*	Notification information used to describe local channel adapter, port,
*	and I/O controller events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_rec
{
	ib_pnp_event_t				pnp_event;

	TO_LONG_PTR(struct _al_pnp *, h_pnp);
	TO_LONG_PTR(struct _al_pnp *, h_ioc_event);

	TO_LONG_PTR(void*,			pnp_context);
	TO_LONG_PTR(void*,			context);
	//NOTE:
	//guid and ca_guid use as key to flexi map need to keep these field together
	ib_net64_t					guid;
	ib_net64_t					ca_guid;

}	ib_pnp_rec_t;
/*
* FIELDS
*	pnp_event
*		Describes the type of plug and play event that is being reported.
*
*	h_pnp
*		A handle to the notification registration for which this PnP record
*		was generated.  This handle will match the handle returned through
*		an ib_reg_pnp call.  It is provided in case a PnP notification event
*		occurs before a client's call to ib_reg_pnp can return.  This handle
*		may be used to cancel further notification of PnP events.
*
*	h_ioc_event
*		A handle that is unique to an I/O controller assignment event.
*		This handle is used to reject the assignment of an I/O controller
*		from within the ib_pfn_pnp_cb_t callback.  Valid for IB_PNP_IOC_ADD
*		events only.
*
*	pnp_context
*		User-defined context information specified when registering for
*		notification of the event.  See the notes section below for
*		more details.
*
*	context
*		This field references a user-specified context on which the event
*		occurred.  See the notes section below for more details.
*
*	guid
*		The GUID of the adapter, port, IOU, or IOC for which
*		the PnP event occurred.
*
*	ca_guid
*		The  GUID of the HCA 
*
* NOTES
*	This structure is returned to the user to notify them of: the addition
*	of a channel adapter, the removal of a channel adapter, a port up or down
*	event, a port pkey change, and I/O controller addition and removal events.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	The context value can be changed by updating its field
*	and will take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.  Context is maintained between port down and subsequent port up
*	events provided that the channel adapter is not removed.
*
*	I/O controller path notifications are only delivered with respect to a
*	previously reported I/O controller.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****s* Access Layer/ib_pnp_ca_rec_t
* NAME
*	ib_pnp_ca_rec_t
*
* DESCRIPTION
*	Notification information used to describe local channel adapter events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_ca_rec
{
	ib_pnp_rec_t				pnp_rec;
	TO_LONG_PTR(ib_ca_attr_t*,	p_ca_attr);

}	ib_pnp_ca_rec_t;
/*
* FIELDS
*	pnp_rec
*		Structure describing the plug and play event being reported.
*
*	p_ca_attr
*		Attributes of the channel adapter that has experienced the event.
*		NULL for IB_PNP_CA_REMOVE, IB_PNP_PORT_REMOVE, and IB_PNP_IOC_REMOVE
*		events.
*
* NOTES
*	This structure is returned to the user to notify them of the addition
*	or the removal of a channel adapter.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	Context values can be changed by updating the appropriate field
*	and will be take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.
*
*	Recipients of CA-related PnP events should cast the ib_pnp_rec_t structure
*	returned in the PnP callback to this type to access CA-specific information.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****s* Access Layer/ib_pnp_port_rec_t
* NAME
*	ib_pnp_port_rec_t
*
* DESCRIPTION
*	Notification information used to describe local port events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_port_rec
{
	ib_pnp_rec_t				pnp_rec;
	TO_LONG_PTR(ib_ca_attr_t*,	p_ca_attr);
	TO_LONG_PTR(ib_port_attr_t*,p_port_attr);

}	ib_pnp_port_rec_t;
/*
* FIELDS
*	pnp_rec
*		Structure describing the plug and play event being reported.
*
*	p_ca_attr
*		Attributes of the channel adapter that has experienced the event.
*		NULL for IB_PNP_CA_REMOVE, IB_PNP_PORT_REMOVE, and IB_PNP_IOC_REMOVE
*		events.
*
*	p_port_attr
*		Attributes of the port that has experienced the event.  Valid only
*		for IB_PNP_PORT_UP, IB_PNP_PORT_DOWN, IB_PNP_PKEY_CHANGE, and
*		IB_PNP_IOC_ADD events.
*
* NOTES
*	This structure is returned to the user to notify them of port events.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	Context values can be changed by updating the appropriate field
*	and will be take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.  Context is maintained between port down and subsequent port up
*	events provided that the channel adapter is not removed.
*
*	Recipients of port related PnP events should cast the ib_pnp_rec_t structure
*	returned in the PnP callback to this type to access port specific information.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****s* Access Layer/ib_pnp_iou_rec_t
* NAME
*	ib_pnp_iou_rec_t
*
* DESCRIPTION
*	Notification information used to describe local I/O unit events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_iou_rec
{
	ib_pnp_rec_t				pnp_rec;
	net64_t						guid;
	net64_t						ca_guid;
	net64_t						chassis_guid;
	uint8_t						slot;
	net32_t						vend_id;
	net16_t						dev_id;
	net32_t						revision;
	char						desc[IB_NODE_DESCRIPTION_SIZE + 1];

}	ib_pnp_iou_rec_t;
/*
* FIELDS
*	pnp_rec
*		Structure describing the plug and play event being reported.
*
*	ca_guid
*		GUID of the local HCA through which the I/O unit is accessible.  Valid
*		only for IB_PNP_IOU_ADD events.
*
*	chassis guid
*		GUID of the chassis in which an I/O unit is installed.  Valid only for
*		IB_PNP_IOU_ADD events.
*
*	slot
*		Chassis slot number in which an I/O unit is installed.  Valid only for
*		IB_PNP_IOU_ADD events.
*
*	guid
*		GUID of an I/O unit from which one or more I/O controllers are assigned
*		to this host.  Valid only for IB_PNP_IOU_ADD events.
*
*	vend_id
*		Vendor ID of an I/O unit from which one or more I/O controllers are
*		assigned to this host.  Valid only for IB_PNP_IOU_ADD events.
*
*	dev_id
*		Device ID of an I/O unit from which one or more I/O controllers are
*		assigned to this host.  Valid only for IB_PNP_IOU_ADD events.
*
*	revision
*		Revision of an I/O unit from which one or more I/O controllers are
*		assigned to this host.  Valid only for IB_PNP_IOU_ADD events.
*
*	desc
*		Node description string for an I/O unit from which one or more I/O
*		controllers are assigned to this host.  Valid only for IB_PNP_IOU_ADD
*		events.
*
* NOTES
*	This structure is returned to the user to notify them of the addition
*	and removal of an I/O Unit.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	Context values can be changed by updating the appropriate field
*	and will be take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.  Context is maintained between port down and subsequent port up
*	events provided that the channel adapter is not removed.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****s* Access Layer/ib_pnp_ioc_rec_t
* NAME
*	ib_pnp_ioc_rec_t
*
* DESCRIPTION
*	Notification information used to describe local channel adapter, port,
*	and I/O controller events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_ioc_rec
{
	ib_pnp_rec_t				pnp_rec;
	net64_t						ca_guid;
	ib_ioc_info_t				info;
	ib_svc_entry_t				svc_entry_array[1];

}	ib_pnp_ioc_rec_t;
/*
* FIELDS
*	pnp_rec
*		Structure describing the plug and play event being reported.
*
*	ca_guid
*		GUID of the local HCA through which the I/O controller is accessible.
*		Valid only for IB_PNP_IOC_ADD events.
*
*	p_ioc_info
*		The I/O controller information for an assigned controller, including
*		information for the I/O unit.  Valid only for IB_PNP_IOC_ADD events.
*
*	svc_entry_array
*		If an I/O controller is being reported, this will reference an array
*		of service entries associated with the I/O controller.  The actual
*		number of entries in the array may be determined by examining the
*		svc_entries field in the I/O controller profile.  Valid only for
*		IB_PNP_IOC_ADD events.
*
* NOTES
*	This structure is returned to the user to notify them of the addition
*	and removal of an I/O controller.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	Context values can be changed by updating the appropriate field
*	and will be take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.  Context is maintained between port down and subsequent port up
*	events provided that the channel adapter is not removed.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****s* Access Layer/ib_pnp_ioc_path_rec_t
* NAME
*	ib_pnp_ioc_path_rec_t
*
* DESCRIPTION
*	Notification information used to describe local channel adapter, port,
*	and I/O controller events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_ioc_path_rec
{
	ib_pnp_rec_t				pnp_rec;
	net64_t						ca_guid;
	net64_t						port_guid;
	ib_path_rec_t				path;

}	ib_pnp_ioc_path_rec_t;
/*
* FIELDS
*	pnp_rec
*		Structure describing the plug and play event being reported.
*
*	ca_guid
*		GUID of the local HCA through which the I/O controller is accessible.
*		Valid only for IB_PNP_IOC_PATH_ADD and IB_PNP_IOC_PATH_REMOVE events.
*
*	port_guid
*		GUID of the local HCA port through which the I/O controller is
*		accessible.  Valid only for IB_PNP_IOC_PATH_ADD and
*		IB_PNP_IOC_PATH_REMOVE events.
*
*	p_path
*		Path record that provides connectivity with a given I/O controller.
*		Valid only for IB_PNP_IOC_PATH_ADD and IB_PNP_IOC_PATH_REMOVE events.
*
* NOTES
*	This structure is returned to the user to notify them of the addition
*	and removal of a path to an I/O controller.  I/O controller path
*	notifications are only delivered with respect to a previously reported
*	I/O controller.
*
*	The context field is NULL unless a context value has already been set
*	by the user.
*
*	Context values can be changed by updating the appropriate field
*	and will be take effect once the notification callback returns.
*
*	Once a device has been removed, all context associated with that device
*	is lost.  Context is maintained between port down and subsequent port up
*	events provided that the channel adapter is not removed.
*
* SEE ALSO
*	ib_open_al, ib_ca_attr_t, ib_reg_pnp, ib_dereg_pnp, ib_pfn_pnp_cb_t,
*	ib_ioc_info_t, ib_reject_ioc, ib_pnp_event_t
*****/


/****f* Access Layer/ib_pfn_pnp_cb_t
* NAME
*	ib_pfn_pnp_cb_t
*
* DESCRIPTION
*	User-defined callback that is invoked to notify a client of the addition
*	or removal of a channel adapter, a port up or down event, port changes,
*	and the assignment of an I/O controller to a local port.
*
* SYNOPSIS
*/
typedef ib_api_status_t
(AL_API *ib_pfn_pnp_cb_t)(
	IN				ib_pnp_rec_t				*p_pnp_rec );
/*
* PARAMETERS
*	p_pnp_rec
*		[in] A reference to a plug and play record.  The plug and play
*		record contains details about the type of local event that has
*		occurred, along with the relevant device information.
*
* RETURN VALUES
*	IB_SUCCESS
*		Indicates to the PnP manager that the callback client requires it
*		to maintain a context for this event.
*
*	Other
*		Indicates to the PnP manager that the callback client does not need
*		a context for this event.
*
* NOTES
*	The callback is used to notify users of local events that have occurred
*	on a given channel adapter.  Information about the type of event that
*	occurred along with the associated device is returned to the user through
*	the p_pnp_rec parameter.
*
*	Users register for plug and play changes by requesting notification from
*	the access layer.  Users may register for notifications either by directly
*	invoking the appropriate function in the access layer, or indirectly by
*	adding the necessary registration data to the access layer device file.
*
*	This callback is invoked from within a system thread context.
*
*	If the callback returns a status other than IB_SUCCESS, no further
*	callback for related events will be delivered.
*
* SEE ALSO
*	ib_pnp_rec_t, ib_reg_pnp, ib_dereg_pnp, ib_reject_ioc
*****/


/****s* Access Layer/ib_pnp_req_t
* NAME
*	ib_pnp_req_t
*
* DESCRIPTION
*	Information used to register for notification of local and I/O
*	controller assignment events.
*
* SYNOPSIS
*/
typedef struct _ib_pnp_req
{
	ib_pnp_class_t				pnp_class;
	const void					*pnp_context;
	ib_pfn_pnp_cb_t				pfn_pnp_cb;

}	ib_pnp_req_t;
/*
* FIELDS
*	pnp_class
*		Specifies the class of PnP events that the client wishes to be
*		notified of.
*
*	pnp_context
*		User-defined context information associated with this notification.
*		The context data is returned to the user as a part of their PnP
*		notification callback.
*
*	pfn_pnp_cb
*		User-defined callback function that is invoked to notify the user of
*		the occurrance of a plug and play event.
*
* NOTES
*	This structure is used when requesting notification of local events from
*	the access layer.  The class of PnP events which to be notified of is
*	specified through the pnp_class field.
*
* SEE ALSO
*	ib_pnp_class_t, ib_pfn_pnp_cb_t, ib_reg_pnp, ib_pnp_rec_t
*****/


/****f* Access Layer/ib_reg_pnp
* NAME
*	ib_reg_pnp
*
* DESCRIPTION
*	Routine used to register for notification of local and I/O controller
*	assignment events.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reg_pnp(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_pnp_req_t* const			p_pnp_req,
		OUT			ib_pnp_handle_t* const		ph_pnp );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	p_pnp_req
*		[in] Specifies the type of events that the user wishes to be notified
*		of, along with information needed to process the completed query.
*
*	ph_pnp
*		[out] Upon successful completion of this call, this references a handle
*		to the PnP notification request.  This handle may be used to cancel the
*		notification registration.
*
* RETURN VALUES
*	IB_SUCCESS
*		The PnP registration was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the PnP request information or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to register for PnP notification.
*
* NOTES
*	This routine registers the calling client with the access layer for
*	notification of locally occurring events, or the assignment of I/O
*	controllers to a local device.  Once registered, a client will receive
*	notification, via a callback, that a given event has occurred on a
*	local device.  Clients may restrict the types of events and devices
*	that are reported.  The p_pnp_req parameter is used to indicate which
*	device events to report to the user.
*
*	Upon invoking this routine, the client may receive a callback through
*	the ib_pfn_pnp_cb_t routine to notify them of the current system state.
*	For example, if a client registers for notification of port up events,
*	then the access layer will notify the client of all available ports when
*	this routine is first invoked.
*
* SEE ALSO
*	ib_dereg_pnp, ib_pnp_req_t, ib_pnp_rec_t, ib_pfn_pnp_cb_t
*****/


/****f* Access Layer/ib_dereg_pnp
* NAME
*	ib_dereg_pnp
*
* DESCRIPTION
*	Routine used to cancel notification of local events or I/O controller
*	assignments.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_dereg_pnp(
	IN		const	ib_pnp_handle_t				h_pnp,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_pnp
*		[in] A handle returned as a result of an ib_reg_pnp operation.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the PnP
*		registration has been successfully deregistered.
*
* NOTES
*	This routine cancels a pending PnP operation.  To avoid a race condition
*	canceling a request at the same time a notification callback is in
*	progress, the cancel operation operates asynchronously.  For additional
*	details see ib_pfn_destroy_cb_t.
*
* RETURN VALUES
*	IB_SUCCESS
*		The PnP deregistration was initiated.
*
*	IB_INVALID_HANDLE
*		The PnP handle was invalid.
*
* SEE ALSO
*	ib_reg_pnp, ib_pfn_destroy_cb_t
*****/


/****s* Access Layer/ib_sub_rec_t
* NAME
*	ib_sub_rec_t
*
* DESCRIPTION
*	Information returned to a user that indicates the result of a subscription
*	request.
*
* SYNOPSIS
*/
typedef struct _ib_sub_rec
{
	const void*					sub_context;
	ib_api_status_t				status;
	ib_sub_handle_t				h_sub;

}	ib_sub_rec_t;
/*
* FIELDS
*	sub_context
*		References user-defined context information associated with the
*		subscription request.  This field is set by the user through the
*		ib_subscribe routine.
*
*	status
*		Indicates the success of the subscription request.
*
*	h_sub
*		The handle to the subscription request that was returned to the user
*		from the ib_subscribe call.  This handle is provided to the user to
*		avoid a race condition between the return of the ib_subscribe routine
*		and the notification of an event.
*
* NOTES
*	This structure is returned to the user to notify them of the results
*	of a subscription request.  After successfully subscribing with a
*	class manager for an event, this structure will be returned to the user
*	with the status set to IB_SUCCESS.  The sub_context field will be set
*	to the context specified through the p_sub_req parameter in the
*	ib_subscribe routine.
*
* SEE ALSO
*	ib_subscribe
*****/


/****f* Access Layer/ib_pfn_sub_cb_t
* NAME
*	ib_pfn_sub_cb_t
*
* DESCRIPTION
*	User-defined callback invoked on completion of a subscription request.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_sub_cb_t)(
	IN				ib_sub_rec_t				*p_sub_rec );
/*
* PARAMETERS
*	p_sub_rec
*		[in] This is a reference to a structure containing the result of the
*		subscription request.
*
* NOTES
*	This routine is invoked to notify a client of the result of a
*	subscription request with a class manager.  If the subscription request
*	was successful, the client will receive future notifications of the
*	subscribed event from the class manager.
*
*	This callback will always be invoked before a client receives information
*	reported on a subscribed event that has occurred.
*
*	In the kernel, this callback is usually invoked using a tasklet, dependent
*	on the implementation of the underlying verbs provider driver.
*
* SEE ALSO
*	ib_subscribe, ib_sub_rec_t
*****/


/****s* Access Layer/ib_report_rec_t
* NAME
*	ib_report_rec_t
*
* DESCRIPTION
*	Reported event information returned to the user when a subscribed for
*	event occurs.
*
* SYNOPSIS
*/
typedef struct _ib_report_rec
{
	const void*						report_context;
	ib_mad_notice_attr_t*			p_notice;

}	ib_report_rec_t;
/*
* FIELDS
*	report_context
*		Client-defined context information specified when registering for
*		the report.
*
*	p_notice
*		Reported information that describes the event that has occurred.
*
* NOTES
*	Subscription for reported events is done through a class manager.  When
*	a class manager detects that such an event occurs, it will generate a
*	report to the subscribed client.  The reported information is referenced
*	through the p_notice field.
*
* SEE ALSO
*	ib_mad_notice_attr_t
*****/


/****f* Access Layer/ib_pfn_report_cb_t
* NAME
*	ib_pfn_report_cb_t
*
* DESCRIPTION
*	User-defined callback that is invoked to notify a client of an event
*	that has occurred on the fabric.
*
* SYNOPSIS
*/
typedef void
(AL_API *ib_pfn_report_cb_t)(
	IN				ib_report_rec_t				*p_report_rec );
/*
* PARAMETERS
*	p_report_rec
*		[in] A reference to an event report.  The report contains
*		details about the type of event that has occurred, along with the
*		relevant device information.
*
* NOTES
*	The callback is used to notify users of remote events that have been seen
*	by a specified class manager.  Information about the type of event that
*	occurred along with the associated device is returned to the user through
*	the p_report_rec parameter.
*
*	Users register for device changes by subscribing with a class manager.
*	Users may subscribe for events either by directly invoking the
*	appropriate function in the access layer, or indirectly by adding the
*	necessary registration data to the access layer device file.
*
*	This callback is invoked from within a system thread context.
*
* SEE ALSO
*	ib_report_rec_t, ib_subscribe, ib_unsubscribe
*****/


/****s* Access Layer/ib_sub_req_t
* NAME
*	ib_sub_req_t
*
* DESCRIPTION
*	Information used to subscribed for event notification from a class
*	manager.
*
* SYNOPSIS
*/
typedef struct _ib_sub_req
{
	ib_svc_name_t*				p_class_mgr_name;
	ib_inform_info_t*			p_inform_info;
	ib_net64_t					port_guid;

	uint32_t					timeout_ms;
	uint32_t					retry_cnt;
	ib_al_flags_t				flags;

	const void*					sub_context;
	ib_pfn_sub_cb_t				pfn_sub_cb;

	const void*					report_context;
	ib_pfn_report_cb_t			pfn_report_cb;

}	ib_sub_req_t;
/*
* FIELDS
*	p_class_mgr_name
*		The service name of the class manager to subscribe for events with.
*
*	p_inform_info
*		Information describing the type of event being subscribed to.
*
*	port_guid
*		Directs the subscription request to use the specified port.  The
*		request will contact the subnet administrator reachable through the
*		given port.
*
*	timeout_ms
*		Specifies the number of milliseconds to wait for a response for
*		this subscription until retrying or timing out the request.
*
*	retry_cnt
*		Specifies the number of times that the query will be retried before
*		failing the request.
*
*	flags
*		Used to describe the mode of operation.  Set to IB_FLAGS_SYNC to
*		process the called routine synchronously.
*
*	sub_context
*		User-defined context information associated with this subscription
*		request.  This context is returned to the user through the function
*		specified by the pfn_sub_cb field.
*
*	pfn_sub_cb
*		A user-defined callback that is invoked upon completion of the
*		subscription request.  This is used to notify a client that of the
*		result of their subscription request.
*
*	report_context
*		User-defined context information associated with this subscription.
*		This context is returned to the user through the client's
*		ib_pfn_report_cb_t callback routine specified in ib_open_al.
*
*	pfn_report_cb
*		A user-defined callback that is invoked to notify the user that an
*		event report has been received.
*
* NOTES
*	This structure is used to subscribe for events with a class manager.  Both
*	the subscription request and any corresponding event notifications operate
*	asynchronously.  Clients will be notified of the result of their
*	subscription request before receiving notification of associated events.
*
* SEE ALSO
*	ib_subscribe, ib_svc_name_t, ib_inform_info_t, ib_pfn_sub_cb_t,
*	ib_pfn_report_cb_t, ib_open_al
*****/


/****f* Access Layer/ib_subscribe
* NAME
*	ib_subscribe
*
* DESCRIPTION
*	Subscribe with a class manager for event notification.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_subscribe(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_sub_req_t* const			p_sub_req,
		OUT			ib_sub_handle_t* const		ph_sub );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	p_sub_req
*		[in] Specifies the type of events that the user wishes to be
*		notified of, along with information needed to process the completed
*		subscription.
*
*	ph_sub
*		[out] Upon successful completion of this call, this references a handle
*		to the subscription request.  This handle may be used to unsubscribe
*		from the events.
*
* RETURN VALUES
*	IB_SUCCESS
*		The subscription request was initiated.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the subscription request or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_INVALID_GUID
*		No port was found for the port_guid specified in the request.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine registers the calling client with a class manager for
*	notification of events.  Once registered, a client will receive
*	notification, via a callback, that a given event has occurred on
*	a device managed by the class manager.
*
* SEE ALSO
*	ib_unsubscribe, ib_sub_req_t, ib_pfn_sub_cb_t, ib_pfn_report_cb_t
*****/


/****f* Access Layer/ib_unsubscribe
* NAME
*	ib_unsubscribe
*
* DESCRIPTION
*	Unsubscribe with a class manager for event notification.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_unsubscribe(
	IN		const	ib_sub_handle_t				h_sub,
	IN		const	ib_pfn_destroy_cb_t			pfn_destroy_cb OPTIONAL );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	h_sub
*		[in] A handle to a subscribed event.
*
*	pfn_destroy_cb
*		[in] A user-specified callback that is invoked after the subscription
*		request has been successfully canceled.
*
* RETURN VALUES
*	IB_SUCCESS
*		The unsubscribe request was initiated.
*
*	IB_INVALID_HANDLE
*		The subscription handle was invalid.
*
*	IB_INVALID_PARAMETER
*		A reference to the subscription request or handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine cancels an active or pending event subscription with a class
*	manager.  To avoid a race condition canceling a subscription at the same
*	time an event notification callback is in progress, the unsubscribe
*	operation operates asynchronously.  For additional details see
*	ib_pfn_destroy_cb_t.
*
* SEE ALSO
*	ib_subscribe, ib_pfn_destroy_cb_t
*****/


/****f* Access Layer/ib_reject_ioc
* NAME
*	ib_reject_ioc
*
* DESCRIPTION
*	Rejects an I/O controller assignment to a host.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_reject_ioc(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_pnp_handle_t				h_ioc_event );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an open instance of the access layer.
*
*	h_ioc_event
*		[in] A handle provided as part of the notification of an I/O controller
*		being assigned.  This handle is obtained through the ib_pnp_rec_t
*		structure given to a client through their ib_pfn_pnp_cb_t callback.
*
* RETURN VALUES
*	IB_SUCCESS
*		The I/O controller reject request was initiated.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_HANDLE
*		The I/O controller handle was invalid.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
*	IB_INSUFFICIENT_RESOURCES
*		There were insufficient resources currently available on the channel
*		adapter to perform the operation.
*
* NOTES
*	This routine rejects an I/O controller assigned by the configuration
*	manager to the local host.  The access layer sends a rejection notification
*	to the configuration manager and disable access to the controller from
*	the local host.  This routine must be called from a client's
*	ib_pfn_pnp_cb_t callback to reject a newly assigned I/O controller.
*
* SEE ALSO
*	ib_pfn_pnp_cb_t, ib_pnp_rec_t
*****/


#define IB_ANY_INDEX	-1
/****d* Access Layer/ib_device_attr_mask_t
* NAME
*	ib_device_attr_mask_t
*
* DESCRIPTION
*	Used to specify desired attributes of a device or port.
*
* SYNOPSIS
*/
#define		IB_DEV_PORT_ACTIVE		0x1
/*
* VALUES
*	IB_DEV_PORT_ACTIVE
*		Specifies that a port state should be active.  Applies only to port
*		GUIDs.
*
* SEE ALSO
*	ib_get_guid
*****/


/****f* Access Layer/ib_get_guid
* NAME
*	ib_get_guid
*
* DESCRIPTION
*	Returns a GUID for a device or port that matches the user-specified
*	attributes.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_get_guid(
	IN				ib_al_handle_t				h_al,
	IN		const	uint32_t					index,
	IN		const	ib_pnp_class_t				device_type,
	IN		const	uint64_t					attr_mask,
		OUT			ib_net64_t* const			p_guid );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an opened instance of the access layer.
*
*	index
*		[in] Specifies the location of the device or port.  Users specify this
*		value to iterate through all devices or ports on the system.  If set
*		to IB_ANY_INDEX, then the first device or port matching the given
*		attributes will be returned.
*
*	device_type
*		[in] Indicates the type of device to retrieve the GUID for.
*
*	attr_mask
*		[in] Specifies a set of attributes that the given device or port
*		must have for a successful match to occur.
*
*	p_guid
*		[out] On successful return, this parameter will reference the GUID
*		of the device or port that contains the specified attributes.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
*	IB_INVALID_SETTING
*		The specified device type is invalid.
*
*	IB_INVALID_PARAMETER
*		No p_guid parameter was specified.
*
*	IB_NO_MATCH
*		The device or port at the specified index does not have the given
*		attributes.
*
*	IB_INVALID_INDEX
*		No device or port exists for the specified index.
*
* NOTES
*	This routine returns a GUID for a device or port that matches the
*	user-specified attributes.  If index is IB_ANY_INDEX, then the first
*	device or port matching the given attributes is returned if a match is
*	found.  If no match is found, the call will return IB_NO_MATCH.  If a
*	valid index is specified, then the device or port located at that index
*	will be examined to see if it has the given attributes.  If the device
*	or port with those attributes is found, its GUID is returned.
*
*	This routine may be used to locate a device or port with a given set
*	of attributes, or iterate through all devices or ports on the system.
*	The specified index values are set by the access layer, but the index
*	associated with a GUID may change if devices are removed from the system.
*
* SEE ALSO
*	ib_open_al, ib_pnp_class_t, ib_get_ca_guids, ib_query_ca_by_guid
*****/


/****f* Access Layer/ib_ci_call
* NAME
*	ib_ci_call
*
* DESCRIPTION
*	Performs a vendor specific CA interface function call.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_ci_call(
	IN				ib_ca_handle_t				h_ca,
	IN		const	void**				const	handle_array	OPTIONAL,
	IN				uint32_t					num_handles,
	IN				ib_ci_op_t*			const	p_ci_op );
/*
* PARAMETERS
*	h_ca
*		[in] An opened instance of a channel adapter.
*
*	handle_array
*		[in] This parameter references an array containing handles of
*		existing CA resources.  This array should contain all of the
*		handles specified in the vendor specific data provided with this
*		call.  All handles specified through this array are validated by
*		the access layer as existing and belonging to the calling process.
*		The verbs provider driver is responsible for verifying that the
*		number and type of handles are correct for the requested operation.
*
*	num_handles
*		[in] The number of the handles in handle array.  This count is
*		verified by the access layer.
*
*	p_ci_op
*		[in] A reference to the vendor specific CA interface data
*		structure containing the operation parameters.
*
* RETURN VALUES
*	IB_SUCCESS
*		The operation was successful.
*
*	IB_INVALID_CA_HANDLE
*		The specified CA handle was invalid.
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
*	The optional p_ci_op structure provides a means to pass vendor
*	specific parameters and data to the verbs provider driver.  If the
*	vendor specific data contains handles, the client should provide the
*	optional handle array that lists all of the handles specified in the
*	vendor specific data.  The handles in the handle array are restricted
*	to the following types:  ib_pd_handle_t, ib_cq_handle_t,
*	ib_av_handle_t, ib_qp_handle_t, ib_mr_handle_t, or ib_mw_handle_t.
*	The contents of the handle array are verified by the access layer and
*	the verbs provider driver.  This call cannot be used to allocate private
*	handles that are passed as parameters in access layer calls.
*
* SEE ALSO
*	ib_open_ca, ib_alloc_pd, ib_create_av, ib_create_cq,
*	ib_create_qp, ib_reg_mr, ib_reg_phys, ib_reg_shared,
*	ib_create_mw, ib_ci_op_t
*****/


/****f* Access Layer/ib_open_al
* NAME
*	ib_open_al
*
* DESCRIPTION
*	This routine opens an instance of the access layer for the user and
*	returns its handle.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_open_al(
		OUT			ib_al_handle_t* const		ph_al );
/*
* PARAMETERS
*	ph_al
*		[in] Upon successful completion of this call, this parameter will
*		reference a handle to the access layer.
*
* RETURN VALUES
*	IB_SUCCESS
*		The access layer was opened successfully.
*
*	IB_INVALID_PARAMETER
*		A reference to the access layer handle was not provided.
*
*	IB_INSUFFICIENT_MEMORY
*		There was insufficient memory to perform the operation.
*
* NOTES
*	This function opens an instance of the access layer.  An instance of the
*	access layer is required before allocating additional resources from the
*	access layer or a channel adapter.  If successful, a handle to the access
*	layer is returned.  User-mode clients should not call ib_open_al from the
*	module initialization routine.
*
* SEE ALSO
*	ib_close_al
*****/


/****f* Access Layer/ib_close_al
* NAME
*	ib_close_al
*
* DESCRIPTION
*	Deregisters a channel driver with the access layer and releases all
*	associated resources, including queue pairs, connection requests,
*	and completion queues.
*
* SYNOPSIS
*/
AL_EXPORT ib_api_status_t AL_API
ib_close_al(
	IN		const	ib_al_handle_t				h_al );
/*
* PARAMETERS
*	h_al
*		[in] A handle to an instance of the access layer.
*
* RETURN VALUES
*	IB_SUCCESS
*		The access layer was closed successfully.
*
*	IB_INVALID_AL_HANDLE
*		The access layer handle was invalid.
*
* NOTES
*	This call destroys an existing instance of the access layer.  Since
*	callbacks may be outstanding against the resources managed by this
*	access layer instance when the destroy operation is invoked, this
*	call may block until all outstanding callbacks complete.  This
*	routine may not be called from a callback invoked by the access layer.
*
* SEE ALSO
*	ib_open_al
*****/


#ifdef __cplusplus
}	/* extern "C" */
#endif	/* __cplusplus */


#endif /* __IB_AL_H__ */

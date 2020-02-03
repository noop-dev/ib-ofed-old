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
 * $Id: ib_al_ioctl.h 4284 2009-05-04 12:04:46Z leonid $
 */



#ifndef __IB_UAL_IOCTL_H__
#define __IB_UAL_IOCTL_H__

#include <complib/cl_types.h>
#include <iba/ib_types.h>
#include <iba/ib_ci.h>
#include <iba/ib_al.h>


/*
* Typedefs
*
*/
/*
* ual_close_ca_ioctl:
* NOTES:
* It is sufficient to pass the ca handle to the kernel proxy on close_ca
* The UAL context for this CA instance maintains the application callback
* So, when the proxy notifies for a close_ca_callback, we know which
* app callback to call
*
*/


/****s* User-mode Access Layer/ual_bind_file_ioctl_t
* NAME
*	ual_bind_file_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	binding a file handle to an existing proxy context.
*
* SYNOPSIS
*/
typedef struct _ual_bind_file_ioctl
{
	void* __ptr64			h_file;	/* __ptr64 is correct for HANDLE types. */

}	ual_bind_file_ioctl_t;
/*
* FIELDS
*	h_file
*		File handle from the user-mode process intended for asynchronous requests.
*		The IOCTL code will specify the type of asynchronous requests to be
*		performed on this file handle.
*
* SEE ALSO
*
* NOTES
*****/


/****s* User-mode Access Layer/ual_get_uvp_name_t
* NAME
*	ual_get_uvp_name_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	getting the user library information.
*
* SYNOPSIS
*/
typedef union _ual_get_uvp_name
{
	struct _ual_get_uvp_name_in
	{
		ib_net64_t				ca_guid;

	}	in;
	struct _ual_get_uvp_name_out
	{
		ib_api_status_t			status;
		char					uvp_lib_name[MAX_LIB_NAME];

	}	out;

}	ual_get_uvp_name_ioctl_t;
/*
* FIELDS
*	in.ca_guid
*		The GUID of the channel adapter
*
*	out.status
*		Status of the operation
*
*	out.uvp_lib_name
*		The vendor's library name associated with the CA
*
* SEE ALSO
*
*
* NOTES
*****/



/****s* User-mode Access Layer/ual_open_ca_ioctl_t
* NAME
*	ual_open_ca_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_open_ca
*
* SYNOPSIS
*/
typedef union _ual_open_ca_ioctl
{
	struct _ual_open_ca_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		ib_net64_t					guid;
		uint64_t					context;

	}	in;

	struct _ual_open_ca_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_ca;

	}	out;

}	ual_open_ca_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.guid
*		The GUID of the channel adapter to open.
*
*	in.context
*		A caller-specified context to associate with this opened instance
*		of the channel adapter.  This context is returned to the user when
*		invoking asynchronous callbacks referencing this channel adapter.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation
*
*	out.h_ca
*		On return from IOCTL, contains the CA Handle from AL.
*****/



/****s* User-mode Access Layer/ual_query_ca_ioctl_t
* NAME
*	ual_query_ca_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_ca
*
* SYNOPSIS
*/
typedef union _ual_query_ca_ioctl
{
	struct _ual_query_ca_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_ca;
		uint32_t					byte_cnt;
		uint64_t					p_ca_attr;

	}	in;
	struct _ual_query_ca_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint32_t					byte_cnt;

	}	out;

}	ual_query_ca_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_ca
*		The handle to an open instance of CA returned via a
*		ual_open_ca_ioctl structure.
*
*	in.byte_cnt
*		Specifies the size of the data buffer referenced by the p_ca_attr
*		parameter.
*
*	p_ca_attr
*		A reference to a buffer where the channel adapter attributes,
*		including port attribute information will be copied.  If this parameter
*		is NULL, then the required buffer size needed to return all of the CA
*		attribute information is returned through the out.byte_cnt parameter.
*		The ib_ca_attr_t structure for the specified channel adapter is stored
*		at the top of this buffer.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation
*
*	out.byte_cnt
*		Contains the number of bytes used or needed to copy all CA attributes.
*****/



/****s* User-mode Access Layer/ual_modify_ca_ioctl_t
* NAME
*	ual_modify_ca_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_ca
*
* SYNOPSIS
*/
typedef union _ual_modify_ca_ioctl
{
	struct _ual_modify_ca_ioctl_in
	{
		uint64_t					h_ca;
		uint8_t						port_num;
		ib_ca_mod_t					ca_mod;
		ib_port_attr_mod_t			port_attr_mod;

	}	in;
	struct _ual_modify_ca_ioclt_out
	{
		ib_api_status_t				status;

	}	out;


}	ual_modify_ca_ioctl_t;
/*
* FIELDS
*	in.h_ca
*		The handle to an open instance of CA (in KAL space).
*
*	in.port_num
*		An index of the port that is being modified.
*
*	in.ca_mod
*		The mask of the attributes and counters to modify.
*
*	in.port_attr_mod
*		List of port attribute information to modify.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_close_ca_ioctl_t
* NAME
*	ual_close_ca_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_close_ca
*
* SYNOPSIS
*/
typedef union _ual_close_ca_ioctl
{
	struct _ual_close_ca_ioctl_in
	{
		uint64_t					h_ca;

	}	in;
	struct _ual_close_ca_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_close_ca_ioctl_t;
/*
* FIELDS
*	in.h_ca
*		The handle to an open instance of CA (in KAL space).
*
*	out.status
*		Status of the operation
*****/

/****s* User-mode Access Layer/ual_ci_call_ioctl_t
* NAME
*	ual_ci_call_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_ci_call
*
* SYNOPSIS
*/
typedef union _ual_ci_call_ioctl
{
	struct _ual_ci_call_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		ib_ci_op_t					ci_op;
		uint64_t					h_ca;
		uint32_t					num_handles;
		uint64_t					handle_array[1];

	}	in;
	struct _ual_ci_call_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_ci_op_t					ci_op;
		ib_api_status_t				status;

	}	out;

}	ual_ci_call_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.ci_op
*		Contains information on the operation that needs to be performed
*		by the verbs provider.  The proxy marshals the data buffer between
*		user mode and kernel space.
*
*	in.h_ca
*		The handle to an open instance of CA returned by a ual_open_ca_ioctl.
*
*	in.num_handles
*		The number of handles in the array at in.p_handle_array.
*
*	in.handle_array
*		First entry in an array of handles used for this operation.  Ignored if
*		in.num_handles is zero.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation
*
*	out.ci_op
*		Contains information on the operation that needs to be performed
*		by the verbs provider.  The proxy marshals the data buffer between
*		user mode and kernel space.
*****/



/****s* User-mode Access Layer/ual_alloc_pd_ioctl_t
* NAME
*	ual_alloc_pd_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_alloc_pd
*
* SYNOPSIS
*/
typedef union _ual_alloc_pd_ioctl
{
	struct _ual_alloc_pd_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_ca;
		ib_pd_type_t				type;
		uint64_t					context;

	}	in;
	struct _ual_alloc_pd_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_pd;

	}	out;

}	ual_alloc_pd_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_ca
*		The handle to an open instance of CA returned in a ual_open_ca_ioctl.
*
*	in.context
*		UAL's pd context. This context will be provided on destroy callback.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation
*
*	out.h_pd
*		The handle to the PD to use in further PD-related IOCTLs.
*****/



/****s* User-mode Access Layer/ual_dealloc_pd_ioctl_t
* NAME
*	ual_dealloc_pd_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_dealloc_pd
*
* SYNOPSIS
*/
typedef union _ual_dealloc_pd_ioctl
{
	struct _ual_dealloc_pd_ioctl_in
	{
		uint64_t					h_pd;

	}	in;
	struct _ual_dealloc_pd_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_dealloc_pd_ioctl_t;
/*
* FIELDS
*	in.h_pd
*		The handle of the PD that is going to be deallocated.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_create_av_ioctl_t
* NAME
*	ual_create_av_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_av
*
* SYNOPSIS
*/
typedef union _ual_create_av_ioctl
{
	struct _ual_create_av_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_pd;
		ib_av_attr_t				attr;

	}	in;
	struct _ual_create_av_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_av;

	}	out;

}	ual_create_av_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_pd
*		The handle to an already allocated PD (in KAL space).
*
*	in.attr
*		Attributes of the address vector that needs to be created.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.h_av
*		Handle to the newly created address vector.
*****/



/****s* User-mode Access Layer/ual_query_av_ioctl_t
* NAME
*	ual_query_av_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_av
*
* SYNOPSIS
*/
typedef union _ual_query_av_ioctl
{
	struct _ual_query_av_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_av;

	}	in;
	struct _ual_query_av_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		ib_av_attr_t				attr;

	}	out;

}	ual_query_av_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_av
*		A handle to an existing address vector.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.attr
*		Attributes of the address vector.
*****/



/****s* User-mode Access Layer/ual_modify_av_ioctl_t
* NAME
*	ual_modify_av_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_av
*
* SYNOPSIS
*/
typedef union _ual_modify_av_ioctl
{
	struct _ual_modify_av_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_av;
		ib_av_attr_t				attr;

	}	in;
	struct _ual_modify_av_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;

	}	out;

}	ual_modify_av_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_av
*		A handle to an existing address vector.
*
*	in.attr
*		The requested attributes to be used for modifying the address vector.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_destroy_av_ioctl_t
* NAME
*	ual_destroy_av_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_destroy_av
*
* SYNOPSIS
*/
typedef union _ual_destroy_av_ioctl
{
	struct _ual_destroy_av_ioctl_in
	{
		uint64_t					h_av;

	}	in;
	struct _ual_destroy_av_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_destroy_av_ioctl_t;
/*
* FIELDS
*	in.h_av
*		A handle to an existing address vector.
*
*	out.status
*		Status of the operation.
*****/

/****s* User-mode Access Layer/ual_create_srq_ioctl_t
* NAME
*	ual_create_srq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_srq
*
* SYNOPSIS
*/
typedef union _ual_create_srq_ioctl
{
	struct _ual_create_srq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_pd;
		ib_srq_attr_t				srq_attr;
		uint64_t					context;
		boolean_t					ev_notify;

	}	in;
	struct _ual_create_srq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_srq;

	}	out;

}	ual_create_srq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_pd
*		Protection domain on which to create the srq.
*
*	in.srq_attr
*		Attributes necessary for creating the srq.
*
*	in.context
*		UAL's srq context that needs to be returned on a callback.
*
*	in.ev_notify
*		Boolean indicating whether asynchronous events should be
*		forwarded to user-mode.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.h_srq
*		Handle for the newly created srq.
*****/


/****s* User-mode Access Layer/ual_modify_srq_ioctl_t
* NAME
*	ual_modify_srq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_srq
*
* SYNOPSIS
*/
typedef union _ual_modify_srq_ioctl
{
	struct _ual_modify_srq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_srq;
		ib_srq_attr_mask_t			srq_attr_mask;
		ib_srq_attr_t				srq_attr;

	}	in;
	struct _ual_modify_srq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t			status;

	}	out;

}	ual_modify_srq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_srq
*		A handle to an existing Queue Pair.
*
*	in.modify_attr
*		Attributes used for modifying the srq.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*****/



/****s* User-mode Access Layer/ual_query_srq_ioctl_t
* NAME
*	ual_query_srq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_srq
*
* SYNOPSIS
*/
typedef union _ual_query_srq_ioctl
{
	struct _ual_query_srq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_srq;

	}	in;
	struct _ual_query_srq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t			status;
		ib_srq_attr_t				srq_attr;

	}	out;

} ual_query_srq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	h_srq
*		Handle to the srq whose attributes to query.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.srq_attr
*		Attributes of the srq.
*****/



/****s* User-mode Access Layer/ual_destroy_srq_ioctl_t
* NAME
*	ual_destroy_srq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_destroy_srq
*
* SYNOPSIS
*/
typedef union _ual_destroy_srq_ioctl
{
	struct _ual_destroy_srq_ioctl_in
	{
		uint64_t					h_srq;

	}	in;
	struct _ual_destroy_srq_ioctl_out
	{
		ib_api_status_t			status;

	}	out;

}	ual_destroy_srq_ioctl_t;
/*
* FIELDS
*	in.h_srq
*		Handle of the srq that needs to be destroyed.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_create_qp_ioctl_t
* NAME
*	ual_create_qp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_qp
*
* SYNOPSIS
*/
typedef union _ual_create_qp_ioctl
{
	struct _ual_create_qp_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_pd;
		ib_qp_create_t				qp_create;
		uint64_t					context;
		boolean_t					ev_notify;

	}	in;
	struct _ual_create_qp_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		ib_qp_attr_t				attr;
		uint64_t					h_qp;

	}	out;

}	ual_create_qp_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_pd
*		Protection domain on which to create the QP.
*
*	in.qp_create
*		Attributes necessary for creating the QP.
*
*	in.context
*		UAL's qp context that needs to be returned on a callback.
*
*	in.ev_notify
*		Boolean indicating whether asynchronous events should be
*		forwarded to user-mode.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.attr
*		Actual attributes of the newly created QP.
*
*	out.h_qp
*		Handle for the newly created QP.
*****/



/****s* User-mode Access Layer/ual_modify_qp_ioctl_t
* NAME
*	ual_modify_qp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_qp
*
* SYNOPSIS
*/
typedef union _ual_modify_qp_ioctl
{
	struct _ual_modify_qp_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_qp;
		ib_qp_mod_t					modify_attr;

	}	in;
	struct _ual_modify_qp_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		//ib_qp_query_t				query_attr; // Not returned by AL

	}	out;

}	ual_modify_qp_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_qp
*		A handle to an existing Queue Pair.
*
*	in.modify_attr
*		Attributes used for modifying the QP.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.query_attr
*		On return from the ioctl, contains the actual attributes of
*		the QP just modified.
*****/



/****s* User-mode Access Layer/ual_query_qp_ioctl_t
* NAME
*	ual_query_qp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_qp
*
* SYNOPSIS
*/
typedef union _ual_query_qp_ioctl
{
	struct _ual_query_qp_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_qp;

	}	in;
	struct _ual_query_qp_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		ib_qp_attr_t				attr;

	}	out;

} ual_query_qp_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	h_qp
*		Handle to the QP whose attributes to query.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.attr
*		Attributes of the QP.
*****/



/****s* User-mode Access Layer/ual_destroy_qp_ioctl_t
* NAME
*	ual_destroy_qp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_destroy_qp
*
* SYNOPSIS
*/
typedef union _ual_destroy_qp_ioctl
{
	struct _ual_destroy_qp_ioctl_in
	{
		uint64_t					h_qp;

	}	in;
	struct _ual_destroy_qp_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_destroy_qp_ioctl_t;
/*
* FIELDS
*	in.h_qp
*		Handle of the QP that needs to be destroyed.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_create_cq_ioctl_t
* NAME
*	ual_create_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_cq
*
* SYNOPSIS
*/
typedef union _ual_create_cq_ioctl
{
	struct _ual_create_cq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_ca;
		void* __ptr64				h_wait_obj;	/* __ptr64 is correct for HANDLE types. */
		uint64_t					context;
		uint32_t					size;
		boolean_t					ev_notify;

	}	in;
	struct _ual_create_cq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_cq;
		uint32_t					size;

	}	out;

}	ual_create_cq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_ca
*		CA handle on which to create the CQ.
*
*	in.cq_create
*		Attributes necessary for creating the cq.
*
*	in.cq_context
*		UAL's cq context that needs to be returned on a callback.
*
*	in.ev_notify
*		Boolean indicating whether asynchronous events should be
*		forwared to user-mode.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	h_cq
*		Handle to the newly created CQ.
*
*	size
*		Actual size of the newly created CQ.
*****/



/****s* User-mode Access Layer/ual_modify_cq_ioctl_t
* NAME
*	ual_modify_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_cq
*
* SYNOPSIS
*/
typedef union _ual_modify_cq_ioctl
{
	struct _ual_modify_cq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_cq;
		uint32_t					size;

	}	in;
	struct _ual_modify_cq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint32_t					size;

	}	out;

}	ual_modify_cq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_cq
*		A handle to the CQ to modify.
*
*	in.size
*		The requested new size of the CQ.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.size
*		The actual size of the CQ.
*****/



/****s* User-mode Access Layer/ual_query_cq_ioctl_t
* NAME
*	ual_query_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_cq
*
* SYNOPSIS
*/
typedef union _ual_query_cq_ioctl
{
	struct _ual_query_cq_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_cq;

	}	in;
	struct _ual_query_cq_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint32_t					size;

	}	out;

}	ual_query_cq_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_cq
*		A handle to an existing CQ.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.size
*		The size of the CQ.
*****/



/****s* User-mode Access Layer/ual_destroy_cq_ioctl_t
* NAME
*	ual_destroy_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_destroy_cq
*
* SYNOPSIS
*/
typedef union _ual_destroy_cq_ioctl
{
	struct _ual_destroy_cq_ioctl_in
	{
		uint64_t					h_cq;

	}	in;
	struct _ual_destroy_cq_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_destroy_cq_ioctl_t;
/*
* FIELDS
*	in.h_cq
*		Handle of the cq that needs to be destroyed.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_reg_mem_ioctl_t
* NAME
*	ual_reg_mem_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_reg_mem
*
* SYNOPSIS
*/
typedef union _ual_reg_mem_ioctl
{
	struct _ual_reg_mem_ioctl_in
	{
		uint64_t					h_pd;
		ib_mr_create_t				mem_create;

	}	in;
	struct _ual_reg_mem_ioctl_out
	{
		ib_api_status_t				status;
		net32_t						lkey;
		net32_t						rkey;
		uint64_t					h_mr;

	}	out;

}	ual_reg_mem_ioctl_t;
/*
* FIELDS
*	in.h_pd
*		Handle to the protection domain on which to register the memory.
*
*	in.mem_create
*		Information for registering the memory region.
*
*	out.status
*		Status of the operation.
*
*	out.lkey
*		LKey value returned by verb.
*
*	out.rkey
*		RKey value returned by verb.
*
*	h_mr
*		Handle to the registered memory region.
*****/



/****s* User-mode Access Layer/ual_query_mr_ioctl_t
* NAME
*	ual_query_mr_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_mr
*
* SYNOPSIS
*/
typedef union _ual_query_mr_ioctl
{
	struct _ual_query_mr_ioctl_in
	{
		uint64_t					h_mr;

	}	in;
	struct _ual_query_mr_ioctl_out
	{
		ib_api_status_t				status;
		ib_mr_attr_t				attr;

	}	out;

}	ual_query_mr_ioctl_t;
/*
* FIELDS
*	in.h_mr
*		A handle to a registered memory region.
*
*	out.status
*		Status of the operation.
*
*	out.attr
*		Attributes of the registered memory region.
*****/



/****s* User-mode Access Layer/ual_rereg_mem_ioctl_t
* NAME
*	ual_rereg_mem_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_modify_mr
*
* SYNOPSIS
*/
typedef union _ual_rereg_mem_ioctl
{
	struct _ual_rereg_mem_ioctl_in
	{
		uint64_t					h_mr;
		ib_mr_mod_t					mem_mod_mask;
		ib_mr_create_t				mem_create;
		uint64_t					h_pd;

	}	in;
	struct _ual_rereg_mem_ioctl_out
	{
		ib_api_status_t				status;
		net32_t						lkey;
		net32_t						rkey;

	}	out;

}	ual_rereg_mem_ioctl_t;
/*
* FIELDS
*	in.h_mr
*		A handle to a registered memory region that is being modified.
*
*	in.mem_mod_mask
*		The attributes to use when modifying the memory region.
*
*	in.mem_create
*		Information to use for modifying the memory region.  Required only
*		for changes other than the PD.
*
*	in.h_pd
*		PD Handle for changing protection domains.
*
*	out.status
*		Status of the operation.
*
*	out.l_key
*		LKey of the memory region.
*
*	out.rkey
*		RKey of the memory region.
*****/



/****s* User-mode Access Layer/ual_reg_shared_ioctl_t
* NAME
*	ual_reg_shared_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_reg_shared
*
* SYNOPSIS
*/
typedef union _ual_reg_shared_ioctl
{
	struct _ual_reg_shared_ioctl_in
	{
		uint64_t					h_mr;
		uint64_t					h_pd;
		ib_access_t					access_ctrl;
		uint64_t					vaddr;

	}	in;
	struct _ual_reg_shared_ioctl_out
	{
		ib_api_status_t				status;
		uint64_t					vaddr;
		net32_t						lkey;
		net32_t						rkey;
		uint64_t					h_new_mr;

	}	out;

}	ual_reg_shared_ioctl_t;
/*
* FIELDS
*	in.h_mr
*		A handle to the existing registered memory region.
*
*	in.h_pd
*		A handle to the PD on which memory is being registered.
*
*	in.access_ctrl
*		Access control for the new memory region.
*
*	in.vaddr
*		Requested virtual address.
*
*	out.status
*		Status of the operation.
*
*	out.vaddr
*		Actual virtual address of the registered region.
*
*	out.l_key
*		LKey of the memory region.
*
*	out.rkey
*		RKey of the memory region.
*
*	h_new_mr
*		Handle to the registered memory region.
*****/



/****s* User-mode Access Layer/ual_dereg_mr_ioctl_t
* NAME
*	ual_dereg_mr_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_dereg_mr
*
* SYNOPSIS
*/
typedef union _ual_dereg_mr_ioctl
{
	struct _ual_dereg_mr_ioctl_in
	{
		uint64_t					h_mr;

	}	in;
	struct _ual_dereg_mr_ioctl_out
	{
		ib_api_status_t				status;
	
	}	out;

} ual_dereg_mr_ioctl_t;
/*
* FIELDS
*	in.h_mr
*		A handle to a registered memory region.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_create_mw_ioctl_t
* NAME
*	ual_create_mw_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_mw
*
* SYNOPSIS
*/
typedef union _ual_create_mw_ioctl
{
	struct _ual_create_mw_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_pd;

	}	in;
	struct _ual_create_mw_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		net32_t						rkey;
		uint64_t					h_mw;

	}	out;

}	ual_create_mw_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_pd
*		A handle to the protection domain on which the memory window should
*		be created.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	out.rkey
*		RKey associated with the memory window.
*
*	h_mw
*		Handle to the newly created MW.
*****/



/****s* User-mode Access Layer/ual_query_mw_ioctl_t
* NAME
*	ual_query_mw_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_query_mw
*
* SYNOPSIS
*/
typedef union _ual_query_mw_ioctl
{
	struct _ual_query_mw_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_mw;

	}	in;
	struct _ual_query_mw_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		net32_t						rkey;

	}	out;

}	ual_query_mw_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_mw
*		A handle to an existing memory window.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	rkey
*		RKey associated with the memory window.
*****/



/****s* User-mode Access Layer/ual_bind_mw_ioctl_t
* NAME
*	ual_bind_mw_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_bind_mw
*
* SYNOPSIS
*/
typedef union _ual_bind_mw_ioctl
{
	struct _ual_bind_mw_ioctl_in
	{
		uint64_t					h_mw;
		uint64_t					h_qp;
		ib_bind_wr_t				mw_bind;

	}	in;
	struct _ual_bind_mw_ioctl_out
	{
		ib_api_status_t				status;
		uint32_t					r_key;

	}	out;

}	ual_bind_mw_ioctl_t;
/*
* FIELDS
*	in.h_mw
*		A handle to an existing memory window.
*
*	in.h_qp
*		The qp handle to post the bind request.
*
*	in.mw_bind
*		Bind request.
*
*	out.status
*		Status of the operation.
*
*	out.rkey
*		RKey for the memory window.
*****/



/****s* User-mode Access Layer/ual_destroy_mw_ioctl_t
* NAME
*	ual_destroy_mw_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_destroy_mw
*
* SYNOPSIS
*/
typedef union _ual_destroy_mw_ioctl
{
	struct _ual_destroy_mw_ioctl_in
	{
		uint64_t					h_mw;

	}	in;
	struct _ual_destroy_mw_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_destroy_mw_ioctl_t;
/*
* FIELDS
*	in.h_mw
*		A handle to an existing memory window.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_post_send_ioctl_t
* NAME
*	ual_post_send_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_post_send
*
* SYNOPSIS
*/
typedef union _ual_post_send_ioctl
{
	struct _ual_post_send_ioctl_in
	{
		uint64_t					h_qp;
		uint32_t					num_wr;
		uint32_t					num_ds;
		ib_send_wr_t				send_wr[1];
		/* Additional work requests follow, followed by data segments. */

	}	in;
	struct _ual_post_send_ioctl_out
	{
		ib_api_status_t				status;
		uint32_t					failed_cnt;

	}	out;

}	ual_post_send_ioctl_t;
/*
* FIELDS
*	in.h_qp
*		A handle to QP where the work request is being posted.
*
*	in.num_wr
*		Number of work request items in the array of work requests.
*
*	in.num_ds
*		Number of data segments following the array of work requests.
*
*	in.send_wr
*		First work request in the array of work requests being posted.
*
*	out.status
*		Status of the operation.
*
*	out.failed_cnt
*		Number of work request that failed.
*****/


/****s* User-mode Access Layer/ual_post_srq_recv_ioctl_t
* NAME
*	ual_post_srq_recv_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_post_srq_recv
*
* SYNOPSIS
*/
typedef union _ual_post_srq_recv_ioctl
{
	struct _ual_post_srq_recv_ioctl_in
	{
		uint64_t					h_srq;
		uint32_t					num_wr;
		uint32_t					num_ds;
		ib_recv_wr_t				recv_wr[1];
		/* Additional work requests follow, followed by data segments. */

	}	in;
	struct _ual_post_srq_recv_ioctl_out
	{
		ib_api_status_t			status;
		uint32_t					failed_cnt;

	}	out;

}	ual_post_srq_recv_ioctl_t;
/*
* FIELDS
*	in.h_srq
*		A handle to SRQ where the work request is being posted.
*
*	in.num_wr
*		Number of work request items in the array of work requests.
*
*	in.num_ds
*		Number of data segments following the array of work requests.
*
*	in.recv_wr
*		First work request in the array of work requests being posted.
*
*	out.status
*		Status of the operation.
*
*	failed_cnt
*		Number of work request that failed.
*****/



/****s* User-mode Access Layer/ual_post_recv_ioctl_t
* NAME
*	ual_post_recv_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_post_recv
*
* SYNOPSIS
*/
typedef union _ual_post_recv_ioctl
{
	struct _ual_post_recv_ioctl_in
	{
		uint64_t					h_qp;
		uint32_t					num_wr;
		uint32_t					num_ds;
		ib_recv_wr_t				recv_wr[1];
		/* Additional work requests follow, followed by data segments. */

	}	in;
	struct _ual_post_recv_ioctl_out
	{
		ib_api_status_t				status;
		uint32_t					failed_cnt;

	}	out;

}	ual_post_recv_ioctl_t;
/*
* FIELDS
*	in.h_qp
*		A handle to QP where the work request is being posted.
*
*	in.num_wr
*		Number of work request items in the array of work requests.
*
*	in.num_ds
*		Number of data segments following the array of work requests.
*
*	in.recv_wr
*		First work request in the array of work requests being posted.
*
*	out.status
*		Status of the operation.
*
*	failed_cnt
*		Number of work request that failed.
*****/



/****s* User-mode Access Layer/ual_peek_cq_ioctl_t
* NAME
*	ual_peek_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_peek_cq
*
* SYNOPSIS
*/
typedef union _ual_peek_cq_ioctl
{
	struct _ual_peek_cq_ioctl_in
	{
		uint64_t					h_cq;

	}	in;
	struct _ual_peek_cq_ioctl_out
	{
		ib_api_status_t				status;
		uint32_t					n_cqes;

	}	out;

} ual_peek_cq_ioctl_t;
/*
* FIELDS
*	in.h_cq
*		A handle to a CQ.
*
*	out.status
*		Status of the operation.
*
*	out.n_cqes
*		The number of completion queue entries currently on the CQ.
*****/



/****s* User-mode Access Layer/ual_poll_cq_ioctl_t
* NAME
*	ual_poll_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_poll_cq
*
* SYNOPSIS
*/
typedef union _ual_poll_cq_ioctl
{
	struct _ual_poll_cq_ioctl_in
	{
		uint64_t					h_cq;
		uint32_t					num_wc;

	}	in;
	struct _ual_poll_cq_ioctl_out
	{
		ib_api_status_t				status;
		uint32_t					num_wc;
		ib_wc_t						wc[1];
		/* Additional WC's follow. */
	}	out;

}	ual_poll_cq_ioctl_t;
/*
* FIELDS
*	in.h_cq
*		A handle to cq that is going to be polled for completions.
*
*	in.num_wc
*		Number of work completions in the output array.
*
*	out.status
*		Status of the operation.
*
*	out.num_wc
*		Number of work completions polled.
*
*	out.wc
*		First work completion in the array to use for polling.
*****/



/****s* User-mode Access Layer/ual_rearm_cq_ioctl_t
* NAME
*	ual_rearm_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_rearm_cq
*
* SYNOPSIS
*/
typedef union _ual_rearm_cq_ioctl
{
	struct _ual_rearm_cq_ioctl_in
	{
		uint64_t					h_cq;
		boolean_t					solicited;

	}	in;
	struct _ual_rearm_cq_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_rearm_cq_ioctl_t;
/*
* FIELDS
*	in.h_cq
*		A handle to a CQ.
*
*	in.solicited
*		Flag for rearm CQ.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_rearm_n_cq_ioctl_t
* NAME
*	ual_rearm_n_cq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_rearm_n_cq
*
* SYNOPSIS
*/
typedef union _ual_rearm_n_cq_ioctl
{
	struct _ual_rearm_n_cq_ioctl_in
	{
		uint64_t					h_cq;
		uint32_t					n_cqes;

	}	in;
	struct _ual_rearm_n_cq_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_rearm_n_cq_ioctl_t;
/*
* FIELDS
*	in.h_cq
*		A handle to a CQ.
*
*	in.n_cqes
*		Rearm the CQ to signal when the next N completions are added.
*
*	in.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_attach_mcast_ioctl_t
* NAME
*	ual_attach_mcast_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	verbs attach multicast call.
*
* SYNOPSIS
*/
typedef union _ual_attach_mcast_ioctl
{
	struct _ual_attach_mcast_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_qp;
		ib_gid_t					mgid;
		ib_net16_t					mlid;

	}	in;
	struct _ual_attach_mcast_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_attach;

	}	out;

}	ual_attach_mcast_ioctl_t;
/*
* FIELDS
*	in.umv_buf
*		Opaque to IBAL buffer descriptor to allow the user-mode HCA library to
*		exchange private information with the kernel-mode HCA driver.
*
*	in.h_qp
*		Handle to the QP that is joining the multicast group.
*
*	in.mgid
*		Multicast GID address for this multicast group.
*
*	in.mlid
*		Multicast LID for this multicast group.
*
*	out.umv_buf
*		Returns the status from the HCA driver to the user-mode HCA library,
*		along with any vendor specific output information.
*
*	out.status
*		Status of the operation.
*
*	h_attach
*		Multicast group handle.
*****/



/****s* User-mode Access Layer/ual_detach_mcast_ioctl_t
* NAME
*	ual_detach_mcast_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	verbs detach call.
*
* SYNOPSIS
*/
typedef union _ual_detach_mcast_ioctl
{
	struct _ual_detach_mcast_ioctl_in
	{
		uint64_t					h_attach;

	}	in;
	struct _ual_detach_mcast_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_detach_mcast_ioctl_t;
/*
* FIELDS
*	in.h_attach
*		A handle to the multicast group.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_reg_mad_svc_ioctl_t
* NAME
*	ual_reg_mad_svc_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_reg_mad_svc
*
* SYNOPSIS
*/
typedef union _ual_reg_mad_svc_ioctl
{
	struct _ual_reg_mad_svc_ioctl_in
	{
		uint64_t					h_qp;
		ib_mad_svc_t				mad_svc;

	}	in;
	struct _ual_reg_mad_svc_ioctl_out
	{
		ib_api_status_t				status;
		uint64_t					h_mad_svc;

	}	out;

}	ual_reg_mad_svc_ioctl_t;
/*
* FIELDS
*	in.h_qp
*		Handle to the special QP or MAD QP.
*
*	in.mad_svc
*		Mad service definition.
*
*	out.status
*		Status of the operation.
*
*	out.h_mad_svc
*		Handle to the mad service.
*****/



/****s* User-mode Access Layer/ual_dereg_mad_svc_ioctl_t
* NAME
*	ual_dereg_mad_svc_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_dereg_mad_svc
*
* SYNOPSIS
*/
typedef union _ual_dereg_mad_svc_ioctl
{
	struct _ual_dereg_mad_svc_ioctl_in
	{
		uint64_t					h_mad_svc;

	}	in;
	struct _ual_dereg_mad_svc_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_dereg_mad_svc_ioctl_t;
/*
* FIELDS
*	in.h_mad_svc
*		Handle to the mad service.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_reg_mad_pool_ioctl_t
* NAME
*	ual_reg_mad_pool_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	registering a mad pool to be used with special qp. This ioctl
*	will result in user-mode pool registered. Additionally,
*	the kernel proxy will allocate a kernel mad pool and register it
*	so that later mad_sends will have the appropriate pool in kernel.
*
* SYNOPSIS
*/
typedef union _ual_reg_mad_pool_ioctl
{
	struct _ual_reg_mad_pool_ioctl_in
	{
		uint64_t					h_pd;

	}	in;
	struct _ual_reg_mad_pool_ioctl_out
	{
		ib_api_status_t				status;
		uint64_t					pool_key;

	}	out;

}	ual_reg_mad_pool_ioctl_t;
/*
* FIELDS
*	in.h_pd
*		PD associated with the pool
*
*	out.status
*		Status of the operation.
*
*	out.pool_key
*		Pool key to the mad pool in kernel space
*****/



/****s* User-mode Access Layer/ual_dereg_mad_pool_ioctl_t
* NAME
*	ual_dereg_mad_pool_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	deregistering a mad pool to be used with special qp.
*	The kernel proxy will deregister and destroy the mad pool
*	created on behalf of the user process.
*
* SYNOPSIS
*/
typedef union _ual_dereg_mad_pool_ioctl
{
	struct _ual_dereg_mad_pool_ioctl_in
	{
		uint64_t					pool_key;

	}	in;
	struct _ual_dereg_mad_pool_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_dereg_mad_pool_ioctl_t;
/*
* FIELDS
*	in.pool_key
*		Pool key to the mad pool in kernel space.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_send_mad_ioctl_t
* NAME
*	ual_send_mad_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_send_mad
*
* SYNOPSIS
*/
typedef union _ual_send_mad_ioctl
{
	struct _ual_send_mad_ioctl_in
	{
		uint64_t						h_mad_svc;
		uint64_t						pool_key;
		uint64_t						h_av;
		uint64_t						p_mad_element;
		uint32_t						size;
		uint64_t						ph_proxy;

	}	in;
	struct _ual_send_mad_ioctl_out
	{
		ib_api_status_t					status;

	}	out;

}	ual_send_mad_ioctl_t;
/*
* FIELDS
*	in.h_mad_svc
*		Handle to the mad service.
*
*	in.pool_key
*		Pool key associated with the pool in kernel space.
*
*	in.h_av
*		handle to address vector of MAD.
*
*	in.p_mad_element
*		Pointer to the user-mode MAD element.  The proxy marshals this data.
*
*	in.size
*		size of MAD buffer to send.
*
*	in.ph_proxy
*		Location to which to write the context used to cancel the MAD.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_cancel_mad_ioctl_t
* NAME
*	ual_cancel_mad_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_cancel_mad
*
* SYNOPSIS
*/
typedef union _ual_cancel_mad_ioctl
{
	struct _ual_cancel_mad_ioctl_in
	{
		uint64_t					h_mad_svc;
		uint64_t					h_proxy_element;

	}	in;
	struct _ual_cancel_mad_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_cancel_mad_ioctl_t;
/*
* FIELDS
*	in.h_mad_svc
*		contains the handle to the mad service
*
*	in.h_mad_send
*		this references the handle to the sent MAD operation.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_spl_qp_ioctl_t
* NAME
*	ual_spl_qp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters to get
*	the alias qp from KAL.
*
* SYNOPSIS
*/
typedef union _ual_spl_qp_ioctl
{
	struct _ual_spl_qp_ioctl_in
	{
		ci_umv_buf_t				umv_buf;
		uint64_t					h_pd;
		ib_net64_t					port_guid;
		ib_qp_create_t				qp_create;
		uint64_t					context;

	}	in;
	struct _ual_spl_qp_ioctl_out
	{
		ci_umv_buf_t				umv_buf;
		ib_api_status_t				status;
		uint64_t					h_qp;

	}	out;

}	ual_spl_qp_ioctl_t;
/*
* FIELDS
*	in.h_pd
*		Protection domain for the special qp.
*
*	in.port_guid
*		Port GUID on which to allocate the special QP.
*
*	in.qp_create
*		Special QP creation parameters.
*
*	in.qp_context
*		Context to associate with the QP, to be used in any notifications.
*
*	out.status
*		Status of the operation.
*
*	out.h_qp
*		Handle to the special QP.
*****/



/****s* User-mode Access Layer/ual_mad_recv_ioctl_t
* NAME
*	ual_mad_recv_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters to get
*	the mad_element information upon receiving MAD.
*
* SYNOPSIS
*/
typedef union _ual_mad_recv_ioctl
{
	struct _ual_mad_recv_comp_ioctl_in
	{
		uint64_t					h_mad;
		uint64_t					p_user_mad;
		uint64_t					p_mad_buf;
		uint64_t					p_grh;

	}	in;
	struct _ual_mad_recv_comp_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

} ual_mad_recv_ioctl_t;
/*
* FIELDS
*	in.h_mad
*		Received MAD handle handed to usermode in the MAD recv notification.
*
*	in.p_user_mad
*		Pointer to a user-mode mad element.
*
*	in.p_mad_buf
*		Pointer to the MAD element's user-mode buffer.
*
*	in.p_grh
*		Ponter to the MAD element's user-mode GRH buffer.
*
*	out.status
*		Status of the operation.
*****/



/****s* User-mode Access Layer/ual_local_mad_ioctl_t
* NAME
*	ual_local_mad_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_local_mad
*
* SYNOPSIS
*/
typedef union _ual_local_mad_ioctl
{
	struct _ual_local_mad_ioctl_in
	{
		uint64_t					h_ca;
		__declspec(align(8)) uint8_t mad_in[MAD_BLOCK_SIZE];
		uint8_t					port_num;
		
		
	}	in;
	struct _ual_local_mad_ioctl_out
	{
		ib_api_status_t			status;
		uint32_t					_pad; /* 8-byte alignment needed for ia64 */
		__declspec(align(8)) uint8_t mad_out[MAD_BLOCK_SIZE];

	}	out;

}	ual_local_mad_ioctl_t;
/*
** FIELDS
*	in.h_ca
*		The handle to an open instance of CA returned via a
*		ual_open_ca_ioctl structure.
*	in.port_num
*		Port number this MAD refere to.
*	in.mad_in
*		Mad structure from user mode to forward to local HCA.
*
**	out.status
*		Status of the operation.
*	out.mad_out
*		Mad structure answer from local HCA for user mode.
*****/



/****s* User-mode Access Layer/ual_create_cep_ioctl_t
* NAME
*	ual_create_cep_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters to
*	create a CEP.
*
* SYNOPSIS
*/
typedef struct _ual_create_cep_ioctl
{
	ib_api_status_t				status;
	net32_t						cid;

}	ual_create_cep_ioctl_t;
/*
* FIELDS
*	status
*		Status of the operation.
*
*	cid
*		CID of the created CEP.
*****/


/****s* User-mode Access Layer/ual_cep_listen_ioctl_t
* NAME
*	ual_cep_listen_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters to
*	perform a CM listen request.
*
* SYNOPSIS
*/
typedef struct _ual_cep_listen_ioctl
{
	net32_t						cid;
	ib_cep_listen_t				cep_listen;
	uint8_t						compare[IB_REQ_PDATA_SIZE];

}	ual_cep_listen_ioctl_t;
/*
* FIELDS
*	in.cid
*		CID of an existing CEP.
*
*	in.cep_listen
*		Information used to direct the listen request to match incoming
*		connection requests.
*****/



/****s* User-mode Access Layer/ual_cm_req_ioctl_t
* NAME
*	ual_cm_req_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	al_cep_pre_req call.
*
* SYNOPSIS
*/
typedef union _ual_cep_req_ioctl
{
	struct _ual_cep_req_ioctl_in
	{
		net32_t						cid;
		ib_cm_req_t					cm_req;
		ib_path_rec_t				paths[2];
		uint8_t						pdata[IB_REQ_PDATA_SIZE];
		uint8_t						compare[IB_REQ_PDATA_SIZE];

	}	in;
	struct _ual_cep_req_ioctl_out
	{
		ib_api_status_t				status;
		ib_qp_mod_t					init;

	}	out;

}	ual_cep_req_ioctl_t;
/*
* FIELDS
*	in.cid
*		CID of the target CEP.
*
*	in.cm_req
*		CM REQ parameters.
*
*	in.paths
*		Array of paths, the first being the primary path to use for the REQ.
*
*	out.status
*		Status of the operation
*
*	out.init
*		QP modify paramters for INIT state transition.
*****/



/****s* User-mode Access Layer/ual_cep_rep_ioctl_t
* NAME
*	ual_cep_rep_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	al_cep_pre_rep call.
*
* SYNOPSIS
*/
typedef union _ual_cep_rep_ioctl
{
	struct _ual_cep_rep_ioctl_in
	{
		uint64_t					context;
		net32_t						cid;
		ib_cm_rep_t					cm_rep;
		uint8_t						pdata[IB_REP_PDATA_SIZE];

	}	in;
	struct _ual_cep_rep_ioctl_out
	{
		ib_api_status_t				status;
		ib_qp_mod_t					init;

	}	out;

}	ual_cep_rep_ioctl_t;
/*
* FIELDS
*	in.h_cm_req
*		The cm_req connection handle got on the callback.
*
*	in.cm_rep
*		CM REP parameters.
*
*	out.status
*		Status of the operation.
*
*	out.init
*		QP modify paramters for INIT state transition.
*****/



/****s* User-mode Access Layer/ual_cep_get_rtr_ioctl_t
* NAME
*	ual_cep_get_rtr_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters for
*	al_cep_get_rtr_attr call.
*
* SYNOPSIS
*/
typedef struct _ual_cep_get_rtr_ioctl
{
	ib_api_status_t				status;
	ib_qp_mod_t					rtr;

}	ual_cep_get_rtr_ioctl_t;
/*
* FIELDS
*	out.status
*		Status of the operation.
*
*	out.rtr
*		QP modify paramters for RTR state transition.
*****/



/****s* User-mode Access Layer/ual_cep_get_rts_ioctl_t
* NAME
*	ual_cep_get_rts_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters for
*	al_cep_get_rts_attr call.
*
* SYNOPSIS
*/
typedef struct _ual_cep_get_rts_ioctl
{
	ib_api_status_t				status;
	ib_qp_mod_t					rts;

}	ual_cep_get_rts_ioctl_t;
/*
* FIELDS
*	out.status
*		Status of the operation.
*
*	out.rts
*		QP modify paramters for RTS state transition.
*****/



/****s* User-mode Access Layer/ual_cep_rtu_ioctl_t
* NAME
*	ual_cep_rtu_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	al_cep_rtu call.
*
* SYNOPSIS
*/
typedef struct _ual_cep_rtu_ioctl
{
	net32_t						cid;
	uint8_t						pdata_len;
	uint8_t						pdata[IB_RTU_PDATA_SIZE];

}	ual_cep_rtu_ioctl_t;
/*
* FIELDS
*	in.cid
*		The cm_rep connection handle got on the callback.
*
*	in.pdata_len
*		Length of private data.
*
*	in.pdata
*		Private data.
*****/



/****s* User-mode Access Layer/ual_cep_rej_ioctl_t
* NAME
*	ual_cep_rej_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	al_cep_rej
*
* SYNOPSIS
*/
typedef struct _ual_cep_rej_ioctl
{
	net32_t						cid;

	ib_rej_status_t				rej_status;
	uint8_t						ari_len;
	uint8_t						pdata_len;
	uint8_t						ari[IB_ARI_SIZE];
	uint8_t						pdata[IB_REJ_PDATA_SIZE];

}	ual_cep_rej_ioctl_t;
/*
* FIELDS
*	in.cid
*		The CID of the target CEP.
*
*	in.rej_status
*		Rejection status as defined in IB spec.
*
*	in.ari_len
*		Length of the ARI data.
*
*	in.pdata_len
*		Length of the private data.
*
*	in.ari
*		ARI data.
*
*	in.pdata
*		Private data.
*****/



/****s* User-mode Access Layer/ual_cep_handoff_ioctl_t
* NAME
*	ual_cep_handoff_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_cm_handoff
*
* SYNOPSIS
*/
typedef union _ual_cep_handoff_ioctl
{
	struct _ual_cep_handoff_ioctl_in
	{
		uint64_t					h_cm;
		net64_t						sid;

	}	in;
	struct _ual_cep_handoff_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_cep_handoff_ioctl_t;
/*
* FIELDS
*	in.h_cm
*		The connection handle got on the callback.
*
*	in.sid
*		Service ID to which to handoff the listen.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_cep_mra_ioctl_t
* NAME
*	ual_cep_mra_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	ib_cm_mra
*
* SYNOPSIS
*/
typedef struct _ual_cep_mra_ioctl
{
	net32_t						cid;
	ib_cm_mra_t					cm_mra;
	uint8_t						pdata[IB_MRA_PDATA_SIZE];

}	ual_cep_mra_ioctl_t;
/*
* FIELDS
*	in.cid
*		The CID for the target CEP.
*
*	in.cm_mra
*		CM MRA parameters.
*****/



/****s* User-mode Access Layer/ual_cep_lap_ioctl_t
* NAME
*	ual_cep_lap_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	ib_cm_lap
*
* SYNOPSIS
*/
typedef struct _ual_cep_lap_ioctl
{
	net32_t						cid;
	ib_cm_lap_t					cm_lap;
	ib_path_rec_t				alt_path;
	uint8_t						pdata[IB_LAP_PDATA_SIZE];

}	ual_cep_lap_ioctl_t;
/*
* FIELDS
*	in.cm_lap
*		CM LAP parameters
*
*	in.alt_path
*		Alternate path information.
*****/



/****s* User-mode Access Layer/ual_cep_apr_ioctl_t
* NAME
*	ual_cep_apr_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	ib_cep_apr
*
* SYNOPSIS
*/
typedef union _ual_cep_apr_ioctl
{
	struct _ual_cep_apr_ioctl_in
	{
		net32_t					cid;
		ib_cm_apr_t				cm_apr;
		uint8_t					apr_info[IB_APR_INFO_SIZE];
		uint8_t					pdata[IB_APR_PDATA_SIZE];

	}	in;

	struct _ual_cep_apr_ioctl_out
	{
		ib_api_status_t			status;
		ib_qp_mod_t				apr;

	}	out;

}	ual_cep_apr_ioctl_t;
/*
* FIELDS
*	in.h_cm_lap
*		The cm_lap connection handle got on the LAP callback.
*
*	in.cm_apr
*		CM APR parameters.
*****/



/****s* User-mode Access Layer/ual_force_apm_ioctl_t
* NAME
*	ual_force_apm_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_force_apm
*
* SYNOPSIS
*/
typedef union _ual_force_apm_ioctl
{
	union _ual_force_apm_ioctl_in
	{
		uint64_t					h_qp;

	}	in;
	struct _ual_force_apm_ioctl_out
	{
		ib_api_status_t				status;

	}	out;

}	ual_force_apm_ioctl_t;
/*
* FIELDS
*	in.h_qp
*		A handle to the QP to migrate.
*
*	out.status
*		Status of the operation
*****/



/****s* User-mode Access Layer/ual_cep_dreq_ioctl_t
* NAME
*	ual_cep_dreq_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	ib_cm_dreq
*
* SYNOPSIS
*/
typedef struct _ual_cep_dreq_ioctl
{
	net32_t						cid;
	uint8_t						pdata_len;
	uint8_t						pdata[IB_DREQ_PDATA_SIZE];

}	ual_cep_dreq_ioctl_t;
/*
* FIELDS
*	cm_dreq
*		CM DREQ parameters.
*****/



/****s* User-mode Access Layer/ual_cep_drep_ioctl_t
* NAME
*	ual_cep_drep_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_cm_drep
*
* SYNOPSIS
*/
typedef struct _ual_cep_drep_ioctl
{
	net32_t						cid;
	uint8_t						pdata_len;
	uint8_t						pdata[IB_DREP_PDATA_SIZE];

}	ual_cep_drep_ioctl_t;
/*
* FIELDS
*	in.h_cm_dreq
*		The cm_dreq connection handle got on the callback.
*
*	in.cm_drep
*		CM DREP parameters.
*****/



/****s* User-mode Access Layer/ual_cep_get_timewait_ioctl_t
* NAME
*	ual_cep_get_timewait_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters for
*	ib_cep_get_timewait
*
* SYNOPSIS
*/
typedef struct _ual_cep_get_timewait_ioctl
{
	ib_api_status_t				status;
	uint64_t					timewait_us;

}	ual_cep_get_timewait_ioctl_t;
/*
* FIELDS
*	in.status
*		Status of the request.
*
*	in.timewait_us
*		Timewait value, in microseconds.
*****/



/****s* User-mode Access Layer/ual_cep_poll_ioctl_t
* NAME
*	ual_cep_poll_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters to
*	poll for incoming events on a CEP.  The input parameter is the CID.
*
* SYNOPSIS
*/
typedef struct _ual_cep_poll_ioctl
{
	ib_api_status_t				status;
	net32_t						new_cid;
	ib_mad_element_t			element;
	ib_grh_t					grh;
	uint8_t						mad_buf[MAD_BLOCK_SIZE];

}	ual_cep_poll_ioctl_t;
/*
* FIELDS
*	status
*		Status of the operation.
*
*	new_cep
*		For listen requests, CEP information of CEPs created in response
*		to incoming REQs.
*
*	mad_buf
*		Payload of a received MAD (or failed send)
*****/



/****s* User-mode Access Layer/ual_cep_get_pdata_t
* NAME
*	ual_cep_get_pdata_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for
*	getting private data of CM RDMA request
*
* SYNOPSIS
*/
typedef union _ual_cep_get_pdata_ioctl
{
	struct _ual_cep_get_pdata_ioctl_in
	{
		net32_t					cid;

	}	in;

	struct _ual_cep_get_pdata_ioctl_out
	{
		uint32_t				pdata_len;
		uint8_t					pdata[IB_REJ_PDATA_SIZE];
        uint8_t                 resp_res;
        uint8_t                 init_depth;

	}	out;

}	ual_cep_get_pdata_ioctl_t;
/*
* FIELDS
*	in.cid
*		The CID for the target CEP.
*
*	out.pdata_len
*		The size of the private data
*
*	out.pdata
*		The private data.
*****/



/****s* User-mode Access Layer/ual_reg_shmid_ioctl_t
* NAME
*	ual_reg_shmid_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_shmid
*
* SYNOPSIS
*/
typedef union _ual_reg_shmid_ioctl
{
	struct _ual_reg_shmid_ioctl_in
	{
		uint64_t					h_pd;
		ib_shmid_t					shmid;
		ib_mr_create_t				mr_create;

	}	in;
	struct _ual_reg_shmid_ioctl_out
	{
		ib_api_status_t				status;
		uint64_t					vaddr;
		net32_t						lkey;
		net32_t						rkey;
		uint64_t					h_mr;

	}	out;

}	ual_reg_shmid_ioctl_t;
/*
* PARAMETERS
*	in.h_pd
*		A handle to an existing protection domain that the memory
*		should be registered with.
*
*	in.shmid
*		An identifier to the shared memory region.
*
*	in.mr_create
*		Information describing the attributes of the memory region to
*		register.
*
*	out.status
*		Status of the operation.
*
*	out.vaddr
*		Assigned I/O virtual address for the memory region.
*
*	out.lkey
*		The local access key associated with this registered memory
*		region.
*
*	out.rkey
*		A key that may be used by a remote end-point when performing RDMA
*		or atomic operations to this registered memory region.
*
*	out.h_mr
*		Upon successful completion of this call, this references a handle
*		to the registered memory region.  This handle is used when performing
*		data transfers and to deregister the memory.
*****/



/****s* User-mode Access Layer/ual_send_sa_req_t
* NAME
*	ual_send_sa_req_t
*
* DESCRIPTION
*	IOCTL structure containing the input and output parameters for
*	ib_create_shmid
*
* SYNOPSIS
*/
typedef union _ual_send_sa_req_ioctl
{
	struct _ual_send_sa_req_ioctl_in
	{
		net64_t						port_guid;
		uint32_t					timeout_ms;
		uint32_t					retry_cnt;
		ib_user_query_t				sa_req;
		uint8_t						attr[IB_SA_DATA_SIZE];
		uint64_t					ph_sa_req;
		uint64_t					p_status;

	}	in;
	struct _ual_send_sa_req_ioctl_out
	{
		ib_api_status_t				status;
		uint64_t					h_resp;
		uint32_t					resp_size;

	}	out;

}	ual_send_sa_req_ioctl_t;
/*
* PARAMETERS
*	in.sa_mad_data
*		The SA request to send.
*
*	in.attr
*		The SA attribute data to send.
*
*	in.ph_sa_req
*		Pointer to UAL's query handle.  The proxy fills this in
*		before returning from the IOCTL handler to allow cancelation.
*
*	in.p_status
*		Pointer to status of the query.
*
*	out.status
*		Status of the query if it was initiated successfully.
*
*	out.h_resp
*		Handle to a response MAD.
*
*	out.resp_size
*		Size, in bytes, of the response MAD's buffer.
*****/



/****s* User-mode Access Layer/ual_cancel_sa_req_ioctl_t
* NAME
*	ual_cancel_sa_req_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for cancelling an
*	SA request.
*
* SYNOPSIS
*/
typedef struct _ual_cancel_sa_req_ioctl
{
	uint64_t			h_sa_req;

}	ual_cancel_sa_req_ioctl_t;
/*
* PARAMETERS
*	h_sa_req
*		Handle to the query to cancel.
*****/



/****s* User-mode Access Layer/ual_reg_pnp_ioctl_in_t
* NAME
*	ual_reg_pnp_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for registering
*	for PnP events.
*
* SYNOPSIS
*/
typedef struct _ual_reg_pnp_ioctl_in
{
	ib_pnp_class_t				pnp_class;
	void* __ptr64				sync_event; /* __ptr64 is correct for HANDLE types. */
	uint64_t					p_status;
	uint64_t					p_hdl;

}	ual_reg_pnp_ioctl_in_t;
/*
* NOTES
*	This is an asynchronous IOCTL.
*
*	The output parameters are a ual_rearm_pnp_ioctl_out_t.
*
* PARAMETERS
*	pnp_class
*		Class of PnP events for which to register.
*
*	p_status
*		Pointer to user-mode status variable to set in failure case.
*
*	p_hdl
*		Pointer to user-mode handle variable to set in success case.
*****/



/****s* User-mode Access Layer/ual_poll_pnp_ioctl_t
* NAME
*	ual_poll_pnp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the parameters for retriveing data for
*	a PnP event.
*
* SYNOPSIS
*/
typedef union _ual_poll_pnp_ioctl
{
	struct _ual_poll_pnp_ioctl_in
	{
		uint64_t					evt_hdl;

	}	in;
	struct _ual_poll_pnp_ioctl_out
	{
		ib_pnp_rec_t				pnp_rec;

	}	out;

}	ual_poll_pnp_ioctl_t;
/*
* NOTES
*	This is a synchronous IOCTL.
*
* PARAMETERS
*	in.evt_hdl
*		Handle to a new PnP event.
*
*	out.pnp_rec
*		Buffer for the new PnP event.
*****/



/****s* User-mode Access Layer/ual_rearm_pnp_ioctl_in_t
* NAME
*	ual_rearm_pnp_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for requesting
*	notification of the next PnP event.
*
* SYNOPSIS
*/
typedef struct _ual_rearm_pnp_ioctl_in
{
	uint64_t					pnp_hdl;
	uint64_t					last_evt_hdl;
	uint64_t					last_evt_context;
	ib_api_status_t				last_evt_status;

}	ual_rearm_pnp_ioctl_in_t;
/*
* NOTES
*	This is an asynchronous IOCTL.
*
*	The output parameters are a ual_rearm_pnp_ioctl_out_t.
*
* PARAMETERS
*	pnp_hdl
*		Handle to the PnP registration to rearm.
*
*	last_evt_hdl
*		Handle to the last PnP event processed.
*
*	last_evt_context
*		Context value to set for the last reported PnP event.
*
*	last_evt_status
*		Status value to return for the last reported PnP event.
*****/



/****s* User-mode Access Layer/ual_rearm_pnp_ioctl_out_t
* NAME
*	ual_rearm_pnp_ioctl_out_t
*
* DESCRIPTION
*	IOCTL structure containing the output parameters for a PnP event.
*
* SYNOPSIS
*/
typedef struct _ual_rearm_pnp_ioctl_out
{
	uint64_t					evt_hdl;
	uint32_t					evt_size;

}	ual_rearm_pnp_ioctl_out_t;
/*
* NOTES
*	This is an asynchronous IOCTL.
*
*	The output parameters are identical to that of ual_reg_pnp_ioctl_t.
*
* PARAMETERS
*	evt_hdl
*		Handle to a new PnP event.
*
*	evt_size
*		Buffer size needed to poll the new PnP event.
*****/



/****s* User-mode Access Layer/ual_dereg_pnp_ioctl_t
* NAME
*	ual_dereg_pnp_ioctl_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for cancelling an
*	SA request.
*
* SYNOPSIS
*/
typedef struct _ual_dereg_pnp_ioctl
{
	uint64_t			h_pnp;

}	ual_dereg_pnp_ioctl_t;
/*
* NOTES
*	This is an asynchronous IOCTL.
*
* PARAMETERS
*	h_pnp
*		Handle to the PnP registration to deregister.
*****/

/****s* User-mode Access Layer/ual_ndi_notify_cq_ioctl_in_t
* NAME
*	ual_ndi_notify_cq_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters for requesting
*	notification of the next event on NDI CQ.
*
* SYNOPSIS
*/
typedef struct _ual_ndi_notify_cq_ioctl_in
{
	uint64_t					h_cq;
	boolean_t					notify_comps;

}	ual_ndi_notify_cq_ioctl_in_t;
/*
* NOTES
*	This is an asynchronous IOCTL.
*
* FIELDS
*	h_cq
*		A handle to the CQ to modify.
*
*	notify_comps
*		Type of notification, requested. If TRUE - completion events, 
*		otherwise - errors.
*
*****/

/****s* User-mode Access Layer/ual_ndi_req_cm_ioctl_in_t
* NAME
*	ual_ndi_req_cm_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters 
*	sending CM REQ .
*
* SYNOPSIS
*/
typedef struct _ual_ndi_req_cm_ioctl_in
{
    ib_path_rec_t               path;
	uint64_t					h_qp;
	net64_t						guid;
	net32_t						cid;
	uint16_t					dst_port;
    uint8_t                     resp_res;
    uint8_t                     init_depth;
	uint8_t						prot;
	uint8_t						pdata_size;
	ib_cm_rdma_req_t			pdata;

}	ual_ndi_req_cm_ioctl_in_t;
/*
* NOTES
*	There is no output parameter.
*
* FIELDS
*	h_qp
*		A handle to the QP to modify.
*
*	guid
*		Local port GUID to which to bind to.
*
*	cid
*		CID of the CEP to use for the connection request.
*
*	dst_port
*		Destination port number.
*
*   path
*       Path record for the connection.
*
*   resp_res
*       Responder resources for the QP.
*
*   init_depth
*       Initiator depth for the QP.
*
*	prot
*		Protocol.
*
* 	pdata_size
*		The size of following private RDMA CM data
*
*	pdata
*		Private data in format RDMA CM
*
*****/

/****s* User-mode Access Layer/ual_ndi_rep_cm_ioctl_in_t
* NAME
*	ual_ndi_rep_cm_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters 
*	sending CM REP response .
*
* SYNOPSIS
*/
typedef struct _ual_ndi_rep_cm_ioctl_in
{
	uint64_t					h_qp;
	net32_t						cid;
	uint8_t						init_depth;
	uint8_t						resp_res;
	uint8_t						pdata_size;
	uint8_t						pdata[IB_REJ_PDATA_SIZE];

}	ual_ndi_rep_cm_ioctl_in_t;
/*
* NOTES
*	The output parameter is the new QP state (RTS).
*
* FIELDS
*	h_qp
*		A handle to the QP to modify.
*
*	cid
*		Connection ID.
*
*	init_depth
*		The maximum number of outstanding RDMA read/atomic operations.
*
*	resp_res
*		The maximum number of RDMA read/atomic operations from the recipient.
*
*	pdata_size
*		The size of following private data
*
*	pdata
*		Private data in format RDMA CM
*
*****/

/****s* User-mode Access Layer/ual_ndi_rej_cm_ioctl_in_t
* NAME
*	ual_ndi_rej_cm_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters 
*	sending CM REJ response .
*
* SYNOPSIS
*/
typedef struct _ual_ndi_rej_cm_ioctl_in
{
	net32_t						cid;
	uint8_t						pdata_size;
	uint8_t						pdata[IB_REJ_PDATA_SIZE];

}	ual_ndi_rej_cm_ioctl_in_t;
/*
* NOTES
*	The output parameter is the new QP state (RTS).
*
* FIELDS
*	cid
*		Connection ID.
*
*	pdata_size
*		The size of following private data
*
*	pdata
*		Private data in format RDMA CM
*
*****/

/****s* User-mode Access Layer/ual_ndi_modify_qp_ioctl_in_t
* NAME
*	ual_ndi_modify_qp_ioctl_in_t
*
* DESCRIPTION
*	IOCTL structure containing the input parameters 
*	for MODIFY_QP  .
*
* SYNOPSIS
*/
typedef struct _ual_ndi_modify_qp_ioctl_in
{
	uint64_t					h_qp;
	ib_qp_mod_t 				qp_mod;

}	ual_ndi_modify_qp_ioctl_in_t;
/*
* NOTES
*	The output parameter is the new QP state.
*
* FIELDS
*	h_qp
*		A handle to the QP to modify.
*
*	qp_mod
*		Attributes used for modifying the QP.
*
*****/



#endif /* __IB_UAL_IOCTL_H__ */

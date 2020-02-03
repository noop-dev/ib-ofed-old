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
 * $Id: cl_pnp_po.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef	_CL_PNP_PO_H_
#define _CL_PNP_PO_H_


#include "complib/cl_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/****h* Component Library/Plug and Play
* NAME
*	Plug and Play
*
* DESCRIPTION
*	Provides plug and play support for kernel drivers.
*
* SEE ALSO
*	Callback Types:
*		cl_pfn_pnp_po_t
*
*	Structures:
*		cl_vfptr_pnp_t
*********/


#define CL_DBG_PNP		(1 << 1)


/****d* Component Library: Plug and Play/cl_irp_action_t
* NAME
*	cl_irp_action_t
*
* DESCRIPTION
*	Indicates what kind of action to take in response to an IRP.  Used
*	when processing PnP IRPs.
*
* SYNOPSIS:
*/
typedef enum _cl_irp_action
{
	IrpPassDown,
	IrpSkip,
	IrpIgnore,
	IrpComplete,
	IrpDoNothing

}	cl_irp_action_t;
/*
* VALUES:
*	IrpPassDown
*		Pass the IRP down to the next driver.  The IRP's current stack location
*		has been setup properly.
*
*	IrpSkip
*		Skip the current IRP stack location but sets the IRP's status.
*
*	IrpIgnore
*		Skip the current IRP without setting its status value since the IRP 
*		is not interesting.
*
*	IrpComplete
*		Complete the IRP.  Sets the status in the IoStatus block, completes the
*		IRP, and returns the status.
*
*	IrpDoNothing
*		The routine assumed control of the IRP completion.  No further 
*		processing is required in the dispatch routine.  The remove lock should
*		not be released.
*********/


/****d* Component Library: Plug and Play/cl_pfn_pnp_po_t
* NAME
*	cl_pfn_pnp_po_t
*
* DESCRIPTION
*	Function prototype for PnP and Power Management IRPs handlers.
*
* SYNOPSIS
*/
typedef NTSTATUS
(*cl_pfn_pnp_po_t)(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t, cl_vfptr_pnp_po_t
*********/


/****d* Component Library: Plug and Play/cl_pfn_release_resources_t
* NAME
*	cl_pfn_release_resources_t
*
* DESCRIPTION
*	Function prototype for releasing resources associated with a device.
*	Called from either the remove handler or the surprise remove handler.
*
* SYNOPSIS
*/
typedef void
(*cl_pfn_release_resources_t)(
	IN				DEVICE_OBJECT* const		p_dev_obj );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object whose resources to release.
*
* RETURN VALUES
*	This function does not return a value.
*
* SEE ALSO
*	Plug and Play, cl_vfptr_pnp_po_t
*********/


/****d* Component Library: Plug and Play/cl_vfptr_pnp_po_t
* NAME
*	cl_vfptr_pnp_po_t
*
* DESCRIPTION
*	Virtual function pointer table for PnP and Power Management IRPs.
*
* SYNOPSIS
*/
typedef struct _cl_vfptr_pnp_po
{
	const char*						identity;
	cl_pfn_pnp_po_t					pfn_start;
	cl_pfn_pnp_po_t					pfn_query_stop;
	cl_pfn_pnp_po_t					pfn_stop;
	cl_pfn_pnp_po_t					pfn_cancel_stop;
	cl_pfn_pnp_po_t					pfn_query_remove;
	cl_pfn_release_resources_t		pfn_release_resources;
	cl_pfn_pnp_po_t					pfn_remove;
	cl_pfn_pnp_po_t					pfn_cancel_remove;
	cl_pfn_pnp_po_t					pfn_surprise_remove;
	cl_pfn_pnp_po_t					pfn_query_capabilities;
	cl_pfn_pnp_po_t					pfn_query_pnp_state;
	cl_pfn_pnp_po_t					pfn_filter_res_req;
	cl_pfn_pnp_po_t					pfn_dev_usage_notification;
	cl_pfn_pnp_po_t					pfn_query_bus_relations;
	cl_pfn_pnp_po_t					pfn_query_ejection_relations;
	cl_pfn_pnp_po_t					pfn_query_removal_relations;
	cl_pfn_pnp_po_t					pfn_query_target_relations;
	cl_pfn_pnp_po_t					pfn_unknown;
	cl_pfn_pnp_po_t					pfn_query_resources;
	cl_pfn_pnp_po_t					pfn_query_res_req;
	cl_pfn_pnp_po_t					pfn_query_bus_info;
	cl_pfn_pnp_po_t					pfn_query_interface;
	cl_pfn_pnp_po_t					pfn_read_config;
	cl_pfn_pnp_po_t					pfn_write_config;
	cl_pfn_pnp_po_t					pfn_eject;
	cl_pfn_pnp_po_t					pfn_set_lock;
	cl_pfn_pnp_po_t					pfn_query_power;
	cl_pfn_pnp_po_t					pfn_set_power;
	cl_pfn_pnp_po_t					pfn_power_sequence;
	cl_pfn_pnp_po_t					pfn_wait_wake;

}	cl_vfptr_pnp_po_t;
/*
* FIELDS
*	identity
*		string identifying the target, used in generating debug output.
*
*	pfn_start
*		IRP_MN_START_DEVICE handler.  Users must send forward the IRP to lower
*		devices as required for their driver type.
*
*	pfn_query_stop
*		IRP_MN_QUERY_STOP_DEVICE handler.
*
*	pfn_stop
*		IRP_MN_STOP_DEVICE handler.
*
*	pfn_cancel_stop
*		IRP_MN_CANCEL_STOP_DEVICE handler.  Users must send forward the IRP to 
*		lower devices as required for their driver type.
*
*	pfn_query_remove
*		IRP_MN_QUERY_REMOVE_DEVICE handler.
*
*	pfn_release_resources
*		Called to release resources allocated for the device.
*
*	pfn_remove
*		IRP_MN_REMOVE_DEVICE handler.
*
*	pfn_cancel_remove
*		IRP_MN_CANCEL_REMOVE_DEVICE handler.  Users must send forward the IRP
*		to lower devices as required for their driver type.
*
*	pfn_surprise_remove
*		IRP_MN_SURPRISE_REMOVE_DEVICE handler.
*
*	pfn_query_capabilities
*		IRP_MN_QUERY_DEVICE_CAPABILITIES handler.
*
*	pfn_query_pnp_state
*		IRP_MN_QUERY_PNP_STATE handler.
*
*	pfn_filter_res_req
*		IRP_MN_FILTER_RESOURCE_REQUIREMENTS handler.
*
*	pfn_dev_usage_notification
*		IRP_MN_QUERY_DEVICE_USAGE_NOTIFICATION handler.
*
*	pfn_query_bus_relations
*		IRP_MN_QUERY_BUS_RELATIONS handler.
*
*	pfn_query_ejection_relations
*		IRP_MN_QUERY_EJECTION_RELATIONS handler.
*
*	pfn_query_removal_relations
*		IRP_MN_QUERY_REMOVAL_RELATIONS handler.
*
*	pfn_query_target_relations
*		IRP_MN_QUERY_TARGET_RELATIONS handler.
*
*	pfn_unknown
*		FDO and Filter drivers should pass this IRP down.  Bus drivers should 
*		complete the request for their PDOs without modifying the status.
*		The component library provides the cl_irp_skip and cl_irp_complete 
*		functions to skip and complete and IRP, respectively.  These functions
*		can be used to perform the required action.  This handler is invoked
*		when an undocumented PNP irp is received.
*
*	pfn_query_resources
*		IRP_MN_QUERY_RESOURCES handler.
*
*	pfn_query_res_req
*		IRP_MN_QUERY_RESOURCE_REQUIREMENTS handler.
*
*	pfn_query_bus_info
*		IRP_MN_QUERY_BUS_INFORMATION handler.
*
*	pfn_query_interface
*		IRP_MN_QUERY_INTERFACE handler.
*
*	pfn_read_config
*		IRP_MN_READ_CONFIG handler.
*
*	pfn_write_config
*		IRP_MN_WRITE_CONFIG handler.
*
*	pfn_eject
*		IRP_MN_EJECT handler.
*
*	pfn_set_lock
*		IRP_MN_SET_LOCK handler.
*
*	pfn_query_power
*		IRP_MN_QUERY_POWER handler.
*
*	pfn_set_power
*		IRP_MN_SET_POWER handler.
*
*	pfn_power_sequence
*		IRP_MN_POWER_SEQUENCE handler.
*
*	pfn_wait_wake
*		IRP_MN_WAIT_WAKE handler.
*
* NOTES
*	The component library provides default handlers for skiping the IRP,
*	completing the IRP without changing the status, and processing the IRP
*	on the way up the device stack.  Users can set the handler to 
*	cl_irp_skip, cl_irp_complete, cl_do_sync_pnp.
*
*	The handlers specified in pfn_query_power, pfn_set_power, 
*	pfn_power_sequence, and pfn_wait_wake, if implemented as pageable code,
*	should be marked in a single pageable code section.  The component library
*	will use the first function that is not a component library handler to
*	lock down the user's power management code.
*
* SEE ALSO
*	Plug and Play, cl_pfn_pnp_po_t, cl_pfn_release_resources_t, cl_do_sync_pnp,
*	cl_irp_skip, cl_irp_complete, cl_irp_unsupported, cl_irp_fail
***********/


/****d* Component Library: Plug and Play/cl_pfn_query_text_t
* NAME
*	cl_pfn_query_text_t
*
* DESCRIPTION
*	Function pointer type for handling IRP_MN_QUERY_DEVICE_TEXT IRPs.
*
* SYNOPSIS
*/
typedef NTSTATUS
(*cl_pfn_query_text_t)(
	IN				DEVICE_OBJECT* const		p_dev_obj,
		OUT			IRP* const					p_irp );
/*
* PARAMETERS
*	pDevObj
*		Pointer to the device object that is the IRP target.
*
*	pIrp
*		[out] Pointer to the request IRP.  Sets the requested device text in
*		pIrp->IoStatus.Information.
*
* RETURN VALUES
*	NTSTATUS value indicating the result of the query.
*
* NOTES
*	Only bus drivers handle the IRP_MN_QUERY_DEVICE_TEXT IRP for their PDOs.
*
* SEE ALSO
*	Plug and Play
*********/


/****d* Component Library: Plug and Play/cl_vfptr_query_txt
* NAME
*	cl_vfptr_query_txt
*
* DESCRIPTION
*	Function pointer table for the various type of text requested by
*	IRP_MN_QUERY_DEVICE_TEXT IRPs.
*
* SYNOPSIS
*/
typedef struct _cl_vfptr_query_txt
{
	cl_pfn_query_text_t		pfn_query_device_id;
	cl_pfn_query_text_t		pfn_query_hardware_id;
	cl_pfn_query_text_t		pfn_query_compatible_id;
	cl_pfn_query_text_t		pfn_query_unique_id;
	cl_pfn_query_text_t		pfn_query_description;
	cl_pfn_query_text_t		pfn_query_location;

}	cl_vfptr_query_txt_t;
/*
* FIELDS
*	pfn_query_device_id
*		The request is for the target device's device ID.
*
*	pfn_query_hardware_id
*		The request is for the target device's device IDs.
*
*	pfn_query_compatible_id
*		The request is for the target device's comptible IDs.
*
*	pfn_query_unique_id
*		The request is for the target device's unique ID.
*
*	pfn_query_description
*		The request is for the target device's description.
*
*	pfn_query_location
*		The request is for the target device's location text.
*
* NOTES
*	Hardware and compatible IDs should be returned in the most specific to
*	most general order.  The IDs are used to match drivers to devices.
*
*	The query text function pointer table is maintained separately from the
*	PnP function pointer table to allow FDO and filter drivers to not define
*	the table since they typically do not handle these requests.
*
* SEE ALSO
*	Plug and Play, cl_pfn_query_text_t
*********/


/****d* Component Library: Plug and Play/cl_pnp_state_t
* NAME
*	cl_pnp_state_t
*
* DESCRIPTION
*	PnP States for device objects managed by this driver.
*
* SYNOPSIS
*/
typedef enum _cl_pnp_state
{
	NotStarted = 0,
	Started,
	StopPending,
	Stopped,
	RemovePending,
	SurpriseRemoved,
	Deleted,
	UnKnown

}	cl_pnp_state_t;
/*
* VALUES
*	NotStarted
*		Not started yet.
*
*	Started
*		Device has received the IPR_MN_START_DEVICE IRP
*
*	StopPending
*		Device has received the IPR_MN_QUERY_STOP_DEVICE IRP
*
*	Stopped
*		Device has received the IPR_MN_STOP_DEVICE IRP
*
*	RemovePending
*		Device has received the IPR_MN_QUERY_REMOVE_DEVICE IRP
*
*	SurpriseRemoved
*		Device has received the IPR_MN_SURPRISE_REMOVE_DEVICE IRP
*
*	Deleted
*		Device has received the IPR_MN_REMOVE_DEVICE IRP
*
*	UnKnown
*		Unknown state
*
* SEE ALSO
*	Plug and Play, cl_pnp_po_ext_t
*********/


/****d* Component Library: Plug and Play/cl_pnp_po_ext_t
* NAME
*	cl_pnp_po_ext_t
*
* DESCRIPTION
*	Device extension structure required for using the component library
*	plug and play helper routines.
*
* SYNOPSIS
*/
typedef struct _cl_pnp_po_ext
{
	cl_pnp_state_t				pnp_state;
	cl_pnp_state_t				last_pnp_state;

	DEVICE_OBJECT				*p_self_do;
	DEVICE_OBJECT				*p_next_do;
	DEVICE_OBJECT				*p_pdo;

	IO_REMOVE_LOCK				remove_lock;
	IO_REMOVE_LOCK				stop_lock;

	atomic32_t					n_paging_files;
	atomic32_t					n_crash_files;
	atomic32_t					n_hibernate_files;

	const cl_vfptr_pnp_po_t		*vfptr_pnp_po;
	const cl_vfptr_query_txt_t	*vfptr_query_txt;

	void						*h_cl_locked_section;
	void						*h_user_locked_section;

	atomic32_t					n_ifc_ref;

	uint32_t					dbg_lvl;

}	cl_pnp_po_ext_t;
/*
* FIELDS
*	pnp_state
*		Current PnP device state.
*
*	last_pnp_state
*		Previous PnP device state, to restore in case a query is cancelled.
*
*	p_self_do
*		Pointer to the device's own device object.
*
*	p_next_do
*		Pointer to the next device object.  Null if the device is a PDO.
*
*	p_pdo
*		The pointer to the PDO for the device node.
*	
*	remove_lock
*		Remove lock used to synchronize access to the device when handling
*		PnP IRPs.
*
*	stop_lock
*		Lock used to track non-PnP and non-Power IO operations.  
*	
*	n_paging_files
*		Number of times the device is in a paging file path.
*
*	n_crash_files
*		Number of times the device is in a dump file path.
*
*	n_hibernate_files
*		Number of times the device is in a hibernation files path.
*	
*	vfptr_pnp
*		Funtion pointer table for the PnP and Power Management handlers.
*
*	vfptr_query_txt
*		Function pointer table for IRP_MN_QUERY_DEVICE_TEXT handlers.
*
* NOTES
*	This structure must be first in the device extension so that the device
*	extension can successfully be cast to a cl_pnp_po_ext_t pointer by the
*	IRP handler routines.
*
*	When performing I/O operations, users should acquire the stop lock and
*	check status before starting an I/O operation, and release the stop lock
*	after completing an I/O operation using the cl_start_io and cl_end_io
*	functions respectively.
*
* SEE ALSO
*	Plug and Play, cl_vfptr_pnp_po_t, cl_pnp_state_t, cl_start_io, cl_end_io
*********/


/****f* Component Library: Plug and Play/cl_init_pnp_po_ext
* NAME
*	cl_init_pnp_po_ext
*
* DESCRIPTION
*	Initializes the component library device extension for use.
*
* SYNOPSIS
*/
CL_EXPORT void
cl_init_pnp_po_ext(
	IN	OUT			DEVICE_OBJECT* const		p_dev_obj,
	IN				DEVICE_OBJECT* const		p_next_do,
	IN				DEVICE_OBJECT* const		p_pdo,
	IN		const	uint32_t					pnp_po_dbg_lvl,
	IN		const	cl_vfptr_pnp_po_t* const	vfptr_pnp,
	IN		const	cl_vfptr_query_txt_t* const	vfptr_query_txt OPTIONAL );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object for the device.
*
*	p_next_do
*		Pointer to the next device object in the device stack.  Must be NULL
*		for PDO objects.
*
*	p_pdo
*		Pointer to the PDO for the device node.
*
*	pnp_po_dbg_lvl
*		Debug level to control flow of debug messages.  If the bit for
*		CL_DBG_PNP is set, verbose debug messages are generated.
*
*	vfptr_pnp_po
*		Pointer to the function table of PnP and Power Management handlers.
*
*	vfptr_query_txt
*		Pointer to the function table for IRP_MN_QUERY_DEVICE_TEXT handlers.
*
* SEE ALSO
*	Plug and Play, cl_pnp_po_ext_t, cl_vfptr_pnp_po_t, cl_vfptr_query_txt_t
*********/


/****f* Component Library: Plug and Play/cl_set_pnp_state
* NAME
*	cl_set_pnp_state
*
* DESCRIPTION
*	Sets the PnP state stored in the common device extension to the desired
*	state.  Stores the previous state for rollback purposes.
*
* SYNOPSIS
*/
CL_INLINE void
cl_set_pnp_state(
		OUT			cl_pnp_po_ext_t* const		p_ext,
	IN		const	cl_pnp_state_t				new_state )
{
	p_ext->last_pnp_state = p_ext->pnp_state;
	p_ext->pnp_state = new_state;
}
/*
* PARAMTETERS
*	p_ext
*		Pointer to the device extension whose PnP state to set.
*
*	new_state
*		New PnP state to store in the device extension.
*
* RETURN VALUES
*	This function does not return a value.
*
* SEE ALSO
*	Plug and Play, cl_pnp_po_ext_t, cl_pnp_state_t
*********/


/****f* Component Library: Plug and Play/cl_rollback_pnp_state
* NAME
*	cl_rollback_pnp_state
*
* DESCRIPTION
*	Rolls back a PnP state change.
*
* SYNOPSIS
*/
CL_INLINE void
cl_rollback_pnp_state(
		OUT			cl_pnp_po_ext_t* const		p_ext )
{
	p_ext->pnp_state = p_ext->last_pnp_state;
}
/*
* PARAMTETERS
*	p_ext
*		Pointer to the device extension whose PnP state to set.
*
* RETURN VALUES
*	This function does not return a value.
*
* SEE ALSO
*	Plug and Play, cl_pnp_po_ext_t
*********/


/****f* Component Library: Plug and Play:/cl_pnp
* NAME
*	cl_pnp
*
* DESCRIPTION
*	Main PnP entry point for the driver.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_pnp(
	IN				PDEVICE_OBJECT				p_dev_obj,
	IN				PIRP						p_irp );
/*
* PARAMTETERS
*	p_dev_obj
*		Pointer to the device object that is the target of the PnP IRP.
*
*	p_irp
*		Pointer to the PnP IRP to perform on the specified device.
*
* RETURN VALUES
*	STATUS_SUCCESS if the operation is successful
*
*	Other NTSTATUS values in case of error.
*
* SEE ALSO
*	Plug and Play
**********/


/****f* Component Library: Plug and Play:/cl_power
* NAME
*	cl_power
*
* DESCRIPTION
*	Main Power Management entry point for the driver.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_power(
	IN				PDEVICE_OBJECT				p_dev_obj,
	IN				PIRP						p_irp );
/*
* PARAMTETERS
*	p_dev_obj
*		Pointer to the device object that is the target of the PnP IRP.
*
*	p_irp
*		Pointer to the PnP IRP to perform on the specified device.
*
* RETURN VALUES
*	STATUS_SUCCESS if the operation is successful
*
*	Other NTSTATUS values in case of error.
*
* SEE ALSO
*	Plug and Play
**********/


/****f* Component Library: Plug and Play/cl_do_sync_pnp
* NAME
*	cl_do_sync_pnp
*
* DESCRIPTION
*	Sends an IRP to the next driver synchronously.  Returns when the lower 
*	drivers have completed the IRP.  Used to process IRPs on the way up the
*	device node.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_do_sync_pnp(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETER
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value returned by lower driver.
*********/


/****f* Component Library: Plug and Play/cl_alloc_relations
* NAME
*	cl_alloc_relations
*
* DESCRIPTION
*	Allocates device relations and copies existing device relations, if any.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_alloc_relations(
	IN				IRP* const					p_irp,
	IN		const	size_t						n_devs );
/*
* PARAMETERS
*	p_irp
*		Pointer to the IPR_MN_QUERY_DEVICE_RELATIONS IRP for which the
*		relations are being allocated.
*
*	n_devs
*		Number of devices the caller will report in the DEVICE_RELATIONS
*		structure stored at pIrp->IoStatus.Infomation upon success.
*
* RETURN VALUES
*	STATUS_SUCCESS if the DEVICE_RELATIONS structure was allocated successfully.
*
*	STATUS_INSUFFICIENT_RESOURCES if there was not enough memory to complete
*	the operation.
*
* NOTES
*	Upon failure, any original relations buffer is freed.  Users should fail
*	the IRP with the returned status value.
*
* SEE ALSO
*	Plug and Play
**********/


/****f* Component Library: Plug and Play/cl_do_remove
* NAME
*	cl_do_remove
*
* DESCRIPTION
*	Propagates an IRP_MN_REMOVE_DEVICE IRP, detaches, and deletes the
*	device object.  Useable by function and filter drivers only.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_do_remove(
	IN					DEVICE_OBJECT* const	pDevObj,
	IN					IRP* const				pIrp, 
		OUT				cl_irp_action_t* const	pAction );
/**********/


/****f* Component Library: Plug and Play:/cl_irp_skip
* NAME
*	cl_irp_skip
*
* DESCRIPTION
*	Default function for skipping a PnP IRP.  Sets the IRP's status value.
*	Useable only by function and filter drivers.  Bus drivers should use 
*	cl_irp_complete for their PDOs to complete an IRP with no change in status.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_irp_skip(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t
*********/


/****f* Component Library: Plug and Play:/cl_irp_ignore
* NAME
*	cl_irp_ignore
*
* DESCRIPTION
*	Default function for skipping a PnP IRP without setting the IRP's status.
*	Useable only by function and filter drivers.  Bus drivers should use 
*	cl_irp_complete for their PDOs to complete an IRP with no change in status.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_irp_ignore(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t
*********/


/****f* Component Library: Plug and Play:/cl_irp_complete
* NAME
*	cl_irp_complete
*
* DESCRIPTION
*	Default handler for completeing a PnP or Power Management IRP with no 
*	action.  Should only be used by bus drivers for their PDOs to complete 
*	an IRP with no change in status.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_irp_complete(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t, cl_vfptr_pnp_po_t
*********/


/****f* Component Library: Plug and Play:/cl_irp_succeed
* NAME
*	cl_irp_succeed
*
* DESCRIPTION
*	Default handler for succeeding an IRP with STATUS_SUCCESS.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_irp_succeed(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t
*********/


/****f* Component Library: Plug and Play:/cl_irp_unsupported
* NAME
*	cl_irp_unsupported
*
* DESCRIPTION
*	Default handler for failing an IRP with STATUS_UNSUPPORTED.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_irp_unsupported(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IRP* const					p_irp, 
		OUT			cl_irp_action_t* const		p_action );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_irp
*		Pointer to the request IRP.
*
*	p_action
*		Action to take for propagating the IRP.
*
* RETURN VALUES
*	IRP status value.
*
* SEE ALSO
*	Plug and Play, cl_irp_action_t
*********/


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* _CL_PNP_PO_H_ */

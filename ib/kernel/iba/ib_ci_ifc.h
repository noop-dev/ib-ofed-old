/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
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
 * $Id: ib_ci_ifc.h 4395 2009-06-01 08:20:57Z leonid $
 */



#if !defined _IB_CI_IFC_H_
#define _IB_CI_IFC_H_


#include "iba/ib_ci.h"


/****h* Access Layer/AL Channel Interface
* NAME
*	AL Channel Interface
*
* DESCRIPTION
*	Header file for the interface exported to HCA drivers to allow them
*	to register with AL for use by AL clients.
*********/


#define IB_CI_INTERFACE_VERSION		(5)


/* Function type for ib_register_ca.  Match the definition in ib_ci.h */
typedef ib_api_status_t
(*ib_pfn_register_ca_t)(
	IN		const	ci_interface_t* const		p_ci,
	IN		const	PDEVICE_OBJECT				p_hca_dev,
	IN		const	PDEVICE_OBJECT				p_fdo
	);


/* Function type for ib_deregister_ca.  Matches the definition in ib_ci.h */
typedef ib_api_status_t
(*ib_pfn_deregister_ca_t)(
	IN		const	net64_t						ca_guid );


/*
 * Appends a CA's device relations given a CA guid to an existing list
 * of relations.
 */
typedef NTSTATUS
(*ib_pfn_get_relations_t)(
	IN				net64_t						ca_guid,
	IN	OUT			IRP* const					p_irp );


typedef const char*
(*ib_pfn_get_err_str_t)(
	IN				ib_api_status_t				status );



/****s* Access Layer: AL Channel Interface/ib_ci_ifc_t
* NAME
*	ib_ci_ifc_t
*
* DESCRIPTION
*	
* SYNOPSIS
*/
typedef struct _ib_ci_ifc
{

	INTERFACE				wdm;

	ib_pfn_register_ca_t	register_ca;
	ib_pfn_deregister_ca_t	deregister_ca;
	ib_pfn_get_relations_t	get_relations;
	ib_pfn_get_err_str_t	get_err_str;

}	ib_ci_ifc_t;
/*
* FIELDS
*	wdmr.
*		Standard interface header.
*
*	register_ca
*		Pointer  to ib_pfn_register_ca_t function
*
*	deregister_ca
*		Pointer  to ib_pfn_deregister_ca_t function
*
*	get_relations
*		Pointer  to ib_pfn_get_relations_t function
*
*	get_err_str
*		Pointer  to ib_pfn_get_err_str_t function
*
* SEE ALSO
*	ib_register_ca,ib_deregister_ca,
*********/


#endif	/* !defined _IB_CI_IFC_H_ */

/*
 * CI interface GUID.  The GUID is defined outside the conditional include
 * on purpose so that it can be instantiated only once where it is actually
 * needed.  See the DDK docs section "Using GUIDs in Drivers" for more info.
 */
/* {9B617D6B-65AB-4fe5-9601-555271F7534D} */
DEFINE_GUID(GUID_IB_CI_INTERFACE, 
0x9b617d6b, 0x65ab, 0x4fe5, 0x96, 0x1, 0x55, 0x52, 0x71, 0xf7, 0x53, 0x4d);

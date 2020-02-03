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
 * $Id: iou_ifc.h 2668 2008-07-01 15:29:00Z leonid $
 */



#include "iba/ib_al_ifc.h"


#if !defined _IOU_IFC_H_
#define _IOU_IFC_H_


/****h* Access Layer/ IO Unit Interface
* NAME
*	IO Unit Interface
*
* DESCRIPTION
*	Header file for the interface exported to IOU client drivers to retrieve
*	IOU device information.
*
*	The interface contains information about the particular instance of an IOU.
*********/


#define IOU_INTERFACE_DATA_VERSION		(4)



/****s* Access Layer: IO Unit Interface/iou_ifc_data_t
* NAME
*	iou_ifc_data_t
*
* DESCRIPTION
*	I/O Unit interface data.
*
*	The iou_ifc_data_t structure 
*
* SYNOPSIS
*/
typedef struct _iou_ifc_data
{
	net64_t					ca_guid;
	net64_t					chassis_guid;
	uint8_t					slot;
	net64_t					guid;

}	iou_ifc_data_t;
/*
* FIELDS
*	ca_guid
*		Local CA GUID through which IOU is accessible.
*
*	chassis_guid
*		IOU chassis GUID.
*
*	slot
*		IOU  slot.
*
*	guid
*		IOU  GUID.
*
* SEE ALSO
*	
*********/

#endif	/* !defined _IOU_IFC_H_ */

/*
 * IOU interface GUID.  The GUID is defined outside the conditional include
 * on purpose so that it can be instantiated only once where it is actually
 * needed.  See the DDK docs section "Using GUIDs in Drivers" for more info.
 */
/* {C78F0228-E564-4d46-8A4B-295030414C1A} */
DEFINE_GUID(GUID_IOU_INTERFACE_DATA, 
0xc78f0228, 0xe564, 0x4d46, 0x8a, 0x4b, 0x29, 0x50, 0x30, 0x41, 0x4c, 0x1a);

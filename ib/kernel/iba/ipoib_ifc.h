/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Copyright (c) 2006 Mellanox Technologies.  All rights reserved.
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
 * $Id: ipoib_ifc.h 3031 2008-08-25 14:32:05Z tzachid $
 */



#include "iba/ib_al_ifc.h"


#if !defined _IPOIB_IFC_H_
#define _IPOIB_IFC_H_


/****h* Access Layer/IPoIB Interface
* NAME
*	IPoIB Interface
*
* DESCRIPTION
*	Header file for the interface exported to IPoIB client drivers for access
*	to IB resources provided by HCAs.
*
*	The actual interface returned is an contains information about the
*	particular instance of an IPoIB device.
*********/


#define IPOIB_INTERFACE_DATA_VERSION		(5)


/****s* Access Layer: IPoIB Interface/ipoib_ifc_data_t
* NAME
*	ipoib_ifc_data_t
*
* DESCRIPTION
*	IPoIB interface datat.
*
*	The port guid combined from guid + PKEY 
*/
typedef struct _port_guid_pkey
{
	net64_t		guid;
	ib_net16_t	pkey;
} port_guid_pkey_t;


/*
*	The ipoib_ifc_data_t structure 
*
* SYNOPSIS
*/
typedef struct _ipoib_ifc_data
{
	net64_t						ca_guid;
	port_guid_pkey_t			port_guid;
	uint8_t						port_num;

}	ipoib_ifc_data_t;
/*
* FIELDS
*	ca_guid
*		HCA GUID for this IPoIB interface
*
*	port_guid
*		Port GUID for this IPoIB interface
*
*	port_num
*		Port Number GUID for this IPoIB interface
*
* SEE ALSO
*	
*********/



#endif	/* !defined _IPOIB_IFC_H_ */

/*
 * IPOIB interface GUID.  The GUID is defined outside the conditional include
 * on purpose so that it can be instantiated only once where it is actually
 * needed.  See the DDK docs section "Using GUIDs in Drivers" for more info.
 */
/* {B40DDB48-5710-487a-B812-6DAF56C7F423} */
DEFINE_GUID(GUID_IPOIB_INTERFACE_DATA, 
0xb40ddb48, 0x5710, 0x487a, 0xb8, 0x12, 0x6d, 0xaf, 0x56, 0xc7, 0xf4, 0x23);

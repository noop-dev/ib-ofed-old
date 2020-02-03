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
 * $Id: ioc_ifc.h 1791 2006-11-12 14:40:09Z sleybo $
 */



#include <iba/ib_al_ifc.h>


#if !defined _IOC_IFC_H_
#define _IOC_IFC_H_


/****h* Access Layer/IO Controler Interface
* NAME
*	IO Controler Interface
*
* DESCRIPTION
*	Header file for the interface exported to IOC client drivers to retrieve
*	IOC device information.
*
*	The interface contains information about the particular instance of an IOC.
*********/


#define IOC_INTERFACE_DATA_VERSION		(3)



/****s* Access Layer: IO Controler Interface/ioc_ifc_data_t
* NAME
*	ioc_ifc_data_t
*
* DESCRIPTION
*	I/O Controler interface data.
*
*	The ioc_ifc_data_t structure 
*
* SYNOPSIS
*/
typedef struct _ioc_ifc_data
{
	net64_t					ca_guid;
	net64_t					guid;

}	ioc_ifc_data_t;
/*
* FIELDS
*	ca_guid
*		Local CA GUID through which IOC is accessible.
*
*	guid
*		IOC GUID.
*
* SEE ALSO
*	
*********/


#endif	/* !defined _IOC_IFC_H_ */

/*
 * IOC interface GUID.  The GUID is defined outside the conditional include
 * on purpose so that it can be instantiated only once where it is actually
 * needed.  See the DDK docs section "Using GUIDs in Drivers" for more info.
 */
/* {20883ACD-57F3-4da3-AD03-73D0178ACAE9} */
DEFINE_GUID(GUID_IOC_INTERFACE_DATA, 
0x20883acd, 0x57f3, 0x4da3, 0xad, 0x3, 0x73, 0xd0, 0x17, 0x8a, 0xca, 0xe9);

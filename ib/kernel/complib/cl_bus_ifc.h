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
 * $Id: cl_bus_ifc.h 1611 2006-08-20 14:48:55Z sleybo $
 */




#ifndef _CL_BUS_IFC_H_
#define _CL_BUS_IFC_H_


#include <complib/cl_types.h>


/****f* Component Library: Plug and Play/cl_fwd_query_ifc
* NAME
*	cl_fwd_query_ifc
*
* DESCRIPTION
*	Forwards a IRP_MN_QUERY_INTERFACE request to the device stack
*	represented by the input device object.
*
* SYNOPSIS
*/
CL_EXPORT NTSTATUS
cl_fwd_query_ifc(
	IN				DEVICE_OBJECT* const		p_dev_obj,
	IN				IO_STACK_LOCATION* const	p_io_stack );
/*
* PARAMETERS
*	p_dev_obj
*		Pointer to the device object that is the IRP target.
*
*	p_io_stack
*		Pointer to the original IRP's I/O stack location, used to format
*		the forwarded IRP.
*
* RETURN VALUES
*	IRP status value.
*
* NOTES
*	The IRP forwarded is synchronous, so this call must be invoked at PASSIVE.
*
* SEE ALSO
*	Plug and Play
*********/

#endif	/* _CL_BUS_IFC_H_ */
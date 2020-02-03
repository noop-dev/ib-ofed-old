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
 * $Id: cl_byteswap_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */



#ifndef _CL_BYTESWAP_OSD_H_
#define _CL_BYTESWAP_OSD_H_


#include "complib/cl_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_LE		1
#define CPU_BE		0

#define cl_ntoh16	_byteswap_ushort
#define cl_hton16	_byteswap_ushort

#define cl_ntoh32	_byteswap_ulong
#define cl_hton32	_byteswap_ulong

#define cl_ntoh64	_byteswap_uint64
#define cl_hton64	_byteswap_uint64

#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// _CL_BYTESWAP_OSD_H_






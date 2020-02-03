/*
 * Copyright (c) 2008 Intel Corporation.  All rights reserved.
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
 */

#pragma once

#ifndef _IB_RDMA_CM_H_
#define _IB_RDMA_CM_H_

#include <ntddk.h>

#define IB_CMA_VERSION 0x00

#define IB_CMA_SERVICE_ID       0x0000000001000000
#define IB_CMA_SERVICE_ID_MASK  0xFFFFFFFFFF000000
#define IB_CMA_CM_RESPONSE_TIMEOUT				20
#define IB_CMA_MAX_CM_RETRIES					15

typedef union _IB_CMA_IP_ADDRESS
{
	UINT8				Ip6Address[16];
	struct {
		UINT32			Pad[3];
		UINT32			Address;

	}	Ip4;

}	IB_CMA_IP_ADDRESS;

typedef struct _IB_CMA_HEADER
{
	UINT8				CmaVersion;
	UINT8				IpVersion;		// IP Version: 7:4
	UINT16				Port;
	IB_CMA_IP_ADDRESS	SrcAddress;
	IB_CMA_IP_ADDRESS	DstAddress;

}	IB_CMA_HEADER;

#endif // _IB_RDMA_CM_H_
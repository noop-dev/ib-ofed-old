/*
 * Copyright (c) 1996-2008 Intel Corporation. All rights reserved.
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
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AWV
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#ifndef _VERBS_H_
#define _VERBS_H_

#include <initguid.h>
#include <iba/ib_ci.h>

static inline USHORT VerbsVersion(UINT8 Major, UINT8 Minor)
{
	return ((USHORT) Major << 8) | ((USHORT) Minor);
}

static inline UINT8 VerbsVersionMajor(USHORT Version)
{
	return (UINT8) (Version >> 8);
}

static inline UINT8 VerbsVersionMinor(USHORT Version)
{
	return (UINT8) Version;
}

DEFINE_GUID(GUID_RDMA_INTERFACE_VERBS, 0xf0ebae86, 0xedb5, 0x4b40,
			0xa1, 0xa, 0x44, 0xd5, 0xdb, 0x3b, 0x96, 0x4e);

typedef struct _RDMA_INTERFACE_VERBS
{
	INTERFACE		InterfaceHeader;
	ci_interface_t	Verbs;

}	RDMA_INTERFACE_VERBS;

#endif // _VERBS_H_

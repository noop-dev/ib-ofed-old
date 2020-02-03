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

#ifndef _WINMAD_H_
#define _WINMAD_H_

#include <initguid.h>

#include <unknwn.h>

typedef UINT64 NET64;
typedef UINT32 NET32;
typedef UINT16 NET16;

typedef struct _WM_REGISTER
{
	NET64			Guid;
	NET32			Qpn;
	UINT8			Port;
	UINT8			Class;
	UINT8			Version;
	UINT8			Reserved[6];
	UINT8			Oui[3];
	UINT8			Methods[16];

}	WM_REGISTER;

typedef struct _WM_MAD_AV
{
	NET32			Qpn;
	NET32			Qkey;
	NET32			VersionClassFlow;	
	UINT16			PkeyIndex;
	UINT8			HopLimit;
	UINT8			GidIndex;
	UINT8			Gid[16];

	UINT16			Reserved;
	NET16			Lid;
	UINT8			ServiceLevel;
	UINT8			PathBits;
	UINT8			StaticRate;
	UINT8			GrhValid;

}	WM_MAD_AV;

#define WM_SUCCESS 0
#define WM_TIMEOUT 138

#pragma warning(push)
#pragma warning(disable: 4200)
typedef struct _WM_MAD
{
	UINT64			Id;
	WM_MAD_AV		Address;

	UINT32			Status;
	UINT32			Timeout;
	UINT32			Retries;
	UINT32			Length;

	UINT8			Data[0];

}	WM_MAD;
#pragma warning(pop)


#undef INTERFACE
#define INTERFACE IWMProvider
// {66094220-A6C0-4206-B620-1D766DB2DE63}
DEFINE_GUID(IID_IWMProvider, 0x66094220, 0xa6c0, 0x4206,
			0xb6, 0x20, 0x1d, 0x76, 0x6d, 0xb2, 0xde, 0x63);

DECLARE_INTERFACE_(IWMProvider, IUnknown)
{
	// IUnknown methods
	__override STDMETHOD(QueryInterface)(
		THIS_
		REFIID riid,
		LPVOID FAR* ppvObj
		) PURE;

	__override STDMETHOD_(ULONG,AddRef)(
		THIS
		) PURE;

	__override STDMETHOD_(ULONG,Release)(
		THIS
		) PURE;

	// IWVProvider methods
	STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	STDMETHOD_(HANDLE,GetFileHandle)(
		THIS
		) PURE;

	STDMETHOD(Register)(
		THIS_
		__in WM_REGISTER* pAttributes,
		__deref_out UINT64* pId
		) PURE;

	STDMETHOD(Deregister)(
		THIS_
		__in UINT64 Id
		) PURE;

	STDMETHOD(Send)(
		THIS_
		__in WM_MAD* pMad,
		__inout_opt OVERLAPPED *pOverlapped
		) PURE;

	STDMETHOD(Receive)(
		THIS_
		__inout_bcount_part(*pBufferSize, *pBufferSize) WM_MAD* pMad,
		__in SIZE_T pBufferSize,
		__inout_opt OVERLAPPED *pOverlapped
		) PURE;
};

__declspec(dllexport) HRESULT WmGetObject(REFIID riid, LPVOID FAR* ppvObj);

#endif // _WINMAD_H_

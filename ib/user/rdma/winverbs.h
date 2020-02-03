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

#ifndef _WINVERBS_H_
#define _WINVERBS_H_

#include <initguid.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <unknwn.h>
#include <rdma\wvstatus.h>

typedef UINT16 NET16;
typedef UINT32 NET32;
typedef UINT64 NET64;

typedef union _WV_GID					// Network byte order
{
	UINT8			Raw[16];

}	WV_GID;

// Device/port capability flags
#define WV_DEVICE_RESIZE_MAX_WR			0x00000001
#define WV_DEVICE_BAD_PKEY_COUNTER		0x00000002
#define WV_DEVICE_BAD_QKEY_COUNTER		0x00000004
// reserved								0x00000008
#define WV_DEVICE_PATH_MIGRATION		0x00000010
#define WV_DEVICE_CHANGE_PHYSICAL_PORT	0x00000020
#define WV_DEVICE_AH_PORT_CHECKING		0x00000040
#define WV_DEVICE_QP_STATE_MODIFIER		0x00000080
#define WV_DEVICE_SHUTDOWN_PORT			0x00000100
#define WV_DEVICE_INIT_TYPE				0x00000200
#define WV_DEVICE_PORT_ACTIVE_EVENT		0x00000400
#define WV_DEVICE_SYSTEM_IMAGE_GUID		0x00000800
#define WV_DEVICE_RC_RNR_NAK_GENERATION	0x00001000
#define WV_DEVICE_SRQ_RESIZE			0x00002000
#define WV_DEVICE_BATCH_NOTIFY_CQ		0x00004000

typedef enum _WV_DEVICE_TYPE
{
	WvDeviceUnknown,
	WvDeviceInfiniband,
	WvDeviceIwarp

}	WV_DEVICE_TYPE;

typedef enum _WV_ATOMIC_CAPABILITIES
{
	WvAtomicNone,
	WvAtomicDevice,
	WvAtomicNode

}	WV_ATOMIC_CAPABILITIES;

typedef struct _WV_DEVICE_ATTRIBUTES
{
	UINT64					FwVersion;
	NET64					NodeGuid;
	NET64					SystemImageGuid;
	UINT32					VendorId;
	UINT32					VendorPartId;
	UINT32					HwVersion;
	DWORD					CapabilityFlags;
	WV_ATOMIC_CAPABILITIES	AtomicCapability;
	DWORD					PageSizeCapabilityFlags;
	SIZE_T					MaxMrSize;
	SIZE_T					MaxQp;
	SIZE_T					MaxQpWr;
	SIZE_T					MaxSge;
	SIZE_T					MaxCq;
	SIZE_T					MaxCqEntries;
	SIZE_T					MaxMr;
	SIZE_T					MaxPd;
	SIZE_T					MaxQpResponderResources;
	SIZE_T					MaxResponderResources;
	SIZE_T					MaxQpInitiatorDepth;
	SIZE_T					MaxInlineSend;
	SIZE_T					MaxMw;
	SIZE_T					MaxMulticast;
	SIZE_T					MaxQpAttach;
	SIZE_T					MaxMulticastQp;
	SIZE_T					MaxAh;
	SIZE_T					MaxFmr;
	SIZE_T					MaxMapPerFmr;
	SIZE_T					MaxSrq;
	SIZE_T					MaxSrqWr;
	SIZE_T					MaxSrqSge;
	SIZE_T					MaxPkeys;
	WV_DEVICE_TYPE			DeviceType;
	UINT8					LocalAckDelay;
	UINT8					PhysPortCount;

}	WV_DEVICE_ATTRIBUTES;

typedef enum _WV_PORT_STATE
{
	WvPortNop,
	WvPortDown,
	WvPortInit,
	WvPortArmed,
	WvPortActive,
	WvPortActiveDefer

}	WV_PORT_STATE;

typedef struct _WV_PORT_ATTRIBUTES
{
	DWORD			PortCabilityFlags;
	WV_PORT_STATE	State;
	UINT32			MaxMtu;
	UINT32			ActiveMtu;
	UINT32			GidTableLength;
	UINT32			MaxMessageSize;
	UINT32			BadPkeyCounter;
	UINT32			QkeyViolationCounter;
	UINT16			PkeyTableLength;
	NET16			Lid;
	NET16			SmLid;
	UINT8			Lmc;
	UINT8			MaxVls;
	UINT8			SmSl;
	UINT8			SubnetTimeout;
	UINT8			InitTypeReply;
	UINT8			ActiveWidth;
	UINT8			ActiveSpeed;
	UINT8			PhysicalState;

}	WV_PORT_ATTRIBUTES;

typedef struct _WV_DEVICE_ADDRESS
{
	NET64				DeviceGuid;
	NET16				Pkey;
	UINT8				PortNumber;

}	WV_DEVICE_ADDRESS;

// Port notification flags
#define WV_EVENT_ERROR				0x00000001
#define WV_EVENT_STATE				0x00000002
#define WV_EVENT_ADDRESS			0x00000004
#define WV_EVENT_LINK_ADDRESS		0x00000008
#define WV_EVENT_PARTITION			0x00000010
#define WV_EVENT_MANAGEMENT			0x00000020

typedef enum _WV_CQ_NOTIFY_TYPE
{
	WvCqError,
	WvCqNextCompletion,
	WvCqSolicited

}	WV_CQ_NOTIFY_TYPE;

typedef enum _WV_OPCODE
{
	WvSend,
	WvRdmaWrite,
	WvRdmaRead,
	WvCompareExchange,
	WvFetchAdd,
	WvBindWindow,
	WvReceive = (1 << 7),
	WvReceiveRdmaWrite

}	WV_OPCODE;

// Completion flags
#define WV_WC_IMMEDIATE		0x00000001
// reserved					0x00000002
#define	WV_WC_GRH_VALID		0x00000004

// Defined to allow casts to WinOF / OFED WCs
typedef struct _WV_COMPLETION
{
	VOID*			QpContext;
	UINT64			WrId;
	WV_OPCODE		Opcode;
	UINT32			Length;
	UINT64			VendorCode;
	HRESULT			Status;

	// Receive completion data
	DWORD			Flags;
	NET32			ImmediateData;
	NET32			SourceQpn;
	UINT16			PkeyIndex;
	NET16			SLid;
	UINT8			SL;
	UINT8			DLidPathBits;

}	WV_COMPLETION;

typedef struct _WV_MEMORY_KEYS
{
	UINT32			Lkey;
	NET32			Rkey;

}	WV_MEMORY_KEYS;

// Access flags
#define WV_ACCESS_REMOTE_READ			0x00000001
#define WV_ACCESS_REMOTE_WRITE			0x0000000A
#define WV_ACCESS_REMOTE_ATOMIC			0x00000004
#define WV_ACCESS_LOCAL_WRITE			0x00000008
#define WV_ACCESS_MW_BIND				0x00000010

// Send queue operation flags
#define WV_SEND_IMMEDIATE				0x00000001
#define WV_SEND_FENCE					0x00000002
#define WV_SEND_SIGNALED				0x00000004
#define WV_SEND_SOLICITED				0x00000008
#define WV_SEND_INLINE					0x00000010

// Defined to allow casts to WinOF / OFED SGEs
#if defined(_WIN64)
typedef struct _WV_SGE
{
	VOID*			pAddress;
	UINT32			Length;
	UINT32			Lkey;

}	WV_SGE;
#else
// 32-bit: little endian support only
typedef struct _WV_SGE
{
	VOID*			pAddress;
	UINT32			Reserved;
	UINT32			Length;
	UINT32			Lkey;

}	WV_SGE;
#endif

typedef struct _WV_SEND_DATAGRAM
{
	UINT64			WrId;
	ULONG_PTR		AhKey;

	WV_SGE*			pSgl;
	SIZE_T			nSge;
	DWORD			Flags;
	NET32			ImmediateData;
	NET32			DestinationQpn;
	NET32			DestinationQkey;

}	WV_SEND_DATAGRAM;

// Defined to allow casts to WinOF / OFED WRs
typedef struct _WV_SEND_REQUEST
{
	UINT64					WrId;
	struct _WV_SEND_REQUEST	*pNext;
	WV_SGE					*pSgl;
	UINT32					nSge;
	WV_OPCODE				Opcode;
	DWORD					Flags;
	NET32					ImmediateData;

	union
	{
		struct
		{
			NET64			RemoteAddress;
			NET32			Rkey;

		}	Rdma;

		struct
		{
			NET64			RemoteAddress;
			NET32			Rkey;
			NET64			Compare;
			NET64			Exchange;

		}	CompareExchange;

		struct
		{
			NET64			RemoteAddress;
			NET32			Rkey;
			NET64			Add;
			UINT64			Reserved;

		}	FetchAdd;

		struct
		{
			ULONG_PTR		AhKey;
			NET32			DestinationQpn;
			NET32			DestinationQkey;

		}	Datagram;
	}	Wr;

}	WV_SEND_REQUEST;

typedef struct _WV_NETWORK_ROUTE
{
	UINT8				Valid;
	UINT8				Reserved;
	UINT8				HopLimit;
	UINT8				TrafficClass;
	NET32				FlowLabel;
	WV_GID				SGid;
	WV_GID				DGid;

}	WV_NETWORK_ROUTE;

typedef struct _WV_ADDRESS_VECTOR
{
	WV_NETWORK_ROUTE	Route;

	UINT16				Reserved;
	NET16				DLid;
	UINT8				ServiceLevel;
	UINT8				SourcePathBits;
	UINT8				StaticRate;
	UINT8				PortNumber;

}	WV_ADDRESS_VECTOR;

typedef enum _WV_QP_TYPE
{
	WvQpTypeRc,
	WvQpTypeUc,
	WvQpTypeReserved,
	WvQpTypeUd

}	WV_QP_TYPE;

typedef enum _WV_QP_STATE
{
	WvQpStateReset,
	WvQpStateInit,
	WvQpStateRtr,
	WvQpStateRts,
	WvQpStateSqd,
	WvQpStateSqError,
	WvQpStateError

}	WV_QP_STATE;

typedef enum _WV_APM_STATE
{
	WvApmMigrated = 1,
	WvApmRearm,
	WvApmArmed

}	WV_APM_STATE;

#define WV_QP_SIGNAL_SENDS				0x00000001
#define WV_QP_MEMORY_MANAGEMENT			0x00000002

typedef struct _WV_QP_CREATE
{
	interface IWVCompletionQueue*		pSendCq;
	interface IWVCompletionQueue*		pReceiveCq;
	interface IWVSharedReceiveQueue*	pSharedReceiveQueue;
	VOID*			Context;

	SIZE_T			SendDepth;
	SIZE_T			SendSge;
	SIZE_T			ReceiveDepth;
	SIZE_T			ReceiveSge;
	SIZE_T			MaxInlineSend;
	SIZE_T			InitiatorDepth;
	SIZE_T			ResponderResources;

	WV_QP_TYPE		QpType;
	DWORD			QpFlags;

}	WV_QP_CREATE;

// QP attribute mask
#define WV_QP_ATTR_CAPABILITIES			0x00000001
#define WV_QP_ATTR_INITIATOR_DEPTH		0x00000002
#define WV_QP_ATTR_RESPONDER_RESOURCES	0x00000004
#define WV_QP_ATTR_CURRENT_STATE		0x00000008
#define WV_QP_ATTR_STATE				0x00000010
#define WV_QP_ATTR_PATH_MIG_STATE		0x00000020
#define WV_QP_ATTR_DESTINATION_QPN		0x00000040
#define WV_QP_ATTR_QKEY					0x00000080
#define WV_QP_ATTR_SEND_PSN				0x00000100
#define WV_QP_ATTR_RECEIVE_PSN			0x00000200
#define WV_QP_ATTR_FLAGS				0x00000400
#define WV_QP_ATTR_ACCESS_FLAGS			0x00000800
#define WV_QP_ATTR_AV					0x00001000
#define WV_QP_ATTR_ALTERNATE_AV			0x00002000
#define WV_QP_ATTR_PORT_NUMBER			0x00004000
#define WV_QP_ATTR_PKEY_INDEX			0x00008000
#define WV_QP_ATTR_ACK_TIMEOUT			0x00010000
#define WV_QP_ATTR_RNR_NAK_TIMEOUT		0x00020000
#define WV_QP_ATTR_ERROR_RETRY_COUNT	0x00040000
#define WV_QP_ATTR_RNR_RETRY_COUNT		0x00080000

typedef struct _WV_QP_ATTRIBUTES
{
	interface IWVProtectionDomain*		pPd;
	interface IWVCompletionQueue*		pSendCq;
	interface IWVCompletionQueue*		pReceiveCq;
	interface IWVSharedReceiveQueue*	pSharedReceiveQueue;

	SIZE_T				SendDepth;
	SIZE_T				SendSge;
	SIZE_T				ReceiveDepth;
	SIZE_T				ReceiveSge;
	SIZE_T				MaxInlineSend;
	SIZE_T				InitiatorDepth;
	SIZE_T				ResponderResources;

	WV_QP_TYPE			QpType;
	WV_QP_STATE			CurrentQpState;
	WV_QP_STATE			QpState;
	WV_APM_STATE		ApmState;
	NET32				Qpn;
	NET32				DestinationQpn;
	NET32				Qkey;
	NET32				SendPsn;
	NET32				ReceivePsn;

	DWORD				QpFlags;
	DWORD				AccessFlags;

	WV_ADDRESS_VECTOR	AddressVector;
	WV_ADDRESS_VECTOR	AlternateAddressVector;
	UINT32				PathMtu;
	UINT32				AlternatePathMtu;
	UINT16				PkeyIndex;
	UINT16				AlternatePkeyIndex;
	UINT8				LocalAckTimeout;
	UINT8				AlternateLocalAckTimeout;

	UINT8				RnrNakTimeout;
	UINT8				SequenceErrorRetryCount;
	UINT8				RnrRetryCount;

}	WV_QP_ATTRIBUTES;

#define WV_CM_UDP_QKEY	0x01234567

typedef struct _WV_CONNECT_PARAM
{
	SIZE_T				DataLength;
	SIZE_T				ResponderResources;
	SIZE_T				InitiatorDepth;
	UINT8				RetryCount;			// Ignored when accepting
	UINT8				RnrRetryCount;
	UINT8				Reserved[2];
	UINT8				Data[56];

}	WV_CONNECT_PARAM;

typedef struct _WV_DATAGRAM_PARAM
{
	SIZE_T				DataLength;
	WV_ADDRESS_VECTOR	AddressVector;
	NET32				Qpn;
	NET32				Qkey;
	UINT8				Data[56];

}	WV_DATAGRAM_PARAM;

typedef union _WV_SOCKADDR
{
	SOCKADDR			Sa;
	SOCKADDR_IN			Sin;
	SOCKADDR_IN6		Sin6;

}	WV_SOCKADDR;

typedef struct _WV_CONNECT_ATTRIBUTES
{
	WV_SOCKADDR			LocalAddress;
	WV_SOCKADDR			PeerAddress;
	WV_DEVICE_ADDRESS	Device;
	WV_CONNECT_PARAM	Param;

}	WV_CONNECT_ATTRIBUTES;

typedef struct _WV_DATAGRAM_ATTRIBUTES
{
	WV_SOCKADDR			LocalAddress;
	WV_SOCKADDR			PeerAddress;		// Specify when querying
	WV_DEVICE_ADDRESS	Device;
	WV_DATAGRAM_PARAM	Param;

}	WV_DATAGRAM_ATTRIBUTES;

// Endpoint options
#define WV_EP_OPTION_ROUTE				0x00000001

#undef INTERFACE
#define INTERFACE IWVOverlapped
// {64687592-aa67-4b55-bc4b-e189bdd2fc4f}
DEFINE_GUID(IID_IWVOverlapped, 0x64687592, 0xaa67, 0x4b55,
			0xbc, 0x4b, 0xe1, 0x89, 0xbd, 0xd2, 0xfc, 0x4f);

DECLARE_INTERFACE_(IWVOverlapped, IUnknown)
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

	// IWVOverlapped methods
	STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVCompletionQueue
// {a5f6a8de-18a6-4086-aa93-7b66607a290a}
DEFINE_GUID(IID_IWVCompletionQueue, 0xa5f6a8de, 0x18a6, 0x4086,
			0xaa, 0x93, 0x7b, 0x66, 0x60, 0x7a, 0x29, 0x0a);

DECLARE_INTERFACE_(IWVCompletionQueue, IWVOverlapped)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVCompletionQueue methods
	STDMETHOD(Resize)(
		THIS_
		__inout SIZE_T* pEntries
		) PURE;

	STDMETHOD(Peek)(
		THIS_
		__out SIZE_T* pCompletedEntries
		) PURE;

	STDMETHOD(Notify)(
		THIS_
		__in WV_CQ_NOTIFY_TYPE Type,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(BatchNotify)(
		THIS_
		__in SIZE_T CompletedEntries,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD_(SIZE_T,Poll)(
		THIS_
		__inout_ecount(Entries) WV_COMPLETION Completions[],
		__in SIZE_T Entries
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVMemoryWindow
// {e8ae206f-b31f-4709-8dc9-6e87625f93fc}
DEFINE_GUID(IID_IWVMemoryWindow, 0xe8ae206f, 0xb31f, 0x4709,
			0x8d, 0xc9, 0x6e, 0x87, 0x62, 0x5f, 0x93, 0xfc);

DECLARE_INTERFACE_(IWVMemoryWindow, IUnknown)
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
};


#undef INTERFACE
#define INTERFACE IWVAddressHandle
// {95903fde-fdac-4007-92b7-e01286cd36e8}
DEFINE_GUID(IID_IWVAddressHandle, 0x95903fde, 0xfdac, 0x4007,
			0x92, 0xb7, 0xe0, 0x12, 0x86, 0xcd, 0x36, 0xe8);

DECLARE_INTERFACE_(IWVAddressHandle, IUnknown)
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
};


#undef INTERFACE
#define INTERFACE IWVSharedReceiveQueue
// {b2bf30e4-8c2e-4659-ac7a-4c5e0d2c7114}
DEFINE_GUID(IID_IWVSharedReceiveQueue, 0xb2bf30e4, 0x8c2e, 0x4659,
			0xac, 0x7a, 0x4c, 0x5e, 0x0d, 0x2c, 0x71, 0x14);

DECLARE_INTERFACE_(IWVSharedReceiveQueue, IWVOverlapped)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVSharedReceiveQueue methods
	STDMETHOD(Query)(
		THIS_
		__out SIZE_T* pMaxWr,
		__out SIZE_T* pMaxSge,
		__out SIZE_T* pSrqLimit
		) PURE;

	STDMETHOD(Modify)(
		THIS_
		__in SIZE_T MaxWr,
		__in SIZE_T SrqLimit
		) PURE;

	STDMETHOD(PostReceive)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge
		) PURE;

	/* Signaled on limit reached event */
	STDMETHOD(Notify)(
		THIS_
		__in_opt OVERLAPPED* pOverlapped
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVQueuePair
// {35C64226-6A1F-4c8d-9465-C6FEE8053CDD}
DEFINE_GUID(IID_IWVQueuePair, 0x35c64226, 0x6a1f, 0x4c8d,
			0x94, 0x65, 0xc6, 0xfe, 0xe8, 0x5, 0x3c, 0xdd);

DECLARE_INTERFACE_(IWVQueuePair, IWVOverlapped)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVQueuePair methods
	STDMETHOD(Query)(
		THIS_
		__out WV_QP_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(Modify)(
		THIS_
		__in WV_QP_ATTRIBUTES* pAttributes,
		__in DWORD Options,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(PostReceive)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge
		) PURE;

	STDMETHOD(PostSend)(
		THIS_
		__in WV_SEND_REQUEST *pSend,
		__out_opt WV_SEND_REQUEST **ppFailed
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVConnectQueuePair
// {DF905570-AEEB-4114-B30E-4DC3EB5A9AD6}
DEFINE_GUID(IID_IWVConnectQueuePair, 0xdf905570, 0xaeeb, 0x4114,
			0xb3, 0xe, 0x4d, 0xc3, 0xeb, 0x5a, 0x9a, 0xd6);

DECLARE_INTERFACE_(IWVConnectQueuePair, IWVQueuePair)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVQueuePair methods
	STDMETHOD(Query)(
		THIS_
		__out WV_QP_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(Modify)(
		THIS_
		__in WV_QP_ATTRIBUTES* pAttributes,
		__in DWORD Options,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(PostReceive)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge
		) PURE;

	STDMETHOD(PostSend)(
		THIS_
		__in WV_SEND_REQUEST *pSend,
		__out_opt WV_SEND_REQUEST **ppFailed
		) PURE;

	// IWVConnectQueuePair methods
	STDMETHOD(Send)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge,
		__in DWORD Flags,
		__in NET32 ImmediateData
		) PURE;

	STDMETHOD(Read)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge,
		__in DWORD Flags,
		__in NET64 Address,
		__in NET32 Rkey
		) PURE;

	STDMETHOD(Write)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge,
		__in DWORD Flags,
		__in NET32 ImmediateData,
		__in NET64 Address,
		__in NET32 Rkey
		) PURE;

	STDMETHOD(CompareExchange)(
		THIS_
		__in UINT64 WrId,
		__in WV_SGE* pSge,
		__in DWORD Flags,
		__in NET64 Compare,
		__in NET64 Exchange,
		__in NET64 Address,
		__in NET32 Rkey
		) PURE;

	STDMETHOD(FetchAdd)(
		THIS_
		__in UINT64 WrId,
		__in WV_SGE* pSge,
		__in DWORD Flags,
		__in NET64 Value,
		__in NET64 Address,
		__in NET32 Rkey
		) PURE;

	STDMETHOD(BindMemoryWindow)(
		THIS_
		__in IWVMemoryWindow* pMw,
		__in UINT64 WrId,
		__in UINT32 Lkey,
		__in DWORD AccessFlags,
		__in DWORD SendFlags,
		__in_bcount(BufferLength) const VOID* pBuffer,
		__in SIZE_T BufferLength,
		__out NET32 *pRkey
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVDatagramQueuePair
// {3BA2F15E-D961-4df8-8FC2-6043A6D67826}
DEFINE_GUID(IID_IWVDatagramQueuePair, 0x3ba2f15e, 0xd961, 0x4df8,
			0x8f, 0xc2, 0x60, 0x43, 0xa6, 0xd6, 0x78, 0x26);

DECLARE_INTERFACE_(IWVDatagramQueuePair, IWVQueuePair)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVQueuePair methods
	STDMETHOD(Query)(
		THIS_
		__out WV_QP_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(Modify)(
		THIS_
		__in WV_QP_ATTRIBUTES* pAttributes,
		__in DWORD Options,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(PostReceive)(
		THIS_
		__in UINT64 WrId,
		__in_ecount(nSge) WV_SGE* pSgl,
		__in SIZE_T nSge
		) PURE;

	STDMETHOD(PostSend)(
		THIS_
		__in WV_SEND_REQUEST *pSend,
		__out_opt WV_SEND_REQUEST **ppFailed
		) PURE;

	// IWVDatagramQueuePair Methods
	STDMETHOD(Send)(
		THIS_
		__in UINT64 WrId,
		__in ULONG_PTR AhKey,
		__in WV_SGE* pSge,
		__in DWORD Flags,
		__in NET32 DestinationQpn,
		__in NET32 DestinationQkey
		) PURE;

	STDMETHOD(SendMessage)(
		THIS_
		__in WV_SEND_DATAGRAM* pSend
		) PURE;

	STDMETHOD(AttachMulticast)(
		THIS_
		__in WV_GID *pGid,
		__in NET16 Lid,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(DetachMulticast)(
		THIS_
		__in WV_GID *pGid,
		__in NET16 Lid,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVProtectionDomain
// {E3657ABE-8C4E-4994-BE32-45B49BAE95E4}
DEFINE_GUID(IID_IWVProtectionDomain, 0xe3657abe, 0x8c4e, 0x4994,
			0xbe, 0x32, 0x45, 0xb4, 0x9b, 0xae, 0x95, 0xe4);

DECLARE_INTERFACE_(IWVProtectionDomain, IWVOverlapped)
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

	// IWVOverlapped methods
	STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVProtectionDomain methods
	STDMETHOD(CreateSharedReceiveQueue)(
		THIS_
		__in SIZE_T MaxWr,
		__in SIZE_T MaxSge,
		__in SIZE_T SrqLimit,
		__deref_out IWVSharedReceiveQueue** ppSrq
		) PURE;

	STDMETHOD(CreateConnectQueuePair)(
		THIS_
		__in WV_QP_CREATE* pAttributes,
		__deref_out IWVConnectQueuePair** ppQp
		) PURE;

	STDMETHOD(CreateDatagramQueuePair)(
		THIS_
		__in WV_QP_CREATE* pAttributes,
		__deref_out IWVDatagramQueuePair** ppQp
		) PURE;

	STDMETHOD(RegisterMemory)(
		THIS_
		__in_bcount(BufferLength) const VOID* pBuffer,
		__in SIZE_T BufferLength,
		__in DWORD AccessFlags,
		__in_opt OVERLAPPED* pOverlapped,
		__out WV_MEMORY_KEYS* pKeys
		) PURE;

	STDMETHOD(DeregisterMemory)(
		THIS_
		__in UINT32 Lkey,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(AllocateMemoryWindow)(
		THIS_
		__deref_out IWVMemoryWindow** ppMw
		) PURE;

	STDMETHOD(CreateAddressHandle)(
		THIS_
		__in WV_ADDRESS_VECTOR* pAddress,
		__deref_out IWVAddressHandle** ppAh,
		__out ULONG_PTR* pAhKey
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVEndpoint
// {eb2fbd8e-b9b6-4b24-9a5e-94c26ae265f0}
DEFINE_GUID(IID_IWVEndpoint, 0xeb2fbd8e, 0xb9b6, 0x4b24,
			0x9a, 0x5e, 0x94, 0xc2, 0x6a, 0xe2, 0x65, 0xf0);

DECLARE_INTERFACE_(IWVEndpoint, IWVOverlapped)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVEndpoint methods
	STDMETHOD(Modify)(
		THIS_
		__in DWORD Option,
		__in_bcount_opt(OptionLength) const VOID* pOptionData,
		__in SIZE_T OptionLength
		) PURE;

	STDMETHOD(BindAddress)(
		THIS_
		__in SOCKADDR* pAddress
		) PURE;

	STDMETHOD(Listen)(
		THIS_
		__in SIZE_T Backlog
		) PURE;

	STDMETHOD(Reject)(
		THIS_
		__in_bcount_opt(UserDataLength) const VOID* pUserData,
		__in SIZE_T UserDataLength
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVConnectEndpoint
// {8FDA0EE7-E7F7-48cc-83D3-17E9C871FEA3}
DEFINE_GUID(IID_IWVConnectEndpoint, 0x8fda0ee7, 0xe7f7, 0x48cc,
			0x83, 0xd3, 0x17, 0xe9, 0xc8, 0x71, 0xfe, 0xa3);

DECLARE_INTERFACE_(IWVConnectEndpoint, IWVEndpoint)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVEndpoint methods
	__override STDMETHOD(Modify)(
		THIS_
		__in DWORD Option,
		__in_bcount_opt(OptionLength) const VOID* pOptionData,
		__in SIZE_T OptionLength
		) PURE;

	__override STDMETHOD(BindAddress)(
		THIS_
		__in SOCKADDR* pAddress
		) PURE;

	__override STDMETHOD(Listen)(
		THIS_
		__in SIZE_T Backlog
		) PURE;

	__override STDMETHOD(Reject)(
		THIS_
		__in_bcount_opt(UserDataLength) const VOID* pUserData,
		__in SIZE_T UserDataLength
		) PURE;

	// IWVConnectEndpoint methods
	STDMETHOD(GetRequest)(
		THIS_
		__in IWVConnectEndpoint* pEndpoint,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Connect)(
		THIS_
		__in IWVConnectQueuePair* pQp,
		__in const SOCKADDR* pAddress,
		__in WV_CONNECT_PARAM* pParam,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Accept)(
		THIS_
		__in_opt IWVConnectQueuePair* pQp,
		__in WV_CONNECT_PARAM* pParam,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Disconnect)(
		THIS_
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(NotifyDisconnect)(
		THIS_
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Query)(
		THIS_
		__out WV_CONNECT_ATTRIBUTES* pAttributes
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVDatagramEndpoint
// {1d879de6-f2af-4a8a-8893-52e0ab868130}
DEFINE_GUID(IID_IWVDatagramEndpoint, 0x1d879de6, 0xf2af, 0x4a8a,
			0x88, 0x93, 0x52, 0xe0, 0xab, 0x86, 0x81, 0x30);

DECLARE_INTERFACE_(IWVDatagramEndpoint, IWVEndpoint)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVEndpoint methods
	__override STDMETHOD(Modify)(
		THIS_
		__in DWORD Option,
		__in_bcount_opt(OptionLength) const VOID* pOptionData,
		__in SIZE_T OptionLength
		) PURE;

	__override STDMETHOD(BindAddress)(
		THIS_
		__in SOCKADDR* pAddress
		) PURE;

	__override STDMETHOD(Listen)(
		THIS_
		__in SIZE_T Backlog
		) PURE;

	__override STDMETHOD(Reject)(
		THIS_
		__in_bcount_opt(UserDataLength) const VOID* pUserData,
		__in SIZE_T UserDataLength
		) PURE;

	// IWVDatagramEndpoint methods
	STDMETHOD(GetRequest)(
		THIS_
		__in IWVDatagramEndpoint* pEndpoint,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Lookup)(
		THIS_
		__in const SOCKADDR* pAddress,
		__in_bcount_opt(UserDataLength) const VOID* pUserData,
		__in SIZE_T UserDataLength,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Accept)(
		THIS_
		__in WV_DATAGRAM_PARAM* pParam,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(JoinMulticast)(
		THIS_
		__in const SOCKADDR* pAddress,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(LeaveMulticast)(
		THIS_
		__in const SOCKADDR* pAddress,
		__in_opt OVERLAPPED* pOverlapped
		) PURE;

	STDMETHOD(Query)(
		THIS_
		__inout WV_DATAGRAM_ATTRIBUTES* pAttributes
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVDevice
// {244af78c-b1ac-40e4-9896-271d58d591b8}
DEFINE_GUID(IID_IWVDevice, 0x244af78c, 0xb1ac, 0x40e4,
			0x98, 0x96, 0x27, 0x1d, 0x58, 0xd5, 0x91, 0xb8);

DECLARE_INTERFACE_(IWVDevice, IWVOverlapped)
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

	// IWVOverlapped methods
	__override STDMETHOD(CancelOverlappedRequests)(
		THIS
		) PURE;

	__override STDMETHOD(GetOverlappedResult)(
		THIS_
		__inout_opt OVERLAPPED *pOverlapped,
		__out DWORD *pNumberOfBytesTransferred,
		__in BOOL bWait
		) PURE;

	// IWVDevice methods
	STDMETHOD(Query)(
		THIS_
		__out WV_DEVICE_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(QueryPort)(
		THIS_
		__in UINT8 PortNumber,
		__out WV_PORT_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(QueryGid)(
		THIS_
		__in UINT8 PortNumber,
		__in DWORD Index,
		__out WV_GID* pGid
		) PURE;

	STDMETHOD(FindGid)(
		THIS_
		__in UINT8 PortNumber,
		__in WV_GID* pGid,
		__out DWORD* pIndex
		) PURE;

	STDMETHOD(QueryPkey)(
		THIS_
		__in UINT8 PortNumber,
		__in UINT16 Index,
		__out NET16* pPkey
		) PURE;

	STDMETHOD(FindPkey)(
		THIS_
		__in UINT8 PortNumber,
		__in NET16 Pkey,
		__out UINT16* pIndex
		) PURE;

	STDMETHOD(CreateCompletionQueue)(
		THIS_
		__inout SIZE_T *pEntries,
		__deref_out IWVCompletionQueue** ppCq
		) PURE;

	STDMETHOD(AllocateProtectionDomain)(
		THIS_
		__deref_out IWVProtectionDomain** ppPd
		) PURE;

	STDMETHOD(Notify)(
		THIS_
		__in UINT8 PortNumber,
		__in_opt OVERLAPPED* pOverlapped,
		__out DWORD* pFlags
		) PURE;
};


#undef INTERFACE
#define INTERFACE IWVProvider
// {75AD0EDA-23C9-4687-8FFF-D1B34E1EF5BE}
DEFINE_GUID(IID_IWVProvider, 0x75ad0eda, 0x23c9, 0x4687,
			0x8f, 0xff, 0xd1, 0xb3, 0x4e, 0x1e, 0xf5, 0xbe);

DECLARE_INTERFACE_(IWVProvider, IUnknown)
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
	STDMETHOD_(HANDLE,GetFileHandle)(
		THIS
		) PURE;

	STDMETHOD(QueryDeviceList)(
		THIS_
		__inout_bcount_part_opt(*pBufferSize, *pBufferSize) NET64* pGuidList,
		__inout SIZE_T* pBufferSize
		) PURE;

	STDMETHOD(QueryDevice)(
		THIS_
		__in NET64 Guid,
		__out WV_DEVICE_ATTRIBUTES* pAttributes
		) PURE;

	STDMETHOD(TranslateAddress)(
		THIS_
		__in const SOCKADDR* pAddress,
		__out WV_DEVICE_ADDRESS* pDeviceAddress
		) PURE;

	STDMETHOD(OpenDevice)(
		THIS_
		__in NET64 Guid,
		__deref_out IWVDevice** ppDevice
		) PURE;

	STDMETHOD(CreateConnectEndpoint)(
		THIS_
		__deref_out IWVConnectEndpoint** ppConnectEndpoint
		) PURE;

	STDMETHOD(CreateDatagramEndpoint)(
		THIS_
		__deref_out IWVDatagramEndpoint** ppDatagramEndpoint
		) PURE;
};

#ifdef __cplusplus
extern "C" __declspec(dllexport) HRESULT WvGetObject(REFIID riid, LPVOID FAR* ppvObj);
#else
__declspec(dllexport) HRESULT WvGetObject(REFIID riid, LPVOID FAR* ppvObj);
#endif

#endif // _WINVERBS_H_

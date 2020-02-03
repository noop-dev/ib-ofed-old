/*
 * Copyright (c) 2008 Intel Corporation. All rights reserved. 
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

#ifndef _WVSTATUS_H_
#define _WVSTATUS_H_

//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_WINVERBS                0x38


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// Values map to NTSTATUS and NDSTATUS where possible
//
#define WV_SUCCESS						((HRESULT)0x00000000L)
#define WV_TIMEOUT						((HRESULT)0x00000102L)
#define WV_PENDING						((HRESULT)0x00000103L)
#define WV_BUFFER_OVERFLOW				((HRESULT)0x80000005L)
#define WV_DEVICE_BUSY					((HRESULT)0x80000011L)
#define WV_NO_MORE_ENTRIES				((HRESULT)0x8000001AL)
#define WV_IO_PENDING					((HRESULT)0x800703E5L)
#define WV_REJECTED						((HRESULT)0x80070047L)
#define WV_UNSUCCESSFUL					((HRESULT)0xC0000001L)
#define WV_ACCESS_VIOLATION				((HRESULT)0xC0000005L)
#define WV_INVALID_HANDLE				((HRESULT)0xC0000008L)
#define WV_INVALID_PARAMETER			((HRESULT)0xC000000DL)
#define WV_NO_MEMORY					((HRESULT)0xC0000017L)
#define WV_INVALID_PARAMETER_MIX		((HRESULT)0xC0000030L)
#define WV_DATA_OVERRUN					((HRESULT)0xC000003CL)
#define WV_INSUFFICIENT_RESOURCES		((HRESULT)0xC000009AL)
#define WV_NOT_INITIALIZED				((HRESULT)0xC00000A3L)
#define WV_IO_TIMEOUT					((HRESULT)0xC00000B5L)
#define WV_NOT_SUPPORTED				((HRESULT)0xC00000BBL)
#define WV_INTERNAL_ERROR				((HRESULT)0xC00000E5L)
#define WV_INVALID_PARAMETER_1			((HRESULT)0xC00000EFL)
#define WV_INVALID_PARAMETER_2			((HRESULT)0xC00000F0L)
#define WV_INVALID_PARAMETER_3			((HRESULT)0xC00000F1L)
#define WV_INVALID_PARAMETER_4			((HRESULT)0xC00000F2L)
#define WV_INVALID_PARAMETER_5			((HRESULT)0xC00000F3L)
#define WV_INVALID_PARAMETER_6			((HRESULT)0xC00000F4L)
#define WV_INVALID_PARAMETER_7			((HRESULT)0xC00000F5L)
#define WV_INVALID_PARAMETER_8			((HRESULT)0xC00000F6L)
#define WV_INVALID_PARAMETER_9			((HRESULT)0xC00000F7L)
#define WV_INVALID_PARAMETER_10			((HRESULT)0xC00000F8L)
#define WV_CANCELLED					((HRESULT)0xC0000120L)
#define WV_REMOTE_ERROR					((HRESULT)0xC000013DL)
#define WV_INVALID_ADDRESS				((HRESULT)0xC0000141L)
#define WV_ADDRESS_ALREADY_EXISTS		((HRESULT)0xC000020AL)
#define WV_CONNECTION_REFUSED			((HRESULT)0xC0000236L)
#define WV_CONNECTION_INVALID			((HRESULT)0xC000023AL)
#define WV_CONNECTION_ACTIVE			((HRESULT)0xC000023BL)
#define WV_HOST_UNREACHABLE				((HRESULT)0xC000023DL)
#define WV_CONNECTION_ABORTED			((HRESULT)0xC0000241L)
#define WV_DEVICE_REMOVED				((HRESULT)0xC00002B6L)
#define WV_LOCAL_LENGTH					((HRESULT)0xC0380001L)
#define WV_INVALIDATION_ERROR			((HRESULT)0xC0380002L)
#define WV_LOCAL_OP_ERROR				((HRESULT)0xC0380003L)
#define WV_LOCAL_PROTECTION_ERROR		((HRESULT)0xC0380004L)
#define WV_WR_FLUSHED_ERROR				((HRESULT)0xC0380005L)
#define WV_BIND_WINDOW_ERROR			((HRESULT)0xC0380006L)
#define WV_REMOTE_ACCESS_ERROR			((HRESULT)0xC0380007L)
#define WV_REMOTE_OP_ERROR				((HRESULT)0xC0380008L)
#define WV_RNR_RETRY_ERROR				((HRESULT)0xC0380009L)
#define WV_TIMEOUT_RETRY_ERROR			((HRESULT)0xC038000AL)
#define WV_REMOTE_INVALID_REQUEST_ERROR	((HRESULT)0xC038000BL)
#define WV_BAD_RESPONSE_ERROR			((HRESULT)0xC038000CL)
#define WV_LOCAL_ACCESS_ERROR			((HRESULT)0xC038000DL)
#define WV_UNKNOWN_ERROR				((HRESULT)0xC038000EL)

#endif // _WVSTATUS_H_

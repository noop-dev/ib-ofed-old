/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Portions Copyright (c) 2008 Microsoft Corporation.  All rights reserved.
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
 * $Id: cl_types_osd.h 2821 2008-07-20 17:47:07Z doront $
 */




#ifndef _CL_TYPES_OSD_H_
#define _CL_TYPES_OSD_H_


#ifdef __cplusplus
extern "C"
{
#endif


#pragma warning( push )
#include <wdmwarn4.h>
#if defined( NDIS_MINIPORT_DRIVER )
#include <ndis.h>
#if NDIS_WDM
#define CL_NTDDK
#endif /* NDIS_WDM */
#elif !defined( _MINIPORT_ )
#include <ntddk.h>
#define CL_NTDDK
#endif	/* defined( NDIS_MINIPORT_DRIVER ) */
#pragma warning( pop )
/*
 * Disable warnings related to taking the address of a dllimport function.
 * This is needed to allow user of the PNP/PO abstraction to use the default
 * handlers.
 */
#pragma warning( disable:4232 )

/*
 * Enable warnings about pointer sign extension.
 */
#pragma warning( default:4826 )

/* For DECLSPEC_EXPORT and DECLSPEC_IMPORT */
#include <ntdef.h>

#define CL_KERNEL

#if DBG
	#define _DEBUG_
#else
	#undef _DEBUG_
#endif

typedef __int8					int8_t;
typedef unsigned __int8			uint8_t;
typedef __int16					int16_t;
typedef unsigned __int16		uint16_t;
typedef __int32					int32_t;
typedef unsigned __int32		uint32_t;
typedef __int64					int64_t;
typedef unsigned __int64		uint64_t;
/* boolean_t must be intergral sized to avoid a bunch of warnings. */
typedef int						boolean_t;
typedef unsigned char			uchar_t;
typedef _W64 __int3264			intn_t;
typedef _W64 unsigned __int3264	uintn_t;
typedef volatile __int32		atomic32_t;

#ifdef _PREFAST_
#define CONDITION_ASSUMED(X) __analysis_assume((X))
#else
#define CONDITION_ASSUMED(X) 
#endif // _PREFAST_


#ifndef CL_ASSERT
#ifdef _DEBUG_
#define CL_ASSERT(X) ASSERT(X);CONDITION_ASSUMED(X)
#else
#define CL_ASSERT	ASSERT
#endif
#endif


#define UNUSED_PARAM	UNREFERENCED_PARAMETER
#if defined(EVENT_TRACING)
#define UNUSED_PARAM_WOWPP(a)
#else
#define UNUSED_PARAM_WOWPP(a)UNREFERENCED_PARAMETER(a)
#endif


#define CL_EXPORT

#if !defined( __cplusplus )
#define inline			__inline
#endif

#define CL_INLINE		CL_EXPORT inline

#define CL_API

#define cl_panic		DbgBreakPoint

#ifndef offsetof
#define offsetof		FIELD_OFFSET
#endif

#define PARENT_STRUCT( P, T, M )	CONTAINING_RECORD( (void*)P, T, M )

typedef enum _cl_status	cl_status_t;

#define CL_CONST64( x )	x##ui64

NTSTATUS
cl_to_ntstatus(
	IN	enum _cl_status	status );

DECLSPEC_DEPRECATED enum _cl_status
cl_from_ntstatus(
	IN	NTSTATUS status );

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif // _CL_TYPES_OSD_H_

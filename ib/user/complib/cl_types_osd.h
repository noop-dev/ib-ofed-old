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


#if !defined( WINVER )
	// Enable Windows 2000 features.
	#define _WIN32_WINNT	0x0500
	#define WINVER			0x0500
#else // !defined( WINVER )
	#if(WINVER < 0x0500)
		// Force a compile error.
		#error The component library requires Windows 2000 features. \
			WINVER and _WIN32_WINNT must be set to 0x0500 or greater.
	#elif !defined(_WIN32_WINNT)
		#define _WIN32_WINNT		WINVER
	#endif
#endif // !defined( WINVER )

#if _WIN32_WINNT != WINVER && _WIN32_WINNT != 0x0500 && WINVER != 0x501
	#error _WIN32_WINNT does not match WINVER.
#endif // _WIN32_WINNT != WINVER

#if !defined( _WINDOWS_ )
	// Include the windows header file.
	#include <windows.h>
#endif // !defined( _WINDOWS_ )

/*
 * Enable warnings about pointer sign extension.
 */
#pragma warning( default:4826 )

#if defined( _DEBUG ) || DBG
	#define _DEBUG_
#else
	#undef _DEBUG_
#endif

#ifdef __GNUC__

#include <stdint.h>
typedef intptr_t				intn_t;
typedef uintptr_t				uintn_t; 
typedef volatile uint32_t		atomic32_t;

#else /* __GNUC__ */

#ifndef _STDINT
typedef __int8					int8_t;
typedef unsigned __int8			uint8_t;
typedef __int16					int16_t;
typedef unsigned __int16		uint16_t;
typedef __int32					int32_t;
typedef unsigned __int32		uint32_t;
typedef __int64					int64_t;
typedef unsigned __int64		uint64_t;
typedef unsigned char			uchar_t;
typedef _W64 __int3264			intn_t;
typedef _W64 unsigned __int3264	uintn_t;
typedef volatile __int32		atomic32_t;
#endif // _STDINT

#endif /* __GNUC__ */

/* boolean_t must be intergral sized to avoid a bunch of warnings. */
typedef int						boolean_t;

#ifdef _PREFAST_
#define CONDITION_ASSUMED(X) __analysis_assume((X))
#else
#define CONDITION_ASSUMED(X) 
#endif // _PREFAST_

#ifndef CL_ASSERT
#ifdef _DEBUG_
#define CL_ASSERT( exp )	(void)(!(exp)?OutputDebugString("Assertion Failed:" #exp "\n"),DebugBreak(),FALSE:TRUE);CONDITION_ASSUMED(exp)
#else
#define CL_ASSERT( exp )
#endif	/* _DEBUG_ */
#endif	/* CL_ASSERT */


#define UNUSED_PARAM		UNREFERENCED_PARAMETER
#if defined(EVENT_TRACING)
#define UNUSED_PARAM_WOWPP(a)
#else
#define UNUSED_PARAM_WOWPP(a)UNREFERENCED_PARAMETER(a)
#endif

#if !defined(EXPORT_CL_SYMBOLS)
#define CL_EXPORT		DECLSPEC_IMPORT
#else
#define CL_EXPORT		__declspec(dllexport)
#endif

#if !defined( __cplusplus )
#define inline	__inline
#endif

#define CL_INLINE		CL_EXPORT inline

#define CL_API			__stdcall

#define cl_panic		abort

#ifndef offsetof
#define offsetof		FIELD_OFFSET
#endif

#define PARENT_STRUCT( P, T, M )	CONTAINING_RECORD( (void*)P, T, M )

#ifdef __GNUC__
#define CL_CONST64( x ) x##LLU
#else
#define CL_CONST64( x )	x##ui64
#endif


#if !defined( __cplusplus )
#define inline	__inline
#endif


#endif // _CL_TYPES_OSD_H_

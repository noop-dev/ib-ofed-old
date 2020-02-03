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
 * $Id: cl_debug_osd.h 4521 2009-06-25 13:42:45Z xalex $
 */




#ifndef _CL_DEBUG_OSD_H_
#define _CL_DEBUG_OSD_H_


#include "complib/cl_types.h"


#if !defined(__MODULE__)
#define __MODULE__			""
#define __MOD_DELIMITER__	""
#else	/* !defined(__MODULE__) */
#define __MOD_DELIMITER__	":"
#endif	/* !defined(__MODULE__) */


#if defined( _WIN64 )
#define PRIdSIZE_T	"I64d"
#else
#define PRIdSIZE_T	"d"
#endif
#define PRId64		"I64d"
#define PRIx64		"I64x"
#define PRIo64		"I64o"
#define PRIu64		"I64u"


#if defined( _DEBUG_ )
#ifdef __cplusplus
extern "C"
{
#endif

VOID cl_dbg_out( IN const char * const  Format, ...);
#ifdef __cplusplus
}
#endif

#else
#define cl_dbg_out	__noop
#endif	/* defined( _DEBUG_ ) */

#define cl_msg_out	DbgPrint


/*
 * The following macros are used internally by the CL_ENTER, CL_TRACE, 
 * CL_TRACE_EXIT, and CL_EXIT macros.
 */
#if defined( _WDMDDK_ )
/* wdm.h does not provide for a way to get the current processor number. */
#define _CL_DBG_ENTER	\
	("%s%s%s() [\n", __MODULE__, __MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_EXIT	\
	("%s%s%s() ]\n", __MODULE__, __MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_INFO	\
	("%s%s%s(): ", __MODULE__, __MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_ERROR	\
	("%s%s%s() !ERROR!: ", __MODULE__, __MOD_DELIMITER__, __FUNCTION__)

#else // !defined( _WDMDDK_ )

#define _CL_DBG_ENTER	\
	("~%d:(%x)%s%s%s() [\n", KeGetCurrentProcessorNumber(), PsGetCurrentThread(), __MODULE__, \
	__MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_EXIT	\
	("~%d:%s%s%s() ]\n", KeGetCurrentProcessorNumber(), __MODULE__, \
	__MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_INFO	\
	("~%d:%s%s%s(): ", KeGetCurrentProcessorNumber(), __MODULE__, \
	__MOD_DELIMITER__, __FUNCTION__)

#define _CL_DBG_ERROR	\
	("~%d:%s%s%s() !ERROR!: ", KeGetCurrentProcessorNumber(), __MODULE__, \
	__MOD_DELIMITER__, __FUNCTION__)

#endif	// !defined( _WDMDDK_ )

#define CL_CHK_STK


#endif /* _CL_DEBUG_OSD_H_ */


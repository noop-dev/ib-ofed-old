/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Copyright (c) 1996-2003 Intel Corporation. All rights reserved. 
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
 * $Id: cl_log.h 4403 2009-06-01 15:28:55Z leonid $
 */


/*
 * Abstract:
 *	Declaration of logging mechanisms.
 *
 * Environment:
 *	All
 */


#ifndef _CL_LOG_H_
#define _CL_LOG_H_

#include <complib/cl_types.h>


/****h* Component Library/Log Provider
* NAME
*	Log Provider
*
* DESCRIPTION
*	The log provider allows users to log information in a system log instead of
*	the console or debugger target.
**********/


/****d* Component Library: Log Provider/cl_log_type_t
* NAME
*	cl_log_type_t
*
* DESCRIPTION
*	The cl_log_type_t enumerated type is used to differentiate between
*	different types of log entries.
*
* SYNOPSIS
*/
typedef enum _cl_log_type
{
	CL_LOG_INFO,
	CL_LOG_WARN,
	CL_LOG_ERROR

} cl_log_type_t;
/*
* VALUES
*	CL_LOG_INFO
*		Indicates a log entry is purely informational.
*
*	CL_LOG_WARN
*		Indicates a log entry is a warning but non-fatal.
*
*	CL_LOG_ERROR
*		Indicates a log entry is a fatal error.
*
* SEE ALSO
*	Log Provider, cl_log_event
*********/


#ifdef __cplusplus
extern "C"
{
#endif


/****f* Component Library: Log Provider/cl_log_event
* NAME
*	cl_log_event
*
* DESCRIPTION
*	The cl_log_event function adds a new entry to the system log.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_log_event(
	IN	const char* const	name,
	IN	const cl_log_type_t	type,
	IN	const char* const	message,
	IN	const void* const	p_data OPTIONAL,
	IN	const uint32_t		data_len );
/*
* PARAMETERS
*	name
*		[in] Pointer to an ANSI string containing the name of the source for
*		the log entry.
*
*	type
*		[in] Defines the type of log entry to add to the system log.
*		See the definition of cl_log_type_t for acceptable values.
*
*	message
*		[in] Pointer to an ANSI string containing the text for the log entry.
*		The message should not be terminated with a new line, as the log
*		provider appends a new line to all log entries.
*
*	p_data
*		[in] Optional pointer to data providing context for the log entry.
*		At most 256 bytes of data can be successfully logged.
*
*	data_len
*		[in] Length of the buffer pointed to by the p_data parameter.  Ignored
*		if p_data is NULL.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	If the data length exceeds the maximum supported, the event is logged
*	without its accompanying data.
*
* SEE ALSO
*	Log Provider, cl_log_type_t
*********/


/****f* Component Library: Debug Output/cl_msg_out
* NAME
*	cl_event_log_write
*
* DESCRIPTION
*	The cl_event_log_write function sends a message to System Event Logger.
*
* SYNOPSIS
*/
CL_EXPORT void
cl_event_log_write(
	PVOID	p_io_object,
	ULONG	p_error_code,
	ULONG	p_unique_error_code,
	ULONG	p_final_status,
	PWCHAR	p_insertion_string,
	ULONG	p_n_data_items,
		...
		);
/*
* PARAMETERS
*	p_io_object
*		[in] The IO object ( driver object or device object ).
*	
*	p_error_code
*		[in] The error code.
*
*	p_unique_error_code
*		[in] A specific error code.
*
*	p_final_status
*		[in] The final status.
*
*	p_insertion_string
*		[in] String to print.
*
*	p_n_data_items
*		[in] Number of data items
*
*	...
*		[in] data items values
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	cl_event_log_write is available in both debug and release builds.
*
*	The function uses IoAllocateErrorLogEntry and IoWriteErrorLogEntry
*	functions to send data to System Event Log.
*
* SEE ALSO
*	
*********/

#ifdef __cplusplus
}
#endif

#ifdef CL_KERNEL
#include <ntddk.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#define CL_LOG_BUF_LEN		512
extern WCHAR g_cl_wlog[ CL_LOG_BUF_LEN ]; 
extern UCHAR g_cl_slog[ CL_LOG_BUF_LEN ]; 

static void __build_str( const char *	format, ... )
{
	NTSTATUS status;
	va_list p_arg;
	va_start(p_arg, format);
	status = RtlStringCbVPrintfA((char *)g_cl_slog, sizeof(g_cl_slog), format , p_arg);
	if (status)
		goto end;
	status = RtlStringCchPrintfW(g_cl_wlog, sizeof(g_cl_wlog)/sizeof(g_cl_wlog[0]), L"%S", g_cl_slog);
	if (status)
		goto end;
end:
	va_end(p_arg);
}

#define CL_PRINT_TO_EVENT_LOG(_obj_,_event_id_,_msg_)  \
	if (_obj_) \
	{ \
		NTSTATUS event_id = _event_id_; \
		__build_str _msg_; \
		cl_event_log_write( _obj_, (ULONG)event_id, 0, 0, g_cl_wlog, 0, 0 ); \
	}
#else

#define CL_PRINT_TO_EVENT_LOG(_obj_,_event_id_,_msg_)

#endif

#endif	/* _CL_LOG_H_ */

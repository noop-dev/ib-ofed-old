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
 * $Id: cl_ioctl.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of IOCTL object
 *
 * Environment:
 *	All
 */


#ifndef _CL_IOCTL_H_
#define _CL_IOCTL_H_


#include <complib/cl_types.h>
#include <complib/cl_ioctl_osd.h>


/****h* Component Library/IOCTL Object
* NAME
*	IOCTL Object
*
* DESCRIPTION
*	The IOCTL object provides functionality for handling IOCTL requests.
*
*	The IOCTL object is only available in kernel mode and provides
*	functionality for accessing information about IO requests initiated
*	by a user-mode application.  The IOCTL_CODE macro is used in both
*	user and kernel mode to initiate and dispatch IOCTL requests, respectively.
*
*	In Linux, in order for the IOCTL object to be used, requests must be
*	initiated and handled using the Device Framework abstraction.
*
* SEE ALSO
*	Structures:
*		cl_ioctl_handle_t
*
*	Callbacks:
*		cl_pfn_ioctl_handler_t
*
*	Control Code Generation
*		IOCTL_CODE
*
*	Kernel Mode Access
*		cl_ioctl_process
*		cl_ioctl_complete
*		cl_ioctl_type
*		cl_ioctl_cmd
*		cl_ioctl_ctl_code
*		cl_ioctl_in_buf
*		cl_ioctl_in_size
*		cl_ioctl_out_buf
*		cl_ioctl_out_size
*
*	User Mode Access
*		cl_ioctl_request
*		cl_ioctl_result
*********/


/****d* Component Library: IOCTL Object/IOCTL_CODE
* NAME
*	IOCTL_CODE
*
* DESCRIPTION
*	Macro for defining IO control command codes.
*
* SYNOPSIS
*	uint32_t IOCTL_CODE( uint16_t type, uint16_t cmd )
*
* PARAMETERS
*	type
*		[in] user-defined type representing the type of command.  For Linux,
*		the type is truncated to 8-bits.  For Windows, the type is a 16-bit
*		value, as described in "Specifying Device Types" in the DDK docs.
*
*	cmd
*		[in] User-defined command.  For Linux, the command field is truncated
*		to 8-bits.  For Windows, the command can be 12-bits, with values
*		below 0x800 reserved by Microsoft for system defined commands.
*
* RETURN VALUE
*	A 32-bit control code.  User-mode clients use the control code to initiate
*	requests.  Kernel-mode clients use the control code to distinguish between
*	different requests.
*
* NOTE
*	In Windows, all IOCTL command codes defined with the IOCTL_CODE command
*	result in FILE_ANY_ACCESS and METHOD_BUFFERED being specified.
*
* SEE ALSO
*	IOCTL Object, cl_dev_ioctl, cl_ioctl_type, cl_ioctl_cmd
*********/


#ifdef CL_KERNEL

/****d* Component Library: IOCTL Object/cl_ioctl_handle_t
* NAME
*	cl_ioctl_handle_t
*
* DESCRIPTION
*	Opaque handle representing an IO request.
*
* NOTES
*	The cl_ioctl_handle_t type is only available in the kernel.
*	The cl_ioctl_handle_t type should be treated as opaque, as it
*	varies from environment to environment.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_type, cl_ioctl_cmd, cl_ioctl_in_buf,
*	cl_ioctl_in_size, cl_ioctl_out_buf, cl_ioctl_out_size,
*	cl_ioctl_set_status, cl_ioctl_set_ret_bytes
*********/


/****d* Component Library: IOCTL Object/cl_pfn_ioctl_handler_t
* NAME
*	cl_pfn_ioctl_handler_t
*
* DESCRIPTION
*	The cl_pfn_ioctl_handler_t function type defines the prototype for
*	IOCTL handlers used when handling IOCTL requests initiated by
*	cl_ioctl_request.
*
* SYNOPSIS
*/
typedef cl_status_t
(CL_API *cl_pfn_ioctl_handler_t)(
	IN	cl_ioctl_handle_t	h_ioctl,
	IN	void				*context_1,
	IN	void				*context_2 );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to the IOCTL request.
*
*	context_1
*		[in] First context parameters, as provided to cl_ioctl_process.
*
*	context_2
*		[in] Second context parameters, as provided to cl_ioctl_process.
*
* RETURN VALUES
*	CL_SUCCESS if the IOCTL was completed successfully.
*
*	CL_PENDING if the IOCTL is being processed asynchronously.
*
*	Other return values in case of errors.
*
* NOTES
*	It is acceptable to complete the IOCTL successfully to report an error
*	status in the output buffer.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_process
*********/


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


/****f* Component Library: IOCTL Object/cl_ioctl_process
* NAME
*	cl_ioctl_process
*
* DESCRIPTION
*	The cl_ioctl_process function unpacks information initiated by a call to
*	cl_ioctl_request function and invokes a user-supplied callback.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_ioctl_process(
	IN	void					*p_ioctl,
	IN	cl_pfn_ioctl_handler_t	pfn_ioctl_handler,
	IN	void					*context_1,
	IN	void					*context_2 );
/*
* PARAMETERS
*	p_ioctl
*		[in] Pointer to an OS specific IOCTL information.  In Linux,
*		this parameter depends on whether the IOCTL is handled synchronously
*		or asynchronously.  See the notes for further detail.
*		In Windows, this is a pointer to an IRP.
*
*	pfn_ioctl_handler
*		[in] Pointer to the callback function to invoke for handling the IOCTL.
*		This callback is independent of the IOCTL command.
*
*	context_1
*		[in] First of two context parameters to pass to the handler.
*
*	context_2
*		[in] Second of two context parameters to pass to the handler.
*
* RETURN VALUES
*	CL_SUCCESS if the IOCTL was processed successfully.
*
*	Other values to indicate various failures.
*
* NOTES
*	Users must call cl_ioctl_complete from within the handler if completing
*	the IOCTL request synchronously.  If the IOCTL request's control code is
*	invalid, the handler should return CL_INVALID_REQUEST.
*
*	In Linux, the p_ioctl parameter is a copy of the argp parameter on input,
*	and on output points to the IOCTL request object passed to the IOCTL
*	handler if and only if the IOCTL handler returned CL_PENDING.
*	This allows the user to cancel the request by passing the same
*	handle to the cancel routine that was passed to the IOCTL handler.
*	If all IOCTLs are handled synchronously, it is acceptable to pass the argp
*	parameter of the IOCTL entry point instead of a copy.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_pfn_ioctl_handler_t, cl_ioctl_complete
*********/


/****f* Component Library: IOCTL Object/cl_ioctl_complete
* NAME
*	cl_ioctl_complete
*
* DESCRIPTION
*	Fills in completion information for an IOCTL and releases the IOCTL request
*	for completion.
*
* SYNOPSIS
*/
CL_EXPORT void CL_API
cl_ioctl_complete(
	IN	cl_ioctl_handle_t	h_ioctl,
	IN	cl_status_t			io_status,
	IN	size_t				ret_bytes );
/*
* PARAMETERS
*	h_ioctl
*		Handle to the IOCTL being completed.  This handle was provided to
*		the IOCTL handler.
*
*	io_status
*		Status of the IOCTL request.
*
*	ret_bytes
*		Number of bytes written to the output buffer.
*
* RETURN VALUES
*	This function does not return a value.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_process
*********/


/****f* Component Library: IOCTL Object/cl_ioctl_type
* NAME
*	cl_ioctl_type
*
* DESCRIPTION
*	Returns the type of an IOCTL.
*
* SYNOPSIS
*/
CL_EXPORT uint16_t CL_API
cl_ioctl_type(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the type of the specified IOCTL request, as defined using
*	the IOCTL_CMD macro.
*
* NOTES
*	The cl_ioctl_type function is only available in the kernel.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_cmd, cl_ioctl_ctl_code
********/


/****f* Component Library: IOCTL Object/cl_ioctl_cmd
* NAME
*	cl_ioctl_cmd
*
* DESCRIPTION
*	Returns the command of an IOCTL
*
* SYNOPSIS
*/
CL_EXPORT uint16_t CL_API
cl_ioctl_cmd(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the command of the specified IOCTL request, as defined using
*	the IOCTL_CMD macro.
*
* NOTES
*	The cl_ioctl_cmd function is only available in the kernel.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_type, cl_ioctl_ctl_code
********/


/****f* Component Library: IOCTL Object/cl_ioctl_ctl_code
* NAME
*	cl_ioctl_ctl_code
*
* DESCRIPTION
*	Returns the 32-bit control code of an IOCTL
*
* SYNOPSIS
*/
CL_EXPORT uint32_t CL_API
cl_ioctl_ctl_code(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the 32-bit control code of the specified IOCTL request,
*	as defined using the IOCTL_CMD macro.
*
* NOTES
*	The cl_ioctl_ctl_code function is only available in the kernel.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_type, cl_ioctl_cmd
********/


/****f* Component Library: IOCTL Object/cl_ioctl_in_buf
* NAME
*	cl_ioctl_in_buf
*
* DESCRIPTION
*	Returns a pointer to the input buffer of an IOCTL.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
cl_ioctl_in_buf(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the input buffer of the specified IOCTL request.
*
* NOTES
*	The cl_ioctl_in_buf function is only available in the kernel.
*
*	In Windows, for IOCTL operations defined as METHOD_IN_DIRECT, the
*	returned pointer points to the MDL describing the input buffer.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_in_size,
*	cl_ioctl_out_buf, cl_ioctl_out_size
********/


/****f* Component Library: IOCTL Object/cl_ioctl_in_size
* NAME
*	cl_ioctl_in_size
*
* DESCRIPTION
*	Returns the size of the input buffer of an IOCTL.
*
* SYNOPSIS
*/
CL_EXPORT ULONG CL_API
cl_ioctl_in_size(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the size, in bytes, of the input buffer of the specified
*	IOCTL request.
*
* NOTES
*	The cl_ioctl_in_size function is only available in the kernel.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_in_buf,
*	cl_ioctl_out_buf, cl_ioctl_out_size
********/


/****f* Component Library: IOCTL Object/cl_ioctl_out_buf
* NAME
*	cl_ioctl_out_buf
*
* DESCRIPTION
*	Returns a pointer to the output buffer of an IOCTL.
*
* SYNOPSIS
*/
CL_EXPORT void* CL_API
cl_ioctl_out_buf(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns a pointer to the output buffer of the specified IOCTL request.
*
* NOTES
*	The cl_ioctl_out_buf function is only available in the kernel.
*
*	In Windows, for IOCTL operations defined as METHOD_IN_DIRECT or
*	METHOD_OUT_DIRECT, the returned pointer points to the MDL describing
*	the input buffer.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_out_size,
*	cl_ioctl_in_buf, cl_ioctl_in_size
********/


/****f* Component Library: IOCTL Object/cl_ioctl_out_size
* NAME
*	cl_ioctl_out_size
*
* DESCRIPTION
*	Returns the size of the output buffer of an IOCTL.
*
* SYNOPSIS
*/
CL_EXPORT ULONG CL_API
cl_ioctl_out_size(
	IN	cl_ioctl_handle_t	h_ioctl );
/*
* PARAMETERS
*	h_ioctl
*		[in] Handle to an IOCTL
*
* RETURN VALUE
*	Returns the size, in bytes, of the input buffer of the specified
*	IOCTL request.
*
* NOTES
*	The cl_ioctl_out_size function is only available in the kernel.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_handle_t, cl_ioctl_out_buf,
*	cl_ioctl_in_buf, cl_ioctl_in_size
********/


#ifdef __cplusplus
}
#endif	/* __cplusplus */


#else	/* CL_KERNEL */


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


/****f* Component Library: IOCTL Object/cl_ioctl_request
* NAME
*	cl_ioctl_request
*
* DESCRIPTION
*	The cl_ioctl_request is used by user-mode clients to initiate IOCTL
*	requests to a device.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_ioctl_request(
	IN		void			*h_dev,
	IN		uint32_t		ioctl_code,
	IN		void			*p_in_buf,
	IN		size_t			in_size,
		OUT	void			*p_out_buf,
	IN		size_t			out_size,
		OUT	size_t			*p_ret_bytes OPTIONAL,
	IN		void			*p_async_info OPTIONAL );
/*
* PARAMETERS
*	h_dev
*		[in] Handle to the device to which the IOCTL request is targetted.
*		In Linux, this is a file descriptor.  In Windows, this is a file
*		handle.
*
*	ioctl_code
*		[in] Control code for the IOCTL request.
*
*	p_in_buf
*		[in] Pointer to the input buffer.
*
*	in_size
*		[in] Size, in bytes, of the input buffer.
*
*	p_out_buf
*		[out] Pointer to the output buffer.
*
*	out_size
*		[in] Size, in bytes, of the output buffer.
*
*	p_ret_bytes
*		[out] Number of bytes written to the output buffer.  This parameter is
*		mutually exclusive of the p_async_info parameter.
*
*	p_async_info
*		[in] For platforms that support asynchronous I/O, supplies a pointer
*		to that platform's async I/O structure, if any.  For Windows, this
*		is a pointer to an OVERLAPPED structure.  This parameter is mutually
*		exclusive of the p_ret_bytes parameter.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_result
*********/


/****f* Component Library: IOCTL Object/cl_ioctl_result
* NAME
*	cl_ioctl_result
*
* DESCRIPTION
*	Checks the status of an asynchronous IOCTL request.
*
* SYNOPSIS
*/
CL_EXPORT cl_status_t CL_API
cl_ioctl_result(
	IN	void		*h_dev,
	IN	void		*p_async_info,
	OUT	size_t		*p_ret_bytes,
	IN	boolean_t	blocking );
/*
* PARAMETERS
*	h_dev
*		[in] Handle to the device to which the IOCTL request is targetted.
*		In Linux, this is a file descriptor.  In Windows, this is a file
*		handle.
*
*	p_async_info
*		[in] For platforms that support asynchronous I/O, supplies a pointer
*		to that platform's async I/O structure, if any.  For Windows, this
*		is a pointer to an OVERLAPPED structure.  This must be the same
*		as that provided in the cl_ioctl_request function.
*
*	p_ret_bytes
*		[out] Number of bytes written to the output buffer.
*
*	blocking
*		[in] If TRUE, indicates that the call should wait until the
*		specified IOCTL request is complete.
*
* RETURN VALUES
*	CL_SUCCESS if the IOCTL request was successful.  p_ret_bytes contains
*	the number bytes written to the output buffer.
*
*	CL_PENDING if the IOCTL request is not yet complete.
*
*	Other status values to indicate errors.
*
* SEE ALSO
*	IOCTL Object, cl_ioctl_request
*********/

#ifdef __cplusplus
}
#endif	/* __cplusplus */


#endif	/* CL_KERNEL */

#endif	/* _CL_IOCTL_H_ */

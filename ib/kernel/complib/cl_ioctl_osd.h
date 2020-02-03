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
 * $Id: cl_ioctl_osd.h 5963 2010-06-15 14:40:35Z leonid $
 */


/*
 * Abstract:
 *	Declaration of IOCTL object
 *
 * Environment:
 *	Windows Kernel Mode
 */


#ifndef _CL_IOCTL_OSD_H_
#define _CL_IOCTL_OSD_H_


#include <complib/cl_types.h>


#define IOCTL_CODE( type, cmd )	\
	CTL_CODE( type, (cmd & 0x0FFF), METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef PIRP	cl_ioctl_handle_t;


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


CL_INLINE cl_status_t
cl_ioctl_process(
	IN	void					*p_ioctl,
	IN	cl_status_t				(pfn_ioctl_handler( cl_ioctl_handle_t, void*, void* ) ),
	IN	void					*context_1,
	IN	void					*context_2 )
{
	return pfn_ioctl_handler( ((PIRP)p_ioctl), context_1, context_2 );
}


CL_INLINE uint16_t
cl_ioctl_type(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	return (uint16_t)DEVICE_TYPE_FROM_CTL_CODE(
		p_io_stack->Parameters.DeviceIoControl.IoControlCode );
}


CL_INLINE uint16_t
cl_ioctl_cmd(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	return (uint16_t)
		((p_io_stack->Parameters.DeviceIoControl.IoControlCode >> 2) & 0x0FFF);
}


CL_INLINE uint32_t
cl_ioctl_ctl_code(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	return p_io_stack->Parameters.DeviceIoControl.IoControlCode;
}


CL_INLINE void*
cl_ioctl_in_buf(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	switch( p_io_stack->Parameters.DeviceIoControl.IoControlCode & 0x03 )
	{
	case METHOD_BUFFERED:
	case METHOD_OUT_DIRECT:
		return (h_ioctl)->AssociatedIrp.SystemBuffer;

	case METHOD_IN_DIRECT:
		return (h_ioctl)->MdlAddress;

	case METHOD_NEITHER:
		return p_io_stack->Parameters.DeviceIoControl.Type3InputBuffer;
	}
	return NULL;
}


CL_INLINE ULONG
cl_ioctl_in_size(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	switch( p_io_stack->Parameters.DeviceIoControl.IoControlCode & 0x03 )
	{
	case METHOD_BUFFERED:
	case METHOD_OUT_DIRECT:
	case METHOD_NEITHER:
		return p_io_stack->Parameters.DeviceIoControl.InputBufferLength;

	case METHOD_IN_DIRECT:
		return p_io_stack->Parameters.DeviceIoControl.OutputBufferLength;
	}
	return 0;
}


CL_INLINE void*
cl_ioctl_out_buf(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	switch( p_io_stack->Parameters.DeviceIoControl.IoControlCode & 0x03 )
	{
	case METHOD_BUFFERED:
		return (h_ioctl)->AssociatedIrp.SystemBuffer;

	case METHOD_IN_DIRECT:
	case METHOD_OUT_DIRECT:
		return (h_ioctl)->MdlAddress;

	case METHOD_NEITHER:
		return (h_ioctl)->UserBuffer;
	}
	return NULL;
}


CL_INLINE ULONG
cl_ioctl_out_size(
	IN	cl_ioctl_handle_t	h_ioctl )
{
	IO_STACK_LOCATION	*p_io_stack;

	p_io_stack = IoGetCurrentIrpStackLocation( h_ioctl );
	return p_io_stack->Parameters.DeviceIoControl.OutputBufferLength;
}


CL_INLINE void
cl_ioctl_complete(
	IN	cl_ioctl_handle_t	h_ioctl,
	IN	cl_status_t			io_status,
	IN	size_t				ret_bytes )
{
	h_ioctl->IoStatus.Status = cl_to_ntstatus( io_status );
	h_ioctl->IoStatus.Information = ret_bytes;
	IoCompleteRequest( h_ioctl, IO_NO_INCREMENT );
}


#ifdef __cplusplus
}
#endif	/* __cplusplus */


#endif	// _CL_IOCTL_OSD_H_

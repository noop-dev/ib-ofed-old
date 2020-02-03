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
 * $Id: cl_ioctl_osd.h 1611 2006-08-20 14:48:55Z sleybo $
 */


/*
 * Abstract:
 *	Declaration of IOCTL object
 *
 * Environment:
 *	Windows User Mode
 */


#ifndef _CL_IOCTL_OSD_H_
#define _CL_IOCTL_OSD_H_


#include <complib/cl_types.h>
#pragma warning(push, 3)
#include <winioctl.h>
#pragma warning(pop)


#define IOCTL_CODE( type, cmd )	\
	CTL_CODE( type, (cmd & 0x0FFF), METHOD_BUFFERED, FILE_ANY_ACCESS)


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


CL_INLINE cl_status_t CL_API
cl_ioctl_request(
	IN		void			*h_dev,
	IN		uint32_t		ioctl_code,
	IN		void			*p_in_buf,
	IN		size_t			in_size,
		OUT	void			*p_out_buf,
	IN		size_t			out_size,
		OUT	size_t			*p_ret_bytes OPTIONAL,
	IN		void			*p_async_info OPTIONAL )
{
	DWORD	bytes_ret;

	if( !DeviceIoControl( h_dev, ioctl_code, p_in_buf, (DWORD)in_size,
		p_out_buf, (DWORD)out_size, &bytes_ret, (LPOVERLAPPED)p_async_info ) )
	{
		if( GetLastError() == ERROR_IO_PENDING )
			return CL_PENDING;
		else
			return CL_ERROR;
	}

	*p_ret_bytes = bytes_ret;
	return CL_SUCCESS;
}


CL_INLINE cl_status_t CL_API
cl_ioctl_result(
	IN	void		*h_dev,
	IN	void		*p_async_info,
	OUT	size_t		*p_ret_bytes,
	IN	boolean_t	blocking )
{
	DWORD	bytes_ret;

	if( !GetOverlappedResult(
		h_dev, (LPOVERLAPPED)p_async_info, &bytes_ret, blocking ) )
	{
		if( GetLastError() == ERROR_IO_INCOMPLETE )
			return CL_NOT_DONE;
		else
			return CL_ERROR;
	}

	*p_ret_bytes = bytes_ret;
	return CL_SUCCESS;
}



#ifdef __cplusplus
}
#endif	/* __cplusplus */


#endif	// _CL_IOCTL_OSD_H_

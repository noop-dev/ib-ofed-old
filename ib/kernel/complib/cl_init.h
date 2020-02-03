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
 * $Id: cl_init.h 1611 2006-08-20 14:48:55Z sleybo $
 */


#ifndef _CL_INIT_H_
#define _CL_INIT_H_


#include <complib/cl_memory.h>
#include <complib/cl_obj.h>


#ifdef CL_TRACK_MEM
#ifdef NEED_CL_OBJ
#define CL_INIT		(__cl_mem_track( TRUE ), cl_obj_mgr_create())
#define CL_DEINIT	(cl_obj_mgr_destroy(), __cl_mem_track( FALSE ))
#else	/* NEED_CL_OBJ */
#define CL_INIT		(__cl_mem_track( TRUE ), STATUS_SUCCESS)
#define CL_DEINIT	(__cl_mem_track( FALSE ))
#endif	/* NEED_CL_OBJ */
#else	/* CL_TRACK_MEM */
#ifdef NEED_CL_OBJ
#define CL_INIT		cl_obj_mgr_create()
#define CL_DEINIT	cl_obj_mgr_destroy()
#else	/* NEED_CL_OBJ */
#define CL_INIT		STATUS_SUCCESS
#define CL_DEINIT
#endif	/* NEED_CL_OBJ */
#endif	/* CL_TRACK_MEM */

#endif // _CL_INIT_H_

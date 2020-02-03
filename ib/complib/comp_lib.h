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
 * $Id: comp_lib.h 1791 2006-11-12 14:40:09Z sleybo $
 */


/*
 * Abstract:
 *	One stop shopping for component library headers.
 *
 * Environment:
 *	All
 */


#ifndef _CL_LIB_H_
#define _CL_LIB_H_


/****h* Component Library/Component Library 
* NAME
*	component library 
*
* DESCRIPTION
*	The component library is a collection of components that can be used to
*	create complex projects quickly and reliably.
*
*	The component library simplifies development by eliminating the need to
*	re-implement existing functionality. This contributes to shorter
*	development cycles as well as smaller code bases, helping reduce the
*	number of bugs by leveraging tried and tested code.
*
*	The component library also provides the same interface in multiple
*	environments, such as kernel mode and user mode, allowing code to be used
*	in both, again reducing code duplication and development life cycles.
*
*	Components of the library all follow the same usage model, as follows:
*		- The constructor for all components should be called before any other
*		function for that component.  It is acceptable to call the initializer
*		without first calling the constructor.
*
*		- The initializer for all components must be called successfully
*		before any function manipulating that component is called.
*
*		- The destructor for all components must be called if the initializer
*		was called.
*
*	In a debug build, the components assert that the proper sequence is
*	followed.
*********/


#include <complib/cl_types.h>
#include <complib/cl_debug.h>
#include <complib/cl_memory.h>
#include <complib/cl_atomic.h>
#include <complib/cl_byteswap.h>
#include <complib/cl_math.h>
#include <complib/cl_passivelock.h>
#include <complib/cl_spinlock.h>
#include <complib/cl_timer.h>
#include <complib/cl_event.h>
#include <complib/cl_waitobj.h>
#include <complib/cl_qlist.h>
#include <complib/cl_list.h>
#include <complib/cl_qcomppool.h>
#include <complib/cl_qpool.h>
#include <complib/cl_comppool.h>
#include <complib/cl_pool.h>
#include <complib/cl_reqmgr.h>
#include <complib/cl_vector.h>
#include <complib/cl_syscallback.h>
#include <complib/cl_thread.h>
#include <complib/cl_threadpool.h>
#include <complib/cl_perf.h>
#include <complib/cl_log.h>
#include <complib/cl_qmap.h>
#include <complib/cl_map.h>
#include <complib/cl_fleximap.h>
#include <complib/cl_async_proc.h>
#include <complib/cl_ptr_vector.h>
#include <complib/cl_qlockpool.h>
#include <complib/cl_mutex.h>
#include <complib/cl_obj.h>
#include <complib/cl_ioctl.h>


#endif	/* _CL_LIB_H_ */

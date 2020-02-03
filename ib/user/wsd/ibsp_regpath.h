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
 * $Id: ibsp_regpath.h 1611 2006-08-20 14:48:55Z sleybo $
 */

#ifndef _IBSP_REGPATH_H_
#define _IBSP_REGPATH_H_

/* these definitions are common for installSP and WSD projects */
#define IBSP_PM_REGISTRY_PATH	\
	TEXT("SYSTEM\\CurrentControlSet\\Services\\IBWSD\\")
#define IBSP_PM_EVENTLOG_PATH	\
	TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\IBWSD")
#define IBSP_PM_SUBKEY_NAME		TEXT("IBWSD")
#define IBSP_PM_SUBKEY_PERF		TEXT("Performance")
#define IBSP_PM_INI_FILE		"ibsp_perfcounters.ini"
#define IBSP_PM_SYM_H_FILE		"ibsp_perfini.h"


enum IBSP_PM_COUNTERS
{
	BYTES_SEND = 0,
	BYTES_RECV,
	BYTES_WRITE,
	BYTES_READ,
	BYTES_TOTAL,
	COMP_SEND,
	COMP_RECV,
	COMP_TOTAL,
	INTR_TOTAL,
	IBSP_PM_NUM_COUNTERS

};


/* counter symbol names */
#define IBSP_PM_OBJ						0
#define IBSP_PM_COUNTER( X )			((X + 1) * 2)

#endif /* _IBSP_REGPATH_H_ */

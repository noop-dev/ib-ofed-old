/*
 * Copyright (c) 2009 Intel Corporation. All rights reserved.
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
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AWV
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include <ntddk.h>

// Allow overlaying across IRP.Tail.Overlay.DriverContext
typedef struct _WORK_ENTRY
{
	LIST_ENTRY			Entry;
	void				(*WorkHandler)(struct _WORK_ENTRY *Work);
	void				*Context;

}	WORK_ENTRY;

static void WorkEntryInit(WORK_ENTRY *pWork,
						  void (*WorkHandler)(struct _WORK_ENTRY *Work), void *Context)
{
	pWork->WorkHandler = WorkHandler;
	pWork->Context = Context;
}

#define WorkEntryFromIrp(pIrp) ((WORK_ENTRY *) (pIrp)->Tail.Overlay.DriverContext)

struct _WORK_QUEUE_TASK;

typedef struct _WORK_QUEUE
{
	LIST_ENTRY					List;
	KSPIN_LOCK					Lock;
	int							TaskCount;
	struct _WORK_QUEUE_TASK		*TaskArray;	// TaskArray[0] is for internal use

}	WORK_QUEUE;

NTSTATUS WorkQueueInit(WORK_QUEUE *pWorkQueue, PDEVICE_OBJECT Device,
					   int TaskCount);
void WorkQueueDestroy(WORK_QUEUE *pWorkQueue);
void WorkQueueInsert(WORK_QUEUE *pWorkQueue, WORK_ENTRY *pWork);

#endif // _WORK_QUEUE_H_

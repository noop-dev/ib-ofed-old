/*
 * Copyright (c) 2009 Intel Corp., Inc.  All rights reserved.
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

#ifndef COMP_CHANNEL_H
#define COMP_CHANNEL_H

#include <windows.h>
#include <dlist.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _COMP_ENTRY
{
	struct _COMP_ENTRY		*Next;
	OVERLAPPED				Overlap;
	struct _COMP_CHANNEL	*Channel;
	LONG volatile			Busy;

}	COMP_ENTRY;

typedef struct _COMP_CHANNEL
{
	struct _COMP_MANAGER	*Manager;
	struct _COMP_CHANNEL	*Next;
	struct _COMP_SET		*Set;
	COMP_ENTRY				*Head;
	COMP_ENTRY				**TailPtr;
	COMP_ENTRY				Entry;
	HANDLE					Event;
	CRITICAL_SECTION		Lock;
	DWORD					Milliseconds;

}	COMP_CHANNEL;

typedef struct _COMP_SET
{
	COMP_CHANNEL			*Head;
	COMP_CHANNEL			**TailPtr;
	HANDLE					Event;

}	COMP_SET;

typedef struct _COMP_MANAGER
{
	HANDLE					CompQueue;
	HANDLE					Thread;
	BOOL					Run;

}	COMP_MANAGER;

DWORD		CompManagerOpen(COMP_MANAGER *pMgr);
void		CompManagerClose(COMP_MANAGER *pMgr);
DWORD		CompManagerMonitor(COMP_MANAGER *pMgr, HANDLE hFile, ULONG_PTR Key);

DWORD		CompSetInit(COMP_SET *pSet);
void		CompSetCleanup(COMP_SET *pSet);
void		CompSetZero(COMP_SET *pSet);
void		CompSetAdd(COMP_CHANNEL *pChannel, COMP_SET *pSet);
DWORD		CompSetPoll(COMP_SET *pSet, DWORD Milliseconds);
void		CompSetCancel(COMP_SET *pSet);

DWORD		CompChannelInit(COMP_MANAGER *pMgr, COMP_CHANNEL *pChannel,
							DWORD Milliseconds);
void		CompChannelCleanup(COMP_CHANNEL *pChannel);
DWORD		CompChannelPoll(COMP_CHANNEL *pChannel, COMP_ENTRY **ppEntry);
void		CompChannelCancel(COMP_CHANNEL *pChannel);

void		CompEntryInit(COMP_CHANNEL *pChannel, COMP_ENTRY *pEntry);
DWORD		CompEntryPost(COMP_ENTRY *pEntry);
COMP_ENTRY	*CompEntryCancel(COMP_ENTRY *pEntry);

#ifdef __cplusplus
}
#endif

#endif /* COMP_CHANNEL_H */

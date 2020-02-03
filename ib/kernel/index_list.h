/*
 * Copyright (c) 2008 Intel Corporation. All rights reserved.
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

#ifndef _INDEX_LIST_H_
#define _INDEX_LIST_H_

#include <ntddk.h>

typedef struct _INDEX_ENTRY
{
	SIZE_T				Next;
	SIZE_T				Prev;
	void				*pItem;

}	INDEX_ENTRY;

extern INDEX_ENTRY EmptyList;

// Synchronization must be provided by the caller.
typedef struct _INDEX_LIST
{
	INDEX_ENTRY			*pArray;  // pArray[0] is list head of used entries
	SIZE_T				FreeList; // singly-linked list of free INDEX_ENTRY's
	SIZE_T				Size;

}	INDEX_LIST;

static void IndexListInit(INDEX_LIST *pIndexList)
{
	pIndexList->pArray = &EmptyList;
	pIndexList->FreeList = 0;
	pIndexList->Size = 0;
}

static void IndexListDestroy(INDEX_LIST *pIndexList)
{
	if (pIndexList->pArray != &EmptyList) {
		ExFreePool(pIndexList->pArray);
	}
}

SIZE_T IndexListInsertHead(INDEX_LIST *pIndexList, void *pItem);

static void *IndexListAt(INDEX_LIST *pIndexList, SIZE_T Index)
{
	return (Index < pIndexList->Size) ? pIndexList->pArray[Index].pItem : NULL;
}

void *IndexListRemove(INDEX_LIST *pIndexList, SIZE_T Index);

static void *IndexListRemoveHead(INDEX_LIST *pIndexList)
{
	return IndexListRemove(pIndexList, pIndexList->pArray[0].Next);
}

#define IndexListForEach(pIndexList, Index)						\
	for (Index = (pIndexList)->pArray[0].Next; Index != 0;	\
		 Index = (pIndexList)->pArray[Index].Next)

#endif // _INDEX_LIST_H_

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
 * $Id: oib_ver.h 3873 2009-02-01 12:14:25Z reuven $
 */


#include <windows.h>
#include <ntverp.h>

#if		(VER_FILEBUILD < 10)
#define VER_FILEBPAD "000"
#elif	(VER_FILEBUILD < 100)
#define VER_FILEBPAD "00"
#elif	(VER_FILEBUILD < 1000)
#define VER_FILEBPAD "0"
#else
#define VER_FILEBPAD
#endif

#define VER_FILEVERSION			VER_FILEMAJORVERSION,\
								VER_FILEMINORVERSION,\
								VER_FILEBUILD,\
								VER_FILEREV

#define VER_FILEVERSION_STR2(M,m,b,r)	#M "." #m "." VER_FILEBPAD #b "." #r
#define VER_FILEVERSION_STR1(M,m,b,r)	VER_FILEVERSION_STR2(M,m,b,r)
#define VER_FILEVERSION_STR		VER_FILEVERSION_STR1( VER_FILEMAJORVERSION, \
													VER_FILEMINORVERSION, \
													VER_FILEBUILD, \
													VER_FILEREV )

#undef __BUILDMACHINE__

#ifdef VER_COMPANYNAME_STR
#undef VER_COMPANYNAME_STR
#endif
#define VER_COMPANYNAME_STR		IB_COMPANYNAME

#ifdef VER_PRODUCTNAME_STR
#undef VER_PRODUCTNAME_STR
#endif
#define VER_PRODUCTNAME_STR		IB_PRODUCTNAME

#define VER_LEGALCOPYRIGHT_STR	"Copyright\xa9 2009 Mellanox Technologies Ltd"

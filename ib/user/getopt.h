/*
 * Copyright (c) 2005 Mellanox Technologies.  All rights reserved.
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
 */

#ifndef _GETOPT_H_
#define _GETOPT_H_

#ifdef __cplusplus
extern "C"
{
#endif

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

struct option
{
	const char	*name;
	int			has_arg;
	int			*flag;
	int			val;
};

enum
{
	no_argument,
	required_argument,
	optional_argument
};

extern int getopt(int argc, char * const argv[], char const *opts);
extern int getopt_long(int argc, char * const argv[], char const *opts,
					   const struct option *longopts, int *longindex);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif

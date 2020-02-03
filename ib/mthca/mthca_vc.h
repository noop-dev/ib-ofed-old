/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Copyright (c) 2004-2005 Mellanox Technologies, Inc. All rights reserved. 
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
 * $Id: mthca_vc.h 4066 2009-03-04 00:29:36Z leonid $
 */

#ifndef MTHCA_VC_H
#define MTHCA_VC_H

#define PTR_ALIGN(size)	(((size) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))

typedef
struct _map_crspace {
	unsigned __int64	va;		/* address of CRSPACE, mapped to user space */
	unsigned long		size;	/* size of CRSPACE, mapped to user space */
	unsigned long		reserved;	/* to align on quadword boundary */
} map_crspace;

/* Definitions for hca_driver commands*/
#define FW_READ			0x00
#define FW_WRITE		0x01
#define FW_READ_CMD		0x08
#define FW_WRITE_CMD	        0x09
#define FW_MAP_CRSPACE	        0x0A
#define FW_UNMAP_CRSPACE	0x0B
#define FW_OPEN_IF		0xe7
#define FW_CLOSE_IF		0x7e

struct msix_info {
	int	valid;
	int	enabled;
	int	masked;
	int requested;
	int granted;
	unsigned granted_mask;
	unsigned pending_mask;
} ;

/* uplink info */
typedef struct {
	uint8_t	bus_type;	/* 1 - PCI, 2 - PCI-X, 3 - PCI_E */
#define UPLINK_BUS_PCI		1	
#define UPLINK_BUS_PCIX		2	
#define UPLINK_BUS_PCIE		3	
	union {
		struct {
			uint8_t	capabilities;
#define UPLINK_BUS_PCIX_133	2	/* 133 MHz capable */
			uint16_t	frequency;	/* in MHz */
		} pci_x;
		struct {
			uint8_t	capabilities;
			uint8_t	link_speed;		/* 1X link speed */
#define UPLINK_BUS_PCIE_SDR	1	/* 2.5 Gbps */
#define UPLINK_BUS_PCIE_DDR	2	/* 5 Gbps */
			uint8_t	link_width;		/* x1, x2, x4, x8, x12, x16, x32 */
		} pci_e;
	} u;
	struct msix_info x;
} uplink_info_t;

/* Defines for get data for vendor specific */
#define MTHCA_BRD_ID_LEN  64

inline char* mthca_get_board_id(ib_ca_attr_t *ca_attr)
{
	return (char*)(ca_attr) + (ca_attr->size - PTR_ALIGN(MTHCA_BRD_ID_LEN) - PTR_ALIGN(sizeof(uplink_info_t)));
}

inline void* mthca_get_uplink_info(ib_ca_attr_t *ca_attr)
{
	return (char*)(ca_attr) + (ca_attr->size - PTR_ALIGN(sizeof(uplink_info_t)));
}

#endif

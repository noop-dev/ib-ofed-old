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
 * $Id: ip_packet.h 2880 2010-08-31 17:21:43Z stansmith $
 */


#ifndef _IP_PACKET_H_
#define _IP_PACKET_H_


#include <complib/cl_types.h>
#include <complib/cl_byteswap.h>


#ifndef HW_ADDR_LEN
#define HW_ADDR_LEN		6
#endif	/* HW_ADDR_LEN */


#define ETH_PROT_TYPE_IP	CL_HTON16(0x800)
#define ETH_PROT_TYPE_IPV6	CL_HTON16(0x86DD)
#define ETH_PROT_TYPE_ARP	CL_HTON16(0x806)
#define ETH_PROT_TYPE_RARP	CL_HTON16(0x8035)
#define ETH_PROT_VLAN_TAG	CL_HTON16(0x8100)

#define IPV6_ADDR_LENGTH	16

#define ETH_IS_LOCALLY_ADMINISTERED(addr) \
	(BOOLEAN)(((PUCHAR)(addr))[0] & ((UCHAR)0x02))

#define IP_BROADCAST_ADDRESS (CL_HTON32(0xffffffff))

#include <complib/cl_packon.h>
/****s* IB Network Drivers/mac_addr_t
* NAME
*	mac_addr_t
*
* DESCRIPTION
*	Defines the ehternet MAC address.
*
* SYNOPSIS
*/
typedef struct _mac_addr
{
	uint8_t		addr[HW_ADDR_LEN];

}	PACK_SUFFIX mac_addr_t;
/*
* FIELDS
*	addr
*		Byte array representing the MAC address.
*
* NOTES
*	The HW_ADDR_LEN value must be defined before including this header in order
*	to support various address lengths.  If not defined, the default length for
*	ethernet (6 bytes) is used.
*
*	addr[0] & 0x1 indicates multicast
*	addr[0] & 0x2 indicates LAA if not multicast
*
* SEE ALSO
*	IB Network Drivers, arp_pkt_t, ip_hdr_t, tcp_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#include <complib/cl_packon.h>
/****s* IB Network Drivers/eth_hdr_t
* NAME
*	eth_hdr_t
*
* DESCRIPTION
*	Defines the ethernet header for IP packets.
*
* SYNOPSIS
*/
typedef struct _eth_hdr
{
	mac_addr_t	dst;
	mac_addr_t	src;
	net16_t		type;

}	PACK_SUFFIX eth_hdr_t;
/*
* FIELDS
*	dst
*		Destination address
*
*	src
*		Source address
*
*	type
*		Ethernet protocol type
*
* NOTES
*	The HW_ADDR_LEN value must be defined before including this header in order
*	to support various address lengths.
*
* SEE ALSO
*	IB Network Drivers, arp_pkt_t, ip_hdr_t, tcp_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#define ARP_HW_TYPE_ETH		CL_HTON16(1)
#define ARP_HW_TYPE_IB		CL_HTON16(32)

#define ARP_OP_REQ			CL_HTON16(1)
#define ARP_OP_REP			CL_HTON16(2)


#include <complib/cl_packon.h>
/****s* IB Network Drivers/arp_pkt_t
* NAME
*	arp_pkt_t
*
* DESCRIPTION
*	Defines the ARP packet for IP packets.
*
* SYNOPSIS
*/
typedef struct _arp_pkt
{
	net16_t		hw_type;
	net16_t		prot_type;
	uint8_t		hw_size;
	uint8_t		prot_size;
	net16_t		op;
	mac_addr_t	src_hw;
	net32_t		src_ip;
	mac_addr_t	dst_hw;
	net32_t		dst_ip;

}	PACK_SUFFIX arp_pkt_t;
/*
* FIELDS
*	hw_type
*		Hardware type
*
*	prot_type
*		Protocol type.  See ETH_PROT_TYPE_XXX definitions.
*
*	hw_size
*		Size of the hardware address
*
*	prot_size
*		Size of the protocol address
*
*	op
*		ARP operation
*
*	src_hw
*		Source HW (MAC) address
*
*	src_ip
*		Source IP address
*
*	dst_hw
*		Destination HW (MAC) address
*
*	dst_ip
*		Destination IP address
*
* NOTES
*	The HW_ADDR_LEN value must be defined before including this header in order
*	to support various MAC address lengths.
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, ip_hdr_t, tcp_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#define IP_PROT_IP			4
#define IP_PROT_TCP			6
#define IP_PROT_UDP			17
#define IP_PROT_IGMP		2
#define IP_PROT_ICMP		1
#define IP_PROT_ICMPV6		58


#include <complib/cl_packon.h>
/****s* IB Network Drivers/ip_hdr_t
* NAME
*	ip_hdr_t
*
* DESCRIPTION
*	Defines the IP header for IP packets.
*
* SYNOPSIS
*/
typedef struct _ip_hdr
{
	uint8_t		ver_hl;
	uint8_t		svc_type;
	net16_t		length;
	net16_t		id;
	net16_t		offset;
	uint8_t		ttl;
	uint8_t		prot;
	net16_t		chksum;
	net32_t		src_ip;
	net32_t		dst_ip;

}	PACK_SUFFIX ip_hdr_t;
/*
* FIELDS
*	ver_hl
*		Header version and length.
*
*	svc_type
*		Service type.
*
*	length
*		Total length.
*
*	id
*		Packet identification.
*
*	offset
*		Fragment offset.
*
*	ttl
*		Time to live.
*
*	prot
*		Protocol.
*
*	chksum
*		Checksum.
*
*	src_ip
*		Source IP address
*
*	dst_ip
*		Destination IP address
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, tcp_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>

#include <complib/cl_packon.h>
/****s* IB Network Drivers/ipv6_hdr_t
* NAME
*	ipv6_hdr_t
*
* DESCRIPTION
*	Defines the IPV6 header for IPV6 packets.
*
* SYNOPSIS
*/
typedef struct _ipv6_hdr
{
	uint32_t	ver_tc_fl;
	uint16_t	payload_length;
	uint8_t		next_header;
	uint8_t		hop_limit;
	uint8_t		src_addr[16];
	uint8_t		dest_addr[16];

}	PACK_SUFFIX ipv6_hdr_t;
/*
* FIELDS
*	ver_tc_fl
*		This field contains of 3 subfields:
*		1.Version (4 bits) 
*		The constant 6 (bit sequence 0110).
*		2.Traffic Class (8 bits) 
* 		The bits of this field hold two values. The 6 most-significant bits are used 
*		for DSCP, which is used to classify packets. The remaining two bits 
*		are used for ECN; priority values subdivide into ranges: traffic where 
*		the source provides congestion control and non-congestion control traffic.
*		3.Flow Label (20 bits) 
*		Originally created for giving real-time applications special service.
*		Flow Label specifications and minimum requirements are described, 
*		and first uses of this field are emerging.
*
*
*	payload_length;
*		The size of the payload in octets, including any extension headers. 
*		The length is set to zero when a Hop-by-Hop extension header carries 
*		a Jumbo Payload option.
*
*	next_header
*		Specifies the type of the next header. This field usually specifies the
*		transport layer protocol used by a packet's payload. When extension headers
*		are present in the packet this field indicates which extension header follows. 
*		The values are shared with those used for the IPv4 protocol field, 
*		as both fields have the same function 
*
*	hop_limit
*		Replaces the time to live field of IPv4. This value is decremented by 
*		one at each intermediate node the packet visits. When the counter reaches 
*		0 the packet is discarded
*
*	src_addr
*		The IPv6 address of the sending node.
*
*	dest_addr
*		The IPv6 address of the destination node(s).
*
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, tcp_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#include <complib/cl_packon.h>
/****s* IB Network Drivers/tcp_hdr_t
* NAME
*	tcp_hdr_t
*
* DESCRIPTION
*	Defines the IP header for IP packets.
*
* SYNOPSIS
*/
typedef struct _tcp_hdr
{
	net16_t		src_port;
	net16_t		dst_port;
	net32_t		seq_num;
	net32_t		ack_num;
	uint8_t		offset;
	uint8_t		flags;
	net16_t		window;
	net16_t		chksum;
	net16_t		urp;

}	PACK_SUFFIX tcp_hdr_t;
/*
* FIELDS
*	src_port
*		Source port.
*
*	dst_port
*		Destination port.
*
*	seq_num
*		Sequence number.
*
*	ack_num
*		Acknowledge number.
*
*	offset
*		data offset.
*
*	flags
*		TCP flags.
*
*	window
*		Window number.
*
*	chksum
*		Checksum.
*
*	urp
*		Urgent pointer.
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, ip_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#include <complib/cl_packon.h>
/****s* IB Network Drivers/udp_hdr_t
* NAME
*	udp_hdr_t
*
* DESCRIPTION
*	Defines the IP header for IP packets.
*
* SYNOPSIS
*/
typedef struct _udp_hdr
{
	net16_t		src_port;
	net16_t		dst_port;
	net16_t		length;
	net16_t		chksum;

}	PACK_SUFFIX udp_hdr_t;
/*
* FIELDS
*	src_port
*		Source port.
*
*	dst_port
*		Destination port.
*
*	length
*		Length of datagram.
*
*	chksum
*		Checksum.
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, ip_hdr_t, tcp_hdr_t
*********/
#include <complib/cl_packoff.h>

#define IP_HEADER_LENGTH(pIpHdr)   \
        ( (ULONG)((pIpHdr->ver_hl & 0x0F) << 2) )

#define TCP_HEADER_LENGTH(pTcpHdr) \
		((pTcpHdr->offset & 0xF0) >> 2)

#define IP_FRAGMENT_OFFSET(p_ip_hdr) \
		( cl_ntoh16( p_ip_hdr->offset & CL_HTON16(0x1fff) ) )

#define IP_DONT_FRAGMENT(p_ip_hdr) \
		( (BOOLEAN)( p_ip_hdr->offset & CL_HTON16(0x4000 ) ) )
		
#define IP_MORE_FRAGMENTS( p_ip_hdr ) \
		( (BOOLEAN)( p_ip_hdr->offset & CL_HTON16(0x2000) ) )

#define IP_SET_MORE_FRAGMENTS( p_ip_hdr ) \
		( p_ip_hdr->offset |= CL_HTON16(0x2000) )

#define IP_SET_LAST_FRAGMENT( p_ip_hdr ) \
		( p_ip_hdr->offset &= (CL_HTON16(~0x2000) ) )

#define PROTOCOL_TCP         IP_PROT_TCP

#define IGMP_V2_MEMBERSHIP_QUERY	0x11
#define IGMP_V2_MEMBERSHIP_REPORT	0x16
#define IGMP_V1_MEMBERSHIP_REPORT	0x12	// for backward compatibility with IGMPv1
#define IGMP_V2_LEAVE_GROUP			0x17
#include <complib/cl_packon.h>
/****s* IB Network Drivers/igmp__v2_hdr_t
* NAME
*	igmp_v2_hdr_t
*
* DESCRIPTION
*	Defines the IGMPv2 header for IP packets.
*
* SYNOPSIS
*/
typedef struct _igmp_v2_hdr
{
	uint8_t		type;
	uint8_t		max_resp_time;
	net16_t		chksum;
	net32_t		group_address;
}	PACK_SUFFIX igmp_v2_hdr_t;
/*
* FIELDS
*	type
*		type of IGMPv2 message: query/report/leave
*
*	max_resp_time
*		The Max Response Time field is meaningful only in Membership Query
*		messages, and specifies the maximum allowed time before sending a
*		responding report in units of 1/10 second.  In all other messages, it
*		is set to zero by the sender and ignored by receivers.
*
*	checksum
*		The checksum is the 16-bit one's complement of the one's complement
*   	sum of the whole IGMP message (the entire IP payload).  
*
*	group_address
*		In a Membership Query message, the group address field is set to zero
*       when sending a General Query, and set to the group address being
*       queried when sending a Group-Specific Query.
*
*       In a Membership Report or Leave Group message, the group address
*       field holds the IP multicast group address of the group being
*       reported or left.
*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, ip_hdr_t, tcp_hdr_t
*********/
#include <complib/cl_packoff.h>

#define DHCP_PORT_SERVER				CL_HTON16(67)
#define DHCP_PORT_CLIENT				CL_HTON16(68)
#define DHCP_IPV6_PORT_SERVER_OR_AGENT	CL_HTON16(547)
#define DHCP_IPV6_PORT_CLIENT			CL_HTON16(546)

#define DHCP_PORT_PROXY_SERVER	CL_HTON16(4011)

#define DHCP_REQUEST			1
#define DHCP_REPLY				2
#define DHCP_HW_TYPE_ETH		1
#define DHCP_HW_TYPE_IB			32
#define DHCP_OPT_PAD			0
#define DHCP_OPT_END			255
#define DHCP_OPT_MSG			53
#define DHCP_OPT_CLIENT_ID		61

#define DHCPDISCOVER			1
#define DHCPOFFER				2
#define DHCPREQUEST				3
#define DHCPDECLINE				4
#define DHCPACK					5
#define DHCPNAK					6
#define DHCPRELEASE				7
#define DHCPINFORM				8

#define DHCP_FLAGS_BROADCAST	CL_HTON16(0x8000)
#define DHCP_COOKIE				0x63538263
#define DHCP_OPTIONS_SIZE		312
#define DHCP_COOKIE_SIZE		4


/* Minimum DHCP size is without options (but with 4-byte magic cookie). */
#define DHCP_MIN_SIZE			(sizeof(dhcp_pkt_t) + DHCP_COOKIE_SIZE - DHCP_OPTIONS_SIZE )

// The length of IPoIB data inside CID field of DHCP packets
static const uint8_t coIPoIB_CID_Len = 20;
static const uint8_t coIPoIB_CID_TotalLen = 22;


//static const uint8_t coIPoIB_HwTypeIB = 0xFF; - This definition will not work for C-code (C- C2099)
#define coIPoIB_HwTypeIB 0xFF

// The default prefix of IPoIB CID field within CID.
static const uint8_t coIBDefaultDHCPPrefix[] = {
	coIPoIB_HwTypeIB, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x2, 0x0,
	0x0, 0x2, 0xc9, 0x0
};

/* The CID will contain of: 
	CID[0] = DHCP_OPT_CLIENT_ID;
	CID[1] = coIPoIB_CID_Len;
	CID[2:13] = coIBDefaultDHCPPrefix; (Here CID[2] always == coIPoIB_HwTypeIB)
	CID[14:21] = GUID;
*/



#include <complib/cl_packon.h>
/****s* IB Network Drivers/dhcp_pkt_t
* NAME
*	dhcp_pkt_t
*
* DESCRIPTION
*	Defines the DHCP packet format as documented in RFC 2131
*	http://www.zvon.org/tmRFC/RFC2131/Output/index.html
*
* SYNOPSIS
*/
typedef struct _dhcp_pkt
{
	uint8_t		op;
	uint8_t		htype;
	uint8_t		hlen;
	uint8_t		hops;
	net32_t		xid;
	net16_t		secs;
	net16_t		flags;
	net32_t		ciaddr;
	net32_t		yiaddr;
	net32_t		siaddr;
	net32_t		giaddr;
	uint8_t		chaddr[16];
	uint8_t		sname[64];
	uint8_t		file[128];
	uint8_t		options[312];

}	PACK_SUFFIX dhcp_pkt_t;
/*
* SEE ALSO
*	IB Network Drivers, eth_hdr_t, arp_pkt_t, ip_hdr_t, udp_hdr_t
*********/
#include <complib/cl_packoff.h>


#include <complib/cl_packon.h>
typedef struct _udp_pkt
{
	udp_hdr_t	hdr;
	dhcp_pkt_t	dhcp;

}	PACK_SUFFIX udp_pkt_t;

typedef union _ip_payload
{
	tcp_hdr_t	tcp;
	udp_pkt_t	udp;

}	PACK_SUFFIX ip_payload_t;


typedef struct _ip_pkt
{
	ip_hdr_t		hdr;
	ip_payload_t	prot;
	

}	PACK_SUFFIX ip_pkt_t;

typedef struct _ipv6_pkt
{
	ipv6_hdr_t		hdr;
	ip_payload_t 	prot;

}	PACK_SUFFIX ipv6_pkt_t;


typedef struct _eth_pkt
{
	eth_hdr_t	hdr;
	union _eth_payload
	{
		arp_pkt_t	arp;
		ip_pkt_t	ip;

	}	PACK_SUFFIX type;

}	PACK_SUFFIX eth_pkt_t;

#include <complib/cl_packoff.h>

#include <complib/cl_packon.h>

#define ICMPV6_MSG_TYPE_NBR_SOL 135
#define ICMPV6_MSG_TYPE_NBR_ADV 136

typedef struct _icmpv6_hdr 
{
	uint8_t	type;        
	uint8_t	code; 
	net16_t	checksum;
} icmpv6_hdr_t;

typedef struct _icmpv6_pkt
{
	icmpv6_hdr_t hdr;

	union
	{
		struct _icmpv6_neighbor_sol
		{
			net32_t 	 reserved;
			uint8_t 	 target_addr[IPV6_ADDR_LENGTH];
		} sol;

		struct _icmpv6_neighbor_adv
		{
			net32_t 	 flags;
			uint8_t 	 target_addr[IPV6_ADDR_LENGTH];
		} adv;
	} u;
} icmpv6_pkt_t;

#define ICMPV6_OPTION_SRC_LINK_ADDR		1
#define ICMPV6_OPTION_TARGET_LINK_ADDR	2

#define ICMPV6_IPOIB_LINK_ADDR_OPTION_LENGTH		24
#define ICMPV6_IPOIB_LINK_ADDR_OPTION_ADDR_LENGTH	(ICMPV6_IPOIB_LINK_ADDR_OPTION_LENGTH - 2)

#define ICMPV6_ETH_LINK_ADDR_OPTION_LENGTH		8
#define ICMPV6_ETH_LINK_ADDR_OPTION_ADDR_LENGTH	(ICMPV6_ETH_LINK_ADDR_OPTION_LENGTH - 2)

typedef struct _icmpv6_option
{
    uint8_t     option_type;
    uint8_t     option_length;  // length of the entire option in 8-byte blocks == 1(MAC Option)

	union
	{
		struct _icmpv6_src_link_addr
		{
    		uint8_t     mac_addr[ICMPV6_IPOIB_LINK_ADDR_OPTION_ADDR_LENGTH];
		} saddr;

		struct _icmpv6_target_link_addr
		{
    		uint8_t     mac_addr[ICMPV6_IPOIB_LINK_ADDR_OPTION_ADDR_LENGTH];
		} taddr;
	} u;
} icmpv6_option_t;


#include <complib/cl_packoff.h>

#endif	/* _IP_PACKET_H_ */

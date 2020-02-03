/*
 * Copyright (c) 2008 Intel Corporation.  All rights reserved.
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

#ifndef _ib_cm_ifc_h_
#define _ib_cm_ifc_h_

#include <initguid.h>
#include <iba/ib_al_ifc.h>
#include <iba/ib_types.h>
#include <iba/ib_al.h>

struct _iba_cm_event;

typedef struct _iba_cm_id
{
	void		*context;
	NTSTATUS	(*callback)(struct _iba_cm_id *p_id, struct _iba_cm_event *p_event);
	net32_t		cid;

}	iba_cm_id;

typedef struct _iba_cm_req
{
	ib_net64_t					service_id;

	ib_path_rec_t				*p_primary_path;
	ib_path_rec_t				*p_alt_path;

	net32_t						qpn;
	ib_qp_type_t				qp_type;
	net32_t						starting_psn;

	void						*p_pdata;
	uint8_t						pdata_len;

	uint8_t						max_cm_retries;
	uint8_t						resp_res;
	uint8_t						init_depth;
	uint8_t						remote_resp_timeout;
	uint8_t						flow_ctrl;
	uint8_t						local_resp_timeout;
	uint8_t						rnr_retry_cnt;
	uint8_t						retry_cnt;
	uint8_t						srq;

}	iba_cm_req;

typedef struct _iba_cm_req_event
{
	ib_net64_t					service_id;

	ib_path_rec_t				primary_path;
	ib_path_rec_t				alt_path;

	net32_t						qpn;
	ib_qp_type_t				qp_type;
	net32_t						starting_psn;

	uint8_t						pdata[IB_REQ_PDATA_SIZE];

	uint8_t						max_cm_retries;
	uint8_t						resp_res;
	uint8_t						init_depth;
	uint8_t						remote_resp_timeout;
	uint8_t						flow_ctrl;
	uint8_t						local_resp_timeout;
	uint8_t						rnr_retry_cnt;
	uint8_t						retry_cnt;
	uint8_t						srq;

	net64_t						local_ca_guid;
	net64_t						remote_ca_guid;
	uint16_t					pkey_index;
	uint8_t						port_num;

}	iba_cm_req_event;

typedef struct _iba_cm_rep
{
	net32_t						qpn;
	net32_t						starting_psn;

	void						*p_pdata;
	uint8_t						pdata_len;

	ib_cm_failover_t			failover_accepted;
	uint8_t						resp_res;
	uint8_t						init_depth;
	uint8_t						flow_ctrl;
	uint8_t						rnr_retry_cnt;
	uint8_t						srq;

}	iba_cm_rep;

typedef struct _iba_cm_rep_event
{
	iba_cm_rep					rep;
	net64_t						ca_guid;
	uint8_t						target_ack_delay;

}	iba_cm_rep_event;

typedef struct _iba_cm_pdata
{
	void						*p_pdata;
	uint8_t						pdata_len;

}	iba_cm_pdata;
typedef iba_cm_pdata			iba_cm_rtu_event;
typedef iba_cm_pdata			iba_cm_dreq_event;
typedef iba_cm_pdata			iba_cm_drep_event;

typedef struct _iba_cm_rej_event
{
	void						*ari;
	void						*p_pdata;
	ib_rej_status_t				reason;
	uint8_t						ari_length;
	uint8_t						pdata_len;
	
}	iba_cm_rej_event;

typedef struct _iba_cm_mra_event
{
	void						*p_pdata;
	uint8_t						pdata_len;
	uint8_t						service_timeout;

}	iba_cm_mra_event;

typedef struct _iba_cm_lap
{
	ib_path_rec_t				*p_alt_path;
	void						*p_pdata;
	uint8_t						pdata_len;
	uint8_t						remote_resp_timeout;

}	iba_cm_lap;
typedef iba_cm_lap iba_cm_lap_event;

typedef struct _iba_cm_apr
{
	void						*p_pdata;
	ib_apr_info_t				*p_info;
	uint8_t						pdata_len;
	uint8_t						info_length;
	ib_apr_status_t				status;

}	iba_cm_apr;
typedef iba_cm_apr iba_cm_apr_event;

typedef struct _iba_cm_sidr_req
{
	ib_net64_t					service_id;
	ib_path_rec_t				*p_path;
	void						*p_pdata;
	uint8_t						pdata_len;
	uint8_t						max_cm_retries;
	uint32_t					timeout;

}	iba_cm_sidr_req;

typedef struct _iba_cm_sidr_req_event
{
	iba_cm_sidr_req				sidr_req;
	net64_t						ca_guid;
	uint16_t					pkey_index;
	uint8_t						port_num;

}	iba_cm_sidr_req_event;

typedef struct _iba_cm_sidr_rep
{
	net32_t						qpn;
	net32_t						qkey;
	void						*p_pdata;
	void						*p_info;
	uint8_t						pdata_len;
	uint8_t						info_len;
	ib_sidr_status_t			status;

}	iba_cm_sidr_rep;
typedef iba_cm_sidr_rep iba_cm_sidr_rep_event;

typedef enum _iba_cm_event_type
{
	iba_cm_req_error,
	iba_cm_req_received,
	iba_cm_rep_error,
	iba_cm_rep_received,
	iba_cm_rtu_received,
	iba_cm_dreq_error,
	iba_cm_dreq_received,
	iba_cm_drep_received,
	iba_cm_rej_received,
	iba_cm_mra_received,
	iba_cm_lap_error,
	iba_cm_lap_received,
	iba_cm_apr_received,
	iba_cm_sidr_req_error,
	iba_cm_sidr_req_received,
	iba_cm_sidr_rep_received

}	iba_cm_event_type;

typedef struct _iba_cm_event
{
	iba_cm_event_type			type;
	union
	{
		iba_cm_req_event		req;
		iba_cm_rep_event		rep;
		iba_cm_rtu_event		rtu;
		iba_cm_dreq_event		dreq;
		iba_cm_drep_event		drep;
		iba_cm_rej_event		rej;
		iba_cm_mra_event		mra;
		iba_cm_lap_event		lap;
		iba_cm_apr_event		apr;
		iba_cm_sidr_req_event	sidr_req;
		iba_cm_sidr_rep_event	sidr_rep;

	}	data;

}	iba_cm_event;

typedef struct _iba_cm_interface
{
	NTSTATUS		(*create_id)(NTSTATUS (*callback)(iba_cm_id *p_id,
													  iba_cm_event *p_event),
								 void *context, iba_cm_id **pp_id);
	void			(*destroy_id)(iba_cm_id *p_id);

	NTSTATUS		(*listen)(iba_cm_id *p_id, net64_t service_id, void *p_compare_buf,
							  uint8_t compare_len, uint8_t compare_offset);
	NTSTATUS		(*get_request)(iba_cm_id *p_listen_id, iba_cm_id **pp_id,
								   iba_cm_event *p_event);

	NTSTATUS		(*send_req)(iba_cm_id *p_id, iba_cm_req *p_req);
	NTSTATUS		(*send_rep)(iba_cm_id *p_id, iba_cm_rep *p_rep);
	NTSTATUS		(*send_rtu)(iba_cm_id *p_id, void *p_pdata, uint8_t pdata_len);

	NTSTATUS		(*send_dreq)(iba_cm_id *p_id, void *p_pdata, uint8_t pdata_len);
	NTSTATUS		(*send_drep)(iba_cm_id *p_id, void *p_pdata, uint8_t pdata_len);

	NTSTATUS		(*send_rej)(iba_cm_id *p_id, ib_rej_status_t status,
								void *p_ari, uint8_t ari_len,
								void *p_pdata, uint8_t pdata_len);
	NTSTATUS		(*send_mra)(iba_cm_id *p_id, uint8_t service_timeout,
								void *p_pdata, uint8_t pdata_len);

	NTSTATUS		(*send_lap)(iba_cm_id *p_id, iba_cm_lap *p_lap);
	NTSTATUS		(*send_apr)(iba_cm_id *p_id, iba_cm_apr *p_apr);

	NTSTATUS		(*send_sidr_req)(iba_cm_id *p_id, iba_cm_sidr_req *p_req);
	NTSTATUS		(*send_sidr_rep)(iba_cm_id *p_id, iba_cm_sidr_rep *p_rep);

	NTSTATUS		(*get_qp_attr)(iba_cm_id *p_id, ib_qp_state_t state,
								   ib_qp_mod_t *p_attr);

	NTSTATUS		(*migrate)(iba_cm_id *p_id);
	NTSTATUS		(*established)(iba_cm_id *p_id);

}	iba_cm_interface;

static inline USHORT IbaCmVersion(UINT8 Major, UINT8 Minor)
{
	return ((USHORT) Major << 8) | ((USHORT) Minor);
}

static inline UINT8 IbaCmVersionMajor(USHORT Version)
{
	return (UINT8) (Version >> 8);
}

static inline UINT8 IbaCmVersionMinor(USHORT Version)
{
	return (UINT8) Version;
}

// {6A11D060-8957-49e6-BE2A-01EDF1BD22B3}
DEFINE_GUID(GUID_INFINIBAND_INTERFACE_CM, 0x6a11d060, 0x8957, 0x49e6,
			0xbe, 0x2a, 0x1, 0xed, 0xf1, 0xbd, 0x22, 0xb3);

typedef struct _INFINIBAND_INTERFACE_CM
{
	INTERFACE			InterfaceHeader;
	iba_cm_interface	CM;

}	INFINIBAND_INTERFACE_CM;

#endif // _ib_cm_ifc_h_
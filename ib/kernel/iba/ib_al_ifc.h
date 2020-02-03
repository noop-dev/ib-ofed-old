/*
 * Copyright (c) 2005 SilverStorm Technologies.  All rights reserved.
 * Portions Copyright (c) 2008 Microsoft Corporation.  All rights reserved.
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
 * $Id: ib_al_ifc.h 3022 2008-08-21 17:57:35Z tzachid $
 */



#if !defined _IB_AL_IFC_H_
#define _IB_AL_IFC_H_


#include <iba/ib_al.h>


/****h* Access Layer/AL Interface
* NAME
*	AL Interface
*
* DESCRIPTION
*	Header file for the interface exported to ICT client drivers for access to
*	IB resources provided by HCAs.
*********/

#define AL_INTERFACE_VERSION		(12)



/* Function prototypes.  Match definitions in ib_al.h. */
typedef void
(*ib_pfn_sync_destroy_t)(
	IN				void						*context );

typedef ib_api_status_t
(*ib_pfn_open_ca_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_net64_t					ca_guid,
	IN		const	ib_pfn_event_cb_t			ca_event_cb OPTIONAL,
	IN		const	void* const					ca_context,
		OUT			ib_ca_handle_t* const		ph_ca );

typedef ib_api_status_t
(*ib_pfn_query_ca_t)(
	IN		const	ib_ca_handle_t				h_ca,
		OUT			ib_ca_attr_t* const			p_ca_attr OPTIONAL,
	IN	OUT			uint32_t* const				p_size );

typedef DEVICE_OBJECT*
(*ib_pfn_get_ca_dev_t)(
	IN		const	ib_ca_handle_t				h_ca );

typedef ib_api_status_t
(*ib_pfn_query_ca_by_guid_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_net64_t					ca_guid,
		OUT			ib_ca_attr_t* const			p_ca_attr OPTIONAL,
	IN	OUT			size_t* const				p_size );

typedef ib_api_status_t
(*ib_pfn_modify_ca_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	uint8_t						port_num,
	IN		const	ib_ca_mod_t					ca_mod,
	IN		const	ib_port_attr_mod_t* const	p_port_attr_mod );

typedef ib_api_status_t
(*ib_pfn_close_ca_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_alloc_pd_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_pd_type_t				pd_type,
	IN		const	void* const					pd_context,
		OUT			ib_pd_handle_t* const		ph_pd );

typedef ib_api_status_t
(*ib_pfn_dealloc_pd_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_create_av_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_av_attr_t* const			p_av_attr,
		OUT			ib_av_handle_t* const		ph_av );

typedef ib_api_status_t
(*ib_pfn_query_av_t)(
	IN		const	ib_av_handle_t				h_av,
		OUT			ib_av_attr_t* const			p_av_attr,
		OUT			ib_pd_handle_t* const		ph_pd );

typedef ib_api_status_t
(*ib_pfn_modify_av_t)(
	IN		const	ib_av_handle_t				h_av,
	IN		const	ib_av_attr_t* const			p_av_attr );

typedef ib_api_status_t
(*ib_pfn_destroy_av_t)(
	IN		const	ib_av_handle_t				h_av );

typedef ib_api_status_t
(*ib_pfn_create_srq_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_srq_attr_t* const			p_srq_attr,
	IN		const	void* const					srq_context,
	IN		const	ib_pfn_event_cb_t				srq_event_cb OPTIONAL,
		OUT			ib_srq_handle_t* const			ph_srq );

typedef ib_api_status_t
(*ib_pfn_query_srq_t)(
	IN		const	ib_srq_handle_t				h_srq,
		OUT			ib_srq_attr_t* const			p_srq_attr );

typedef ib_api_status_t
(*ib_pfn_modify_srq_t)(
	IN		const	ib_srq_handle_t				h_srq,
	IN		const	ib_srq_attr_t* const			p_srq_attr,
	IN		const	ib_srq_attr_mask_t				srq_attr_mask );

typedef ib_api_status_t
(*ib_pfn_destroy_srq_t)(
	IN		const	ib_srq_handle_t				h_srq,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_post_srq_recv_t)(
	IN		const	ib_srq_handle_t				h_srq,
	IN				ib_recv_wr_t* const			p_recv_wr,
		OUT			ib_recv_wr_t				**pp_recv_failure OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_create_qp_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_qp_create_t* const		p_qp_create,
	IN		const	void* const					qp_context,
	IN		const	ib_pfn_event_cb_t			qp_event_cb OPTIONAL,
		OUT			ib_qp_handle_t* const		ph_qp );

typedef ib_api_status_t
(*ib_pfn_get_spl_qp_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_net64_t					port_guid,
	IN		const	ib_qp_create_t* const		p_qp_create,
	IN		const	void* const					qp_context,
	IN		const	ib_pfn_event_cb_t			qp_event_cb OPTIONAL,
		OUT			ib_pool_key_t* const		p_pool_key OPTIONAL,
		OUT			ib_qp_handle_t* const		ph_qp );

typedef ib_api_status_t
(*ib_pfn_query_qp_t)(
	IN		const	ib_qp_handle_t				h_qp,
		OUT			ib_qp_attr_t* const			p_qp_attr );

typedef ib_api_status_t
(*ib_pfn_modify_qp_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_qp_mod_t* const			p_qp_mod );

typedef ib_api_status_t
(*ib_pfn_destroy_qp_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_create_cq_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN	OUT			ib_cq_create_t* const		p_cq_create,
	IN		const	void* const					cq_context,
	IN		const	ib_pfn_event_cb_t			cq_event_cb OPTIONAL,
		OUT			ib_cq_handle_t* const		ph_cq );

typedef ib_api_status_t
(*ib_pfn_modify_cq_t)(
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			uint32_t* const				p_size );

typedef ib_api_status_t
(*ib_pfn_query_cq_t)(
	IN		const	ib_cq_handle_t				h_cq,
		OUT			uint32_t* const				p_size );

typedef ib_api_status_t
(*ib_pfn_destroy_cq_t)(
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_reg_mem_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_mr_create_t* const		p_mr_create,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );

typedef ib_api_status_t
(*ib_pfn_reg_phys_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_phys_create_t* const		p_phys_create,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );

typedef ib_api_status_t
(*ib_pfn_query_mr_t)(
	IN		const	ib_mr_handle_t				h_mr,
		OUT			ib_mr_attr_t* const			p_mr_attr );

typedef ib_api_status_t
(*ib_pfn_rereg_mem_t)(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_mod_mask,
	IN		const	ib_mr_create_t* const		p_mr_create OPTIONAL,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_rereg_phys_t)(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_mr_mod_t					mr_mod_mask,
	IN		const	ib_phys_create_t* const		p_phys_create OPTIONAL,
	IN	OUT			void** const				p_vaddr,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
	IN		const	ib_pd_handle_t				h_pd OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_reg_shared_t)(
	IN		const	ib_mr_handle_t				h_mr,
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_access_t					access_ctrl,
	IN	OUT			void** const				p_vaddr,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );

typedef ib_api_status_t
(*ib_pfn_create_shmid_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	int							shmid,
	IN		const	ib_mr_create_t* const		p_mr_create,
		OUT			uint32_t* const				p_lkey,
		OUT			uint32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );

typedef ib_api_status_t
(*ib_pfn_reg_shmid_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	ib_shmid_t					shmid,
	IN		const	ib_mr_create_t* const		p_mr_create,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey,
		OUT			ib_mr_handle_t* const		ph_mr );

typedef ib_api_status_t
(*ib_pfn_dereg_mr_t)(
	IN		const	ib_mr_handle_t				h_mr );

typedef ib_api_status_t
(*mlnx_pfn_create_fmr_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	mlnx_fmr_create_t* const	p_fmr_create,
		OUT			mlnx_fmr_handle_t* const	ph_fmr );

typedef ib_api_status_t
(*mlnx_pfn_map_phys_fmr_t)(
	IN		const	mlnx_fmr_handle_t			h_fmr,
	IN		const	uint64_t* const				paddr_list,
	IN		const	int							list_len,
	IN	OUT			uint64_t* const				p_vaddr,
		OUT			net32_t* const				p_lkey,
		OUT			net32_t* const				p_rkey );

typedef ib_api_status_t
(*mlnx_pfn_unmap_fmr_t)(
	IN		const	mlnx_fmr_handle_t			h_fmr );

typedef ib_api_status_t
(*mlnx_pfn_destroy_fmr_t)(
	IN		mlnx_fmr_handle_t  const				h_fmr );


typedef ib_api_status_t
(*mlnx_pfn_create_fmr_pool_t)(
	IN		const	ib_pd_handle_t				h_pd,
	IN		const	mlnx_fmr_pool_create_t		*p_fmr_pool_attr,
	OUT		mlnx_fmr_pool_handle_t* const			ph_pool );


typedef ib_api_status_t
(*mlnx_pfn_destroy_fmr_pool_t)(
	IN		const	mlnx_fmr_pool_handle_t			h_pool );


typedef ib_api_status_t
(*mlnx_pfn_map_phys_fmr_pool_t)(
	IN		const	mlnx_fmr_pool_handle_t		h_pool ,
	IN		const	uint64_t* const				paddr_list,
	IN		const	int							list_len,
	IN	OUT			uint64_t* const				p_vaddr,
	OUT		net32_t* const					p_lkey,
	OUT		net32_t* const					p_rkey,
	OUT				mlnx_fmr_pool_el_t			*p_fmr_el);

typedef ib_api_status_t
(*mlnx_pfn_unmap_fmr_pool_t)(
	IN			mlnx_fmr_pool_el_t				p_fmr_el );

typedef ib_api_status_t
(*mlnx_pfn_flush_fmr_pool_t)(
	IN		const	mlnx_fmr_pool_handle_t			h_pool );

typedef ib_api_status_t
(*ib_pfn_create_mw_t)(
	IN		const	ib_pd_handle_t				h_pd,
		OUT			uint32_t* const				p_rkey,
		OUT			ib_mw_handle_t* const			ph_mw );

typedef ib_api_status_t
(*ib_pfn_query_mw_t)(
	IN		const	ib_mw_handle_t				h_mw,
		OUT			ib_pd_handle_t* const			ph_pd,
		OUT			uint32_t* const				p_rkey );

typedef ib_api_status_t
(*ib_pfn_bind_mw_t)(
	IN		const	ib_mw_handle_t				h_mw,
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_bind_wr_t* const			p_mw_bind,
		OUT			uint32_t* const				p_rkey );

typedef ib_api_status_t
(*ib_pfn_destroy_mw_t)(
	IN		const	ib_mw_handle_t				h_mw );

typedef ib_api_status_t
(*ib_pfn_post_send_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_send_wr_t* const			p_send_wr,
		OUT			ib_send_wr_t				**pp_send_failure OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_post_recv_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN				ib_recv_wr_t* const			p_recv_wr,
		OUT			ib_recv_wr_t				**pp_recv_failure OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_send_mad_t)(
	IN		const	ib_mad_svc_handle_t			h_mad_svc,
	IN				ib_mad_element_t* const		p_mad_element_list,
		OUT			ib_mad_element_t			**pp_mad_failure OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_cancel_mad_t)(
	IN		const	ib_mad_svc_handle_t			h_mad_svc,
	IN				ib_mad_element_t* const		p_mad_element );

typedef ib_api_status_t
(*ib_pfn_poll_cq_t)(
	IN		const	ib_cq_handle_t				h_cq,
	IN	OUT			ib_wc_t** const				pp_free_wclist,
		OUT			ib_wc_t** const				pp_done_wclist );

typedef ib_api_status_t
(*ib_pfn_rearm_cq_t)(
	IN		const	ib_cq_handle_t				h_cq,
	IN		const	boolean_t					solicited );

typedef ib_api_status_t
(*ib_pfn_join_mcast_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_mcast_req_t* const		p_mcast_req );

typedef ib_api_status_t
(*ib_pfn_leave_mcast_t)(
	IN		const	ib_mcast_handle_t			h_mcast,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb );

typedef ib_api_status_t
(*ib_pfn_local_mad_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	uint8_t						port_num,
	IN		const	void* const					p_mad_in,
		OUT			void*						p_mad_out );

typedef ib_api_status_t
(*ib_pfn_cm_listen_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_cm_listen_t* const		p_cm_listen,
	IN		const	void* const					listen_context,
		OUT			ib_listen_handle_t* const	ph_cm_listen );

typedef ib_api_status_t
(*ib_pfn_cm_cancel_t)(
	IN		const	ib_listen_handle_t			h_cm_listen,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_cm_req_t)(
	IN		const	ib_cm_req_t* const			p_cm_req );

typedef ib_api_status_t
(*ib_pfn_cm_rep_t)(
	IN		const	ib_cm_handle_t				h_cm_req,
	IN		const	ib_cm_rep_t* const			p_cm_rep );

typedef ib_api_status_t
(*ib_pfn_cm_rtu_t)(
	IN		const	ib_cm_handle_t				h_cm_rep,
	IN		const	ib_cm_rtu_t* const			p_cm_rtu );

typedef ib_api_status_t
(*ib_pfn_cm_rej_t)(
	IN		const	ib_cm_handle_t				h_cm,
	IN		const	ib_cm_rej_t* const			p_cm_rej );

typedef ib_api_status_t
(*ib_pfn_cm_mra_t)(
	IN		const	ib_cm_handle_t				h_cm,
	IN		const	ib_cm_mra_t* const			p_cm_mra );

typedef ib_api_status_t
(*ib_pfn_cm_lap_t)(
	IN		const	ib_cm_lap_t* const			p_cm_lap );

typedef ib_api_status_t
(*ib_pfn_cm_apr_t)(
	IN		const	ib_cm_handle_t				h_cm_lap,
	IN		const	ib_cm_apr_t* const			p_cm_apr );

typedef ib_api_status_t
(*ib_pfn_force_apm_t)(
	IN		const	ib_qp_handle_t				h_qp );

typedef ib_api_status_t
(*ib_pfn_cm_dreq_t)(
	IN		const	ib_cm_dreq_t* const			p_cm_dreq );

typedef ib_api_status_t
(*ib_pfn_cm_drep_t)(
	IN		const	ib_cm_handle_t				h_cm_dreq,
	IN		const	ib_cm_drep_t* const			p_cm_drep );

typedef ib_api_status_t
(*ib_pfn_cm_handoff_t)(
	IN		const	ib_cm_handle_t				h_cm_req,
	IN		const	ib_net64_t					svc_id );

typedef ib_api_status_t
(*ib_pfn_create_ioc_t)(
	IN		const	ib_ca_handle_t				h_ca,
	IN		const	ib_ioc_profile_t* const		p_ioc_profile,
		OUT			ib_ioc_handle_t* const		ph_ioc );

typedef ib_api_status_t
(*ib_pfn_destroy_ioc_t)(
	IN		const	ib_ioc_handle_t				h_ioc );

typedef ib_api_status_t
(*ib_pfn_reg_ioc_t)(
	IN		const	ib_ioc_handle_t				h_ioc );

typedef ib_api_status_t
(*ib_pfn_add_svc_entry_t)(
	IN		const	ib_ioc_handle_t				h_ioc,
	IN		const	ib_svc_entry_t* const		p_svc_entry,
		OUT			ib_svc_handle_t* const		ph_svc );

typedef ib_api_status_t
(*ib_pfn_remove_svc_entry_t)(
	IN		const	ib_svc_handle_t				h_svc );

typedef ib_api_status_t
(*ib_pfn_get_ca_guids_t)(
	IN				ib_al_handle_t				h_al,
		OUT			ib_net64_t* const			p_guid_array OPTIONAL,
	IN	OUT			uintn_t* const				p_guid_cnt );

typedef ib_api_status_t
(*ib_pfn_get_ca_by_gid_t)(
	IN				ib_al_handle_t				h_al,
	IN		const	ib_gid_t* const				p_gid,
		OUT			ib_net64_t* const			p_ca_guid );

typedef ib_api_status_t
(*ib_pfn_get_ca_by_gid_t)(
	IN				ib_al_handle_t				h_al,
	IN		const	ib_gid_t* const				p_gid,
		OUT			ib_net64_t* const			p_ca_guid );

typedef ib_api_status_t
(*ib_pfn_get_port_by_gid_t)(
	IN				ib_al_handle_t				h_al,
	IN		const	ib_gid_t* const				p_gid,
		OUT			ib_net64_t* const			p_port_guid );

typedef ib_api_status_t
(*ib_pfn_create_mad_pool_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	size_t						min,
	IN		const	size_t						max,
	IN		const	size_t						grow_size,
		OUT			ib_pool_handle_t* const		ph_pool );

typedef ib_api_status_t
(*ib_pfn_destroy_mad_pool_t)(
	IN		const	ib_pool_handle_t			h_pool );

typedef ib_api_status_t
(*ib_pfn_reg_mad_pool_t)(
	IN		const	ib_pool_handle_t			h_pool,
	IN		const	ib_pd_handle_t				h_pd,
		OUT			ib_pool_key_t* const		p_pool_key );

typedef ib_api_status_t
(*ib_pfn_dereg_mad_pool_t)(
	IN		const	ib_pool_key_t				pool_key );

typedef ib_api_status_t
(*ib_pfn_get_mad_t)(
	IN		const	ib_pool_key_t				pool_key,
	IN		const	size_t						buf_size,
		OUT			ib_mad_element_t			**pp_mad_element );

typedef ib_api_status_t
(*ib_pfn_put_mad_t)(
	IN		const	ib_mad_element_t*			p_mad_element_list );

typedef ib_api_status_t
(*ib_pfn_init_dgrm_svc_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_dgrm_info_t* const		p_dgrm_info OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_reg_mad_svc_t)(
	IN		const	ib_qp_handle_t				h_qp,
	IN		const	ib_mad_svc_t* const			p_mad_svc,
		OUT			ib_mad_svc_handle_t* const	ph_mad_svc );

typedef ib_api_status_t
(*ib_pfn_reg_svc_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_reg_svc_req_t* const		p_reg_svc_req,
		OUT			ib_reg_svc_handle_t* const	ph_reg_svc );

typedef ib_api_status_t
(*ib_pfn_dereg_svc_t)(
	IN		const	ib_reg_svc_handle_t			h_reg_svc,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_query_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_query_req_t* const		p_query_req,
		OUT			ib_query_handle_t* const	ph_query OPTIONAL );

typedef void
(*ib_pfn_cancel_query_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_query_handle_t			query_hndl );

typedef ib_api_status_t
(*ib_pfn_reg_pnp_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_pnp_req_t* const			p_pnp_req,
		OUT			ib_pnp_handle_t* const		ph_pnp );

typedef ib_api_status_t
(*ib_pfn_dereg_pnp_t)(
	IN		const	ib_pnp_handle_t				h_pnp,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_subscribe_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_sub_req_t* const			p_sub_req,
		OUT			ib_sub_handle_t* const		ph_sub );

typedef ib_api_status_t
(*ib_pfn_unsubscribe_t)(
	IN		const	ib_sub_handle_t				h_sub,
	IN		const	ib_pfn_destroy_cb_t			destroy_cb OPTIONAL );

typedef ib_api_status_t
(*ib_pfn_reject_ioc_t)(
	IN		const	ib_al_handle_t				h_al,
	IN		const	ib_pnp_handle_t				h_ioc_event );

typedef ib_api_status_t
(*ib_pfn_ci_call_t)(
	IN				ib_ca_handle_t				h_ca,
	IN		const	void**				const	handle_array	OPTIONAL,
	IN				uint32_t					num_handles,
	IN				ib_ci_op_t*			const	p_ci_op );

typedef ib_api_status_t
(*ib_pfn_open_al_t)(
		OUT			ib_al_handle_t* const		ph_al );

typedef ib_api_status_t
(*ib_pfn_close_al_t)(
	IN		const	ib_al_handle_t				h_al );

typedef const char*
(*ib_pfn_get_err_str_t)(
	IN				ib_api_status_t				status );

typedef const char*
(*ib_pfn_get_wc_status_str_t)(
	IN				ib_wc_status_t				wc_status );


/* Interface specific data header. */
typedef struct _ib_al_ifc_data
{
	const GUID					*type;
	USHORT						size;
	USHORT						version;
	void						*p_data;

}	ib_al_ifc_data_t;


/* Interface definitions */
typedef struct _ib_al_ifc
{
	/* Standard interface header. */
	INTERFACE					wdm;

	/* AL entry points. */
	ib_pfn_sync_destroy_t		sync_destroy;
	ib_pfn_open_ca_t			open_ca;
	ib_pfn_query_ca_t			query_ca;
	ib_pfn_get_ca_dev_t			get_dev;
	ib_pfn_close_ca_t			close_ca;
	ib_pfn_alloc_pd_t			alloc_pd;
	ib_pfn_dealloc_pd_t			dealloc_pd;
	ib_pfn_create_av_t			create_av;
	ib_pfn_query_av_t			query_av;
	ib_pfn_modify_av_t			modify_av;
	ib_pfn_destroy_av_t			destroy_av;
	ib_pfn_create_qp_t			create_qp;
	ib_pfn_get_spl_qp_t			get_spl_qp;
	ib_pfn_query_qp_t			query_qp;
	ib_pfn_modify_qp_t			modify_qp;
	ib_pfn_destroy_qp_t			destroy_qp;
	ib_pfn_create_cq_t			create_cq;
	ib_pfn_modify_cq_t			modify_cq;
	ib_pfn_query_cq_t			query_cq;
	ib_pfn_destroy_cq_t			destroy_cq;
	ib_pfn_reg_mem_t			reg_mem;
	ib_pfn_reg_phys_t			reg_phys;
	ib_pfn_query_mr_t			query_mr;
	ib_pfn_rereg_mem_t			rereg_mem;
	ib_pfn_create_shmid_t		create_shmid;
	ib_pfn_reg_shmid_t			reg_shmid;
	ib_pfn_dereg_mr_t			dereg_mr;
	ib_pfn_create_mw_t			create_mw;
	ib_pfn_query_mw_t			query_mw;
	ib_pfn_bind_mw_t			bind_mw;
	ib_pfn_destroy_mw_t			destroy_mw;
	ib_pfn_post_send_t			post_send;
	ib_pfn_post_recv_t			post_recv;
	ib_pfn_send_mad_t			send_mad;
	ib_pfn_cancel_mad_t			cancel_mad;
	ib_pfn_poll_cq_t			poll_cq;
	ib_pfn_rearm_cq_t			rearm_cq;
	ib_pfn_join_mcast_t			join_mcast;
	ib_pfn_leave_mcast_t		leave_mcast;
	ib_pfn_local_mad_t			local_mad;
	ib_pfn_cm_listen_t			cm_listen;
	ib_pfn_cm_cancel_t			cm_cancel;
	ib_pfn_cm_req_t				cm_req;
	ib_pfn_cm_rep_t				cm_rep;
	ib_pfn_cm_rtu_t				cm_rtu;
	ib_pfn_cm_rej_t				cm_rej;
	ib_pfn_cm_mra_t				cm_mra;
	ib_pfn_cm_lap_t				cm_lap;
	ib_pfn_cm_apr_t				cm_apr;
	ib_pfn_force_apm_t			force_apm;
	ib_pfn_cm_dreq_t			cm_dreq;
	ib_pfn_cm_drep_t			cm_drep;
	ib_pfn_cm_handoff_t			cm_handoff;
	ib_pfn_create_ioc_t			create_ioc;
	ib_pfn_destroy_ioc_t		destroy_ioc;
	ib_pfn_reg_ioc_t			reg_ioc;
	ib_pfn_add_svc_entry_t		add_svc_entry;
	ib_pfn_remove_svc_entry_t	remove_svc_entry;
	ib_pfn_get_ca_guids_t		get_ca_guids;
	ib_pfn_get_ca_by_gid_t		get_ca_by_gid;
	ib_pfn_get_port_by_gid_t	get_port_by_gid;
	ib_pfn_create_mad_pool_t	create_mad_pool;
	ib_pfn_destroy_mad_pool_t	destroy_mad_pool;
	ib_pfn_reg_mad_pool_t		reg_mad_pool;
	ib_pfn_dereg_mad_pool_t		dereg_mad_pool;
	ib_pfn_get_mad_t			get_mad;
	ib_pfn_put_mad_t			put_mad;
	ib_pfn_init_dgrm_svc_t		init_dgrm_svc;
	ib_pfn_reg_mad_svc_t		reg_mad_svc;
	ib_pfn_reg_svc_t			reg_svc;
	ib_pfn_dereg_svc_t			dereg_svc;
	ib_pfn_query_t				query;
	ib_pfn_cancel_query_t		cancel_query;
	ib_pfn_reg_pnp_t			reg_pnp;
	ib_pfn_dereg_pnp_t			dereg_pnp;
	ib_pfn_subscribe_t			subscribe;
	ib_pfn_unsubscribe_t		unsubscribe;
	ib_pfn_reject_ioc_t			reject_ioc;
	ib_pfn_ci_call_t			ci_call;
	ib_pfn_open_al_t			open_al;
	ib_pfn_close_al_t			close_al;
	ib_pfn_get_err_str_t		get_err_str;
	ib_pfn_get_wc_status_str_t	get_wc_status_str;
	mlnx_pfn_create_fmr_t		create_mlnx_fmr;
	mlnx_pfn_map_phys_fmr_t		map_phys_mlnx_fmr;
	mlnx_pfn_unmap_fmr_t		unmap_mlnx_fmr;
	mlnx_pfn_destroy_fmr_t		destroy_mlnx_fmr;
	mlnx_pfn_create_fmr_pool_t		create_mlnx_fmr_pool;
	mlnx_pfn_destroy_fmr_pool_t	destroy_mlnx_fmr_pool;
	mlnx_pfn_map_phys_fmr_pool_t		map_phys_mlnx_fmr_pool;
	mlnx_pfn_unmap_fmr_pool_t		unmap_mlnx_fmr_pool;
	mlnx_pfn_flush_fmr_pool_t		flush_mlnx_fmr_pool;
	
	ib_pfn_create_srq_t		create_srq;
	ib_pfn_query_srq_t			query_srq;
	ib_pfn_modify_srq_t		modify_srq;
	ib_pfn_destroy_srq_t		destroy_srq;
	ib_pfn_post_srq_recv_t		post_srq_recv;

}	ib_al_ifc_t;


#endif	/* !defined _IB_AL_IFC_H_ */

/*
 * AL interface GUID.  The GUID is defined outside the conditional include
 * on purpose so that it can be instantiated only once where it is actually
 * needed.  See the DDK docs section "Using GUIDs in Drivers" for more info.
 */
/* {707A1BDE-BF9F-4565-8FDD-144EF6514FE8} */
DEFINE_GUID(GUID_IB_AL_INTERFACE, 
0x707a1bde, 0xbf9f, 0x4565, 0x8f, 0xdd, 0x14, 0x4e, 0xf6, 0x51, 0x4f, 0xe8);

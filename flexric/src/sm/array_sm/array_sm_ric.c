#include "array_sm_ric.h"
#include "array_sm_id.h"
#include "array_sm_msg.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ──────────────────────────────────────────────────────────────
   Minimal RIC-side object.  Mirrors the slice_sm_ric.c pattern,
   but only implements Indication + Control paths.
   ────────────────────────────────────────────────────────────── */
typedef struct {
    sm_ric_t base;
} sm_array_ric_t;

/* ========== 1. Indication handler  (Agent → RIC) ========== */
static sm_ag_if_rd_ind_t 
on_indication_array_sm_ric(sm_ric_t const* sm_ric,
                                             sm_ind_data_t const* data)
{
    assert(sm_ric && data);
    sm_ag_if_rd_ind_t rd_if = { .type = ARRAY_STATS_V0 };

    array_ind_msg_t ind;
    memcpy(&ind, data->ind_msg,
           data->len_msg < sizeof(ind) ? data->len_msg : sizeof(ind));

    rd_if.array.msg = array_dec_ind_msg(NULL, data->len_msg, data->ind_msg);

    return rd_if;
}

/* ========== 2. Control-request encoder (RIC → Agent) ========== */
static sm_ctrl_req_data_t 
on_control_req_array_sm_ric(sm_ric_t const* sm_ric,
                                               void* ctrl)
{
    assert(sm_ric && ctrl);
    array_ctrl_msg_t const* msg = (array_ctrl_msg_t const*)ctrl;

    sm_ctrl_req_data_t req = {0};
    req.ctrl_hdr = NULL;               /* no header for plain mode */
    req.len_hdr  = 0;
    req.len_msg  = sizeof(array_ctrl_msg_t);

    void* buf = malloc(req.len_msg);
    memcpy(buf, msg, req.len_msg);
    req.ctrl_msg = buf;

    return req;
}

/* ========== 3. Control-output decoder (Agent → RIC) ========== */
static sm_ag_if_ans_ctrl_t 
on_control_out_array_sm_ric(sm_ric_t const* sm_ric,
                                                const sm_ctrl_out_data_t* out)
{
    assert(sm_ric && out);
    sm_ag_if_ans_ctrl_t ans = { .type = ARRAY_AGENT_IF_CTRL_ANS_V0 };

    array_ctrl_out_t* decoded = malloc(sizeof(array_ctrl_out_t));
    memcpy(decoded, out->ctrl_out,
           out->len_out < sizeof(array_ctrl_out_t)
               ? out->len_out
               : sizeof(array_ctrl_out_t));
    ans.array = *decoded;
    free(decoded);
    return ans;
}

/* ========== 4. Required but unused procedures ========== */
static sm_subs_data_t
on_subscription_array_sm_ric(sm_ric_t const* sm_ric, void* cmd)
{
    (void)sm_ric; (void)cmd;
    sm_subs_data_t s = {0};
    return s;
}

static sm_ag_if_rd_e2setup_t
on_e2setup_array_sm_ric(sm_ric_t const* sm_ric, sm_e2_setup_data_t const* setup)
{
    (void)sm_ric; (void)setup;
    sm_ag_if_rd_e2setup_t r = {0};
    return r;
}

static sm_ag_if_rd_rsu_t
on_rsu_array_sm_ric(sm_ric_t const* sm_ric, const sm_ric_service_update_data_t* data)
{
    (void)sm_ric; (void)data;
    sm_ag_if_rd_rsu_t r = {0};
    return r;
}

/* ----------  Free the object  ---------- */
static void free_array_sm_ric(sm_ric_t* sm_ric)
{
    /* If you ever malloc() other sub-structures inside sm_ric, free them here */
    free(sm_ric);
}

/* ───────────  Factory exported to the RIC loader  ─────────── */
sm_ric_t* make_array_sm_ric(void)
{
    sm_array_ric_t* sm = calloc(1, sizeof(*sm));
    assert(sm && "Memory exhausted");

    *((uint16_t*)&sm->base.ran_func_id) = SM_ARRAY_ID;
    memcpy(sm->base.ran_func_name, SM_ARRAY_STR, strlen(SM_ARRAY_STR));

    sm->base.free_sm = free_array_sm_ric;

    /* bind callbacks */
    sm->base.proc.on_subscription       = on_subscription_array_sm_ric;
    sm->base.proc.on_indication         = on_indication_array_sm_ric;
    sm->base.proc.on_control_req        = on_control_req_array_sm_ric;
    sm->base.proc.on_control_out        = on_control_out_array_sm_ric;
    sm->base.proc.on_e2_setup           = on_e2setup_array_sm_ric;
    sm->base.proc.on_ric_service_update = on_rsu_array_sm_ric;

    printf("[array_sm_ric] Registered SM_ARRAY_ID=%u (%s)\n",
           SM_ARRAY_ID, SM_ARRAY_STR);

    return &sm->base;
}

#include "array_sm_agent.h"
#include "array_sm_id.h"
#include "array_sm_msg.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 *  Minimal ARRAY Service Model — Agent side
 * ============================================================ */

typedef struct {
    sm_agent_t base;
} sm_array_agent_t;

/* ---------- Indication (Agent → RIC) ---------- */
static exp_ind_data_t
on_indication_array_sm_ag(sm_agent_t const* sm_agent, void* act_def)
{
    (void)sm_agent; (void)act_def;

    exp_ind_data_t ret = {.has_value = true};
    array_ind_msg_t ind = {0};

    /* Fill telemetry values (replace with real RAN data later) */
    ind.tstamp_us = 0;
    ind.Nt = 2;
    ind.Nr = 2;
    ind.snr_lin = 10.0f;

    size_t len = sizeof(ind);
    void* buf = malloc(len);
    memcpy(buf, &ind, len);

    ret.data.ind_hdr = NULL;
    ret.data.len_hdr = 0;
    ret.data.ind_msg = buf;
    ret.data.len_msg = len;
    ret.data.call_process_id = NULL;
    ret.data.len_cpid = 0;
    return ret;
}

/* ---------- Control (RIC → Agent) ---------- */
static sm_ctrl_out_data_t
on_control_array_sm_ag(sm_agent_t const* sm_agent,
                       sm_ctrl_req_data_t const* data)
{
    (void)sm_agent;
    assert(data);

    const array_ctrl_msg_t* msg = (const array_ctrl_msg_t*)data->ctrl_msg;

    printf("[array_sm_agent] apply control: dt = %.4f  sel_idx[0] = %u\n",
           msg->dt, msg->sel_idx[0]);

    array_ctrl_out_t result = {.status = 1, .applied_dt = msg->dt};

    sm_ctrl_out_data_t out = {0};
    out.len_out  = sizeof(result);
    out.ctrl_out = malloc(out.len_out);
    memcpy(out.ctrl_out, &result, out.len_out);
    return out;
}

/* ---------- E2 Setup (advertise identity) ---------- */
static sm_e2_setup_data_t
on_e2_setup_array_sm_ag(sm_agent_t const* sm_agent)
{
    (void)sm_agent;

    sm_e2_setup_data_t setup = {0};
    size_t len = strlen(SM_ARRAY_STR);

    setup.len_rfd = len;
    setup.ran_fun_def = calloc(1, len+1);
    memcpy(setup.ran_fun_def, SM_ARRAY_STR, len);
    return setup;
}

/* ---------- Cleanup ---------- */
static void free_array_sm_ag(sm_agent_t* sm_agent)
{
    free(sm_agent);
}

/* ---- metadata helpers ---- */
static char const* def_array_sm_ag(void) { return SM_ARRAY_STR; }
static uint16_t    id_array_sm_ag(void)  { return SM_ARRAY_ID;  }
static uint16_t    rev_array_sm_ag(void) { return SM_ARRAY_REV; }
static char const* oid_array_sm_ag(void) { return SM_ARRAY_OID; }

/* ---- factory ---- */
__attribute__ ((visibility ("default")))
sm_agent_t* make_array_sm_agent(sm_io_ag_ran_t io)
{
    (void)io;
    sm_agent_t* sm = calloc(1, sizeof(sm_agent_t));
    assert(sm);

    sm->free_sm = free_array_sm_ag;

    /* mandatory procedure callbacks */
    sm->proc.on_indication = on_indication_array_sm_ag;
    sm->proc.on_control    = on_control_array_sm_ag;
    sm->proc.on_e2_setup   = on_e2_setup_array_sm_ag;

    /* fill info block so loader can call sm->info.id() safely */
    sm->info.def = def_array_sm_ag;
    sm->info.id  = id_array_sm_ag;
    sm->info.rev = rev_array_sm_ag;
    sm->info.oid = oid_array_sm_ag;

    printf("[array_sm_agent] Created SM_ARRAY_ID = %u (%s)\n",
           SM_ARRAY_ID, SM_ARRAY_STR);

    return sm;
}


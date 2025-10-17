#ifndef ARRAY_SERVICE_MODEL_MSG_H
#define ARRAY_SERVICE_MODEL_MSG_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_ARRAY_ELEMS 256  // set higher than any expected Nr

/* ===== Indication: Agent → RIC =====
   Carries the array/system state your xApp needs to compute control. */
typedef struct {
    uint64_t tstamp_us;   // microsecond timestamp
    uint32_t Nt;          // # Tx elements (if relevant)
    uint32_t Nr;          // # Rx elements (selection size will equal this)
    float    snr_lin;     // example telemetry; keep what you need
    float    tht;         // AoD (rad)  — optional, keep if you use it
    float    the;         // AoA (rad)  — optional, keep if you use it
    float    R;           // range (m)  — optional, keep if you use it
} array_ind_msg_t;

/* ===== Control: RIC/xApp → Agent =====
   Your algorithm returns:
   - dt: the Tx spacing (or other single scalar you optimize),
   - sel_idx: the EXACT set of Nr indices to activate on Rx (num_sel is omitted
              because it MUST equal Nr — the agent will take the first Nr entries). */
typedef struct {
    uint64_t tstamp_us;   // microsecond timestamp (when xApp computed control)
    float    dt;          // optimized Tx spacing (m) — no 'dr' here
    uint32_t sel_idx[MAX_ARRAY_ELEMS];  // use first Nr entries; no num_sel field
} array_ctrl_msg_t;

/* ===== Optional confirmation after applying control (Agent → RIC) */
typedef struct {
    uint32_t status;      // 1=applied, 0=failed
    float    applied_dt;  // dt actually applied (echo or quantized value)
} array_ctrl_out_t;

/* ===== Optional subscription trigger (if you want periodic reports) */
typedef struct {
    uint32_t periodicity_ms;  // e.g., 10 for 10 ms
} array_sub_data_t;

/* helpers */
static inline void free_array_ind_msg(array_ind_msg_t* m)  { if (m) free(m); }
static inline void free_array_ctrl_msg(array_ctrl_msg_t* m){ if (m) free(m); }

#endif /* ARRAY_SERVICE_MODEL_MSG_H */


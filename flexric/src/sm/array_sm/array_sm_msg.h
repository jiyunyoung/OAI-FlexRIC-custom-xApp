#ifndef ARRAY_SERVICE_MODEL_MSG_H
#define ARRAY_SERVICE_MODEL_MSG_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_ARRAY_ELEMS 256  
/* ===== Indication: Agent → RIC =====
   Carries the array/system state your xApp needs to compute control. */
typedef struct {
    uint64_t tstamp_us;   // microsecond timestamp (when xApp computed control)
    uint32_t Nt;         
    uint32_t Nr;         
    float    snr_lin;     
    float    tht;         
    float    the;         
    float    R;           
} array_ind_msg_t;

/* ===== Control: RIC/xApp → Agent =====
   Your algorithm returns:
   - dt: the Tx spacing (or other single scalar you optimize),
   - sel_idx: the EXACT set of Nr indices to activate on Rx (num_sel is omitted
              because it MUST equal Nr — the agent will take the first Nr entries). */
typedef struct {
    uint64_t tstamp_us;   // microsecond timestamp (when xApp computed control)
    float    dt;          
    uint32_t sel_idx[MAX_ARRAY_ELEMS];  
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


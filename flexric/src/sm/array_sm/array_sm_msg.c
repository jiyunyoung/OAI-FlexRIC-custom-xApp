#include "array_sm_msg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* ===== Simple helper implementations ===== */

void free_array_ind_msg(array_ind_msg_t* m)
{
    if (!m) return;
    free(m);
}

void free_array_ctrl_msg(array_ctrl_msg_t* m)
{
    if (!m) return;
    free(m);
}

void free_array_ctrl_out(array_ctrl_out_t* m)
{
    if (!m) return;
    free(m);
}

void free_array_sub_data(array_sub_data_t* m)
{
    if (!m) return;
    free(m);
}

/* Optional debug printer (for tracing) */
void print_array_ind_msg(const array_ind_msg_t* m)
{
    if (!m) return;
    printf("[ARRAY IND] tstamp=%lu Nt=%u Nr=%u SNR=%.3f tht=%.3f the=%.3f R=%.3f\n",
           (unsigned long)m->tstamp_us, m->Nt, m->Nr,
           m->snr_lin, m->tht, m->the, m->R);
}

void print_array_ctrl_msg(const array_ctrl_msg_t* m, uint32_t Nr)
{
    if (!m) return;
    printf("[ARRAY CTRL] tstamp=%lu dt=%.5f sel_idx=", (unsigned long)m->tstamp_us, m->dt);
    for (uint32_t i = 0; i < Nr; i++)
        printf("%u ", m->sel_idx[i]);
    printf("\n");
}

array_ind_msg_t array_dec_ind_msg(void const* enc, size_t len, const uint8_t* src)
{
  (void)enc;
  assert(src != NULL);
  assert(len == sizeof(array_ind_msg_t));

  array_ind_msg_t dst={0};
  memcpy(&dst, src, len);
  return dst;
}

byte_array_t array_enc_ind_msg(void const* enc, const array_ind_msg_t* msg)
{
  (void)enc;
  assert(msg != NULL);

  byte_array_t ba = {0};
  ba.len = sizeof(array_ind_msg_t);
  ba.buf = malloc(ba.len);
  memcpy(ba.buf, msg, ba.len);
  return ba;
}

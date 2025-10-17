#ifndef ARRAY_SERVICE_MODEL_AGENT_H
#define ARRAY_SERVICE_MODEL_AGENT_H

#include <stddef.h>
#include <stdint.h>

#include "../sm_agent.h"   // path from src/sm/array_sm/ → src/sm/sm_agent.h

#ifdef __cplusplus
extern "C" {
#endif

__attribute__ ((visibility ("default")))
sm_agent_t* make_array_sm_agent(sm_io_ag_ran_t io);

#ifdef __cplusplus
}
#endif

#endif /* ARRAY_SERVICE_MODEL_AGENT_H */


#ifndef MICRLINK_TASK_H
#define MICRLINK_TASK_H

#include "microlink/status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ml_task_state {
    ML_TASK_DORMANT = 0,
    ML_TASK_READY,
    ML_TASK_RUNNING,
    ML_TASK_BLOCKED,
    ML_TASK_DELAYED
} ml_task_state_t;

struct ml_sys;

typedef void (*ml_task_fn)(struct ml_sys *sys);

typedef struct ml_task {
    const char *name;
    ml_task_fn fn;
    uint8_t priority;
    ml_task_state_t state;
    uint32_t delay_ticks;
    uint32_t run_count;
    uint32_t notify;
} ml_task_t;

#ifdef __cplusplus
}
#endif

#endif

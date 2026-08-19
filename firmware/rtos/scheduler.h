#ifndef MICRLINK_SCHEDULER_H
#define MICRLINK_SCHEDULER_H

#include "queue.h"
#include "semaphore.h"
#include "task.h"
#include "tick.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ML_RTOS_MAX_TASKS 8

struct ml_sys;

typedef struct ml_rtos {
    ml_task_t tasks[ML_RTOS_MAX_TASKS];
    uint8_t ntasks;
    int current;
    ml_rtos_tick_t tick;
    ml_queue_t events;
    ml_sem_t timer_sem;
    ml_mutex_t log_mutex;
    uint32_t switches;
} ml_rtos_t;

void ml_rtos_init(ml_rtos_t *rtos);
ml_status_t ml_rtos_add_task(ml_rtos_t *rtos, const char *name, ml_task_fn fn, uint8_t prio);
void ml_rtos_tick(struct ml_sys *sys);
void ml_rtos_schedule(struct ml_sys *sys);
void ml_rtos_delay_current(struct ml_sys *sys, uint32_t ticks);
void ml_rtos_notify(ml_rtos_t *rtos, int task_id);

#ifdef __cplusplus
}
#endif

#endif

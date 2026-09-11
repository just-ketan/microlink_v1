#include "scheduler.h"

#include "microlink/sys.h"

#include <string.h>

void ml_rtos_init(ml_rtos_t *rtos)
{
    memset(rtos, 0, sizeof(*rtos));
    rtos->current = -1;
    ml_queue_init(&rtos->events);
    ml_sem_init(&rtos->timer_sem, 0);
    ml_mutex_init(&rtos->log_mutex);
}

ml_status_t ml_rtos_add_task(ml_rtos_t *rtos, const char *name, ml_task_fn fn, uint8_t prio)
{
    if (rtos->ntasks >= ML_RTOS_MAX_TASKS || fn == NULL) {
        return ML_ERR_NOMEM;
    }
    ml_task_t *t = &rtos->tasks[rtos->ntasks++];
    t->name = name;
    t->fn = fn;
    t->priority = prio;
    t->state = ML_TASK_READY;
    t->delay_ticks = 0;
    t->run_count = 0;
    t->notify = 0;
    return ML_OK;
}

void ml_rtos_tick(ml_sys_t *sys)
{
    ml_rtos_t *rtos = &sys->rtos;
    rtos->tick.ticks++;
    for (uint8_t i = 0; i < rtos->ntasks; i++) {
        ml_task_t *t = &rtos->tasks[i];
        if (t->state == ML_TASK_DELAYED && t->delay_ticks > 0u) {
            t->delay_ticks--;
            if (t->delay_ticks == 0u) {
                t->state = ML_TASK_READY;
            }
        }
    }
}

void ml_rtos_delay_current(ml_sys_t *sys, uint32_t ticks)
{
    if (sys->rtos.current < 0) {
        return;
    }
    ml_task_t *t = &sys->rtos.tasks[sys->rtos.current];
    t->delay_ticks = ticks;
    t->state = ML_TASK_DELAYED;
}

void ml_rtos_notify(ml_rtos_t *rtos, int task_id)
{
    if (task_id < 0 || task_id >= (int)rtos->ntasks) {
        return;
    }
    rtos->tasks[task_id].notify++;
    if (rtos->tasks[task_id].state == ML_TASK_BLOCKED) {
        rtos->tasks[task_id].state = ML_TASK_READY;
    }
}

void ml_rtos_schedule(ml_sys_t *sys)
{
    ml_rtos_t *rtos = &sys->rtos;
    int best = -1;
    uint8_t best_p = 0;
    for (uint8_t i = 0; i < rtos->ntasks; i++) {
        if (rtos->tasks[i].state == ML_TASK_READY) {
            if (best < 0 || rtos->tasks[i].priority >= best_p) {
                best = (int)i;
                best_p = rtos->tasks[i].priority;
            }
        }
    }
    if (best < 0) {
        return;
    }
    rtos->current = best;
    rtos->tasks[best].state = ML_TASK_RUNNING;
    rtos->switches++;
    rtos->tasks[best].fn(sys);
    if (rtos->tasks[best].state == ML_TASK_RUNNING) {
        rtos->tasks[best].state = ML_TASK_READY;
    }
    rtos->tasks[best].run_count++;
    rtos->current = -1;
}

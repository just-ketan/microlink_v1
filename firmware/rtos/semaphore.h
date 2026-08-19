#ifndef MICRLINK_SEMAPHORE_H
#define MICRLINK_SEMAPHORE_H

#include "microlink/status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_sem {
    int32_t count;
} ml_sem_t;

typedef struct ml_mutex {
    int locked;
    int owner;
} ml_mutex_t;

void ml_sem_init(ml_sem_t *s, int32_t initial);
ml_status_t ml_sem_give(ml_sem_t *s);
ml_status_t ml_sem_take(ml_sem_t *s);

void ml_mutex_init(ml_mutex_t *m);
ml_status_t ml_mutex_lock(ml_mutex_t *m, int owner);
ml_status_t ml_mutex_unlock(ml_mutex_t *m, int owner);

#ifdef __cplusplus
}
#endif

#endif

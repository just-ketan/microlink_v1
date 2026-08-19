#include "semaphore.h"

void ml_sem_init(ml_sem_t *s, int32_t initial)
{
    s->count = initial;
}

ml_status_t ml_sem_give(ml_sem_t *s)
{
    s->count++;
    return ML_OK;
}

ml_status_t ml_sem_take(ml_sem_t *s)
{
    if (s->count <= 0) {
        return ML_ERR_TIMEOUT;
    }
    s->count--;
    return ML_OK;
}

void ml_mutex_init(ml_mutex_t *m)
{
    m->locked = 0;
    m->owner = -1;
}

ml_status_t ml_mutex_lock(ml_mutex_t *m, int owner)
{
    if (m->locked) {
        return ML_ERR_BUSY;
    }
    m->locked = 1;
    m->owner = owner;
    return ML_OK;
}

ml_status_t ml_mutex_unlock(ml_mutex_t *m, int owner)
{
    if (!m->locked || m->owner != owner) {
        return ML_ERR_STATE;
    }
    m->locked = 0;
    m->owner = -1;
    return ML_OK;
}

#include "queue.h"

void ml_queue_init(ml_queue_t *q)
{
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

ml_status_t ml_queue_push(ml_queue_t *q, uint32_t v)
{
    if (q->count >= ML_QUEUE_CAP) {
        return ML_ERR_FULL;
    }
    q->data[q->head] = v;
    q->head = (uint8_t)((q->head + 1u) % ML_QUEUE_CAP);
    q->count++;
    return ML_OK;
}

ml_status_t ml_queue_pop(ml_queue_t *q, uint32_t *v)
{
    if (q->count == 0u) {
        return ML_ERR_EMPTY;
    }
    *v = q->data[q->tail];
    q->tail = (uint8_t)((q->tail + 1u) % ML_QUEUE_CAP);
    q->count--;
    return ML_OK;
}

#ifndef MICRLINK_QUEUE_H
#define MICRLINK_QUEUE_H

#include "microlink/status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ML_QUEUE_CAP 16

typedef struct ml_queue {
    uint32_t data[ML_QUEUE_CAP];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} ml_queue_t;

void ml_queue_init(ml_queue_t *q);
ml_status_t ml_queue_push(ml_queue_t *q, uint32_t v);
ml_status_t ml_queue_pop(ml_queue_t *q, uint32_t *v);

#ifdef __cplusplus
}
#endif

#endif

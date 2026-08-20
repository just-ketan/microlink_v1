#include "cycle_model.h"

#include <string.h>

void ml_cycle_model_init(ml_cycle_model_t *cm)
{
    if (cm != NULL) {
        memset(cm, 0, sizeof(*cm));
    }
}

void ml_cycle_model_consume(ml_cycle_model_t *cm, uint32_t n)
{
    if (cm != NULL) {
        cm->cycles += n;
    }
}

void ml_cycle_model_stall(ml_cycle_model_t *cm, uint32_t n)
{
    if (cm != NULL) {
        cm->stalls += n;
        cm->cycles += n;
    }
}

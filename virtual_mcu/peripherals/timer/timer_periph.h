#ifndef MICRLINK_TIMER_PERIPH_H
#define MICRLINK_TIMER_PERIPH_H

#include "../../registers/register.h"
#include "../gpio/gpio_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_timer {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    uint32_t pres_acc;
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
    int fail;
} ml_timer_t;

ml_status_t ml_timer_init(ml_timer_t *t, ml_raise_irq_fn raise, void *ctx);
void ml_timer_tick(ml_timer_t *t);

#ifdef __cplusplus
}
#endif

#endif

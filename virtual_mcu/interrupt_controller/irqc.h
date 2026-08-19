#ifndef MICRLINK_IRQC_H
#define MICRLINK_IRQC_H

#include "../registers/register.h"
#include "microlink/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ml_isr_fn)(void *ctx, uint32_t irq);

typedef struct ml_irqc {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    ml_isr_fn isr[ML_IRQ_COUNT];
    void *isr_ctx[ML_IRQ_COUNT];
    uint8_t prio[ML_IRQ_COUNT];
} ml_irqc_t;

ml_status_t ml_irqc_init(ml_irqc_t *irqc);
void ml_irqc_raise(ml_irqc_t *irqc, uint32_t irq);
void ml_irqc_clear(ml_irqc_t *irqc, uint32_t irq);
void ml_irqc_set_isr(ml_irqc_t *irqc, uint32_t irq, ml_isr_fn fn, void *ctx);
int ml_irqc_dispatch(ml_irqc_t *irqc);

#ifdef __cplusplus
}
#endif

#endif

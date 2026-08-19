#include "irqc.h"

#include "../registers/register_definitions.h"

#include <string.h>

static void irqc_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_irqc_t *irqc = (ml_irqc_t *)owner;
    (void)old_value;
    if (offset == ML_IRQC_REG_PRIORITY) {
        for (uint32_t i = 0; i < ML_IRQ_COUNT; i++) {
            irqc->prio[i] = (uint8_t)((new_value >> (i * 4u)) & 0xFu);
        }
    }
}

ml_status_t ml_irqc_init(ml_irqc_t *irqc)
{
    memset(irqc, 0, sizeof(*irqc));
    return ml_reg_bank_init(&irqc->bank, ML_IRQC_BASE, ML_REGION_SIZE, irqc->storage,
                            ml_irqc_regs, ml_irqc_reg_count, irqc, irqc_on_write, NULL);
}

void ml_irqc_raise(ml_irqc_t *irqc, uint32_t irq)
{
    if (irq >= ML_IRQ_COUNT) {
        return;
    }
    uint32_t pending = ml_reg_peek32(&irqc->bank, ML_IRQC_REG_PENDING);
    ml_reg_poke32(&irqc->bank, ML_IRQC_REG_PENDING, pending | (1u << irq));
}

void ml_irqc_clear(ml_irqc_t *irqc, uint32_t irq)
{
    if (irq >= ML_IRQ_COUNT) {
        return;
    }
    uint32_t pending = ml_reg_peek32(&irqc->bank, ML_IRQC_REG_PENDING);
    ml_reg_poke32(&irqc->bank, ML_IRQC_REG_PENDING, pending & ~(1u << irq));
}

void ml_irqc_set_isr(ml_irqc_t *irqc, uint32_t irq, ml_isr_fn fn, void *ctx)
{
    if (irq >= ML_IRQ_COUNT) {
        return;
    }
    irqc->isr[irq] = fn;
    irqc->isr_ctx[irq] = ctx;
}

int ml_irqc_dispatch(ml_irqc_t *irqc)
{
    uint32_t pending = ml_reg_peek32(&irqc->bank, ML_IRQC_REG_PENDING);
    uint32_t enable = ml_reg_peek32(&irqc->bank, ML_IRQC_REG_ENABLE);
    uint32_t mask = ml_reg_peek32(&irqc->bank, ML_IRQC_REG_MASK);
    uint32_t active = pending & enable & ~mask;
    if (active == 0u) {
        return 0;
    }
    uint32_t best = ML_IRQ_COUNT;
    uint8_t best_prio = 0;
    for (uint32_t i = 0; i < ML_IRQ_COUNT; i++) {
        if ((active & (1u << i)) != 0u) {
            if (best == ML_IRQ_COUNT || irqc->prio[i] >= best_prio) {
                best = i;
                best_prio = irqc->prio[i];
            }
        }
    }
    if (best < ML_IRQ_COUNT && irqc->isr[best] != NULL) {
        irqc->isr[best](irqc->isr_ctx[best], best);
        return 1;
    }
    return 0;
}

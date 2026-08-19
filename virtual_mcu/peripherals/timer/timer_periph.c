#include "timer_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

ml_status_t ml_timer_init(ml_timer_t *t, ml_raise_irq_fn raise, void *ctx)
{
    memset(t, 0, sizeof(*t));
    t->raise_irq = raise;
    t->irq_ctx = ctx;
    return ml_reg_bank_init(&t->bank, ML_TIMER_BASE, ML_REGION_SIZE, t->storage, ml_timer_regs,
                            ml_timer_reg_count, t, NULL, NULL);
}

void ml_timer_tick(ml_timer_t *t)
{
    if (t->fail) {
        return;
    }
    uint32_t ctrl = ml_reg_peek32(&t->bank, ML_TMR_REG_CTRL);
    if ((ctrl & ML_TMR_CTRL_EN) == 0u) {
        return;
    }
    uint32_t pres = ml_reg_peek32(&t->bank, ML_TMR_REG_PRESCALER);
    t->pres_acc++;
    if (t->pres_acc <= pres) {
        return;
    }
    t->pres_acc = 0;
    uint32_t cnt = ml_reg_peek32(&t->bank, ML_TMR_REG_COUNTER);
    uint32_t next = cnt + 1u;
    uint32_t st = ml_reg_peek32(&t->bank, ML_TMR_REG_STATUS);
    if (next < cnt) {
        st |= ML_TMR_STAT_OVF;
    }
    uint32_t cmp = ml_reg_peek32(&t->bank, ML_TMR_REG_COMPARE);
    if (next == cmp) {
        st |= ML_TMR_STAT_CMP;
        if (t->raise_irq != NULL) {
            t->raise_irq(t->irq_ctx, ML_IRQ_TIMER);
        }
    }
    ml_reg_poke32(&t->bank, ML_TMR_REG_COUNTER, next);
    ml_reg_poke32(&t->bank, ML_TMR_REG_STATUS, st);
}

#include "radio_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

static void radio_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_radio_t *r = (ml_radio_t *)owner;
    (void)old_value;
    if (offset != ML_RADIO_REG_CMD) {
        return;
    }
    uint32_t rssi = 0;
    uint32_t snr = 0;
    ml_status_t st = ML_OK;
    if (r->cmd_fn != NULL) {
        st = r->cmd_fn(r->cmd_ctx, new_value, ml_reg_peek32(&r->bank, ML_RADIO_REG_FREQ),
                       ml_reg_peek32(&r->bank, ML_RADIO_REG_GAIN),
                       ml_reg_peek32(&r->bank, ML_RADIO_REG_MOD),
                       ml_reg_peek32(&r->bank, ML_RADIO_REG_BW),
                       ml_reg_peek32(&r->bank, ML_RADIO_REG_FILTER), &rssi, &snr);
    }
    ml_reg_poke32(&r->bank, ML_RADIO_REG_RSSI, rssi);
    ml_reg_poke32(&r->bank, ML_RADIO_REG_SNR, snr);
    ml_reg_poke32(&r->bank, ML_RADIO_REG_STATUS, (st == ML_OK) ? 1u : 0x80000000u);
    if (r->raise_irq != NULL) {
        r->raise_irq(r->irq_ctx, ML_IRQ_RADIO);
    }
}

ml_status_t ml_radio_init(ml_radio_t *r, ml_raise_irq_fn raise, void *ctx)
{
    memset(r, 0, sizeof(*r));
    r->raise_irq = raise;
    r->irq_ctx = ctx;
    return ml_reg_bank_init(&r->bank, ML_RADIO_BASE, ML_REGION_SIZE, r->storage, ml_radio_regs,
                            ml_radio_reg_count, r, radio_on_write, NULL);
}

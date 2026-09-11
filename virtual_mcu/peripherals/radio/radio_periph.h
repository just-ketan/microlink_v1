#ifndef MICRLINK_RADIO_PERIPH_H
#define MICRLINK_RADIO_PERIPH_H

#include "../../registers/register.h"
#include "../gpio/gpio_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ml_status_t (*ml_radio_cmd_fn)(void *ctx, uint32_t cmd, uint32_t freq_khz, uint32_t gain,
                                       uint32_t mod, uint32_t bw, uint32_t filter_sel,
                                       uint32_t *rssi_out, uint32_t *snr_out);

typedef struct ml_radio {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
    ml_radio_cmd_fn cmd_fn;
    void *cmd_ctx;
} ml_radio_t;

ml_status_t ml_radio_init(ml_radio_t *r, ml_raise_irq_fn raise, void *ctx);

#ifdef __cplusplus
}
#endif

#endif

#ifndef MICRLINK_SPI_PERIPH_H
#define MICRLINK_SPI_PERIPH_H

#include "../../registers/register.h"
#include "../gpio/gpio_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_spi {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    uint8_t slave_mem[256];
    uint8_t last_rx;
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
    int fail;
} ml_spi_t;

ml_status_t ml_spi_init(ml_spi_t *s, ml_raise_irq_fn raise, void *ctx);

#ifdef __cplusplus
}
#endif

#endif

#ifndef MICRLINK_I2C_PERIPH_H
#define MICRLINK_I2C_PERIPH_H

#include "../../registers/register.h"
#include "../gpio/gpio_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_i2c {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    uint8_t slave_addr;
    uint8_t slave_mem[256];
    uint8_t mem_ptr;
    int started;
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
} ml_i2c_t;

ml_status_t ml_i2c_init(ml_i2c_t *i, ml_raise_irq_fn raise, void *ctx);

#ifdef __cplusplus
}
#endif

#endif

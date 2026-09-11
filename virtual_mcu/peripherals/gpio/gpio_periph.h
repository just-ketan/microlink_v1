#ifndef MICRLINK_GPIO_PERIPH_H
#define MICRLINK_GPIO_PERIPH_H

#include "../../registers/register.h"
#include "microlink/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ml_raise_irq_fn)(void *ctx, uint32_t irq);

typedef struct ml_gpio {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    uint16_t output;
    uint16_t input_ext;
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
    int stuck_bit;
} ml_gpio_t;

ml_status_t ml_gpio_init(ml_gpio_t *g, ml_raise_irq_fn raise, void *ctx);
void ml_gpio_inject_input(ml_gpio_t *g, uint32_t pin, int level);
uint32_t ml_gpio_read_pins(const ml_gpio_t *g);

#ifdef __cplusplus
}
#endif

#endif

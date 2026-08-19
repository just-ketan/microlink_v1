#include "gpio_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

static uint16_t gpio_effective(const ml_gpio_t *g)
{
    uint16_t dir = (uint16_t)ml_reg_peek32(&g->bank, ML_GPIO_REG_DIR);
    uint16_t data = (uint16_t)ml_reg_peek32(&g->bank, ML_GPIO_REG_DATA);
    uint16_t pins = (uint16_t)((data & dir) | (g->input_ext & (uint16_t)~dir));
    if (g->stuck_bit >= 0 && g->stuck_bit < 16) {
        pins |= (uint16_t)(1u << g->stuck_bit);
    }
    return pins;
}

static void gpio_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_gpio_t *g = (ml_gpio_t *)owner;
    (void)old_value;
    if (offset == ML_GPIO_REG_INJECT) {
        g->input_ext = (uint16_t)new_value;
    }
    if (offset == ML_GPIO_REG_DATA || offset == ML_GPIO_REG_DIR) {
        g->output = gpio_effective(g);
        ml_reg_poke32(&g->bank, ML_GPIO_REG_STATUS, g->output);
        if (g->raise_irq != NULL) {
            g->raise_irq(g->irq_ctx, ML_IRQ_GPIO);
        }
    }
}

static void gpio_on_read(void *owner, uint32_t offset, uint32_t *value)
{
    ml_gpio_t *g = (ml_gpio_t *)owner;
    if (offset == ML_GPIO_REG_DATA || offset == ML_GPIO_REG_STATUS) {
        *value = gpio_effective(g);
    }
}

ml_status_t ml_gpio_init(ml_gpio_t *g, ml_raise_irq_fn raise, void *ctx)
{
    memset(g, 0, sizeof(*g));
    g->raise_irq = raise;
    g->irq_ctx = ctx;
    g->stuck_bit = -1;
    return ml_reg_bank_init(&g->bank, ML_GPIO_BASE, ML_REGION_SIZE, g->storage, ml_gpio_regs,
                            ml_gpio_reg_count, g, gpio_on_write, gpio_on_read);
}

void ml_gpio_inject_input(ml_gpio_t *g, uint32_t pin, int level)
{
    if (pin >= ML_GPIO_PINS) {
        return;
    }
    if (level) {
        g->input_ext = (uint16_t)(g->input_ext | (uint16_t)(1u << pin));
    } else {
        g->input_ext = (uint16_t)(g->input_ext & (uint16_t)~(1u << pin));
    }
    ml_reg_poke32(&g->bank, ML_GPIO_REG_INJECT, g->input_ext);
}

uint32_t ml_gpio_read_pins(const ml_gpio_t *g)
{
    return gpio_effective(g);
}

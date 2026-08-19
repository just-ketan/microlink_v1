#include "spi_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

static void spi_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_spi_t *s = (ml_spi_t *)owner;
    (void)old_value;
    if (offset == ML_SPI_REG_TX) {
        if (s->fail) {
            ml_reg_poke32(&s->bank, ML_SPI_REG_STATUS, 0);
            return;
        }
        uint8_t tx = (uint8_t)(new_value & 0xFFu);
        uint32_t cs = ml_reg_peek32(&s->bank, ML_SPI_REG_CS);
        uint8_t rx = 0xFFu;
        if (cs == 0u) {
            rx = (uint8_t)(s->slave_mem[tx] ^ 0x5Au);
            s->slave_mem[tx] = tx;
        }
        s->last_rx = rx;
        ml_reg_poke32(&s->bank, ML_SPI_REG_RX, rx);
        ml_reg_poke32(&s->bank, ML_SPI_REG_STATUS, ML_SPI_STAT_DONE);
        if (s->raise_irq != NULL) {
            s->raise_irq(s->irq_ctx, ML_IRQ_SPI);
        }
    }
}

ml_status_t ml_spi_init(ml_spi_t *s, ml_raise_irq_fn raise, void *ctx)
{
    memset(s, 0, sizeof(*s));
    s->raise_irq = raise;
    s->irq_ctx = ctx;
    for (int i = 0; i < 256; i++) {
        s->slave_mem[i] = (uint8_t)i;
    }
    return ml_reg_bank_init(&s->bank, ML_SPI_BASE, ML_REGION_SIZE, s->storage, ml_spi_regs,
                            ml_spi_reg_count, s, spi_on_write, NULL);
}

#include "i2c_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

static void i2c_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_i2c_t *i = (ml_i2c_t *)owner;
    (void)old_value;
    if (offset != ML_I2C_REG_CTRL) {
        return;
    }
    uint32_t ctrl = new_value;
    if ((ctrl & ML_I2C_CTRL_START) != 0u) {
        i->started = 1;
        uint32_t addr = ml_reg_peek32(&i->bank, ML_I2C_REG_ADDR) & 0x7Fu;
        uint32_t ack = (addr == i->slave_addr) ? ML_I2C_STAT_ACK : 0u;
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, ack | ML_I2C_STAT_BUSY);
        ml_reg_poke32(&i->bank, ML_I2C_REG_CTRL, ctrl & ~(uint32_t)ML_I2C_CTRL_START);
        return;
    }
    if ((ctrl & ML_I2C_CTRL_STOP) != 0u) {
        i->started = 0;
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, 0);
        ml_reg_poke32(&i->bank, ML_I2C_REG_CTRL, ctrl & ~(uint32_t)ML_I2C_CTRL_STOP);
        return;
    }
    if (!i->started) {
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, 0);
        return;
    }
    uint32_t addr = ml_reg_peek32(&i->bank, ML_I2C_REG_ADDR) & 0x7Fu;
    if (addr != i->slave_addr) {
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, ML_I2C_STAT_BUSY);
        return;
    }
    if ((ctrl & ML_I2C_CTRL_READ) != 0u) {
        uint8_t b = i->slave_mem[i->mem_ptr];
        i->mem_ptr = (uint8_t)(i->mem_ptr + 1u);
        ml_reg_poke32(&i->bank, ML_I2C_REG_DATA, b);
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, ML_I2C_STAT_ACK);
    } else {
        uint8_t b = (uint8_t)(ml_reg_peek32(&i->bank, ML_I2C_REG_DATA) & 0xFFu);
        i->slave_mem[i->mem_ptr] = b;
        i->mem_ptr = (uint8_t)(i->mem_ptr + 1u);
        ml_reg_poke32(&i->bank, ML_I2C_REG_STATUS, ML_I2C_STAT_ACK);
    }
    if (i->raise_irq != NULL) {
        i->raise_irq(i->irq_ctx, ML_IRQ_I2C);
    }
}

ml_status_t ml_i2c_init(ml_i2c_t *i, ml_raise_irq_fn raise, void *ctx)
{
    memset(i, 0, sizeof(*i));
    i->raise_irq = raise;
    i->irq_ctx = ctx;
    i->slave_addr = 0x50u;
    for (int n = 0; n < 256; n++) {
        i->slave_mem[n] = (uint8_t)n;
    }
    return ml_reg_bank_init(&i->bank, ML_I2C_BASE, ML_REGION_SIZE, i->storage, ml_i2c_regs,
                            ml_i2c_reg_count, i, i2c_on_write, NULL);
}

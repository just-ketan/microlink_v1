#include "mcu.h"

#include "../registers/register.h"

#include <string.h>

static void mcu_raise_irq(void *ctx, uint32_t irq)
{
    ml_mcu_t *mcu = (ml_mcu_t *)ctx;
    ml_irqc_raise(&mcu->irqc, irq);
}

ml_status_t ml_mcu_init(ml_mcu_t *mcu)
{
    ml_status_t st;
    if (mcu == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    memset(mcu, 0, sizeof(*mcu));
    ml_fault_cfg_clear(&mcu->faults);
    ml_clock_init(&mcu->clock, 1000u);
    ml_cycle_model_init(&mcu->cycles);
    st = ml_memory_map_init(&mcu->map);
    if (st != ML_OK) {
        return st;
    }
    st = ml_gpio_init(&mcu->gpio, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    st = ml_uart_init(&mcu->uart, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    st = ml_spi_init(&mcu->spi, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    st = ml_i2c_init(&mcu->i2c, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    st = ml_timer_init(&mcu->timer, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    st = ml_irqc_init(&mcu->irqc);
    if (st != ML_OK) {
        return st;
    }
    st = ml_radio_init(&mcu->radio, mcu_raise_irq, mcu);
    if (st != ML_OK) {
        return st;
    }
    ml_memory_map_add(&mcu->map, &mcu->gpio.bank);
    ml_memory_map_add(&mcu->map, &mcu->uart.bank);
    ml_memory_map_add(&mcu->map, &mcu->spi.bank);
    ml_memory_map_add(&mcu->map, &mcu->i2c.bank);
    ml_memory_map_add(&mcu->map, &mcu->timer.bank);
    ml_memory_map_add(&mcu->map, &mcu->irqc.bank);
    ml_memory_map_add(&mcu->map, &mcu->radio.bank);
    return ML_OK;
}

void ml_mcu_apply_faults(ml_mcu_t *mcu)
{
    mcu->gpio.stuck_bit = mcu->faults.stuck_gpio_bit;
    mcu->uart.rx_corrupt_mask = (uint8_t)mcu->faults.uart_rx_corrupt_mask;
    mcu->spi.fail = mcu->faults.spi_fail;
    mcu->timer.fail = mcu->faults.timer_fail;
    if (mcu->faults.gpio_corrupt) {
        mcu->gpio.output ^= 0xFFFFu;
    }
}

void ml_mcu_tick(ml_mcu_t *mcu)
{
    ml_clock_tick(&mcu->clock);
    ml_cycle_model_consume(&mcu->cycles, ML_CYCLES_PER_TICK);
    uint32_t tctrl = ml_reg_peek32(&mcu->timer.bank, ML_TMR_REG_CTRL);
    if ((tctrl & ML_TMR_CTRL_EN) != 0u) {
        ml_cycle_model_consume(&mcu->cycles, ML_CYCLES_TIMER_ACTIVE);
    }
    ml_timer_tick(&mcu->timer);
    if (ml_irqc_dispatch(&mcu->irqc)) {
        mcu->irq_dispatches++;
        mcu->cycles.irq_events++;
        ml_cycle_model_consume(&mcu->cycles, ML_CYCLES_IRQ_DISPATCH);
    }
}

ml_status_t ml_mcu_read32(ml_mcu_t *mcu, uint32_t addr, uint32_t *out)
{
    ml_status_t st = ml_memory_map_read32(&mcu->map, addr, out);
    if (st == ML_OK) {
        mcu->cycles.mmio_reads++;
        ml_cycle_model_consume(&mcu->cycles, ML_CYCLES_MMIO_READ);
    } else if (st == ML_ERR_FAULT) {
        ml_cycle_model_stall(&mcu->cycles, ML_CYCLES_FAULT_STALL);
    }
    return st;
}

ml_status_t ml_mcu_write32(ml_mcu_t *mcu, uint32_t addr, uint32_t value)
{
    ml_status_t st = ml_memory_map_write32(&mcu->map, addr, value);
    if (st == ML_OK) {
        mcu->cycles.mmio_writes++;
        ml_cycle_model_consume(&mcu->cycles, ML_CYCLES_MMIO_WRITE);
    } else if (st == ML_ERR_FAULT) {
        ml_cycle_model_stall(&mcu->cycles, ML_CYCLES_FAULT_STALL);
    }
    return st;
}

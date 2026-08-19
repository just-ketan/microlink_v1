#include "i2c_driver.h"

ml_status_t i2c_driver_init(ml_sys_t *sys, uint32_t khz)
{
    ml_status_t st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CLOCK, khz == 0u ? 100u : khz);
    if (st != ML_OK) {
        return st;
    }
    return ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN);
}

static ml_status_t i2c_start(ml_sys_t *sys, uint8_t addr)
{
    ml_status_t st;
    uint32_t status = 0;
    st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_ADDR, addr);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN | ML_I2C_CTRL_START);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_read32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_STATUS, &status);
    if (st != ML_OK) {
        return st;
    }
    if ((status & ML_I2C_STAT_ACK) == 0u) {
        return ML_ERR_NACK;
    }
    return ML_OK;
}

ml_status_t i2c_driver_write(ml_sys_t *sys, uint8_t addr, uint8_t data)
{
    ml_status_t st = i2c_start(sys, addr);
    uint32_t status = 0;
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_DATA, data);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_read32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_STATUS, &status);
    if (st != ML_OK) {
        return st;
    }
    (void)ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN | ML_I2C_CTRL_STOP);
    if ((status & ML_I2C_STAT_ACK) == 0u) {
        return ML_ERR_NACK;
    }
    return ML_OK;
}

ml_status_t i2c_driver_read(ml_sys_t *sys, uint8_t addr, uint8_t *data)
{
    uint32_t v = 0;
    ml_status_t st;
    if (data == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    st = i2c_start(sys, addr);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN | ML_I2C_CTRL_READ);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_read32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_DATA, &v);
    (void)ml_mcu_write32(&sys->mcu, ML_I2C_BASE + ML_I2C_REG_CTRL, ML_I2C_CTRL_EN | ML_I2C_CTRL_STOP);
    if (st != ML_OK) {
        return st;
    }
    *data = (uint8_t)v;
    return ML_OK;
}

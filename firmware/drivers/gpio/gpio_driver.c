#include "gpio_driver.h"

ml_status_t gpio_driver_init(ml_sys_t *sys)
{
    if (sys == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    return ml_mcu_write32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_CTRL, 1u);
}

ml_status_t gpio_driver_set_dir(ml_sys_t *sys, uint32_t pin, int output)
{
    uint32_t dir = 0;
    ml_status_t st;
    if (pin >= ML_GPIO_PINS) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_read32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_DIR, &dir);
    if (st != ML_OK) {
        return st;
    }
    if (output) {
        dir |= (1u << pin);
    } else {
        dir &= ~(1u << pin);
    }
    return ml_mcu_write32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_DIR, dir);
}

ml_status_t gpio_driver_write(ml_sys_t *sys, uint32_t pin, int level)
{
    uint32_t data = 0;
    ml_status_t st;
    if (pin >= ML_GPIO_PINS) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_read32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, &data);
    if (st != ML_OK) {
        return st;
    }
    if (level) {
        data |= (1u << pin);
    } else {
        data &= ~(1u << pin);
    }
    return ml_mcu_write32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, data);
}

ml_status_t gpio_driver_read(ml_sys_t *sys, uint32_t pin, int *level)
{
    uint32_t data = 0;
    ml_status_t st;
    if (pin >= ML_GPIO_PINS || level == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_read32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, &data);
    if (st != ML_OK) {
        return st;
    }
    *level = (int)((data >> pin) & 1u);
    return ML_OK;
}

ml_status_t gpio_driver_set_mux(ml_sys_t *sys, uint32_t pin, int alt)
{
    uint32_t mux = 0;
    ml_status_t st;
    if (pin >= ML_GPIO_PINS) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_read32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_MUX, &mux);
    if (st != ML_OK) {
        return st;
    }
    if (alt) {
        mux |= (1u << pin);
    } else {
        mux &= ~(1u << pin);
    }
    return ml_mcu_write32(&sys->mcu, ML_GPIO_BASE + ML_GPIO_REG_MUX, mux);
}

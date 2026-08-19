#include "timer_driver.h"

ml_status_t timer_driver_init(ml_sys_t *sys, uint32_t compare, uint32_t prescaler)
{
    ml_status_t st = ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_COMPARE, compare);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_PRESCALER, prescaler);
    if (st != ML_OK) {
        return st;
    }
    return ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_COUNTER, 0u);
}

ml_status_t timer_driver_start(ml_sys_t *sys)
{
    return ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_CTRL, ML_TMR_CTRL_EN);
}

ml_status_t timer_driver_stop(ml_sys_t *sys)
{
    return ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_CTRL, 0u);
}

ml_status_t timer_driver_clear_status(ml_sys_t *sys)
{
    return ml_mcu_write32(&sys->mcu, ML_TIMER_BASE + ML_TMR_REG_STATUS, 0u);
}

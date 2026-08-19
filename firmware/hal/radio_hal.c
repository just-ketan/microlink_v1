#include "radio_hal.h"

ml_status_t radio_hal_init(ml_sys_t *sys)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_CTRL, ML_RADIO_CTRL_EN);
}

ml_status_t radio_hal_set_freq(ml_sys_t *sys, uint32_t khz)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_FREQ, khz);
}

ml_status_t radio_hal_set_gain(ml_sys_t *sys, uint32_t gain)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_GAIN, gain);
}

ml_status_t radio_hal_set_mod(ml_sys_t *sys, uint32_t mod)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_MOD, mod);
}

ml_status_t radio_hal_set_bw(ml_sys_t *sys, uint32_t bw)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_BW, bw);
}

ml_status_t radio_hal_set_filter(ml_sys_t *sys, uint32_t sel)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_FILTER, sel);
}

ml_status_t radio_hal_command(ml_sys_t *sys, uint32_t cmd)
{
    return ml_mcu_write32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_CMD, cmd);
}

ml_status_t radio_hal_read_rssi(ml_sys_t *sys, uint32_t *rssi)
{
    if (rssi == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    return ml_mcu_read32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_RSSI, rssi);
}

ml_status_t radio_hal_read_snr(ml_sys_t *sys, uint32_t *snr)
{
    if (snr == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    return ml_mcu_read32(&sys->mcu, ML_RADIO_BASE + ML_RADIO_REG_SNR, snr);
}

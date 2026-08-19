#include "radio_fw.h"

#include "firmware/hal/radio_hal.h"

ml_status_t radio_fw_init(ml_sys_t *sys)
{
    ml_status_t st = radio_hal_init(sys);
    if (st != ML_OK) {
        return st;
    }
    return radio_fw_configure(sys, 2400u, 10u, 1u, 200u, 1u);
}

ml_status_t radio_fw_configure(ml_sys_t *sys, uint32_t freq_khz, uint32_t gain, uint32_t mod,
                               uint32_t bw, uint32_t filter_sel)
{
    ml_status_t st;
    st = radio_hal_set_freq(sys, freq_khz);
    if (st != ML_OK) {
        return st;
    }
    st = radio_hal_set_gain(sys, gain);
    if (st != ML_OK) {
        return st;
    }
    st = radio_hal_set_mod(sys, mod);
    if (st != ML_OK) {
        return st;
    }
    st = radio_hal_set_bw(sys, bw);
    if (st != ML_OK) {
        return st;
    }
    st = radio_hal_set_filter(sys, filter_sel);
    if (st != ML_OK) {
        return st;
    }
    return radio_hal_command(sys, ML_RADIO_CMD_CFG);
}

ml_status_t radio_fw_tx(ml_sys_t *sys)
{
    sys->radio_cycles++;
    return radio_hal_command(sys, ML_RADIO_CMD_TX);
}

ml_status_t radio_fw_rx(ml_sys_t *sys)
{
    ml_status_t st = radio_hal_command(sys, ML_RADIO_CMD_RX);
    if (st != ML_OK) {
        return st;
    }
    (void)radio_hal_read_rssi(sys, &sys->last_rssi);
    (void)radio_hal_read_snr(sys, &sys->last_snr);
    return ML_OK;
}

ml_status_t radio_fw_telemetry(ml_sys_t *sys, uint32_t *rssi, uint32_t *snr)
{
    if (rssi != NULL) {
        *rssi = sys->last_rssi;
    }
    if (snr != NULL) {
        *snr = sys->last_snr;
    }
    return ML_OK;
}

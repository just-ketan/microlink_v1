#ifndef MICRLINK_RADIO_HAL_H
#define MICRLINK_RADIO_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t radio_hal_init(ml_sys_t *sys);
ml_status_t radio_hal_set_freq(ml_sys_t *sys, uint32_t khz);
ml_status_t radio_hal_set_gain(ml_sys_t *sys, uint32_t gain);
ml_status_t radio_hal_set_mod(ml_sys_t *sys, uint32_t mod);
ml_status_t radio_hal_set_bw(ml_sys_t *sys, uint32_t bw);
ml_status_t radio_hal_set_filter(ml_sys_t *sys, uint32_t sel);
ml_status_t radio_hal_command(ml_sys_t *sys, uint32_t cmd);
ml_status_t radio_hal_read_rssi(ml_sys_t *sys, uint32_t *rssi);
ml_status_t radio_hal_read_snr(ml_sys_t *sys, uint32_t *snr);

#ifdef __cplusplus
}
#endif

#endif

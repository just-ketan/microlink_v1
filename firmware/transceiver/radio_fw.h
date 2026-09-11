#ifndef MICRLINK_RADIO_FW_H
#define MICRLINK_RADIO_FW_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t radio_fw_init(ml_sys_t *sys);
ml_status_t radio_fw_configure(ml_sys_t *sys, uint32_t freq_khz, uint32_t gain, uint32_t mod,
                               uint32_t bw, uint32_t filter_sel);
ml_status_t radio_fw_tx(ml_sys_t *sys);
ml_status_t radio_fw_rx(ml_sys_t *sys);
ml_status_t radio_fw_telemetry(ml_sys_t *sys, uint32_t *rssi, uint32_t *snr);

#ifdef __cplusplus
}
#endif

#endif

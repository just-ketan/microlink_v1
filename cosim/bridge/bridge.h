#ifndef MICRLINK_BRIDGE_H
#define MICRLINK_BRIDGE_H

#include "cosim/ipc/tcp_ipc.h"
#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ml_bridge {
    ml_tcp_t tcp;
    uint32_t seq;
    int drop_next;
};

ml_status_t ml_bridge_init(ml_bridge_t *b);
ml_status_t ml_bridge_connect(ml_bridge_t *b, const char *host, uint16_t port);
void ml_bridge_attach(ml_sys_t *sys, ml_bridge_t *b);
ml_status_t ml_bridge_radio_cmd(void *ctx, uint32_t cmd, uint32_t freq_khz, uint32_t gain,
                                uint32_t mod, uint32_t bw, uint32_t filter_sel, uint32_t *rssi_out,
                                uint32_t *snr_out);

#ifdef __cplusplus
}
#endif

#endif

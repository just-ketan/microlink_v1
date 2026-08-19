#include "bridge.h"

#include "cosim/protocol/cosim_protocol.h"
#include "cosim/serialization/jsonish.h"

#include <stdio.h>
#include <string.h>

ml_status_t ml_bridge_init(ml_bridge_t *b)
{
    if (b == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    memset(b, 0, sizeof(*b));
    return ML_OK;
}

ml_status_t ml_bridge_connect(ml_bridge_t *b, const char *host, uint16_t port)
{
    return ml_tcp_connect(&b->tcp, host, port);
}

static ml_status_t local_model(uint32_t cmd, uint32_t freq_khz, uint32_t gain, uint32_t mod,
                               uint32_t bw, uint32_t filter_sel, uint32_t *rssi_out, uint32_t *snr_out)
{
    (void)cmd;
    (void)freq_khz;
    (void)bw;
    *rssi_out = 40u + (gain % 50u) + (filter_sel * 3u);
    *snr_out = 8u + (mod * 4u) + (gain / 8u);
    return ML_OK;
}

ml_status_t ml_bridge_radio_cmd(void *ctx, uint32_t cmd, uint32_t freq_khz, uint32_t gain,
                                uint32_t mod, uint32_t bw, uint32_t filter_sel, uint32_t *rssi_out,
                                uint32_t *snr_out)
{
    ml_sys_t *sys = (ml_sys_t *)ctx;
    ml_bridge_t *b = sys->bridge;
    char line[512];
    char resp[512];
    uint32_t rssi = 0;
    uint32_t snr = 0;

    if (rssi_out == NULL || snr_out == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    if (b == NULL || !b->tcp.connected) {
        return local_model(cmd, freq_khz, gain, mod, bw, filter_sel, rssi_out, snr_out);
    }
    if (b->drop_next) {
        b->drop_next = 0;
        return ML_ERR_IO;
    }
    b->seq++;
    (void)snprintf(line, sizeof(line),
                   "{\"v\":%d,\"ts\":%llu,\"type\":\"TXRX\",\"src\":\"mcu\",\"dst\":\"rf\","
                   "\"seq\":%u,\"cmd\":%u,\"freq_khz\":%u,\"gain\":%u,\"mod\":%u,\"bw\":%u,"
                   "\"filter\":%u}",
                   ML_COSIM_VERSION, (unsigned long long)ml_clock_time_ns(&sys->mcu.clock), b->seq,
                   cmd, freq_khz, gain, mod, bw, filter_sel);
    if (ml_tcp_send_line(&b->tcp, line) != ML_OK) {
        return local_model(cmd, freq_khz, gain, mod, bw, filter_sel, rssi_out, snr_out);
    }
    if (ml_tcp_recv_line(&b->tcp, resp, sizeof(resp)) != ML_OK) {
        return ML_ERR_IO;
    }
    if (!ml_json_get_u32(resp, "rssi", &rssi) || !ml_json_get_u32(resp, "snr", &snr)) {
        return ML_ERR_PROTOCOL;
    }
    *rssi_out = rssi;
    *snr_out = snr;
    return ML_OK;
}

void ml_bridge_attach(ml_sys_t *sys, ml_bridge_t *b)
{
    sys->bridge = b;
    sys->mcu.radio.cmd_fn = ml_bridge_radio_cmd;
    sys->mcu.radio.cmd_ctx = sys;
}

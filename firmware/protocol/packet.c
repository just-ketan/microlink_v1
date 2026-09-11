#include "packet.h"

#include <string.h>

uint16_t ml_crc16(const uint8_t *data, size_t n)
{
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0; i < n; i++) {
        crc = (uint16_t)(crc ^ data[i]);
        for (int b = 0; b < 8; b++) {
            if ((crc & 1u) != 0u) {
                crc = (uint16_t)((crc >> 1) ^ 0xA001u);
            } else {
                crc = (uint16_t)(crc >> 1);
            }
        }
    }
    return crc;
}

ml_status_t ml_packet_init(ml_packet_t *p, uint8_t type, uint16_t seq, const uint8_t *payload, uint16_t len)
{
    if (p == NULL || (len > 0u && payload == NULL) || len > ML_PKT_MAX) {
        return ML_ERR_INVALID_ARG;
    }
    memset(p, 0, sizeof(*p));
    p->version = 1;
    p->type = type;
    p->seq = seq;
    p->length = len;
    if (len > 0u) {
        memcpy(p->payload, payload, len);
    }
    uint8_t tmp[8 + ML_PKT_MAX];
    tmp[0] = p->version;
    tmp[1] = p->type;
    tmp[2] = (uint8_t)(p->seq & 0xFFu);
    tmp[3] = (uint8_t)((p->seq >> 8) & 0xFFu);
    tmp[4] = (uint8_t)(p->length & 0xFFu);
    tmp[5] = (uint8_t)((p->length >> 8) & 0xFFu);
    if (len > 0u) {
        memcpy(tmp + 6, payload, len);
    }
    p->crc = ml_crc16(tmp, (size_t)6u + len);
    return ML_OK;
}

size_t ml_packet_serialize(const ml_packet_t *p, uint8_t *out, size_t cap)
{
    size_t need = (size_t)8u + p->length;
    if (out == NULL || cap < need) {
        return 0;
    }
    out[0] = p->version;
    out[1] = p->type;
    out[2] = (uint8_t)(p->seq & 0xFFu);
    out[3] = (uint8_t)((p->seq >> 8) & 0xFFu);
    out[4] = (uint8_t)(p->length & 0xFFu);
    out[5] = (uint8_t)((p->length >> 8) & 0xFFu);
    if (p->length > 0u) {
        memcpy(out + 6, p->payload, p->length);
    }
    out[6 + p->length] = (uint8_t)(p->crc & 0xFFu);
    out[7 + p->length] = (uint8_t)((p->crc >> 8) & 0xFFu);
    return need;
}

ml_status_t ml_packet_parse(const uint8_t *in, size_t n, ml_packet_t *out)
{
    if (in == NULL || out == NULL || n < 8u) {
        return ML_ERR_PROTOCOL;
    }
    uint16_t len = (uint16_t)(in[4] | ((uint16_t)in[5] << 8));
    if (len > ML_PKT_MAX || n < (size_t)8u + len) {
        return ML_ERR_PROTOCOL;
    }
    uint16_t crc = (uint16_t)(in[6 + len] | ((uint16_t)in[7 + len] << 8));
    if (ml_crc16(in, (size_t)6u + len) != crc) {
        return ML_ERR_PROTOCOL;
    }
    return ml_packet_init(out, in[1], (uint16_t)(in[2] | ((uint16_t)in[3] << 8)), in + 6, len);
}

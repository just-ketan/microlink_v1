#ifndef MICRLINK_PACKET_H
#define MICRLINK_PACKET_H

#include "microlink/status.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ML_PKT_MAX 48

typedef struct ml_packet {
    uint8_t version;
    uint8_t type;
    uint16_t seq;
    uint16_t length;
    uint8_t payload[ML_PKT_MAX];
    uint16_t crc;
} ml_packet_t;

uint16_t ml_crc16(const uint8_t *data, size_t n);
ml_status_t ml_packet_init(ml_packet_t *p, uint8_t type, uint16_t seq, const uint8_t *payload, uint16_t len);
size_t ml_packet_serialize(const ml_packet_t *p, uint8_t *out, size_t cap);
ml_status_t ml_packet_parse(const uint8_t *in, size_t n, ml_packet_t *out);

#ifdef __cplusplus
}
#endif

#endif

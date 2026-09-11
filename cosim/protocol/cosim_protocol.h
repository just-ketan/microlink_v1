#ifndef MICRLINK_COSIM_PROTOCOL_H
#define MICRLINK_COSIM_PROTOCOL_H

#include "microlink/status.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ML_COSIM_VERSION 1

typedef enum ml_cosim_type {
    ML_COSIM_SET_FREQUENCY = 1,
    ML_COSIM_SET_GAIN = 2,
    ML_COSIM_SET_MODULATION = 3,
    ML_COSIM_SET_FILTER = 4,
    ML_COSIM_READ_RSSI = 5,
    ML_COSIM_READ_SNR = 6,
    ML_COSIM_TX_BLOCK = 7,
    ML_COSIM_RX_BLOCK = 8,
    ML_COSIM_RADIO_STATUS = 9,
    ML_COSIM_TXRX = 10
} ml_cosim_type_t;

#ifdef __cplusplus
}
#endif

#endif

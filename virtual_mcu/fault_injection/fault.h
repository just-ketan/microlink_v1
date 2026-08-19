#ifndef MICRLINK_FAULT_H
#define MICRLINK_FAULT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_fault_cfg {
    int stuck_gpio_bit;
    uint32_t uart_rx_corrupt_mask;
    int spi_fail;
    int timer_fail;
    int drop_ipc;
    int gpio_corrupt;
} ml_fault_cfg_t;

void ml_fault_cfg_clear(ml_fault_cfg_t *f);

#ifdef __cplusplus
}
#endif

#endif

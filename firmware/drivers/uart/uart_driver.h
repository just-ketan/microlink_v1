#ifndef MICRLINK_UART_DRIVER_H
#define MICRLINK_UART_DRIVER_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t uart_driver_init(ml_sys_t *sys, uint32_t baud);
ml_status_t uart_driver_send(ml_sys_t *sys, uint8_t byte);
ml_status_t uart_driver_recv(ml_sys_t *sys, uint8_t *byte);
ml_status_t uart_driver_write(ml_sys_t *sys, const uint8_t *data, size_t n);

#ifdef __cplusplus
}
#endif

#endif

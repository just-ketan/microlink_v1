#ifndef MICRLINK_UART_HAL_H
#define MICRLINK_UART_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t uart_hal_init(ml_sys_t *sys, uint32_t baud);
ml_status_t uart_send(ml_sys_t *sys, uint8_t byte);
ml_status_t uart_receive(ml_sys_t *sys, uint8_t *byte);
ml_status_t uart_send_str(ml_sys_t *sys, const char *s);

#ifdef __cplusplus
}
#endif

#endif

#include "uart_hal.h"

#include "firmware/drivers/uart/uart_driver.h"

#include <string.h>

ml_status_t uart_hal_init(ml_sys_t *sys, uint32_t baud)
{
    return uart_driver_init(sys, baud);
}

ml_status_t uart_send(ml_sys_t *sys, uint8_t byte)
{
    return uart_driver_send(sys, byte);
}

ml_status_t uart_receive(ml_sys_t *sys, uint8_t *byte)
{
    return uart_driver_recv(sys, byte);
}

ml_status_t uart_send_str(ml_sys_t *sys, const char *s)
{
    if (s == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    return uart_driver_write(sys, (const uint8_t *)s, strlen(s));
}

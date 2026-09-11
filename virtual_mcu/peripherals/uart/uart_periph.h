#ifndef MICRLINK_UART_PERIPH_H
#define MICRLINK_UART_PERIPH_H

#include "../../registers/register.h"
#include "../gpio/gpio_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ML_UART_FIFO 64

typedef struct ml_uart {
    ml_reg_bank_t bank;
    uint8_t storage[ML_REGION_SIZE];
    uint8_t tx[ML_UART_FIFO];
    uint8_t rx[ML_UART_FIFO];
    uint8_t tx_head, tx_tail, tx_count;
    uint8_t rx_head, rx_tail, rx_count;
    ml_raise_irq_fn raise_irq;
    void *irq_ctx;
    uint8_t rx_corrupt_mask;
} ml_uart_t;

ml_status_t ml_uart_init(ml_uart_t *u, ml_raise_irq_fn raise, void *ctx);
void ml_uart_inject_rx(ml_uart_t *u, uint8_t byte);
size_t ml_uart_pop_tx(ml_uart_t *u, uint8_t *dst, size_t max);

#ifdef __cplusplus
}
#endif

#endif

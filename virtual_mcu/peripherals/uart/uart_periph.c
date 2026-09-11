#include "uart_periph.h"

#include "../../registers/register_definitions.h"

#include <string.h>

static void uart_update_status(ml_uart_t *u)
{
    uint32_t st = 0;
    if (u->tx_count < ML_UART_FIFO) {
        st |= ML_UART_STAT_TXE;
    }
    if (u->rx_count > 0u) {
        st |= ML_UART_STAT_RXNE;
    }
    ml_reg_poke32(&u->bank, ML_UART_REG_STATUS, st);
}

static void uart_maybe_irq(ml_uart_t *u)
{
    uint32_t ien = ml_reg_peek32(&u->bank, ML_UART_REG_IEN);
    uint32_t st = ml_reg_peek32(&u->bank, ML_UART_REG_STATUS);
    if ((ien & st) != 0u && u->raise_irq != NULL) {
        u->raise_irq(u->irq_ctx, ML_IRQ_UART);
    }
}

static void uart_on_write(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value)
{
    ml_uart_t *u = (ml_uart_t *)owner;
    (void)old_value;
    if (offset == ML_UART_REG_TX) {
        if (u->tx_count < ML_UART_FIFO) {
            u->tx[u->tx_head] = (uint8_t)(new_value & 0xFFu);
            u->tx_head = (uint8_t)((u->tx_head + 1u) % ML_UART_FIFO);
            u->tx_count++;
        }
        uart_update_status(u);
        uart_maybe_irq(u);
    }
}

static void uart_on_read(void *owner, uint32_t offset, uint32_t *value)
{
    ml_uart_t *u = (ml_uart_t *)owner;
    if (offset == ML_UART_REG_RX) {
        if (u->rx_count == 0u) {
            *value = 0u;
            return;
        }
        uint8_t b = u->rx[u->rx_tail];
        u->rx_tail = (uint8_t)((u->rx_tail + 1u) % ML_UART_FIFO);
        u->rx_count--;
        *value = b;
        uart_update_status(u);
    } else if (offset == ML_UART_REG_STATUS) {
        uart_update_status(u);
        *value = ml_reg_peek32(&u->bank, ML_UART_REG_STATUS);
    }
}

ml_status_t ml_uart_init(ml_uart_t *u, ml_raise_irq_fn raise, void *ctx)
{
    memset(u, 0, sizeof(*u));
    u->raise_irq = raise;
    u->irq_ctx = ctx;
    ml_status_t st = ml_reg_bank_init(&u->bank, ML_UART_BASE, ML_REGION_SIZE, u->storage, ml_uart_regs,
                                      ml_uart_reg_count, u, uart_on_write, uart_on_read);
    uart_update_status(u);
    return st;
}

void ml_uart_inject_rx(ml_uart_t *u, uint8_t byte)
{
    byte = (uint8_t)(byte ^ u->rx_corrupt_mask);
    if (u->rx_count >= ML_UART_FIFO) {
        return;
    }
    u->rx[u->rx_head] = byte;
    u->rx_head = (uint8_t)((u->rx_head + 1u) % ML_UART_FIFO);
    u->rx_count++;
    uart_update_status(u);
    uart_maybe_irq(u);
}

size_t ml_uart_pop_tx(ml_uart_t *u, uint8_t *dst, size_t max)
{
    size_t n = 0;
    while (n < max && u->tx_count > 0u) {
        dst[n++] = u->tx[u->tx_tail];
        u->tx_tail = (uint8_t)((u->tx_tail + 1u) % ML_UART_FIFO);
        u->tx_count--;
    }
    uart_update_status(u);
    return n;
}

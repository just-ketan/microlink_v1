#include "uart_driver.h"

ml_status_t uart_driver_init(ml_sys_t *sys, uint32_t baud)
{
    ml_status_t st;
    if (sys == NULL || baud == 0u) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_write32(&sys->mcu, ML_UART_BASE + ML_UART_REG_BAUD, baud);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_UART_BASE + ML_UART_REG_IEN, ML_UART_IEN_RXNE);
    if (st != ML_OK) {
        return st;
    }
    return ml_mcu_write32(&sys->mcu, ML_UART_BASE + ML_UART_REG_CTRL, 1u);
}

ml_status_t uart_driver_send(ml_sys_t *sys, uint8_t byte)
{
    uint32_t status = 0;
    ml_status_t st = ml_mcu_read32(&sys->mcu, ML_UART_BASE + ML_UART_REG_STATUS, &status);
    if (st != ML_OK) {
        return st;
    }
    if ((status & ML_UART_STAT_TXE) == 0u) {
        return ML_ERR_BUSY;
    }
    return ml_mcu_write32(&sys->mcu, ML_UART_BASE + ML_UART_REG_TX, byte);
}

ml_status_t uart_driver_recv(ml_sys_t *sys, uint8_t *byte)
{
    uint32_t status = 0;
    uint32_t rx = 0;
    ml_status_t st;
    if (byte == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    st = ml_mcu_read32(&sys->mcu, ML_UART_BASE + ML_UART_REG_STATUS, &status);
    if (st != ML_OK) {
        return st;
    }
    if ((status & ML_UART_STAT_RXNE) == 0u) {
        return ML_ERR_EMPTY;
    }
    st = ml_mcu_read32(&sys->mcu, ML_UART_BASE + ML_UART_REG_RX, &rx);
    if (st != ML_OK) {
        return st;
    }
    *byte = (uint8_t)rx;
    return ML_OK;
}

ml_status_t uart_driver_write(ml_sys_t *sys, const uint8_t *data, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        ml_status_t st = uart_driver_send(sys, data[i]);
        if (st != ML_OK) {
            return st;
        }
    }
    return ML_OK;
}

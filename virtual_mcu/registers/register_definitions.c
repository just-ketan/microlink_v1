#include "register_definitions.h"

#include "microlink/types.h"

#include <stdint.h>

#define RW UINT32_MAX

const ml_reg_def_t ml_gpio_regs[] = {
    {ML_GPIO_REG_CTRL, 0u, RW, RW, ML_REG_RW, "CTRL"},
    {ML_GPIO_REG_STATUS, 0u, RW, 0u, ML_REG_RO, "STATUS"},
    {ML_GPIO_REG_DATA, 0u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "DATA"},
    {ML_GPIO_REG_DIR, 0u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "DIRECTION"},
    {ML_GPIO_REG_MUX, 0u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "MUX"},
    {ML_GPIO_REG_INJECT, 0u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "INJECT"},
};
const size_t ml_gpio_reg_count = sizeof(ml_gpio_regs) / sizeof(ml_gpio_regs[0]);

const ml_reg_def_t ml_uart_regs[] = {
    {ML_UART_REG_CTRL, 0u, RW, RW, ML_REG_RW, "CTRL"},
    {ML_UART_REG_STATUS, ML_UART_STAT_TXE, 0x3u, 0u, ML_REG_RO, "STATUS"},
    {ML_UART_REG_TX, 0u, 0u, 0xFFu, ML_REG_WO, "TX"},
    {ML_UART_REG_RX, 0u, 0xFFu, 0u, ML_REG_RO, "RX"},
    {ML_UART_REG_BAUD, 115200u, RW, RW, ML_REG_RW, "BAUD"},
    {ML_UART_REG_IEN, 0u, 0x3u, 0x3u, ML_REG_RW, "IEN"},
};
const size_t ml_uart_reg_count = sizeof(ml_uart_regs) / sizeof(ml_uart_regs[0]);

const ml_reg_def_t ml_spi_regs[] = {
    {ML_SPI_REG_CTRL, 0u, 0x7u, 0x7u, ML_REG_RW, "CTRL"},
    {ML_SPI_REG_STATUS, 0u, 0x1u, 0u, ML_REG_RO, "STATUS"},
    {ML_SPI_REG_TX, 0u, 0u, 0xFFu, ML_REG_WO, "TX"},
    {ML_SPI_REG_RX, 0u, 0xFFu, 0u, ML_REG_RO, "RX"},
    {ML_SPI_REG_CLOCK, 1u, 0xFFu, 0xFFu, ML_REG_RW, "CLOCK"},
    {ML_SPI_REG_CS, 1u, 0x1u, 0x1u, ML_REG_RW, "CS"},
};
const size_t ml_spi_reg_count = sizeof(ml_spi_regs) / sizeof(ml_spi_regs[0]);

const ml_reg_def_t ml_i2c_regs[] = {
    {ML_I2C_REG_CTRL, 0u, 0xFu, 0xFu, ML_REG_RW, "CTRL"},
    {ML_I2C_REG_STATUS, 0u, 0x3u, 0u, ML_REG_RO, "STATUS"},
    {ML_I2C_REG_ADDR, 0u, 0x7Fu, 0x7Fu, ML_REG_RW, "ADDRESS"},
    {ML_I2C_REG_DATA, 0u, 0xFFu, 0xFFu, ML_REG_RW, "DATA"},
    {ML_I2C_REG_CLOCK, 100u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "CLOCK"},
};
const size_t ml_i2c_reg_count = sizeof(ml_i2c_regs) / sizeof(ml_i2c_regs[0]);

const ml_reg_def_t ml_timer_regs[] = {
    {ML_TMR_REG_CTRL, 0u, 0x1u, 0x1u, ML_REG_RW, "CONTROL"},
    {ML_TMR_REG_COUNTER, 0u, RW, RW, ML_REG_RW, "COUNTER"},
    {ML_TMR_REG_COMPARE, 0u, RW, RW, ML_REG_RW, "COMPARE"},
    {ML_TMR_REG_STATUS, 0u, 0x3u, 0x3u, ML_REG_RW, "STATUS"},
    {ML_TMR_REG_PRESCALER, 0u, 0xFFFFu, 0xFFFFu, ML_REG_RW, "PRESCALER"},
};
const size_t ml_timer_reg_count = sizeof(ml_timer_regs) / sizeof(ml_timer_regs[0]);

const ml_reg_def_t ml_irqc_regs[] = {
    {ML_IRQC_REG_PENDING, 0u, 0x3Fu, 0x3Fu, ML_REG_RW, "PENDING"},
    {ML_IRQC_REG_ENABLE, 0u, 0x3Fu, 0x3Fu, ML_REG_RW, "ENABLE"},
    {ML_IRQC_REG_MASK, 0u, 0x3Fu, 0x3Fu, ML_REG_RW, "MASK"},
    {ML_IRQC_REG_PRIORITY, 0u, RW, RW, ML_REG_RW, "PRIORITY"},
};
const size_t ml_irqc_reg_count = sizeof(ml_irqc_regs) / sizeof(ml_irqc_regs[0]);

const ml_reg_def_t ml_radio_regs[] = {
    {ML_RADIO_REG_CTRL, 0u, 0x1u, 0x1u, ML_REG_RW, "CTRL"},
    {ML_RADIO_REG_FREQ, 2400u, RW, RW, ML_REG_RW, "FREQ"},
    {ML_RADIO_REG_GAIN, 0u, 0xFFu, 0xFFu, ML_REG_RW, "GAIN"},
    {ML_RADIO_REG_MOD, 0u, 0x3u, 0x3u, ML_REG_RW, "MOD"},
    {ML_RADIO_REG_BW, 200u, RW, RW, ML_REG_RW, "BW"},
    {ML_RADIO_REG_FILTER, 0u, 0x3u, 0x3u, ML_REG_RW, "FILTER"},
    {ML_RADIO_REG_STATUS, 0u, RW, 0u, ML_REG_RO, "STATUS"},
    {ML_RADIO_REG_RSSI, 0u, RW, 0u, ML_REG_RO, "RSSI"},
    {ML_RADIO_REG_SNR, 0u, RW, 0u, ML_REG_RO, "SNR"},
    {ML_RADIO_REG_CMD, 0u, 0u, 0xFFu, ML_REG_WO, "CMD"},
};
const size_t ml_radio_reg_count = sizeof(ml_radio_regs) / sizeof(ml_radio_regs[0]);

#ifndef MICRLINK_TYPES_H
#define MICRLINK_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ML_GPIO_BASE   0x40000000u
#define ML_UART_BASE   0x40000100u
#define ML_SPI_BASE    0x40000200u
#define ML_I2C_BASE    0x40000300u
#define ML_TIMER_BASE  0x40000400u
#define ML_IRQC_BASE   0x40000500u
#define ML_RADIO_BASE  0x40000600u

#define ML_REGION_SIZE 0x100u

#define ML_IRQ_GPIO  0u
#define ML_IRQ_UART  1u
#define ML_IRQ_SPI   2u
#define ML_IRQ_I2C   3u
#define ML_IRQ_TIMER 4u
#define ML_IRQ_RADIO 5u
#define ML_IRQ_COUNT 6u

#define ML_GPIO_PINS 16u

enum {
    ML_GPIO_REG_CTRL = 0x00,
    ML_GPIO_REG_STATUS = 0x04,
    ML_GPIO_REG_DATA = 0x08,
    ML_GPIO_REG_DIR = 0x0C,
    ML_GPIO_REG_MUX = 0x10,
    ML_GPIO_REG_INJECT = 0x14
};

enum {
    ML_UART_REG_CTRL = 0x00,
    ML_UART_REG_STATUS = 0x04,
    ML_UART_REG_TX = 0x08,
    ML_UART_REG_RX = 0x0C,
    ML_UART_REG_BAUD = 0x10,
    ML_UART_REG_IEN = 0x14
};

#define ML_UART_STAT_TXE  (1u << 0)
#define ML_UART_STAT_RXNE (1u << 1)
#define ML_UART_IEN_TXE   (1u << 0)
#define ML_UART_IEN_RXNE  (1u << 1)

enum {
    ML_SPI_REG_CTRL = 0x00,
    ML_SPI_REG_STATUS = 0x04,
    ML_SPI_REG_TX = 0x08,
    ML_SPI_REG_RX = 0x0C,
    ML_SPI_REG_CLOCK = 0x10,
    ML_SPI_REG_CS = 0x14
};

#define ML_SPI_STAT_DONE (1u << 0)
#define ML_SPI_CTRL_EN   (1u << 0)
#define ML_SPI_CTRL_CPOL (1u << 1)
#define ML_SPI_CTRL_CPHA (1u << 2)

enum {
    ML_I2C_REG_CTRL = 0x00,
    ML_I2C_REG_STATUS = 0x04,
    ML_I2C_REG_ADDR = 0x08,
    ML_I2C_REG_DATA = 0x0C,
    ML_I2C_REG_CLOCK = 0x10
};

#define ML_I2C_CTRL_START (1u << 0)
#define ML_I2C_CTRL_STOP  (1u << 1)
#define ML_I2C_CTRL_READ  (1u << 2)
#define ML_I2C_CTRL_EN    (1u << 3)
#define ML_I2C_STAT_ACK   (1u << 0)
#define ML_I2C_STAT_BUSY  (1u << 1)

enum {
    ML_TMR_REG_CTRL = 0x00,
    ML_TMR_REG_COUNTER = 0x04,
    ML_TMR_REG_COMPARE = 0x08,
    ML_TMR_REG_STATUS = 0x0C,
    ML_TMR_REG_PRESCALER = 0x10
};

#define ML_TMR_CTRL_EN     (1u << 0)
#define ML_TMR_STAT_CMP    (1u << 0)
#define ML_TMR_STAT_OVF    (1u << 1)

enum {
    ML_IRQC_REG_PENDING = 0x00,
    ML_IRQC_REG_ENABLE = 0x04,
    ML_IRQC_REG_MASK = 0x08,
    ML_IRQC_REG_PRIORITY = 0x0C
};

enum {
    ML_RADIO_REG_CTRL = 0x00,
    ML_RADIO_REG_FREQ = 0x04,
    ML_RADIO_REG_GAIN = 0x08,
    ML_RADIO_REG_MOD = 0x0C,
    ML_RADIO_REG_BW = 0x10,
    ML_RADIO_REG_FILTER = 0x14,
    ML_RADIO_REG_STATUS = 0x18,
    ML_RADIO_REG_RSSI = 0x1C,
    ML_RADIO_REG_SNR = 0x20,
    ML_RADIO_REG_CMD = 0x24
};

#define ML_RADIO_CTRL_EN  (1u << 0)
#define ML_RADIO_CMD_TX   1u
#define ML_RADIO_CMD_RX   2u
#define ML_RADIO_CMD_CFG  3u

#ifdef __cplusplus
}
#endif

#endif /* MICRLINK_TYPES_H */

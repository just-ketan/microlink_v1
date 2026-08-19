#ifndef MICRLINK_REGISTER_DEFINITIONS_H
#define MICRLINK_REGISTER_DEFINITIONS_H

#include "register.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const ml_reg_def_t ml_gpio_regs[];
extern const size_t ml_gpio_reg_count;

extern const ml_reg_def_t ml_uart_regs[];
extern const size_t ml_uart_reg_count;

extern const ml_reg_def_t ml_spi_regs[];
extern const size_t ml_spi_reg_count;

extern const ml_reg_def_t ml_i2c_regs[];
extern const size_t ml_i2c_reg_count;

extern const ml_reg_def_t ml_timer_regs[];
extern const size_t ml_timer_reg_count;

extern const ml_reg_def_t ml_irqc_regs[];
extern const size_t ml_irqc_reg_count;

extern const ml_reg_def_t ml_radio_regs[];
extern const size_t ml_radio_reg_count;

#ifdef __cplusplus
}
#endif

#endif

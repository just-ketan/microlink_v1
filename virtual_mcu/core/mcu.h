#ifndef MICRLINK_MCU_H
#define MICRLINK_MCU_H

#include "../clock/clock.h"
#include "../fault_injection/fault.h"
#include "../interrupt_controller/irqc.h"
#include "../memory_map/memory_map.h"
#include "../peripherals/gpio/gpio_periph.h"
#include "../peripherals/i2c/i2c_periph.h"
#include "../peripherals/radio/radio_periph.h"
#include "../peripherals/spi/spi_periph.h"
#include "../peripherals/timer/timer_periph.h"
#include "../peripherals/uart/uart_periph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_mcu {
    ml_clock_t clock;
    ml_memory_map_t map;
    ml_gpio_t gpio;
    ml_uart_t uart;
    ml_spi_t spi;
    ml_i2c_t i2c;
    ml_timer_t timer;
    ml_irqc_t irqc;
    ml_radio_t radio;
    ml_fault_cfg_t faults;
    uint64_t irq_dispatches;
} ml_mcu_t;

ml_status_t ml_mcu_init(ml_mcu_t *mcu);
void ml_mcu_tick(ml_mcu_t *mcu);
ml_status_t ml_mcu_read32(ml_mcu_t *mcu, uint32_t addr, uint32_t *out);
ml_status_t ml_mcu_write32(ml_mcu_t *mcu, uint32_t addr, uint32_t value);
void ml_mcu_apply_faults(ml_mcu_t *mcu);

#ifdef __cplusplus
}
#endif

#endif

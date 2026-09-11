#include "application.h"

#include "firmware/hal/gpio_hal.h"
#include "firmware/hal/i2c_hal.h"
#include "firmware/hal/spi_hal.h"
#include "firmware/hal/timer_hal.h"
#include "firmware/hal/uart_hal.h"
#include "firmware/interrupts/isr.h"
#include "firmware/protocol/packet.h"
#include "firmware/rtos/scheduler.h"
#include "firmware/transceiver/radio_fw.h"

#include <stdio.h>

static void control_task(ml_sys_t *sys)
{
    static int level = 0;
    level = !level;
    (void)gpio_write(sys, 0u, level ? GPIO_HIGH : GPIO_LOW);
    ml_rtos_delay_current(sys, 5u);
}

static void telemetry_task(ml_sys_t *sys)
{
    uint8_t payload[8];
    ml_packet_t pkt;
    uint8_t wire[64];
    payload[0] = (uint8_t)(sys->last_rssi & 0xFFu);
    payload[1] = (uint8_t)(sys->last_snr & 0xFFu);
    (void)ml_packet_init(&pkt, 0x10u, (uint16_t)sys->rtos.tick.ticks, payload, 2u);
    (void)ml_packet_serialize(&pkt, wire, sizeof(wire));
    (void)uart_send_str(sys, "TLM\n");
    ml_rtos_delay_current(sys, 8u);
}

static void radio_task(ml_sys_t *sys)
{
    (void)radio_fw_tx(sys);
    (void)radio_fw_rx(sys);
    ml_rtos_delay_current(sys, 10u);
}

static void logging_task(ml_sys_t *sys)
{
    uint8_t rx;
    if (ml_mutex_lock(&sys->rtos.log_mutex, sys->rtos.current) == ML_OK) {
        if (uart_receive(sys, &rx) == ML_OK) {
            (void)ml_queue_push(&sys->rtos.events, rx);
        }
        (void)ml_mutex_unlock(&sys->rtos.log_mutex, sys->rtos.current);
    }
    ml_rtos_delay_current(sys, 4u);
}

ml_status_t application_init(ml_sys_t *sys)
{
    ml_status_t st;
    uint8_t spi_rx = 0;
    uint8_t i2c_b = 0;

    sys->running = 1;
    ml_rtos_init(&sys->rtos);
    ml_isr_install(sys);

    st = gpio_hal_init(sys);
    if (st != ML_OK) {
        return st;
    }
    st = gpio_set_direction(sys, 0u, 1);
    if (st != ML_OK) {
        return st;
    }
    st = uart_hal_init(sys, 115200u);
    if (st != ML_OK) {
        return st;
    }
    st = spi_hal_init(sys);
    if (st != ML_OK) {
        return st;
    }
    st = i2c_hal_init(sys);
    if (st != ML_OK) {
        return st;
    }
    st = timer_hal_init(sys, 50u);
    if (st != ML_OK) {
        return st;
    }
    st = timer_start(sys);
    if (st != ML_OK) {
        return st;
    }
    st = radio_fw_init(sys);
    if (st != ML_OK) {
        return st;
    }

    (void)spi_transfer(sys, 0x11u, &spi_rx);
    (void)i2c_transfer_write(sys, 0x50u, 0xABu);
    (void)i2c_transfer_read(sys, 0x50u, &i2c_b);
    (void)uart_send_str(sys, "boot\n");

    ml_rtos_add_task(&sys->rtos, "radio", radio_task, 3u);
    ml_rtos_add_task(&sys->rtos, "telemetry", telemetry_task, 2u);
    ml_rtos_add_task(&sys->rtos, "control", control_task, 2u);
    ml_rtos_add_task(&sys->rtos, "logging", logging_task, 1u);
    return ML_OK;
}

void application_step(ml_sys_t *sys)
{
    ml_mcu_tick(&sys->mcu);
    if (ml_clock_rtos_due(&sys->mcu.clock)) {
        ml_rtos_tick(sys);
    }
    ml_rtos_schedule(sys);
    sys->sim_ticks++;
}

void application_shutdown(ml_sys_t *sys)
{
    (void)timer_stop(sys);
    sys->running = 0;
}

#include "tests/common/test.h"

#include "firmware/app/application.h"
#include "firmware/hal/gpio_hal.h"
#include "firmware/hal/i2c_hal.h"
#include "firmware/hal/spi_hal.h"
#include "firmware/hal/timer_hal.h"
#include "firmware/hal/uart_hal.h"
#include "firmware/protocol/packet.h"

#include <string.h>

void test_gpio(void)
{
    ml_sys_t sys;
    int level = 0;
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(gpio_hal_init(&sys), ML_OK);
    T_EQ_ST(gpio_set_direction(&sys, 1u, 1), ML_OK);
    T_EQ_ST(gpio_write(&sys, 1u, GPIO_HIGH), ML_OK);
    T_EQ_ST(gpio_read(&sys, 1u, &level), ML_OK);
    T_EQ_U32((uint32_t)level, 1u);
    ml_gpio_inject_input(&sys.mcu.gpio, 2u, 1);
    T_EQ_ST(gpio_set_direction(&sys, 2u, 0), ML_OK);
    T_EQ_ST(gpio_read(&sys, 2u, &level), ML_OK);
    T_EQ_U32((uint32_t)level, 1u);
}

void test_uart(void)
{
    ml_sys_t sys;
    uint8_t b = 0;
    uint8_t tx[8];
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(uart_hal_init(&sys, 9600u), ML_OK);
    T_EQ_ST(uart_send(&sys, 0x41u), ML_OK);
    T_CHECK(ml_uart_pop_tx(&sys.mcu.uart, tx, 8) == 1);
    T_EQ_U32(tx[0], 0x41u);
    ml_uart_inject_rx(&sys.mcu.uart, 0x5Au);
    T_EQ_ST(uart_receive(&sys, &b), ML_OK);
    T_EQ_U32(b, 0x5Au);
}

void test_spi(void)
{
    ml_sys_t sys;
    uint8_t rx = 0;
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(spi_hal_init(&sys), ML_OK);
    T_EQ_ST(spi_transfer(&sys, 0x10u, &rx), ML_OK);
    T_EQ_U32(rx, (uint32_t)(0x10u ^ 0x5Au));
}

void test_i2c(void)
{
    ml_sys_t sys;
    uint8_t b = 0;
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(i2c_hal_init(&sys), ML_OK);
    T_EQ_ST(i2c_transfer_write(&sys, 0x50u, 0x22u), ML_OK);
    T_EQ_ST(i2c_transfer_read(&sys, 0x50u, &b), ML_OK);
}

void test_timer(void)
{
    ml_sys_t sys;
    uint32_t stv = 0;
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(timer_hal_init(&sys, 3u), ML_OK);
    T_EQ_ST(timer_start(&sys), ML_OK);
    for (int i = 0; i < 8; i++) {
        ml_timer_tick(&sys.mcu.timer);
    }
    T_EQ_ST(ml_mcu_read32(&sys.mcu, ML_TIMER_BASE + ML_TMR_REG_STATUS, &stv), ML_OK);
    T_CHECK((stv & ML_TMR_STAT_CMP) != 0u);
}

void test_irq_rtos(void)
{
    ml_sys_t sys;
    memset(&sys, 0, sizeof(sys));
    T_EQ_ST(ml_mcu_init(&sys.mcu), ML_OK);
    T_EQ_ST(application_init(&sys), ML_OK);
    for (int i = 0; i < 200; i++) {
        application_step(&sys);
    }
    T_CHECK(sys.timer_events > 0);
    T_CHECK(sys.rtos.switches > 0);
    T_CHECK(sys.radio_cycles > 0);
    application_shutdown(&sys);
}

void test_drivers_hal(void)
{
    test_gpio();
    test_uart();
    test_spi();
    test_i2c();
}

void test_protocol(void)
{
    ml_packet_t a;
    ml_packet_t b;
    uint8_t wire[64];
    uint8_t payload[2] = {1, 2};
    uint8_t bad[8] = {0};
    T_EQ_ST(ml_packet_init(&a, 7u, 3u, payload, 2u), ML_OK);
    T_CHECK(ml_packet_serialize(&a, wire, sizeof(wire)) > 0);
    T_EQ_ST(ml_packet_parse(wire, sizeof(wire), &b), ML_OK);
    T_EQ_U32(b.type, 7u);
    T_EQ_ST(ml_packet_parse(bad, sizeof(bad), &b), ML_ERR_PROTOCOL);
}

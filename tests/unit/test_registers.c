#include "tests/common/test.h"

#include "virtual_mcu/core/mcu.h"
#include "virtual_mcu/registers/register_definitions.h"

void test_registers(void)
{
    ml_mcu_t mcu;
    uint32_t v = 0;
    T_EQ_ST(ml_mcu_init(&mcu), ML_OK);

    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_CTRL, &v), ML_OK);
    T_EQ_U32(v, 0u);

    T_EQ_ST(ml_mcu_write32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_CTRL, 0x5u), ML_OK);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_CTRL, &v), ML_OK);
    T_EQ_U32(v, 0x5u);

    T_EQ_ST(ml_mcu_write32(&mcu, 0x40100000u, 1u), ML_ERR_INVALID_ADDR);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + 1u, &v), ML_ERR_UNALIGNED);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + 0xFCu, &v), ML_ERR_INVALID_ADDR);

    T_EQ_ST(ml_mcu_write32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_STATUS, 1u), ML_ERR_ACCESS);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_UART_BASE + ML_UART_REG_TX, &v), ML_ERR_ACCESS);

    T_EQ_ST(ml_mcu_write32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, 0x1FFFFu), ML_OK);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DIR, &v), ML_OK);
    T_EQ_ST(ml_mcu_write32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DIR, 0xFFFFu), ML_OK);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, &v), ML_OK);
    T_EQ_U32(v & 0xFFFF0000u, 0u);

    ml_reg_bank_reset(&mcu.gpio.bank);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_CTRL, &v), ML_OK);
    T_EQ_U32(v, 0u);
}

void test_memory_map(void)
{
    ml_mcu_t mcu;
    ml_reg_bank_t *b = NULL;
    T_EQ_ST(ml_mcu_init(&mcu), ML_OK);
    T_EQ_ST(ml_memory_map_lookup(&mcu.map, ML_SPI_BASE, &b), ML_OK);
    T_CHECK(b == &mcu.spi.bank);
    T_EQ_ST(ml_memory_map_lookup(&mcu.map, 0u, &b), ML_ERR_INVALID_ADDR);
}

void test_cycle_model(void)
{
    ml_mcu_t mcu;
    uint32_t v = 0;
    T_EQ_ST(ml_mcu_init(&mcu), ML_OK);
    T_EQ_U32((uint32_t)mcu.cycles.cycles, 0u);
    for (int i = 0; i < 10; i++) {
        ml_mcu_tick(&mcu);
    }
    T_CHECK(mcu.cycles.cycles >= 10u);
    T_EQ_ST(ml_mcu_read32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, &v), ML_OK);
    T_CHECK(mcu.cycles.mmio_reads == 1u);
    T_EQ_ST(ml_mcu_write32(&mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, 1u), ML_OK);
    T_CHECK(mcu.cycles.mmio_writes == 1u);
}

#include "spi_driver.h"

ml_status_t spi_driver_init(ml_sys_t *sys, uint32_t divider, uint32_t mode)
{
    uint32_t ctrl = ML_SPI_CTRL_EN;
    ml_status_t st;
    if (mode & 1u) {
        ctrl |= ML_SPI_CTRL_CPOL;
    }
    if (mode & 2u) {
        ctrl |= ML_SPI_CTRL_CPHA;
    }
    st = ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CLOCK, divider == 0u ? 1u : divider);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 1u);
    if (st != ML_OK) {
        return st;
    }
    return ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CTRL, ctrl);
}

ml_status_t spi_driver_transfer(ml_sys_t *sys, uint8_t tx, uint8_t *rx)
{
    uint32_t status = 0;
    uint32_t rxv = 0;
    ml_status_t st;
    st = ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 0u);
    if (st != ML_OK) {
        return st;
    }
    st = ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_TX, tx);
    if (st != ML_OK) {
        (void)ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 1u);
        return st;
    }
    st = ml_mcu_read32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_STATUS, &status);
    if (st != ML_OK) {
        (void)ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 1u);
        return st;
    }
    if ((status & ML_SPI_STAT_DONE) == 0u) {
        (void)ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 1u);
        return ML_ERR_FAULT;
    }
    st = ml_mcu_read32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_RX, &rxv);
    (void)ml_mcu_write32(&sys->mcu, ML_SPI_BASE + ML_SPI_REG_CS, 1u);
    if (st != ML_OK) {
        return st;
    }
    if (rx != NULL) {
        *rx = (uint8_t)rxv;
    }
    return ML_OK;
}

#include "spi_hal.h"

#include "firmware/drivers/spi/spi_driver.h"

ml_status_t spi_hal_init(ml_sys_t *sys)
{
    return spi_driver_init(sys, 4u, 0u);
}

ml_status_t spi_transfer(ml_sys_t *sys, uint8_t tx, uint8_t *rx)
{
    return spi_driver_transfer(sys, tx, rx);
}

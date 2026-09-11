#include "i2c_hal.h"

#include "firmware/drivers/i2c/i2c_driver.h"

ml_status_t i2c_hal_init(ml_sys_t *sys)
{
    return i2c_driver_init(sys, 100u);
}

ml_status_t i2c_transfer_write(ml_sys_t *sys, uint8_t addr, uint8_t data)
{
    return i2c_driver_write(sys, addr, data);
}

ml_status_t i2c_transfer_read(ml_sys_t *sys, uint8_t addr, uint8_t *data)
{
    return i2c_driver_read(sys, addr, data);
}

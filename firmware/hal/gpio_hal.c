#include "gpio_hal.h"

#include "firmware/drivers/gpio/gpio_driver.h"

ml_status_t gpio_hal_init(ml_sys_t *sys)
{
    return gpio_driver_init(sys);
}

ml_status_t gpio_write(ml_sys_t *sys, uint32_t pin, int level)
{
    return gpio_driver_write(sys, pin, level);
}

ml_status_t gpio_read(ml_sys_t *sys, uint32_t pin, int *level)
{
    return gpio_driver_read(sys, pin, level);
}

ml_status_t gpio_set_direction(ml_sys_t *sys, uint32_t pin, int output)
{
    return gpio_driver_set_dir(sys, pin, output);
}

#include "timer_hal.h"

#include "firmware/drivers/timer/timer_driver.h"

ml_status_t timer_hal_init(ml_sys_t *sys, uint32_t compare)
{
    return timer_driver_init(sys, compare, 0u);
}

ml_status_t timer_start(ml_sys_t *sys)
{
    return timer_driver_start(sys);
}

ml_status_t timer_stop(ml_sys_t *sys)
{
    return timer_driver_stop(sys);
}

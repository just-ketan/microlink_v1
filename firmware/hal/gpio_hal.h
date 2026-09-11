#ifndef MICRLINK_GPIO_HAL_H
#define MICRLINK_GPIO_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_HIGH 1
#define GPIO_LOW  0

ml_status_t gpio_hal_init(ml_sys_t *sys);
ml_status_t gpio_write(ml_sys_t *sys, uint32_t pin, int level);
ml_status_t gpio_read(ml_sys_t *sys, uint32_t pin, int *level);
ml_status_t gpio_set_direction(ml_sys_t *sys, uint32_t pin, int output);

#ifdef __cplusplus
}
#endif

#endif

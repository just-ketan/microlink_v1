#ifndef MICRLINK_GPIO_DRIVER_H
#define MICRLINK_GPIO_DRIVER_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t gpio_driver_init(ml_sys_t *sys);
ml_status_t gpio_driver_set_dir(ml_sys_t *sys, uint32_t pin, int output);
ml_status_t gpio_driver_write(ml_sys_t *sys, uint32_t pin, int level);
ml_status_t gpio_driver_read(ml_sys_t *sys, uint32_t pin, int *level);
ml_status_t gpio_driver_set_mux(ml_sys_t *sys, uint32_t pin, int alt);

#ifdef __cplusplus
}
#endif

#endif

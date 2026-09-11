#ifndef MICRLINK_I2C_DRIVER_H
#define MICRLINK_I2C_DRIVER_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t i2c_driver_init(ml_sys_t *sys, uint32_t khz);
ml_status_t i2c_driver_write(ml_sys_t *sys, uint8_t addr, uint8_t data);
ml_status_t i2c_driver_read(ml_sys_t *sys, uint8_t addr, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif

#ifndef MICRLINK_I2C_HAL_H
#define MICRLINK_I2C_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t i2c_hal_init(ml_sys_t *sys);
ml_status_t i2c_transfer_write(ml_sys_t *sys, uint8_t addr, uint8_t data);
ml_status_t i2c_transfer_read(ml_sys_t *sys, uint8_t addr, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif

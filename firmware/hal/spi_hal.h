#ifndef MICRLINK_SPI_HAL_H
#define MICRLINK_SPI_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t spi_hal_init(ml_sys_t *sys);
ml_status_t spi_transfer(ml_sys_t *sys, uint8_t tx, uint8_t *rx);

#ifdef __cplusplus
}
#endif

#endif

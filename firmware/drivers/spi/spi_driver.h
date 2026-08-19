#ifndef MICRLINK_SPI_DRIVER_H
#define MICRLINK_SPI_DRIVER_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t spi_driver_init(ml_sys_t *sys, uint32_t divider, uint32_t mode);
ml_status_t spi_driver_transfer(ml_sys_t *sys, uint8_t tx, uint8_t *rx);

#ifdef __cplusplus
}
#endif

#endif

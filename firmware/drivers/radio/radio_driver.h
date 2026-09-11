#ifndef MICRLINK_RADIO_DRIVER_H
#define MICRLINK_RADIO_DRIVER_H

#include "firmware/hal/radio_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline ml_status_t radio_driver_init(ml_sys_t *sys)
{
    return radio_hal_init(sys);
}

#ifdef __cplusplus
}
#endif

#endif

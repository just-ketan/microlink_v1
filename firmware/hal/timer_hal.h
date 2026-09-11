#ifndef MICRLINK_TIMER_HAL_H
#define MICRLINK_TIMER_HAL_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t timer_hal_init(ml_sys_t *sys, uint32_t compare);
ml_status_t timer_start(ml_sys_t *sys);
ml_status_t timer_stop(ml_sys_t *sys);

#ifdef __cplusplus
}
#endif

#endif

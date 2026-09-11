#ifndef MICRLINK_TIMER_DRIVER_H
#define MICRLINK_TIMER_DRIVER_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t timer_driver_init(ml_sys_t *sys, uint32_t compare, uint32_t prescaler);
ml_status_t timer_driver_start(ml_sys_t *sys);
ml_status_t timer_driver_stop(ml_sys_t *sys);
ml_status_t timer_driver_clear_status(ml_sys_t *sys);

#ifdef __cplusplus
}
#endif

#endif

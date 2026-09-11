#ifndef MICRLINK_SYS_H
#define MICRLINK_SYS_H

#include "firmware/rtos/scheduler.h"
#include "virtual_mcu/core/mcu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_bridge ml_bridge_t;

typedef struct ml_sys {
    ml_mcu_t mcu;
    ml_rtos_t rtos;
    ml_bridge_t *bridge;
    uint32_t radio_cycles;
    uint32_t last_rssi;
    uint32_t last_snr;
    int timer_events;
    int running;
    uint32_t sim_ticks;
} ml_sys_t;

#ifdef __cplusplus
}
#endif

#endif

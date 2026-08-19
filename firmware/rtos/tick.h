#ifndef MICRLINK_TICK_H
#define MICRLINK_TICK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_rtos_tick {
    uint64_t ticks;
} ml_rtos_tick_t;

#ifdef __cplusplus
}
#endif

#endif

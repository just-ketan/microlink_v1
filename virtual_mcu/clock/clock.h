#ifndef MICRLINK_CLOCK_H
#define MICRLINK_CLOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Simulated time is independent of host wall-clock time.
 * One call to ml_clock_tick() advances simulated time by tick_ns nanoseconds
 * (default 1000 ns = 1 us). Host time is used only for benchmarks.
 */
typedef struct ml_clock {
    uint64_t ticks;
    uint64_t tick_ns;
    uint32_t rtos_div;
    uint32_t rtos_acc;
} ml_clock_t;

void ml_clock_init(ml_clock_t *clk, uint64_t tick_ns);
void ml_clock_tick(ml_clock_t *clk);
uint64_t ml_clock_time_ns(const ml_clock_t *clk);
int ml_clock_rtos_due(ml_clock_t *clk);

#ifdef __cplusplus
}
#endif

#endif

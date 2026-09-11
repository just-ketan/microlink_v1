#ifndef MICRLINK_CYCLE_MODEL_H
#define MICRLINK_CYCLE_MODEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Behavioral cycle budget model (NOT ARM ISA emulation).
 * Costs are inspired by typical Cortex-M MMIO / IRQ latencies for
 * firmware timing analysis in simulation. Values are documented constants.
 */
typedef struct ml_cycle_model {
    uint64_t cycles;
    uint64_t stalls;
    uint32_t mmio_reads;
    uint32_t mmio_writes;
    uint32_t irq_events;
} ml_cycle_model_t;

enum {
    ML_CYCLES_PER_TICK = 1u,
    ML_CYCLES_MMIO_READ = 2u,
    ML_CYCLES_MMIO_WRITE = 2u,
    ML_CYCLES_IRQ_DISPATCH = 12u,
    ML_CYCLES_TIMER_ACTIVE = 1u,
    ML_CYCLES_FAULT_STALL = 8u
};

void ml_cycle_model_init(ml_cycle_model_t *cm);
void ml_cycle_model_consume(ml_cycle_model_t *cm, uint32_t n);
void ml_cycle_model_stall(ml_cycle_model_t *cm, uint32_t n);

#ifdef __cplusplus
}
#endif

#endif

#include "clock.h"

void ml_clock_init(ml_clock_t *clk, uint64_t tick_ns)
{
    clk->ticks = 0;
    clk->tick_ns = (tick_ns == 0u) ? 1000u : tick_ns;
    clk->rtos_div = 10u; /* RTOS tick every 10 MCU ticks (10 us default) */
    clk->rtos_acc = 0u;
}

void ml_clock_tick(ml_clock_t *clk)
{
    clk->ticks += 1u;
    clk->rtos_acc += 1u;
}

uint64_t ml_clock_time_ns(const ml_clock_t *clk)
{
    return clk->ticks * clk->tick_ns;
}

int ml_clock_rtos_due(ml_clock_t *clk)
{
    if (clk->rtos_acc >= clk->rtos_div) {
        clk->rtos_acc = 0u;
        return 1;
    }
    return 0;
}

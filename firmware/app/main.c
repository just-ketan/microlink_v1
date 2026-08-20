#include "application.h"
#include "cosim/bridge/bridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *argv0)
{
    printf("Usage: %s [--ticks N] [--host ADDR] [--port P] [--fault-timer] [--fault-spi]\n", argv0);
}

int main(int argc, char **argv)
{
    ml_sys_t sys;
    ml_bridge_t bridge;
    uint32_t ticks = 400u;
    const char *host = NULL;
    uint16_t port = 8765;
    ml_status_t st;
    int i;

    memset(&sys, 0, sizeof(sys));
    st = ml_mcu_init(&sys.mcu);
    if (st != ML_OK) {
        fprintf(stderr, "mcu init failed: %s\n", ml_status_str(st));
        return 1;
    }
    ml_bridge_init(&bridge);
    ml_bridge_attach(&sys, &bridge);

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--ticks") == 0 && i + 1 < argc) {
            ticks = (uint32_t)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--host") == 0 && i + 1 < argc) {
            host = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = (uint16_t)strtoul(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "--fault-timer") == 0) {
            sys.mcu.faults.timer_fail = 1;
        } else if (strcmp(argv[i], "--fault-spi") == 0) {
            sys.mcu.faults.spi_fail = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        }
    }
    ml_mcu_apply_faults(&sys.mcu);

    if (host != NULL) {
        st = ml_bridge_connect(&bridge, host, port);
        if (st != ML_OK) {
            fprintf(stderr, "cosim connect failed (%s); using local radio model\n", ml_status_str(st));
        } else {
            printf("connected to RF engine %s:%u\n", host, (unsigned)port);
        }
    }

    st = application_init(&sys);
    if (st != ML_OK) {
        fprintf(stderr, "application init failed: %s\n", ml_status_str(st));
        return 1;
    }

    printf("MicroLink Virtual MCU boot\n");
    printf("sim_tick_ns=%llu  rtos_div=%u\n", (unsigned long long)sys.mcu.clock.tick_ns,
           sys.mcu.clock.rtos_div);

    for (uint32_t t = 0; t < ticks; t++) {
        application_step(&sys);
    }
    application_shutdown(&sys);

    printf("ticks=%u irq_disp=%llu timer_events=%d radio_cycles=%u last_rssi=%u last_snr=%u "
           "sched_switches=%u\n",
           ticks, (unsigned long long)sys.mcu.irq_dispatches, sys.timer_events, sys.radio_cycles,
           sys.last_rssi, sys.last_snr, sys.rtos.switches);
    printf("cycles=%llu stalls=%llu mmio_r=%u mmio_w=%u irq_cycles=%u\n",
           (unsigned long long)sys.mcu.cycles.cycles, (unsigned long long)sys.mcu.cycles.stalls,
           sys.mcu.cycles.mmio_reads, sys.mcu.cycles.mmio_writes, sys.mcu.cycles.irq_events);
    printf("tasks:");
    for (uint8_t n = 0; n < sys.rtos.ntasks; n++) {
        printf(" %s=%u", sys.rtos.tasks[n].name, sys.rtos.tasks[n].run_count);
    }
    printf("\n");
    ml_tcp_close(&bridge.tcp);
    return 0;
}

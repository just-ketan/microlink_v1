#include "microlink/sys.h"

#include <string.h>
#include "firmware/app/application.h"
#include "cosim/bridge/bridge.h"

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
static double now_s(void)
{
    LARGE_INTEGER f, c;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / (double)f.QuadPart;
}
#else
#include <time.h>
static double now_s(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
#endif

int main(void)
{
    ml_sys_t sys;
    ml_bridge_t bridge;
    const int n = 200000;
    double t0, t1;
    uint32_t dummy = 0;

    memset(&sys, 0, sizeof(sys));
    ml_mcu_init(&sys.mcu);
    ml_bridge_init(&bridge);
    ml_bridge_attach(&sys, &bridge);

    t0 = now_s();
    for (int i = 0; i < n; i++) {
        (void)ml_mcu_write32(&sys.mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, (uint32_t)i);
        (void)ml_mcu_read32(&sys.mcu, ML_GPIO_BASE + ML_GPIO_REG_DATA, &dummy);
    }
    t1 = now_s();
    printf("register_access_per_s=%.0f\n", (2.0 * n) / (t1 - t0 + 1e-12));

    t0 = now_s();
    for (int i = 0; i < n; i++) {
        ml_mcu_tick(&sys.mcu);
    }
    t1 = now_s();
    printf("mcu_ticks_per_s=%.0f\n", n / (t1 - t0 + 1e-12));

    application_init(&sys);
    t0 = now_s();
    for (int i = 0; i < 20000; i++) {
        application_step(&sys);
    }
    t1 = now_s();
    printf("scheduler_steps_per_s=%.0f\n", 20000.0 / (t1 - t0 + 1e-12));
    application_shutdown(&sys);
    return 0;
}

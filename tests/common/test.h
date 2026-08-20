#ifndef MICRLINK_TEST_H
#define MICRLINK_TEST_H

#include "microlink/status.h"

#include <stdint.h>
#include <stdio.h>

extern int g_test_fails;
extern int g_test_passes;

#define T_CHECK(cond)                                                                              \
    do {                                                                                           \
        if (cond) {                                                                                \
            g_test_passes++;                                                                       \
        } else {                                                                                   \
            g_test_fails++;                                                                        \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);                               \
        }                                                                                          \
    } while (0)

#define T_EQ_U32(a, b)                                                                             \
    do {                                                                                           \
        uint32_t _aa = (uint32_t)(a);                                                              \
        uint32_t _bb = (uint32_t)(b);                                                              \
        if (_aa == _bb) {                                                                          \
            g_test_passes++;                                                                       \
        } else {                                                                                   \
            g_test_fails++;                                                                        \
            printf("  FAIL %s:%d: %u != %u\n", __FILE__, __LINE__, _aa, _bb);                      \
        }                                                                                          \
    } while (0)

#define T_EQ_ST(a, b) T_EQ_U32((uint32_t)(a), (uint32_t)(b))

void test_registers(void);
void test_memory_map(void);
void test_cycle_model(void);
void test_gpio(void);
void test_uart(void);
void test_spi(void);
void test_i2c(void);
void test_timer(void);
void test_irq_rtos(void);
void test_drivers_hal(void);
void test_protocol(void);

#endif

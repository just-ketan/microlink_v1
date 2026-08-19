#include "tests/common/test.h"

#include <stdio.h>

int g_test_fails;
int g_test_passes;

int main(void)
{
    printf("MicroLink C unit tests\n");
    test_registers();
    test_memory_map();
    test_gpio();
    test_uart();
    test_spi();
    test_i2c();
    test_timer();
    test_irq_rtos();
    test_protocol();
    printf("passed=%d failed=%d\n", g_test_passes, g_test_fails);
    return g_test_fails == 0 ? 0 : 1;
}

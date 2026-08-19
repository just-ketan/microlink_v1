#ifndef MICRLINK_ISR_H
#define MICRLINK_ISR_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

void ml_isr_timer(void *ctx, uint32_t irq);
void ml_isr_uart(void *ctx, uint32_t irq);
void ml_isr_install(ml_sys_t *sys);

#ifdef __cplusplus
}
#endif

#endif

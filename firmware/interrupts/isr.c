#include "isr.h"

#include "firmware/drivers/timer/timer_driver.h"

void ml_isr_timer(void *ctx, uint32_t irq)
{
    ml_sys_t *sys = (ml_sys_t *)ctx;
    (void)irq;
    sys->timer_events++;
    (void)ml_sem_give(&sys->rtos.timer_sem);
    (void)timer_driver_clear_status(sys);
    ml_irqc_clear(&sys->mcu.irqc, ML_IRQ_TIMER);
}

void ml_isr_uart(void *ctx, uint32_t irq)
{
    ml_sys_t *sys = (ml_sys_t *)ctx;
    (void)irq;
    ml_irqc_clear(&sys->mcu.irqc, ML_IRQ_UART);
}

void ml_isr_install(ml_sys_t *sys)
{
    ml_irqc_set_isr(&sys->mcu.irqc, ML_IRQ_TIMER, ml_isr_timer, sys);
    ml_irqc_set_isr(&sys->mcu.irqc, ML_IRQ_UART, ml_isr_uart, sys);
    ml_reg_poke32(&sys->mcu.irqc.bank, ML_IRQC_REG_ENABLE, (1u << ML_IRQ_TIMER) | (1u << ML_IRQ_UART));
    ml_reg_poke32(&sys->mcu.irqc.bank, ML_IRQC_REG_MASK, 0u);
}

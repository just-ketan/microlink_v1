# RTOS model

Educational cooperative kernel:

- tasks: READY / RUNNING / DELAYED / BLOCKED
- schedule: highest numeric priority READY task runs until it calls `ml_rtos_delay_current`
- tick: decrements delays; timer ISR `ml_sem_give`s
- queue: 16 × uint32
- mutex: non-recursive, owner-checked
- notify: increments a per-task counter and unblocks BLOCKED

Not SMP-safe. Not a FreeRTOS clone.

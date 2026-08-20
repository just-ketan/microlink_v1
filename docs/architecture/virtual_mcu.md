# Virtual MCU

Firmware never does `(uint32_t *)0x40000000` on the host. Each region is host-backed storage with little-endian register access.

## Cycle budget model (v0.2)

`ml_cycle_model_t` accumulates simulated cycles for:

- each MCU tick
- MMIO reads/writes (via `ml_mcu_read32` / `ml_mcu_write32`)
- IRQ dispatch
- timer-active ticks
- fault-induced stalls

Constants are in `cycle_model.h`. This models **timing budgets** for firmware analysis, not ARM instruction decoding.

See [enhancements.md](enhancements.md).

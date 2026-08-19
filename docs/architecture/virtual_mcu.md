# Virtual MCU

Firmware never does `(uint32_t *)0x40000000` on the host. Addresses are **logical**. Each region is a host `uint8_t storage[0x100]` with little-endian 32-bit register access.

Register definitions (`register_definitions.c`) are separate from mechanics (`register.c`):

- 4-byte alignment
- region bounds (no 32-bit access that would cross `size`)
- lookup by offset
- RO writes and WO reads return `ML_ERR_ACCESS`
- `write_mask` preserves reserved bits; `read_mask` is applied on read
- optional `on_write` / `on_read` hooks implement peripheral side effects

`volatile` is **not** used as a synchronization primitive. Simulation is single-threaded except the optional TCP server.

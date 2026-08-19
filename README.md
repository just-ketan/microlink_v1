# MicroLink — Virtual Software-Defined RF Transceiver Controller

Software-only co-simulation of **embedded C firmware** on a **virtual MCU** (logical MMIO, never host-deref of `0x40000000`) and a **Python RF/DSP engine** (behavioral models, not a full-wave EM solver). Built to be runnable, testable, and resume-defensible.

## Motivation

Firmware, radio control, RF filters (including LC ladder topology), S-parameters, Smith-chart matching, acoustic-wave equivalent circuits, audio-over-radio, networking/CRC packets, fault injection, and a small ML radio-adaptation loop are implemented as real subsystems rather than keyword lists.

## Architecture

```
Application (C) → HAL → drivers → register bank → virtual peripherals
                                          ↓ IRQ / timer / RTOS
Radio firmware  → co-sim JSON/TCP bridge → Python I/Q + filters + ML
```

See [docs/architecture/system.md](docs/architecture/system.md).

## Repository structure

| Path | Role |
| --- | --- |
| `virtual_mcu/` | Logical memory map, register engine, peripherals, clock, IRQC, faults |
| `firmware/` | HAL, drivers, cooperative RTOS, radio firmware, packet CRC, app |
| `rf/` | I/Q, modulation, channel, digital/ladder filters, S-params, Smith, BVD, audio |
| `cosim/` | Versioned JSON protocol, TCP IPC, RF engine |
| `ml/` | Dataset from simulations, Random Forest adapter, adaptive loop |
| `tests/` | CTest unit tests + pytest |
| `docs/` | Architecture notes |

## Build (C17, CMake)

Requires CMake 3.16+, a C17 compiler (GCC/Clang/MSVC), Python 3.10+.

```bash
python scripts/build.py
# or
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Sanitizers (GCC/Clang **with** libasan/libubsan; not all MinGW distros ship them):

```bash
cmake -S . -B build -DMICRLINK_SANITIZE=ON
cmake --build build
```

## Python

```bash
python -m pip install -r requirements.txt
```

## Test

```bash
python scripts/test.py
# or
./build/microlink_tests
python -m pytest -q
```

## Run

```bash
./build/microlink_sim --ticks 400
python scripts/run_demo.py          # MCU + RF TCP engine + ML
python scripts/run_experiments.py   # RF/ML plots and JSON under results/
python scripts/run_benchmarks.py
```

Co-simulation only:

```bash
python cosim/bridge/server.py
./build/microlink_sim --host 127.0.0.1 --port 8765 --ticks 300
```

## Design decisions

- **Host-backed MMIO**: each peripheral owns a byte array; `register_read32/write32` enforce alignment, maps, RO/WO, and masks.
- **RTOS**: deterministic cooperative scheduler (not FreeRTOS). One task runs to yield/`delay`; timer ISR gives a semaphore.
- **Co-sim**: newline JSON control plane (debuggable). Sample-rate I/Q stays inside Python; the MCU exchanges configuration and metrics.
- **RF models** are labeled behavioral (ABCD ladder, BVD circuit, RRC pulse shaping).

## Limitations

- Not cycle-accurate ARM; not a FEM piezoelectric solver; not a 3D EM field solver.
- Cooperative RTOS has no real preemption or MPU.
- TCP localhost only (no Ethernet PHY).

## License

MIT. See `LICENSE`.

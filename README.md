# MicroLink — Virtual Software-Defined RF Transceiver Controller

Software-only co-simulation of **embedded C firmware** on a **virtual MCU** (logical MMIO, never host-deref of `0x40000000`) and a **Python RF/DSP engine** (behavioral models, not a full-wave EM solver). Built to be runnable, testable, and resume-defensible.

## Motivation

Firmware, radio control, RF filters (LC ladder, S-parameters), Smith-chart matching, acoustic-wave BVD circuits, audio-over-radio, networking/CRC packets, fault injection, host IQ replay, educational field slices, cycle-budget timing, and ML radio adaptation are implemented as real subsystems.

## Architecture

```
Application (C) → HAL → drivers → register bank → virtual peripherals
                          ↑ cycle budget model
Radio firmware  → co-sim JSON/TCP → Python RF (sim or IQ file) → ML registry
```

| Doc | Topic |
| --- | --- |
| [system.md](docs/architecture/system.md) | End-to-end flow |
| [enhancements.md](docs/architecture/enhancements.md) | v0.2 improvements & migration |
| [virtual_mcu.md](docs/architecture/virtual_mcu.md) | MMIO + cycles |
| [rf_chain.md](docs/architecture/rf_chain.md) | DSP + host SDR |
| [cosimulation.md](docs/architecture/cosimulation.md) | JSON/TCP + IQ paths |
| [ml_adaptation.md](docs/architecture/ml_adaptation.md) | Multi-model registry |

## Repository structure

| Path | Role |
| --- | --- |
| `virtual_mcu/` | Memory map, registers, peripherals, **cycle model**, clock, IRQC |
| `firmware/` | HAL, drivers, cooperative RTOS, radio firmware, packets, app |
| `rf/` | I/Q, modulation, filters, ladder, S-params, Smith, BVD, **host_sdr**, **em** |
| `cosim/` | JSON protocol, TCP IPC, bridge, RF engine with IQ file support |
| `ml/` | Expanded features, RF+GB+MLP training, adaptive loop |
| `cmake/` | **Sanitizer toolchain probe** |
| `tests/` | CTest + pytest |
| `docs/` | Architecture notes |

## Build (C17, CMake)

Requires CMake 3.16+, C17 compiler, Python 3.10+.

```bash
python scripts/build.py
```

Sanitizers (auto-disabled if `libasan`/`libubsan` unavailable):

```bash
cmake -S . -B build -DMICRLINK_SANITIZE=ON
cmake --build build
```

Configure prints whether sanitizers were enabled or skipped.

## Python

```bash
python -m pip install -r requirements.txt
```

## Test

```bash
python scripts/test.py
```

## Run

```bash
./build/microlink_sim --ticks 400          # prints cycle stats
python scripts/run_demo.py
python scripts/run_experiments.py
python scripts/run_benchmarks.py
```

Host IQ replay (no USB hardware):

```bash
export MICRLINK_IQ_RX=path/to/capture.npy   # Linux/macOS
python cosim/bridge/server.py
```

Co-simulation:

```bash
python cosim/bridge/server.py
./build/microlink_sim --host 127.0.0.1 --port 8765 --ticks 300
```

## Design decisions

- **Host-backed MMIO** with alignment, masks, and RO/WO enforcement.
- **Behavioral cycle model** for firmware timing analysis (not ARM ISA).
- **Host SDR** reads/writes IQ files when paths are set; otherwise internal link sim.
- **2D EM slice** for visualization only — not 3D FEM.
- **ML registry** trains three sklearn models and selects the best on validation data.
- **Co-sim** JSON control plane; optional `iq_rx` / `iq_tx` in messages.

## Limitations (honest)

| Area | Status |
| --- | --- |
| ARM ISA | Behavioral cycle **budget**, not instruction-accurate emulation |
| Physical radio | IQ **file** replay only — no USB SDR driver |
| Electromagnetics | 2D educational slice — **not** 3D FEM / full-wave solver |
| Sanitizers | Enabled only when toolchain ships ASan/UBSan libs |
| RTOS | Cooperative scheduler, no MPU / hard preemption |
| Networking | Localhost TCP, not Ethernet PHY |

## Changelog

See [docs/CHANGELOG.md](docs/CHANGELOG.md).

## License

MIT. See `LICENSE`.

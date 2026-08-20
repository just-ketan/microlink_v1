# MicroLink enhancements (v0.2)

This document records architectural improvements that address prior limitations while staying honest about scope (software-only, behavioral models).

## 1. Behavioral cycle budget model

**Prior:** No cycle accounting; timing was tick-only.

**Now:** `virtual_mcu/core/cycle_model.c` tracks simulated CPU cycles for MMIO, IRQ dispatch, timer activity, and fault stalls. Costs are documented constants inspired by Cortex-M latencies — **not** ARM ISA emulation.

| Operation | Cycles |
| --- | --- |
| MCU tick | 1 |
| MMIO read/write | 2 |
| IRQ dispatch | 12 |
| Active timer tick | +1 |
| Fault stall | 8 |

`microlink_sim` prints `cycles`, `stalls`, `mmio_r`, `mmio_w` at shutdown.

## 2. Host SDR / IQ file interface

**Prior:** RF metrics only from internal `simulate_link`.

**Now:** `rf/host_sdr/` provides `IQFileSource`, `IQFileSink`, and `HostRadio` — a documented API for replaying or capturing IQ from `.npy` or interleaved float32 files. The RF TCP engine accepts `iq_rx` / `iq_tx` JSON fields or `MICRLINK_IQ_RX` env var.

This is a **host sample stream**, not USB SDR hardware.

## 3. Educational 2D EM field slice

**Prior:** No spatial field visualization.

**Now:** `rf/em/educational_field.py` computes a 2D |E| grid from a standing-wave proxy on the ladder input impedance plus exponential y-decay. Experiment `exp_em_field.json` + `em_field_2d.png`.

**Not** a 3D FEM or full-wave Maxwell solver.

## 4. Sanitizer auto-detection

**Prior:** `-DMICRLINK_SANITIZE=ON` failed link on MinGW without libasan.

**Now:** `cmake/CheckSanitizers.cmake` probes the toolchain at configure time. If libraries are missing, CMake emits a warning and disables sanitizers instead of failing the build.

## 5. ML model registry

**Prior:** Single Random Forest, four features.

**Now:** Trains Random Forest, Gradient Boosting, and MLP; picks best on hold-out set. Features expanded to six (`ber_probe`, `gain_proxy`). Bundle stores all models, scores, and `best` name. `predict_with_meta` returns model lineage.

## Migration notes

- Retrain ML artifacts: `python -m ml.training.train` or delete `results/adapter.joblib`.
- Co-sim clients may send `"iq_rx": "/path/to/iq.npy"` in TXRX messages.
- Cycle stats require rebuilding C targets after pulling.

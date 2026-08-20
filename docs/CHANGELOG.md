# Changelog

## [0.2.0] — enhancements

### Added
- Behavioral cycle budget model (`virtual_mcu/core/cycle_model.c`)
- Host SDR IQ file source/sink (`rf/host_sdr/`)
- Educational 2D EM field slice (`rf/em/educational_field.py`)
- CMake sanitizer toolchain probe (`cmake/CheckSanitizers.cmake`)
- ML multi-model registry (Random Forest, Gradient Boosting, MLP)
- Expanded ML features (`ber_probe`, `gain_proxy`)
- RF engine IQ path via JSON `iq_rx`/`iq_tx` or `MICRLINK_IQ_RX`
- Cycle statistics in `microlink_sim` output

### Documentation
- `docs/architecture/enhancements.md`
- Updated README limitation matrix
- Architecture cross-links

## [0.1.0] — initial

- Virtual MCU, firmware stack, RF/DSP, co-sim, baseline ML, tests, experiments

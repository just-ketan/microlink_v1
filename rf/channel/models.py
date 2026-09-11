from __future__ import annotations

import numpy as np


def apply_channel(
    x: np.ndarray,
    rng: np.random.Generator,
    *,
    snr_db: float = 20.0,
    fade: float = 0.0,
    freq_off: float = 0.0,
    phase_off: float = 0.0,
    timing_off: int = 0,
    fs: float = 1.0,
) -> np.ndarray:
    y = x.copy()
    if timing_off:
        y = np.roll(y, timing_off)
    t = np.arange(y.size) / fs
    y = y * np.exp(1j * (phase_off + 2.0 * np.pi * freq_off * t))
    if fade > 0:
        h = rng.normal(1.0, fade) + 1j * rng.normal(0.0, fade)
        y = y * h
    p = np.mean(np.abs(y) ** 2) + 1e-18
    n0 = p / (10.0 ** (snr_db / 10.0))
    noise = np.sqrt(n0 / 2.0) * (rng.standard_normal(y.size) + 1j * rng.standard_normal(y.size))
    return y + noise

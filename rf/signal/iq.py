from __future__ import annotations

import numpy as np


def tone(n: int, freq: float, fs: float, *, seed: int | None = None) -> np.ndarray:
    t = np.arange(n, dtype=np.float64) / fs
    return np.exp(1j * 2.0 * np.pi * freq * t)


def awgn(x: np.ndarray, snr_db: float, rng: np.random.Generator) -> np.ndarray:
    p = np.mean(np.abs(x) ** 2)
    n0 = p / (10.0 ** (snr_db / 10.0))
    noise = np.sqrt(n0 / 2.0) * (rng.standard_normal(x.size) + 1j * rng.standard_normal(x.size))
    return x + noise.astype(x.dtype)


def evm_percent(ref: np.ndarray, rx: np.ndarray) -> float:
    err = np.mean(np.abs(rx - ref) ** 2)
    den = np.mean(np.abs(ref) ** 2) + 1e-18
    return float(100.0 * np.sqrt(err / den))


def snr_db(ref: np.ndarray, rx: np.ndarray) -> float:
    sig = np.mean(np.abs(ref) ** 2)
    noise = np.mean(np.abs(rx - ref) ** 2) + 1e-18
    return float(10.0 * np.log10(sig / noise))

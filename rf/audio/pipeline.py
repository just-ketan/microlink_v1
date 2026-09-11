from __future__ import annotations

import numpy as np
from scipy.signal import butter, lfilter

from rf.filters.digital.fir_iir import pulse_shape
from rf.modulation.digital import demap, map_bits


def audio_tone(n: int, fs: float, f0: float = 440.0) -> np.ndarray:
    t = np.arange(n) / fs
    return 0.5 * np.sin(2 * np.pi * f0 * t)


def audio_over_radio(audio: np.ndarray, mod: str, snr_db: float, rng: np.random.Generator) -> dict:
    bits = (audio > 0).astype(np.uint8)
    sym = map_bits(bits, mod)
    tx = pulse_shape(sym, sps=4)
    p = np.mean(np.abs(tx) ** 2) + 1e-18
    n0 = p / (10 ** (snr_db / 10))
    rx = tx + np.sqrt(n0 / 2) * (rng.standard_normal(tx.size) + 1j * rng.standard_normal(tx.size))
    rx_sym = rx[::4][: sym.size]
    bits_hat = demap(rx_sym, mod)[: bits.size]
    rec = bits_hat.astype(np.float64) * 2 - 1
    b, a = butter(3, 0.2)
    rec_f = lfilter(b, a, rec)
    snr = 10 * np.log10(np.mean(audio**2) / (np.mean((rec_f[: audio.size] - audio) ** 2) + 1e-12))
    return {"snr_proxy_db": float(snr), "ber": float(np.mean(bits_hat != bits)), "reconstructed": rec_f}

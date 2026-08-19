from __future__ import annotations

import numpy as np
from scipy.signal import firwin, freqz, lfilter, butter


def rrc_taps(sps: int, beta: float, span: int) -> np.ndarray:
    n = span * sps
    t = np.arange(-n / 2, n / 2 + 1) / sps
    h = np.zeros_like(t, dtype=np.float64)
    for i, ti in enumerate(t):
        if abs(ti) < 1e-12:
            h[i] = 1.0 - beta + 4.0 * beta / np.pi
        elif beta > 0 and abs(abs(ti) - 1.0 / (4.0 * beta)) < 1e-12:
            h[i] = (beta / np.sqrt(2.0)) * (
                (1 + 2 / np.pi) * np.sin(np.pi / (4 * beta))
                + (1 - 2 / np.pi) * np.cos(np.pi / (4 * beta))
            )
        else:
            num = np.sin(np.pi * ti * (1 - beta)) + 4 * beta * ti * np.cos(np.pi * ti * (1 + beta))
            den = np.pi * ti * (1 - (4 * beta * ti) ** 2)
            h[i] = num / den
    h = h / np.sqrt(np.sum(h**2))
    return h


def pulse_shape(symbols: np.ndarray, sps: int, beta: float = 0.35, span: int = 6) -> np.ndarray:
    h = rrc_taps(sps, beta, span)
    up = np.zeros(symbols.size * sps, dtype=np.complex128)
    up[::sps] = symbols
    return np.convolve(up, h, mode="same")


def fir_lpf(x: np.ndarray, cutoff: float, fs: float, taps: int = 51) -> np.ndarray:
    b = firwin(taps, cutoff, fs=fs)
    return lfilter(b, [1.0], x)


def iir_lpf(x: np.ndarray, cutoff: float, fs: float, order: int = 4) -> np.ndarray:
    b, a = butter(order, cutoff, fs=fs)
    return lfilter(b, a, x)


def freq_response(b, a, fs: float, n: int = 512):
    w, h = freqz(b, a, worN=n, fs=fs)
    mag = 20.0 * np.log10(np.abs(h) + 1e-12)
    phase = np.unwrap(np.angle(h))
    return w, mag, phase

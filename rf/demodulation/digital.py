from __future__ import annotations

from rf.modulation.digital import demap


def demodulate(symbols: np.ndarray, name: str):
    return demap(symbols, name)

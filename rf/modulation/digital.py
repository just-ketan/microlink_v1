from __future__ import annotations

import numpy as np

CONSTELLATIONS: dict[str, np.ndarray] = {
    "bpsk": np.array([-1 + 0j, 1 + 0j], dtype=np.complex128),
    "qpsk": np.array([1 + 1j, -1 + 1j, -1 - 1j, 1 - 1j], dtype=np.complex128) / np.sqrt(2),
    "qam16": (
        np.array(
            [x + 1j * y for y in (-3, -1, 1, 3) for x in (-3, -1, 1, 3)],
            dtype=np.complex128,
        )
        / np.sqrt(10)
    ),
}


def bits_per_symbol(name: str) -> int:
    return int(np.log2(CONSTELLATIONS[name.lower()].size))


def map_bits(bits: np.ndarray, name: str) -> np.ndarray:
    c = CONSTELLATIONS[name.lower()]
    k = bits_per_symbol(name)
    bits = bits.astype(np.uint8).reshape(-1)
    pad = (-bits.size) % k
    if pad:
        bits = np.concatenate([bits, np.zeros(pad, dtype=np.uint8)])
    idx = np.zeros(bits.size // k, dtype=np.int64)
    for i in range(k):
        idx = (idx << 1) | bits[i::k][: idx.size]
    return c[idx]


def demap(symbols: np.ndarray, name: str) -> np.ndarray:
    c = CONSTELLATIONS[name.lower()]
    k = bits_per_symbol(name)
    d = np.abs(symbols.reshape(-1, 1) - c.reshape(1, -1))
    idx = np.argmin(d, axis=1)
    bits = np.zeros(idx.size * k, dtype=np.uint8)
    for i, v in enumerate(idx):
        for b in range(k):
            bits[i * k + (k - 1 - b)] = (v >> b) & 1
    return bits

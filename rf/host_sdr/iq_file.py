"""Host-backed IQ source/sink — software stand-in for SDR sample streams (not a physical radio)."""

from __future__ import annotations

from pathlib import Path

import numpy as np


class IQFileSource:
    """Read complex baseband from .npy or interleaved float32 IQ files."""

    def __init__(self, path: Path | str, fs: float = 1e6) -> None:
        self.path = Path(path)
        self.fs = fs
        self._idx = 0
        if self.path.suffix == ".npy":
            self._data = np.load(self.path).astype(np.complex128)
        else:
            raw = np.fromfile(self.path, dtype=np.float32)
            if raw.size % 2 != 0:
                raw = raw[:-1]
            self._data = raw[0::2] + 1j * raw[1::2]

    @property
    def size(self) -> int:
        return int(self._data.size)

    def read_block(self, n: int) -> np.ndarray:
        if n <= 0:
            return np.array([], dtype=np.complex128)
        end = min(self._idx + n, self._data.size)
        block = self._data[self._idx:end]
        self._idx = end
        if block.size < n:
            pad = np.zeros(n - block.size, dtype=np.complex128)
            block = np.concatenate([block, pad])
        return block

    def reset(self) -> None:
        self._idx = 0


class IQFileSink:
    """Write complex samples to .npy or interleaved float32."""

    def __init__(self, path: Path | str) -> None:
        self.path = Path(path)
        self._chunks: list[np.ndarray] = []

    def write_block(self, samples: np.ndarray) -> None:
        self._chunks.append(np.asarray(samples, dtype=np.complex128).copy())

    def flush(self) -> Path:
        data = np.concatenate(self._chunks) if self._chunks else np.array([], dtype=np.complex128)
        if self.path.suffix == ".npy":
            np.save(self.path, data)
        else:
            inter = np.empty(data.size * 2, dtype=np.float32)
            inter[0::2] = data.real
            inter[1::2] = data.imag
            inter.tofile(self.path)
        return self.path


def measure_block(samples: np.ndarray) -> dict:
    p = float(np.mean(np.abs(samples) ** 2) + 1e-18)
    return {
        "rssi_db": float(10.0 * np.log10(p)),
        "peak": float(np.max(np.abs(samples))),
        "n": int(samples.size),
    }

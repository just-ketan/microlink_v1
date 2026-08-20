"""
2D educational EM field slice — NOT a 3D FEM or full-wave solver.

Computes a standing-wave magnitude pattern along a 1D transmission-line lumped
model for visualization of pass/stop band behavior.
"""

from __future__ import annotations

import numpy as np

from rf.filters.ladder.lc_ladder import ladder_abcd


def standing_wave_slice(
    freq_hz: float,
    x_m: np.ndarray,
    z0: float = 50.0,
    v_phase: float = 2e8,
    inductors_h: list[float] | None = None,
    capacitors_f: list[float] | None = None,
) -> np.ndarray:
    """|E|(x) proxy from forward/reflected waves on a line terminated by ladder Zin."""
    inductors_h = inductors_h or [2.2e-6, 3.3e-6]
    capacitors_f = capacitors_f or [1.5e-9, 1.0e-9]
    f = np.array([freq_hz], dtype=np.float64)
    abcd = ladder_abcd(f, inductors_h, capacitors_f)[0]
    a, b, c, d = abcd[0, 0], abcd[0, 1], abcd[1, 0], abcd[1, 1]
    zin = (a * z0 + b) / (c * z0 + d)
    gamma_l = (zin - z0) / (zin + z0)
    k = 2.0 * np.pi * freq_hz / v_phase
    e_mag = np.abs(np.exp(-1j * k * x_m) + gamma_l * np.exp(1j * k * x_m))
    return e_mag / np.max(e_mag)


def field_grid_2d(
    freq_hz: float,
    x_m: np.ndarray,
    y_m: np.ndarray,
    decay: float = 0.02,
) -> np.ndarray:
    """2D slice: standing wave along x with exponential decay along y (skin-like proxy)."""
    line = standing_wave_slice(freq_hz, x_m)
    grid = np.zeros((y_m.size, x_m.size), dtype=np.float64)
    for i, yi in enumerate(y_m):
        grid[i, :] = line * np.exp(-decay * abs(yi))
    return grid

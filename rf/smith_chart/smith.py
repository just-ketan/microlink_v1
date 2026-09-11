from __future__ import annotations

import numpy as np


def z_to_gamma(z: complex, z0: float = 50.0) -> complex:
    zn = z / z0
    return (zn - 1.0) / (zn + 1.0)


def gamma_to_z(gamma: complex, z0: float = 50.0) -> complex:
    zn = (1.0 + gamma) / (1.0 - gamma)
    return zn * z0


def vswr(gamma: complex) -> float:
    g = abs(gamma)
    if g >= 0.999:
        return float("inf")
    return (1.0 + g) / (1.0 - g)


def return_loss_db(gamma: complex) -> float:
    return float(-20.0 * np.log10(abs(gamma) + 1e-18))


def smith_grid(n: int = 400) -> tuple[np.ndarray, np.ndarray]:
    """Return (x, y) of Γ for a set of normalized resistance/reactance circles."""
    r_vals = [0.2, 0.5, 1.0, 2.0, 5.0]
    x_vals = [-5.0, -2.0, -1.0, -0.5, 0.5, 1.0, 2.0, 5.0]
    xs = []
    ys = []
    th = np.linspace(0, 2 * np.pi, n)
    for r in r_vals:
        cx, cy, rad = r / (r + 1.0), 0.0, 1.0 / (r + 1.0)
        xs.append(cx + rad * np.cos(th))
        ys.append(cy + rad * np.sin(th))
    for x in x_vals:
        cx, cy, rad = 1.0, 1.0 / x, abs(1.0 / x)
        xs.append(cx + rad * np.cos(th))
        ys.append(cy + rad * np.sin(th))
    return np.array(xs), np.array(ys)

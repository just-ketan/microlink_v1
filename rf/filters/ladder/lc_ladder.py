from __future__ import annotations

import numpy as np

# Behavioral LC ladder using cascaded ABCD matrices. Not a full EM solver.


def _series_z(z: complex) -> np.ndarray:
    return np.array([[1, z], [0, 1]], dtype=np.complex128)


def _shunt_y(y: complex) -> np.ndarray:
    return np.array([[1, 0], [y, 1]], dtype=np.complex128)


def ladder_abcd(freq_hz: np.ndarray, inductors_h: list[float], capacitors_f: list[float]) -> np.ndarray:
    """Alternate series-L / shunt-C low-pass prototype (Butterworth-like ladder)."""
    w = 2.0 * np.pi * np.asarray(freq_hz, dtype=np.float64)
    abcd = np.zeros((w.size, 2, 2), dtype=np.complex128)
    n_l = len(inductors_h)
    n_c = len(capacitors_f)
    for i, wi in enumerate(w):
        m = np.eye(2, dtype=np.complex128)
        for k in range(max(n_l, n_c)):
            if k < n_l:
                m = m @ _series_z(1j * wi * inductors_h[k])
            if k < n_c:
                m = m @ _shunt_y(1j * wi * capacitors_f[k])
        abcd[i] = m
    return abcd


def abcd_to_s(abcd: np.ndarray, z0: float = 50.0) -> dict[str, np.ndarray]:
    a = abcd[:, 0, 0]
    b = abcd[:, 0, 1]
    c = abcd[:, 1, 0]
    d = abcd[:, 1, 1]
    den = a + b / z0 + c * z0 + d
    s11 = (a + b / z0 - c * z0 - d) / den
    s21 = 2.0 / den
    s12 = 2.0 * (a * d - b * c) / den
    s22 = (-a + b / z0 - c * z0 + d) / den
    return {"s11": s11, "s21": s21, "s12": s12, "s22": s22}


def transfer_response(freq_hz: np.ndarray, inductors_h: list[float], capacitors_f: list[float], z0: float = 50.0):
    abcd = ladder_abcd(freq_hz, inductors_h, capacitors_f)
    s = abcd_to_s(abcd, z0)
    mag = 20.0 * np.log10(np.abs(s["s21"]) + 1e-18)
    phase = np.unwrap(np.angle(s["s21"]))
    return mag, phase, s, abcd


def behavioral_rf_filter(freq_hz: np.ndarray, kind: str, fc: float, bw: float, q: float, il_db: float):
    """Simple pole/zero behavioral RF filter (not EM)."""
    f = np.asarray(freq_hz, dtype=np.float64)
    if kind == "lowpass":
        h = 1.0 / (1.0 + 1j * (f / fc) * q)
    elif kind == "highpass":
        h = 1.0 / (1.0 + 1j * (fc / np.maximum(f, 1.0)) * q)
    else:
        h = 1.0 / (1.0 + 1j * q * (f / fc - fc / np.maximum(f, 1.0)) * (fc / max(bw, 1.0)))
    scale = 10.0 ** (-il_db / 20.0)
    return h * scale

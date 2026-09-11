from __future__ import annotations

import numpy as np

# Butterworth-Van Dyke equivalent circuit (motional RLC || C0). Educational model, not FEM.


def bvd_impedance(freq_hz: np.ndarray, rm: float, lm: float, cm: float, c0: float) -> np.ndarray:
    w = 2.0 * np.pi * np.asarray(freq_hz, dtype=np.float64)
    zm = rm + 1j * w * lm + 1.0 / (1j * w * cm)
    z0 = 1.0 / (1j * w * c0)
    return 1.0 / (1.0 / zm + 1.0 / z0)


def resonance_freqs(lm: float, cm: float, c0: float) -> tuple[float, float]:
    fs = 1.0 / (2.0 * np.pi * np.sqrt(lm * cm))
    fa = 1.0 / (2.0 * np.pi * np.sqrt(lm * (cm * c0 / (cm + c0))))
    return float(fs), float(fa)


def motional_q(rm: float, lm: float, cm: float) -> float:
    return float(1.0 / rm * np.sqrt(lm / cm))

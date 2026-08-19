from __future__ import annotations

import numpy as np

from rf.filters.ladder.lc_ladder import abcd_to_s, ladder_abcd


def ladder_sparams(freq_hz: np.ndarray, l_h: list[float], c_f: list[float], z0: float = 50.0):
    return abcd_to_s(ladder_abcd(freq_hz, l_h, c_f), z0)


def mag_phase(s: np.ndarray):
    return 20.0 * np.log10(np.abs(s) + 1e-18), np.angle(s, deg=True)

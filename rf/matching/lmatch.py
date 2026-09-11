from __future__ import annotations

import math

from rf.smith_chart.smith import gamma_to_z, return_loss_db, vswr, z_to_gamma


def l_section_match(zs: complex, zl: complex, f_hz: float, z0: float = 50.0) -> dict:
    """Simplified L-section using a series L and shunt C toward a real 50 Ω target."""
    w = 2.0 * math.pi * f_hz
    rs, xs = zs.real, zs.imag
    rl, xl = zl.real, zl.imag
    q = math.sqrt(max(rl / max(rs, 1e-9) - 1.0, 0.0)) if rl > rs else math.sqrt(max(rs / max(rl, 1e-9) - 1.0, 0.0))
    l = abs(xs + q * rs) / w if w else 0.0
    c = q / (w * max(rl, 1e-9)) if w else 0.0
    z_series = zs + 1j * w * l
    z_shunt = 1.0 / (1.0 / z_series + 1j * w * c)
    g = z_to_gamma(z_shunt, z0)
    return {
        "L_H": l,
        "C_F": c,
        "q": q,
        "z_in": z_shunt,
        "gamma": g,
        "return_loss_db": return_loss_db(g),
        "vswr": vswr(g),
        "z_load": zl,
        "z_source": zs,
    }

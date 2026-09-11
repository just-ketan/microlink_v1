"""Generate reproducible MicroLink experiment artifacts under results/."""

from __future__ import annotations

import json
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

from ml.models.adapter import adaptive_loop
from ml.training.train import train_model
from rf.acoustic_wave.bvd import bvd_impedance, resonance_freqs
from rf.experiments.link import simulate_link
from rf.filters.ladder.lc_ladder import transfer_response
from rf.em.educational_field import field_grid_2d
from rf.matching.lmatch import l_section_match
from rf.smith_chart.smith import smith_grid, z_to_gamma

RESULTS = ROOT / "results"


def _save_json(name: str, obj) -> None:
    RESULTS.mkdir(parents=True, exist_ok=True)
    p = RESULTS / name
    p.write_text(json.dumps(obj, indent=2, default=str), encoding="utf-8")


def exp_modulation():
    out = {}
    for mod in ("bpsk", "qpsk", "qam16"):
        snrs = list(range(0, 16, 2))
        bers = [simulate_link(n_bits=512, mod=mod, snr_awgn=s, seed=11)["ber"] for s in snrs]
        out[mod] = {"snr_db": snrs, "ber": bers}
        plt.figure()
        plt.semilogy(snrs, np.maximum(bers, 1e-4), marker="o")
        plt.xlabel("SNR (dB)")
        plt.ylabel("BER")
        plt.title(f"{mod.upper()} over AWGN (behavioral)")
        plt.grid(True, which="both")
        plt.savefig(RESULTS / f"ber_{mod}.png", dpi=120)
        plt.close()
    _save_json("exp_modulation.json", out)


def exp_ladder():
    f = np.logspace(5, 7, 256)
    mag, phase, s, _ = transfer_response(f, [2.2e-6, 3.3e-6], [1.5e-9, 1.0e-9])
    plt.figure()
    plt.semilogx(f, mag)
    plt.xlabel("Hz")
    plt.ylabel("|S21| dB")
    plt.title("LC ladder |S21| (ABCD model)")
    plt.grid(True, which="both")
    plt.savefig(RESULTS / "ladder_mag.png", dpi=120)
    plt.close()
    plt.figure()
    plt.semilogx(f, phase)
    plt.xlabel("Hz")
    plt.ylabel("phase rad")
    plt.title("LC ladder phase")
    plt.savefig(RESULTS / "ladder_phase.png", dpi=120)
    plt.close()
    _save_json(
        "exp_ladder.json",
        {
            "model": "cascaded ABCD LC ladder; not a full-wave solver",
            "f_hz": f[::8].tolist(),
            "s21_db": mag[::8].tolist(),
        },
    )


def exp_smith():
    xs, ys = smith_grid()
    g = z_to_gamma(30 + 40j)
    plt.figure(figsize=(5, 5))
    for x, y in zip(xs, ys):
        plt.plot(x, y, color="0.8", lw=0.6)
    th = np.linspace(0, 2 * np.pi, 256)
    plt.plot(np.cos(th), np.sin(th), "k", lw=1)
    plt.plot(g.real, g.imag, "ro")
    plt.axis("equal")
    plt.title("Smith chart (unit Γ disk)")
    plt.savefig(RESULTS / "smith.png", dpi=120)
    plt.close()
    m = l_section_match(50 + 0j, 30 + 40j, 1e9)
    _save_json("exp_smith.json", {"gamma_demo": [g.real, g.imag], "match": m})


def exp_acoustic():
    lm, cm, c0, rm = 1.2e-3, 18e-15, 2.5e-12, 8.0
    fs, fa = resonance_freqs(lm, cm, c0)
    f = np.linspace(fs * 0.6, fa * 1.4, 400)
    z = bvd_impedance(f, rm, lm, cm, c0)
    plt.figure()
    plt.semilogy(f, np.abs(z))
    plt.axvline(fs, color="g", ls="--", label="fs")
    plt.axvline(fa, color="r", ls="--", label="fa")
    plt.legend()
    plt.title("BVD resonator |Z| (equivalent circuit)")
    plt.savefig(RESULTS / "acoustic_z.png", dpi=120)
    plt.close()
    _save_json("exp_acoustic.json", {"fs": fs, "fa": fa, "model": "Butterworth-Van Dyke"})


def exp_em_field():
    x = np.linspace(0, 0.5, 64)
    y = np.linspace(-0.15, 0.15, 32)
    grid = field_grid_2d(1e6, x, y)
    plt.figure(figsize=(6, 3))
    plt.imshow(grid, extent=[x[0], x[-1], y[0], y[-1]], aspect="auto", origin="lower")
    plt.xlabel("x (m)")
    plt.ylabel("y (m)")
    plt.title("Educational 2D |E| slice (standing wave + decay; not 3D FEM)")
    plt.colorbar(label="|E| norm")
    plt.savefig(RESULTS / "em_field_2d.png", dpi=120)
    plt.close()
    _save_json(
        "exp_em_field.json",
        {"model": "1D standing wave on ladder Zin + y decay proxy", "freq_hz": 1e6},
    )


def exp_ml():
    info = train_model(RESULTS / "adapter.joblib", seed=7)
    loop = adaptive_loop(RESULTS / "adapter.joblib", snr=8.0, seed=5)
    _save_json("exp_ml.json", {"train": info, "loop": loop})


def exp_firmware_stub():
    _save_json(
        "exp_firmware_note.json",
        {
            "gpio_uart_spi_i2c_timer_rtos": "executed by microlink_tests / microlink_sim",
            "cosim": "scripts/run_cosim.py",
        },
    )


def main():
    t0 = time.time()
    RESULTS.mkdir(parents=True, exist_ok=True)
    exp_modulation()
    exp_ladder()
    exp_smith()
    exp_acoustic()
    exp_em_field()
    exp_ml()
    exp_firmware_stub()
    _save_json("experiments_meta.json", {"seconds": time.time() - t0, "seed_policy": "fixed per experiment"})
    print("experiments written to", RESULTS)


if __name__ == "__main__":
    main()

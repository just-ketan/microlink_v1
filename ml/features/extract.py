from __future__ import annotations

import numpy as np
import pandas as pd

from rf.experiments.link import simulate_link

FEATURE_COLS = [
    "rssi",
    "snr_meas_db",
    "evm_pct",
    "snr_awgn_db",
    "ber_probe",
    "gain_proxy",
]


def _policy_mod(snr: float) -> str:
    """AWGN-oriented modulation policy used as training labels."""
    if snr < 7.0:
        return "bpsk"
    if snr < 14.0:
        return "qpsk"
    return "qam16"


def extract_features(metrics: dict, snr_awgn: float | None = None, gain: int = 10) -> dict:
    snr_ch = float(snr_awgn if snr_awgn is not None else metrics.get("snr_awgn_db", 0.0))
    return {
        "rssi": float(metrics.get("rssi", 0.0)),
        "snr_meas_db": float(metrics.get("snr_meas_db", 0.0)),
        "evm_pct": float(metrics.get("evm_pct", 0.0)),
        "snr_awgn_db": snr_ch,
        "ber_probe": float(metrics.get("ber", 0.0)),
        "gain_proxy": float(gain),
    }


def generate_dataset(n: int = 80, seed: int = 1) -> pd.DataFrame:
    rng = np.random.default_rng(seed)
    rows = []
    for i in range(n):
        # Stratified SNR bands improve label diversity for ML training.
        band = i % 3
        if band == 0:
            snr = float(rng.uniform(0, 8))
        elif band == 1:
            snr = float(rng.uniform(8, 16))
        else:
            snr = float(rng.uniform(16, 24))
        gain = int(rng.integers(0, 32))
        fade_seed = int(rng.integers(0, 10_000))
        probe = simulate_link(n_bits=128, mod="qpsk", snr_awgn=snr, seed=fade_seed)
        feats = extract_features(probe, snr_awgn=snr, gain=gain)
        best_mod = _policy_mod(snr)
        best_ber = float(probe["ber"])
        rows.append({**feats, "best_mod": best_mod, "best_ber": best_ber})
    return pd.DataFrame(rows)

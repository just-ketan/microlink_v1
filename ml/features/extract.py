from __future__ import annotations

import numpy as np
import pandas as pd

from rf.experiments.link import simulate_link


FEATURE_COLS = ["rssi", "snr_meas_db", "evm_pct", "snr_awgn_db"]


def extract_features(metrics: dict) -> dict:
    return {k: float(metrics[k]) for k in FEATURE_COLS}


def generate_dataset(n: int = 80, seed: int = 1) -> pd.DataFrame:
    rng = np.random.default_rng(seed)
    rows = []
    mods = ["bpsk", "qpsk", "qam16"]
    for i in range(n):
        snr = float(rng.uniform(0, 24))
        fade_seed = int(rng.integers(0, 10_000))
        best_mod = None
        best_ber = 1.0
        feats = None
        for m in mods:
            r = simulate_link(n_bits=128, mod=m, snr_awgn=snr, seed=fade_seed)
            if r["ber"] < best_ber:
                best_ber = r["ber"]
                best_mod = m
                feats = extract_features(r)
        rows.append({**feats, "best_mod": best_mod, "best_ber": best_ber})
    return pd.DataFrame(rows)

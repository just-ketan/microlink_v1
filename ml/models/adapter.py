from __future__ import annotations

from pathlib import Path

from ml.inference.infer import load_model, predict_mod
from ml.training.train import train_model
from rf.experiments.link import simulate_link


def adaptive_loop(model_path: Path, snr: float, seed: int = 3) -> dict:
    if not model_path.exists():
        train_model(model_path)
    bundle = load_model(model_path)
    probe = simulate_link(n_bits=256, mod="qpsk", snr_awgn=snr, seed=seed)
    choice = predict_mod(
        bundle,
        {
            "rssi": probe["rssi"],
            "snr_meas_db": probe["snr_meas_db"],
            "evm_pct": probe["evm_pct"],
            "snr_awgn_db": snr,
        },
    )
    after = simulate_link(n_bits=256, mod=choice, snr_awgn=snr, seed=seed + 1)
    return {"probe": probe, "chosen_mod": choice, "after": after}

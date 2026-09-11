from __future__ import annotations

from pathlib import Path

from ml.features.extract import extract_features
from ml.inference.infer import load_model, predict_with_meta
from ml.training.train import train_model
from rf.experiments.link import simulate_link


def adaptive_loop(model_path: Path, snr: float, seed: int = 3, gain: int = 10) -> dict:
    if not model_path.exists():
        train_model(model_path)
    bundle = load_model(model_path)
    probe = simulate_link(n_bits=256, mod="qpsk", snr_awgn=snr, seed=seed)
    meta = predict_with_meta(bundle, extract_features(probe, snr_awgn=snr, gain=gain))
    choice = meta["mod"]
    after = simulate_link(n_bits=256, mod=choice, snr_awgn=snr, seed=seed + 1)
    return {
        "probe": probe,
        "chosen_mod": choice,
        "ml_meta": meta,
        "after": after,
    }

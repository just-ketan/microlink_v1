from pathlib import Path

from ml.features.extract import FEATURE_COLS, extract_features
from ml.models.adapter import adaptive_loop
from ml.training.train import train_model
from rf.experiments.link import simulate_link


def test_feature_cols_expanded():
    r = simulate_link(n_bits=64, seed=0)
    f = extract_features(r, snr_awgn=10.0, gain=12)
    assert len(f) == len(FEATURE_COLS)
    assert "ber_probe" in f


def test_train_multi_model(tmp_path: Path):
    info = train_model(tmp_path / "m.joblib", seed=3)
    assert info["best"] in {"random_forest", "gradient_boosting", "mlp"}
    assert len(info["scores"]) == 3


def test_adaptive(tmp_path: Path):
    out = adaptive_loop(tmp_path / "m2.joblib", snr=10.0, seed=2)
    assert "ml_meta" in out
    assert out["ml_meta"]["mod"] in {"bpsk", "qpsk", "qam16"}

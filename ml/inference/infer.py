from __future__ import annotations

from pathlib import Path

import joblib
import numpy as np

from ml.features.extract import FEATURE_COLS


def load_model(path: Path):
    return joblib.load(path)


def _feature_vector(bundle, features: dict) -> np.ndarray:
    cols = bundle.get("feature_cols", FEATURE_COLS)
    return np.array([[features.get(c, 0.0) for c in cols]], dtype=float)


def predict_mod(bundle, features: dict) -> str:
    model = bundle.get("model")
    if model is None:
        raise ValueError("bundle missing model")
    x = _feature_vector(bundle, features)
    y = model.predict(x)[0]
    return str(bundle["enc"].inverse_transform([y])[0])


def predict_with_meta(bundle, features: dict) -> dict:
    mod = predict_mod(bundle, features)
    return {
        "mod": mod,
        "best_model": bundle.get("best", "unknown"),
        "model_scores": bundle.get("scores", {}),
    }

from __future__ import annotations

from pathlib import Path

import joblib
import numpy as np

from ml.features.extract import FEATURE_COLS


def load_model(path: Path):
    return joblib.load(path)


def predict_mod(bundle, features: dict) -> str:
    x = np.array([[features[c] for c in FEATURE_COLS]], dtype=float)
    y = bundle["model"].predict(x)[0]
    return str(bundle["enc"].inverse_transform([y])[0])

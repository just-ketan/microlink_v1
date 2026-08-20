from __future__ import annotations

from pathlib import Path
from typing import Any

import joblib
import numpy as np
from sklearn.ensemble import GradientBoostingClassifier, RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier
from sklearn.preprocessing import LabelEncoder

from ml.features.extract import FEATURE_COLS, generate_dataset


def _build_candidates(seed: int) -> dict[str, Any]:
    return {
        "random_forest": RandomForestClassifier(n_estimators=120, random_state=seed),
        "gradient_boosting": GradientBoostingClassifier(random_state=seed),
        "mlp": MLPClassifier(hidden_layer_sizes=(32, 16), max_iter=400, random_state=seed),
    }


def train_model(path: Path, seed: int = 7) -> dict:
    df = None
    enc = LabelEncoder()
    for attempt in range(5):
        df = generate_dataset(n=96, seed=seed + attempt)
        y_try = enc.fit_transform(df["best_mod"])
        if len(enc.classes_) >= 2:
            break
    if df is None or len(enc.classes_) < 2:
        raise ValueError("could not build multi-class training set")
    x = df[FEATURE_COLS].to_numpy()
    y = enc.transform(df["best_mod"])

    try:
        xtr, xte, ytr, yte = train_test_split(
            x, y, test_size=0.25, random_state=seed, stratify=y
        )
    except ValueError:
        xtr, xte, ytr, yte = train_test_split(x, y, test_size=0.25, random_state=seed)

    scores: dict[str, float] = {}
    fitted: dict[str, Any] = {}
    for name, clf in _build_candidates(seed).items():
        try:
            if len(np.unique(ytr)) < 2:
                continue
            clf.fit(xtr, ytr)
            acc = float(clf.score(xte, yte))
            scores[name] = acc
            fitted[name] = clf
        except ValueError:
            continue

    if not fitted:
        clf = RandomForestClassifier(n_estimators=80, random_state=seed)
        clf.fit(x, y)
        fitted["random_forest"] = clf
        scores["random_forest"] = float(clf.score(x, y))

    best_name = max(scores, key=scores.get)
    bundle = {
        "models": fitted,
        "scores": scores,
        "best": best_name,
        "model": fitted[best_name],
        "enc": enc,
        "feature_cols": FEATURE_COLS,
        "acc": scores[best_name],
    }
    path.parent.mkdir(parents=True, exist_ok=True)
    joblib.dump(bundle, path)
    df.to_csv(path.with_suffix(".csv"), index=False)
    return {"acc": scores[best_name], "best": best_name, "scores": scores, "n": len(df), "path": str(path)}

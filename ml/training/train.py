from __future__ import annotations

from pathlib import Path

import joblib
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

from ml.features.extract import FEATURE_COLS, generate_dataset


def train_model(path: Path, seed: int = 7) -> dict:
    df = generate_dataset(n=24, seed=seed)
    x = df[FEATURE_COLS].to_numpy()
    enc = LabelEncoder()
    y = enc.fit_transform(df["best_mod"])
    xtr, xte, ytr, yte = train_test_split(x, y, test_size=0.25, random_state=seed)
    clf = RandomForestClassifier(n_estimators=80, random_state=seed)
    clf.fit(xtr, ytr)
    acc = float(clf.score(xte, yte))
    path.parent.mkdir(parents=True, exist_ok=True)
    joblib.dump({"model": clf, "enc": enc, "acc": acc}, path)
    df.to_csv(path.with_suffix(".csv"), index=False)
    return {"acc": acc, "n": len(df), "path": str(path)}

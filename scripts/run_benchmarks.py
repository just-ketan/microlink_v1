#!/usr/bin/env python3
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    bench = ROOT / "build" / ("microlink_bench.exe" if sys.platform.startswith("win") else "microlink_bench")
    out = subprocess.check_output([str(bench)], text=True)
    print(out)
    (ROOT / "results").mkdir(exist_ok=True)
    metrics = {}
    for line in out.splitlines():
        if "=" in line:
            k, v = line.split("=", 1)
            metrics[k.strip()] = float(v)
    import time

    t0 = time.perf_counter()
    sys.path.insert(0, str(ROOT))
    from rf.experiments.link import simulate_link
    from ml.inference.infer import load_model, predict_mod
    from ml.training.train import train_model

    simulate_link(n_bits=1024, seed=0)
    metrics["rf_link_s"] = time.perf_counter() - t0
    p = ROOT / "results" / "adapter.joblib"
    if not p.exists():
        train_model(p)
    bundle = load_model(p)
    t1 = time.perf_counter()
    predict_mod(bundle, {"rssi": -20, "snr_meas_db": 8, "evm_pct": 12, "snr_awgn_db": 8})
    metrics["ml_infer_s"] = time.perf_counter() - t1
    (ROOT / "results" / "benchmarks.json").write_text(json.dumps(metrics, indent=2), encoding="utf-8")
    print("wrote results/benchmarks.json")


if __name__ == "__main__":
    main()

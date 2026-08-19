#!/usr/bin/env python3
"""End-to-end demo: start RF engine, run Virtual MCU, ML adaptation, report."""

from __future__ import annotations

import json
import os
import socket
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from ml.models.adapter import adaptive_loop  # noqa: E402


def find_sim() -> Path:
    for p in (ROOT / "build" / "microlink_sim.exe", ROOT / "build" / "microlink_sim"):
        if p.exists():
            return p
    raise FileNotFoundError("build microlink_sim first")


def wait_port(host: str, port: int, timeout: float = 8.0) -> None:
    t0 = time.time()
    while time.time() - t0 < timeout:
        s = socket.socket()
        s.settimeout(0.3)
        try:
            s.connect((host, port))
            s.close()
            return
        except OSError:
            time.sleep(0.1)
    raise TimeoutError("RF engine did not start")


def main() -> int:
    results = ROOT / "results"
    results.mkdir(exist_ok=True)
    env = os.environ.copy()
    env["PYTHONPATH"] = str(ROOT)
    server = subprocess.Popen(
        [sys.executable, str(ROOT / "cosim" / "bridge" / "server.py")],
        cwd=str(ROOT),
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    try:
        wait_port("127.0.0.1", 8765)
        sim = find_sim()
        print("=== Virtual MCU + RF co-simulation ===")
        r = subprocess.run([str(sim), "--host", "127.0.0.1", "--port", "8765", "--ticks", "250"], cwd=str(ROOT))
        print("sim exit", r.returncode)
        print("=== ML adaptation ===")
        loop = adaptive_loop(results / "adapter.joblib", snr=9.0, seed=9)
        report = {
            "sim_exit": r.returncode,
            "chosen_mod": loop["chosen_mod"],
            "ber_after": loop["after"]["ber"],
            "snr_after": loop["after"]["snr_meas_db"],
        }
        (results / "e2e_report.json").write_text(json.dumps(report, indent=2), encoding="utf-8")
        print(json.dumps(report, indent=2))
        return 0 if r.returncode == 0 else r.returncode
    finally:
        server.terminate()
        try:
            server.wait(timeout=2)
        except subprocess.TimeoutExpired:
            server.kill()


if __name__ == "__main__":
    raise SystemExit(main())

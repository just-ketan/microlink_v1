#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    import os

    os.environ["PYTHONPATH"] = str(ROOT)
    sim = ROOT / "build" / ("microlink_tests.exe" if sys.platform.startswith("win") else "microlink_tests")
    subprocess.check_call([str(sim)])
    subprocess.check_call([sys.executable, "-m", "pytest", "-q"], cwd=str(ROOT))


if __name__ == "__main__":
    main()

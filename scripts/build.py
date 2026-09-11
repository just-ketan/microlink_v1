#!/usr/bin/env python3
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    extra = [
        Path(r"C:\Program Files\w64devkit\bin"),
        Path(r"C:\Program Files (x86)\gcc-15.2.0\bin"),
    ]
    for p in extra:
        if p.is_dir():
            os.environ["PATH"] = str(p) + os.pathsep + os.environ.get("PATH", "")
    build = ROOT / "build"
    build.mkdir(exist_ok=True)
    cmake = ["cmake", "-S", str(ROOT), "-B", str(build), "-DCMAKE_BUILD_TYPE=Debug"]
    if sys.platform.startswith("win"):
        cmake += ["-G", "MinGW Makefiles", "-DCMAKE_C_COMPILER=gcc"]
    subprocess.check_call(cmake)
    subprocess.check_call(["cmake", "--build", str(build)])
    print("build ok")


if __name__ == "__main__":
    main()

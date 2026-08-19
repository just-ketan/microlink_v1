from ml.models.adapter import adaptive_loop
from pathlib import Path
import json
from cosim.bridge.server import MODS


def test_adaptive(tmp_path):
    out = adaptive_loop(tmp_path / "model.joblib", snr=12.0, seed=4)
    assert out["chosen_mod"] in {"bpsk", "qpsk", "qam16"}
    assert "ber" in out["after"]


def test_mod_table():
    assert MODS[1] == "qpsk"

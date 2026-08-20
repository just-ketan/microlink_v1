from pathlib import Path

import numpy as np

from rf.em.educational_field import field_grid_2d, standing_wave_slice
from rf.host_sdr.iq_file import IQFileSink, IQFileSource
from rf.host_sdr.radio_host import HostRadio, HostRadioConfig


def test_standing_wave():
    x = np.linspace(0, 1, 32)
    e = standing_wave_slice(1e6, x)
    assert e.shape == (32,)
    assert np.max(e) <= 1.0 + 1e-9


def test_field_grid():
    x = np.linspace(0, 0.5, 16)
    y = np.linspace(-0.1, 0.1, 8)
    g = field_grid_2d(5e5, x, y)
    assert g.shape == (8, 16)


def test_iq_file_roundtrip(tmp_path: Path):
    data = np.exp(1j * np.linspace(0, 1, 64))
    p = tmp_path / "iq.npy"
    np.save(p, data)
    src = IQFileSource(p)
    block = src.read_block(32)
    assert block.size == 32
    out = tmp_path / "out.npy"
    sink = IQFileSink(out)
    sink.write_block(block)
    sink.flush()
    assert out.exists()


def test_host_radio_sim():
    r = HostRadio(HostRadioConfig()).txrx(n_bits=64, snr_db=15, seed=1)
    assert r["mode"] == "simulated"
    assert "ber" in r

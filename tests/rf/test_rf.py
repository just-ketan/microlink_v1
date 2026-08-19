from rf.acoustic_wave.bvd import bvd_impedance, motional_q, resonance_freqs
from rf.channel.models import apply_channel
from rf.filters.ladder.lc_ladder import transfer_response
from rf.modulation.digital import demap, map_bits
from rf.sparameters.twoport import mag_phase
from rf.smith_chart.smith import gamma_to_z, z_to_gamma
from rf.experiments.link import simulate_link
import numpy as np


def test_map_demap_qpsk():
    rng = np.random.default_rng(0)
    bits = rng.integers(0, 2, 64, dtype=np.uint8)
    sym = map_bits(bits, "qpsk")
    hat = demap(sym, "qpsk")[: bits.size]
    assert np.all(hat == bits)


def test_ladder_passband():
    f = np.logspace(5, 8, 64)
    mag, phase, s, _ = transfer_response(f, [1e-6, 1e-6], [2e-9, 2e-9])
    assert mag.shape == f.shape
    assert "s21" in s
    assert np.all(np.isfinite(phase))


def test_sparam_mag():
    f = np.array([1e6])
    mag, phase, s, _ = transfer_response(f, [2e-6], [1e-9])
    m, p = mag_phase(s["s11"])
    assert m.shape == (1,)


def test_smith_roundtrip():
    z = 75 + 20j
    g = z_to_gamma(z)
    z2 = gamma_to_z(g)
    assert abs(z2 - z) < 1e-9


def test_acoustic_resonance():
    fs, fa = resonance_freqs(1e-3, 20e-15, 2e-12)
    assert fa > fs
    f = np.linspace(fs * 0.5, fa * 1.5, 64)
    z = bvd_impedance(f, 10.0, 1e-3, 20e-15, 2e-12)
    assert z.size == 64
    assert motional_q(10.0, 1e-3, 20e-15) > 0


def test_channel_deterministic():
    x = np.ones(32, dtype=np.complex128)
    a = apply_channel(x, np.random.default_rng(1), snr_db=30)
    b = apply_channel(x, np.random.default_rng(1), snr_db=30)
    assert np.allclose(a, b)


def test_link_ber_high_snr():
    r = simulate_link(n_bits=256, mod="bpsk", snr_awgn=20, seed=2)
    assert r["ber"] < 0.05


def test_ml_features():
    from ml.features.extract import extract_features

    f = extract_features(simulate_link(n_bits=128, seed=0))
    assert "rssi" in f

from rf.experiments.link import simulate_link
from ml.models.adapter import adaptive_loop


def test_system_measure_adapt(tmp_path):
    a = simulate_link(n_bits=256, mod="qpsk", snr_awgn=10, seed=1)
    b = adaptive_loop(tmp_path / "m.joblib", snr=10, seed=1)
    assert a["n_bits"] == 256
    assert b["after"]["ber"] >= 0.0

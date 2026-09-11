from __future__ import annotations

import numpy as np

from rf.channel.models import apply_channel
from rf.filters.digital.fir_iir import pulse_shape
from rf.filters.ladder.lc_ladder import behavioral_rf_filter
from rf.modulation.digital import demap, map_bits
from rf.signal.iq import evm_percent, snr_db


def simulate_link(
    n_bits: int = 2048,
    mod: str = "qpsk",
    snr_awgn: float = 12.0,
    seed: int = 0,
    filter_kind: str = "bandpass",
    fc: float = 1e6,
    bw: float = 2e5,
) -> dict:
    rng = np.random.default_rng(seed)
    bits = rng.integers(0, 2, size=n_bits, dtype=np.uint8)
    sym = map_bits(bits, mod)
    tx = pulse_shape(sym, sps=4)
    fs = 4e6
    t = np.arange(tx.size) / fs
    passband = tx * np.exp(1j * 2 * np.pi * fc * t)
    h = behavioral_rf_filter(np.array([fc]), filter_kind, fc, bw, q=8.0, il_db=1.0)[0]
    filt = passband * h
    bb = filt * np.exp(-1j * 2 * np.pi * fc * t)
    rx = apply_channel(bb, rng, snr_db=snr_awgn, fs=fs)
    rx_sym = rx[::4][: sym.size]
    bits_hat = demap(rx_sym, mod)[: bits.size]
    ber = float(np.mean(bits_hat != bits))
    return {
        "mod": mod,
        "snr_awgn_db": snr_awgn,
        "ber": ber,
        "evm_pct": evm_percent(sym, rx_sym),
        "snr_meas_db": snr_db(sym, rx_sym),
        "rssi": float(10 * np.log10(np.mean(np.abs(rx) ** 2) + 1e-18)),
        "n_bits": n_bits,
        "seed": seed,
    }

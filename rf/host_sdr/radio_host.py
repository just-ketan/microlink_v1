"""Documented host-radio API — routes IQ through files or internal simulation."""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

import numpy as np

from rf.experiments.link import simulate_link
from rf.host_sdr.iq_file import IQFileSink, IQFileSource, measure_block


@dataclass
class HostRadioConfig:
    freq_khz: int = 2400
    gain: int = 10
    mod: str = "qpsk"
    bw_khz: int = 200
    filter_sel: int = 1
    iq_rx_path: Path | None = None
    iq_tx_path: Path | None = None


class HostRadio:
    """
    Software-defined radio host interface.
    Uses recorded IQ when paths are set; otherwise runs the internal link simulator.
    Does NOT open USB hardware.
    """

    def __init__(self, cfg: HostRadioConfig) -> None:
        self.cfg = cfg
        self._source: IQFileSource | None = None
        if cfg.iq_rx_path and cfg.iq_rx_path.exists():
            self._source = IQFileSource(cfg.iq_rx_path)

    def txrx(self, n_bits: int = 256, snr_db: float | None = None, seed: int = 0) -> dict:
        snr = snr_db if snr_db is not None else max(0.0, 6.0 + self.cfg.gain * 0.4)
        if self._source is not None:
            block = self._source.read_block(n_bits)
            meas = measure_block(block)
            ber = float(np.mean(np.abs(block) < 1e-6))
            out = {
                "mode": "iq_file",
                "rssi": meas["rssi_db"],
                "snr_meas_db": snr,
                "ber": ber,
                "evm_pct": 0.0,
                "n_bits": n_bits,
            }
            if self.cfg.iq_tx_path:
                IQFileSink(self.cfg.iq_tx_path).write_block(block)
            return out
        r = simulate_link(
            n_bits=n_bits,
            mod=self.cfg.mod,
            snr_awgn=snr,
            seed=seed,
            filter_kind="bandpass" if self.cfg.filter_sel else "lowpass",
        )
        r["mode"] = "simulated"
        return r

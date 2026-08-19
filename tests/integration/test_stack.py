"""Integration-style checks that do not require the C binary."""

from rf.audio.pipeline import audio_over_radio, audio_tone
from rf.matching.lmatch import l_section_match
import numpy as np


def test_audio_path():
    rng = np.random.default_rng(0)
    audio = audio_tone(256, 8000.0)
    r = audio_over_radio(audio, "bpsk", snr_db=25, rng=rng)
    assert "ber" in r


def test_matching_metrics():
    m = l_section_match(50 + 0j, 25 + 10j, 2.4e9)
    assert m["vswr"] >= 1.0
    assert m["return_loss_db"] > 0

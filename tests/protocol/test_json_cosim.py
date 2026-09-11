import json

from cosim.bridge.server import MODS, RadioState


def test_control_message_roundtrip():
    msg = {
        "v": 1,
        "ts": 1000,
        "type": "TXRX",
        "src": "mcu",
        "dst": "rf",
        "seq": 3,
        "cmd": 2,
        "freq_khz": 2400,
        "gain": 12,
        "mod": 1,
        "bw": 200,
        "filter": 1,
    }
    parsed = json.loads(json.dumps(msg))
    assert parsed["type"] == "TXRX"
    assert MODS[parsed["mod"]] == "qpsk"
    s = RadioState()
    assert s.freq_khz == 2400

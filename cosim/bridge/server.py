from __future__ import annotations

import json
import socketserver
from dataclasses import dataclass

from rf.experiments.link import simulate_link


@dataclass
class RadioState:
    freq_khz: int = 2400
    gain: int = 10
    mod: int = 1
    bw: int = 200
    filt: int = 1
    extra_noise: float = 0.0


MODS = {0: "bpsk", 1: "qpsk", 2: "qam16"}


class RfHandler(socketserver.StreamRequestHandler):
    def handle(self) -> None:
        state: RadioState = self.server.state  # type: ignore[attr-defined]
        while True:
            line = self.rfile.readline()
            if not line:
                break
            msg = json.loads(line.decode("utf-8"))
            if self.server.drop:  # type: ignore[attr-defined]
                self.server.drop = False  # type: ignore[attr-defined]
                continue
            state.freq_khz = int(msg.get("freq_khz", state.freq_khz))
            state.gain = int(msg.get("gain", state.gain))
            state.mod = int(msg.get("mod", state.mod))
            state.bw = int(msg.get("bw", state.bw))
            state.filt = int(msg.get("filter", state.filt))
            snr = max(0.0, 6.0 + state.gain * 0.4 - state.extra_noise)
            r = simulate_link(
                n_bits=256,
                mod=MODS.get(state.mod, "qpsk"),
                snr_awgn=snr,
                seed=state.freq_khz % 997,
            )
            rssi = int(max(0, min(255, 90 + state.gain - abs(r["rssi"]))))
            snr_i = int(max(0, min(255, r["snr_meas_db"] + 20)))
            resp = {
                "v": 1,
                "type": "RADIO_STATUS",
                "rssi": rssi,
                "snr": snr_i,
                "ber": r["ber"],
                "seq": msg.get("seq", 0),
            }
            self.wfile.write((json.dumps(resp) + "\n").encode("utf-8"))
            self.wfile.flush()


class RfServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True

    def __init__(self, addr):
        super().__init__(addr, RfHandler)
        self.state = RadioState()
        self.drop = False


def serve(host: str = "127.0.0.1", port: int = 8765) -> None:
    with RfServer((host, port)) as srv:
        print(f"MicroLink RF engine listening on {host}:{port}", flush=True)
        srv.serve_forever()


if __name__ == "__main__":
    serve()

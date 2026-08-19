# Co-simulation

Versioned newline-delimited JSON on TCP `127.0.0.1:8765`.

Example TXRX request from the MCU:

```json
{"v":1,"ts":123000,"type":"TXRX","src":"mcu","dst":"rf","seq":4,"cmd":2,"freq_khz":2400,"gain":10,"mod":1,"bw":200,"filter":1}
```

Response:

```json
{"v":1,"type":"RADIO_STATUS","rssi":100,"snr":28,"ber":0.0,"seq":4}
```

**Tradeoff**: JSON is inspectable and easy to version. Dense I/Q blocks would use length-prefixed binary; this design keeps I/Q inside Python and returns metrics so the firmware register map stays small.

If the engine is down, firmware uses a local deterministic radio model so C tests still run.

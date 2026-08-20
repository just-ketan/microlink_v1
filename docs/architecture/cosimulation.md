# Co-simulation

Versioned newline JSON on TCP `127.0.0.1:8765`.

## IQ file fields (v0.2)

TXRX messages may include:

```json
{"iq_rx": "/path/to/capture.npy", "iq_tx": "/path/to/out.npy"}
```

Or set `MICRLINK_IQ_RX` before starting the server.

Response includes `"mode": "iq_file"` or `"simulated"`.

Tradeoff: JSON control plane stays debuggable; IQ bulk data uses files rather than inline base64.

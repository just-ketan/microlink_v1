# RF chain (behavioral)

```
bits → map → RRC → (optional) passband → behavioral filter / ladder ABCD
    → channel → demap → BER / EVM / SNR
```

## Host SDR (v0.2)

When `HostRadio` is configured with `iq_rx_path`, samples are read from disk instead of generating a fresh link simulation. Supports `.npy` complex arrays and interleaved float32 IQ.

`cosim/bridge/server.py` honors `iq_rx` / `iq_tx` message fields.

## Educational EM slice (v0.2)

`rf/em/educational_field.py` produces a 2D |E| magnitude grid for visualization. Standing wave along x from ladder Zin; exponential decay along y. **Not** 3D FEM.

Ladder filters use cascaded ABCD matrices. Acoustic devices use BVD equivalent circuits.

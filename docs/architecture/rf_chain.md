# RF chain (behavioral)

```
bits → constellation map → RRC pulse shape → (optional) passband mix
    → behavioral RF filter / LC ladder (ABCD)
    → AWGN / fade / CFO / phase / timing
    → downsample → demap → BER / EVM / SNR
```

Ladder filters use cascaded ABCD matrices of series L and shunt C. S-parameters are converted from ABCD with a 50 Ω reference. This is **not** a full electromagnetic solver.

Acoustic-wave devices use a Butterworth–Van Dyke RLC+C0 equivalent circuit, not piezoelectric FEM.

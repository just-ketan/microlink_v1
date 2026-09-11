# ML adaptation

```
probe link → features (6) → best of {RF, GB, MLP} → chosen modulation → second link
```

## Model registry (v0.2)

`ml/training/train.py` fits:

- `RandomForestClassifier`
- `GradientBoostingClassifier`
- `MLPClassifier` (32→16)

Hold-out accuracy selects `best`. Bundle stores all models and scores.

Features: `rssi`, `snr_meas_db`, `evm_pct`, `snr_awgn_db`, `ber_probe`, `gain_proxy`.

Training labels follow an AWGN-oriented modulation policy (BPSK &lt; 7 dB, QPSK 7–14 dB, 16-QAM above) with probe features from real `simulate_link` runs.

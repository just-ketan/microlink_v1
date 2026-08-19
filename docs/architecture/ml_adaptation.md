# ML adaptation

```
measure (simulate_link) → features {rssi, snr, evm, snr_awgn}
  → RandomForestClassifier
  → modulation choice
  → second simulate_link
```

The training set is generated from the same RF simulator (not hand-labeled toy rows). Accuracy is modest by design: the feature space is small and the model is a legitimate baseline, not a paper-grade PHY controller.

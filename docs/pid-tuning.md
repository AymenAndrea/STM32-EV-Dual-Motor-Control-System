# PID Tuning

**Method:** change one gain at a time; run the Stage 3 step test (40 → 80 RPM); record the result.

**Controller:** PI + feed-forward, anti-windup (conditional integration), derivative on measurement (Kd = 0 by default because encoder quantization ≈ 1.4 RPM/count at 100 Hz).

---

## Starting point

| Source | Kp | Ki | Kd | Kff |
|---|---|---|---|---|
| Python simulation (`simulation/`) | ___ | ___ | ___ | ___ |
| Firmware first try (`config.h`) | 0.008 | 0.12 | 0.0 | 0.003 |
| Kff measured from Stage 2 data | — | — | — | ___ |

## Tuning log

| # | Kp | Ki | Kd | Kff | Overshoot | Settling time | Steady-state error | Oscillation? | Notes |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0.008 | 0.12 | 0 | 0.003 | ___ | ___ s | ___ | ___ | First run |
| 2 | ___ | ___ | ___ | ___ | ___ | ___ s | ___ | ___ | ___ |
| 3 | ___ | ___ | ___ | ___ | ___ | ___ s | ___ | ___ | ___ |
| 4 | ___ | ___ | ___ | ___ | ___ | ___ s | ___ | ___ | ___ |
| 5 | ___ | ___ | ___ | ___ | ___ | ___ s | ___ | ___ | ___ |

## Final gains

| Kp | Ki | Kd | Kff |
|---|---|---|---|
| ___ | ___ | ___ | ___ |

**Why these:** ___

## Simulation vs real

![Sim vs real](images/sim-vs-real.png)

| | Simulation | Real hardware |
|---|---|---|
| Rise time | ___ s | ___ s |
| Overshoot | ___ | ___ |
| Settling time | ___ s | ___ s |

**Differences and why:** ___

## What I learned

- ___
- ___

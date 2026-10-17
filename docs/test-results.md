# Test Results

Results from each bring-up stage on real hardware.
Fill in every `___`.

**Setup:** motor supply ___ V (limit ___ A) · wheels off the ground · serial at 115200 baud

---

## Stage 1 — Board, LED, UART

| Test | Expected | Result | Pass |
|---|---|---|---|
| LD2 blinks | Every 500 ms | ___ | ✅ |
| Serial heartbeat | `alive 0, 1, 2…` every 1 s | ___ | ✅ |
| Clock printout | `Clock: 180000000 Hz` | ___ | ___ |

## Stage 2 — Open loop: direction and encoder sign

| Step | Command | Wheel turns (fwd / rev) | RPM sign (+ / −) | Correct? |
|---|---|---|---|---|
| A forward 30 | `+0.3` | ___ | ___ | ___ |
| A reverse 50 | `−0.5` | ___ | ___ | ___ |
| B forward 30 | `+0.3` | ___ | ___ | ___ |
| B reverse 50 | `−0.5` | ___ | ___ | ___ |

**Config changes needed:** `MOTOR_A_INVERT` = ___ · `MOTOR_B_INVERT` = ___ · `ENC_A_INVERT` = ___ · `ENC_B_INVERT` = ___

## Stage 2 — Duty → RPM (open-loop characterization)

Steady-state RPM after ~2 s at each duty.

| Duty | Motor A RPM | Motor B RPM | Motor A current (mA) | Motor B current (mA) |
|---|---|---|---|---|
| 30 % | ___ | ___ | ___ | ___ |
| 50 % | ___ | ___ | ___ | ___ |
| 70 % | ___ | ___ | ___ | ___ |
| 100 % | ___ | ___ | ___ | ___ |

**Lowest duty that starts the wheel (deadband):** A ___ % · B ___ %

**Feed-forward gain:** `kff = duty ÷ RPM` → at 50 %: A = ___ · B = ___ → value used in `config.h`: `PID_KFF = ___`

## Stage 3 — Closed loop step response

Gains used: Kp ___ · Ki ___ · Kd ___ · Kff ___

| Target | Motor | Rise time (to 90 %) | Overshoot (RPM) | Settling time (±2 RPM) | Steady-state error (RPM) | Duty at steady state |
|---|---|---|---|---|---|---|
| 0 → 40 | A | ___ s | ___ | ___ s | ___ | ___ % |
| 0 → 40 | B | ___ s | ___ | ___ s | ___ | ___ % |
| 40 → 80 | A | ___ s | ___ | ___ s | ___ | ___ % |
| 40 → 80 | B | ___ s | ___ | ___ s | ___ | ___ % |

**Disturbance test (pinch the wheel at 80 RPM):** RPM drops to ___ · duty rises to ___ % · current rises to ___ mA · recovers in ___ s

![Step response](images/step-response.png)

## Stage 3 — Sensor accuracy

| Quantity | Firmware reading | Reference (DMM / thermometer) | Error | Pass |
|---|---|---|---|---|
| Bus voltage (INA219 A) | ___ V | ___ V | ___ V | ___ |
| Motor A current at 50 % | ___ mA | ___ mA (DMM in series) | ___ | ___ |
| Motor B current at 50 % | ___ mA | ___ mA | ___ | ___ |
| Temperature (LM35) | ___ °C | ___ °C | ___ °C | ___ |

## Fault tests (Day 4)

Each fault triggered on purpose.

| Fault | How triggered | Expected response | Observed | Time to react | Pass |
|---|---|---|---|---|---|
| Overcurrent | ___ | Motors cut, FAULT state | ___ | ___ ms | ___ |
| Overtemperature | Warm LM35 (fingers / hair dryer) | Motors cut, FAULT state | ___ | ___ ms | ___ |
| INA219 lost | Unplug SDA | FAULT after 5 failed reads | ___ | ___ ms | ___ |
| LM35 unplugged | Remove signal wire | FAULT (invalid temp) | ___ | ___ ms | ___ |
| Stalled wheel | Hold wheel at high duty | FAULT (current high, RPM ≈ 0) | ___ | ___ ms | ___ |
| E-stop button | Press B1 | Motors cut immediately | ___ | ___ ms | ___ |
| Fault clear | ___ | Returns to IDLE only when safe | ___ | — | ___ |

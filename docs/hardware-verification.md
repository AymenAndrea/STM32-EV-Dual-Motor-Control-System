# Hardware Verification

Breadboard checks done **before** powering the motors.
Fill in every `___`. Mark Pass with ✅ or ❌.

**Date:** ___
**Equipment:** DMM model ___ · Bench supply model ___

---

## 1. Power OFF — shorts and continuity

USB unplugged, bench supply off.

| # | Check | DMM mode | Expected | Measured | Pass |
|---|---|---|---|---|---|
| 1.1 | 3.3 V rail ↔ GND | Resistance | Not ~0 Ω | ___ Ω | ___ |
| 1.2 | Motor rail (VM) ↔ GND | Resistance | Not ~0 Ω | ___ Ω | ___ |
| 1.3 | 3.3 V rail ↔ motor rail | Resistance | Not ~0 Ω | ___ Ω | ___ |
| 1.4 | Nucleo GND ↔ TB6612 GND | Continuity | Beep | ___ | ___ |
| 1.5 | Nucleo GND ↔ bench supply GND | Continuity | Beep | ___ | ___ |
| 1.6 | Nucleo GND ↔ INA219 A GND / INA219 B GND | Continuity | Beep | ___ | ___ |
| 1.7 | Nucleo GND ↔ LM35 GND | Continuity | Beep | ___ | ___ |
| 1.8 | Nucleo GND ↔ encoder A GND / encoder B GND | Continuity | Beep | ___ | ___ |

## 2. Power OFF — signal wires (end to end)

| Signal | From (Nucleo) | To | Beep | Pass |
|---|---|---|---|---|
| PWMA | PA8 (D7) | TB6612 PWMA | ___ | ___ |
| AIN1 | PB5 (D4) | TB6612 AIN1 | ___ | ___ |
| AIN2 | PB4 (D5) | TB6612 AIN2 | ___ | ___ |
| PWMB | PA9 (D8) | TB6612 PWMB | ___ | ___ |
| BIN1 | PB10 (D6) | TB6612 BIN1 | ___ | ___ |
| BIN2 | PA10 (D2) | TB6612 BIN2 | ___ | ___ |
| STBY | PC7 (D9) | TB6612 STBY | ___ | ___ |
| Enc A ch1 | PA6 (D12) | Motor A C1 | ___ | ___ |
| Enc A ch2 | PA7 (D11) | Motor A C2 | ___ | ___ |
| Enc B ch1 | PB6 (D10) | Motor B C1 | ___ | ___ |
| Enc B ch2 | PB7 (CN7-21) | Motor B C2 | ___ | ___ |
| I2C SCL | PB8 | INA219 A SCL + INA219 B SCL | ___ | ___ |
| I2C SDA | PB9 | INA219 A SDA + INA219 B SDA | ___ | ___ |
| LM35 out | PA0 (A0) | LM35 middle pin | ___ | ___ |

## 3. USB power only (motor supply still OFF)

| # | Measure (to GND) | Expected | Measured | Pass |
|---|---|---|---|---|
| 3.1 | 3.3 V rail | 3.2 – 3.4 V | ___ V | ___ |
| 3.2 | TB6612 VCC | ≈ 3.3 V | ___ V | ___ |
| 3.3 | Encoder A VCC / Encoder B VCC | ≈ 3.3 V | ___ V / ___ V | ___ |
| 3.4 | INA219 A VCC / INA219 B VCC | ≈ 3.3 V | ___ V / ___ V | ___ |
| 3.5 | LM35 output | 0.20 – 0.30 V (room temp) | ___ V (= ___ °C) | ___ |
| 3.6 | Stage 1 firmware still runs (LED + `alive` counter) | Yes | ___ | ___ |

## 4. Bench supply

| # | Check | Expected | Measured | Pass |
|---|---|---|---|---|
| 4.1 | Supply set voltage (before connecting) | 6.0 V | ___ V | ___ |
| 4.2 | Supply current limit (before connecting) | 0.5 A | ___ A | ___ |
| 4.3 | VM at TB6612 after connecting | ≈ 6.0 V | ___ V | ___ |
| 4.4 | Idle current draw (motors stopped) | Small (mA) | ___ mA | ___ |

## Problems found and fixes

| Problem | Cause | Fix |
|---|---|---|
| ___ | ___ | ___ |

## Result

☐ All checks passed — safe to run Stage 2.

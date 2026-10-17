# Logic Analyzer Captures

**Analyzer:** ___ · **Software:** ___ · **Sample rate:** ___ MS/s

Connect analyzer GND to the Nucleo GND first.

---

## 1. PWM — Motor A (PA8)

| Setting | Expected | Measured |
|---|---|---|
| Frequency | 20.00 kHz | ___ kHz |
| Period | 50.0 µs | ___ µs |
| Duty at 30 % command | 30 % (15.0 µs high) | ___ % (___ µs) |
| Duty at 50 % command | 50 % (25.0 µs high) | ___ % (___ µs) |

![PWM 20 kHz](images/la-pwm-20khz.png)

**Notes:** ___

## 2. Encoder quadrature — Motor A (PA6 + PA7)

| Check | Expected | Observed |
|---|---|---|
| Two square waves | Yes | ___ |
| Phase shift | ≈ 90° | ___ |
| Which channel leads when forward | — | ___ |
| Pulse frequency at ___ RPM | (RPM × 4200 ÷ 4 ÷ 60) Hz = ___ Hz | ___ Hz |

![Quadrature](images/la-quadrature.png)

## 3. I2C — INA219 (PB8 SCL, PB9 SDA)

| Check | Expected | Observed |
|---|---|---|
| Addresses seen | 0x40 and 0x41 | ___ |
| ACK after address | Yes | ___ |
| Clock speed | 100 kHz (or 400 kHz) | ___ kHz |
| Time for one full read (shunt + bus) | — | ___ µs |

![I2C decode](images/la-i2c-decode.png)

## 4. Control-loop timing (PC8 probe, Day 4)

PC8 goes HIGH at the start of the TIM6 callback and LOW at the end.

| Measurement | Expected | Measured |
|---|---|---|
| Period between pulses | 10.00 ms (100 Hz) | ___ ms |
| ISR duration (pulse width) | ≪ 10 ms | ___ µs |
| CPU load from the control loop | width ÷ period | ___ % |
| Jitter in period | Small | ± ___ µs |

![ISR timing](images/la-isr-timing.png)

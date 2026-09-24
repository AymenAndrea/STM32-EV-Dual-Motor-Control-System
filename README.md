# STM32 EV Dual-Motor Control System

A closed-loop, dual-motor speed controller built on an STM32 Nucleo-F446RE,
meant as a small-scale demonstration of the control principles behind real
EV traction motor systems. Each motor runs its own PID loop, taking target
RPM in and using quadrature encoder feedback to hold that speed, with
current, voltage, and temperature monitored live and a basic fault state
machine to shut things down safely if something goes wrong.

This is a solo project — hardware, firmware, and PID tuning all done and
tested by me — built to get hands-on with embedded control systems ahead
of co-op applications in embedded/hardware roles.

## What it does

- Independent closed-loop PID speed control on two motors at once
  (target RPM → PWM → measured RPM via encoder → correction)
- Live current/voltage sensing per motor (INA219, I2C)
- Temperature sensing (LM35, ADC)
- Overcurrent and overtemperature fault detection, with the motors cut
  until the fault clears
- A simple state machine: Normal / Regen-simulated / Fault
- UART telemetry (target/measured RPM, current, voltage, temperature,
  state) streamed over the onboard ST-LINK virtual COM port

## Why these parts

- **TB6612FNG** instead of the more common L298N — it's MOSFET-based, so
  the voltage drop across the driver is much smaller, which matters more
  on small 6V motors
- **INA219** instead of an ACS712 — it reads current, voltage, and power
  over I2C instead of needing its own ADC calibration, and it's good
  practice with I2C
- **Nucleo-F446RE** over the G474RE — deliberately picking the board
  without built-in FOC peripherals for this build, and saving that board
  for a possible BLDC/FOC project later

## Control loop

```
Target RPM → PID Controller → PWM Output → Motor Driver → Motor → Encoder
                    ↑                                                 |
                    └──────────────── Measured RPM ────────────────────┘
```

Both motors run this loop independently on the same sample period, so they
can be commanded and monitored at the same time for differential-drive-
style behavior.

## Hardware

| Component | Part | Why |
|---|---|---|
| MCU | STM32 Nucleo-F446RE | Genuine board, sourced from DigiKey |
| Motor Driver | TB6612FNG | MOSFET-based, low voltage drop |
| Motors | 2× N20 DC gear motor, 6V, w/ quadrature encoder | |
| Current/Voltage Sensing | INA219 (I2C) ×2 | Digital readout, one part per motor |
| Temperature Sensing | LM35 (ADC) | |
| Test Equipment | Bench power supply, USB logic analyzer, DMM | |

## Scope

This is deliberately kept to what's achievable in the project timeline:

- Brushed DC motors with PID control, **not** 3-phase BLDC/FOC (that's a
  possible future upgrade, not part of this build)
- No full dual-STM32 CAN network — if CAN gets added at all, it'll be a
  single node, not a second ECU
- No chassis — motors are bench-mounted for testing, not built into a
  rolling platform

## Repo layout

```
├── docs/                    requirements, architecture, parts list, dev log
├── simulation/              Python PID simulation used for gain tuning
└── firmware/                STM32CubeIDE project (HAL library)
```

## Status

Still in progress. See `docs/development-log.md` for a running log of what's
been done and what's next, and `docs/system-architecture.md` for pin
assignments and design decisions. Breadboard bring-up is happening in
stages (bare board → single motor open-loop → closed-loop PID → second
motor → sensors → fault detection → telemetry) before any custom PCB gets
designed.

## License

MIT — see `LICENSE`.

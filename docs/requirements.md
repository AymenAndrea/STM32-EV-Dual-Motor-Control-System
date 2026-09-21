# Requirements

## Functional Goal
A closed-loop, dual-motor speed control system built on the STM32F446RE,
designed as a simplified demonstration of the control principles used in
real EV traction motor systems.

## Core Requirements
- Closed-loop PID speed control per motor (target RPM → PWM → measured RPM
  via encoder → correction)
- Two independently controlled motors (differential drive), synchronized
- Real-time current and voltage monitoring per motor (INA219, I2C)
- Temperature monitoring (LM35, analog/ADC)
- Fault detection:
  - Overcurrent cutoff
  - Overtemperature cutoff
- Mode state machine: **Normal** / **Regenerative-braking-simulated** / **Fault**
- UART telemetry output (for live monitoring / future Python dashboard)

## Non-Functional Requirements
- Solo-built and solo-coded, for genuine hands-on skill development
- Breadboard-verified before any custom PCB is designed
- Documented incrementally (development log) rather than written up only
  after completion

## Explicit Scope Boundaries
To keep the project achievable within the intended timeline, the following
are **deliberately out of scope** for this build:
- **3-phase BLDC/FOC motor control** — brushed DC + PID is used instead.
  A BLDC/FOC upgrade is a possible future stretch goal, not part of this
  build.
- **Full dual-STM32 CAN diagnostic network** — CAN, if added at all, will
  be a single-node addition to the existing board, not a second ECU with
  a full CAN bus.
- **Physical chassis/vehicle body** — motors are bench-mounted for testing,
  not built into a rolling platform.

## Hardware List
| Component | Part |
|---|---|
| MCU | STM32 Nucleo-F446RE |
| Motor Driver | TB6612FNG |
| Motors | 2× N20 DC gear motor w/ quadrature encoder, 6V |
| Current/Voltage Sensing | INA219 (I2C) |
| Temperature Sensing | LM35 (analog) |
| Bench Test Equipment | Adjustable bench power supply, USB logic analyzer, digital multimeter |

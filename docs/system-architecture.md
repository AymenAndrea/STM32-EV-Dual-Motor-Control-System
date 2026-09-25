# System Architecture

## Control Loop (per motor)

```
Target RPM → PID Controller → PWM Output → Motor Driver → Motor → Encoder
                    ↑                                                 |
                    └──────────────── Measured RPM ────────────────────┘
```

Each motor runs its own independent PID loop on the same sample period, so
both motors can be commanded and monitored simultaneously.

## Component Roles

| Component | Role |
|---|---|
| STM32 Nucleo-F446RE | Runs the control loops, reads sensors, drives outputs |
| TB6612FNG | Converts PWM + direction signals into actual motor drive current |
| N20 motor + encoder | Actuator + feedback sensor (quadrature encoder gives speed and direction) |
| INA219 | Reports live current/voltage draw per motor over I2C |
| LM35 | Reports ambient/component temperature via analog voltage into the ADC |

## Component Selection Reasoning

**TB6612FNG over L298N:** the L298N is an older bipolar-transistor design
with a significant (~2V) voltage drop across its outputs. The TB6612FNG is
a modern MOSFET-based driver with much lower loss, better suited to small
motors like the N20s used here.

**INA219 over ACS712:** the ACS712 is a simple analog current sensor
requiring ADC calibration and only measures current. The INA219 communicates
digitally over I2C and reports current, bus voltage, and power in one part —
more convenient for this project's telemetry needs, and it builds I2C skill
directly.

## Pin Assignments

| Function | Peripheral | Pin | Notes |
|---|---|---|---|
| Motor A PWM | TIMx_CHx | PA8 | |
| Motor A direction (AIN1/AIN2) | GPIO Output |  | |
| Motor B PWM | TIMx_CHx | PA15 | |
| Motor B direction (BIN1/BIN2) | GPIO Output |  | |
| Driver standby (STBY) | GPIO Output |  | Shared by both channels |
| Encoder A (channels A/B) | TIMx encoder mode | PA6/PA7 | Consumes a full timer |
| Encoder B (channels A/B) | TIMx encoder mode | PB6/PB7 | Consumes a full timer |
| INA219 ×2 | I2Cx_SDA / SCL | PB8/PB9 | Shared bus, distinct I2C addresses |
| LM35 | ADCx_INx | PA0-WKUP | Analog input |
| UART telemetry | USARTx_TX / RX | PA2/PA3 | Via onboard ST-LINK virtual COM port |
| Status LEDs | GPIO Output |  | Fault/mode indication |
| Mode button(s) | GPIO Input |  | Pull-up/down configured |

**Reserved, do not reassign:** SWD debug pins (ST-LINK), onboard user
LED/button pins, virtual COM port UART pins.

## State Machine

| State | Behavior |
|---|---|
| Normal | Standard closed-loop PID speed control, both motors |
| Regen-simulated | Simulated regenerative-braking behavior on deceleration |
| Fault | Triggered by overcurrent or overtemperature; motors disabled until cleared |

## Data Flow Out (Telemetry)

STM32 → UART → host computer, streaming: target RPM, measured RPM, current,
voltage, temperature, and current state, per motor. Intended to eventually
feed a simple Python-side dashboard/logger (separate from the offline PID
simulation used for gain tuning).

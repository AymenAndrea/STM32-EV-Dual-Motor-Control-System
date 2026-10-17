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
| Motor A PWM | TIM1_CH1 | PA8 | 20 kHz (PSC 0, ARR 8999), 180 MHz timer clock |
| Motor A direction (AIN1/AIN2) | GPIO Output | PB5/PB4 | User labels AIN1/AIN2 |
| Motor B PWM | TIM1_CH2 | PA9 | Shares TIM1 with Motor A, same 20 kHz |
| Motor B direction (BIN1/BIN2) | GPIO Output | PB10/PA10 | User labels BIN1/BIN2 |
| Driver standby (STBY) | GPIO Output | PC7 | Shared by both channels; HIGH = driver enabled |
| Encoder A (channels A/B) | TIM3 encoder mode (CH1/CH2) | PA6/PA7 | Consumes a full timer; 4200 counts per wheel rev |
| Encoder B (channels A/B) | TIM4 encoder mode (CH1/CH2) | PB6/PB7 | Consumes a full timer; 4200 counts per wheel rev |
| Control loop tick | TIM6 (basic timer) | — (internal) | 100 Hz (PSC 8999, ARR 99), update interrupt enabled |
| INA219 ×2 | I2C1_SCL / I2C1_SDA | PB8/PB9 | Shared bus; 0x40 = motor A, 0x41 = motor B (A0 bridged) |
| LM35 | ADC1_IN0 | PA0-WKUP | Analog input, 12-bit, 480-cycle sampling, 16-sample average |
| UART telemetry | USART2_TX / USART2_RX | PA2/PA3 | 115200 baud, via onboard ST-LINK virtual COM port |
| Status LED | GPIO Output (LD2) | PA5 | Onboard green LED; fault/mode indication |
| Mode button | GPIO Input (B1) | PC13 | Onboard blue button, active-low (external pull-up on the Nucleo) |
| Timing probe (scope/LA) | GPIO Output | PC8 | Toggled in the control loop to measure ISR timing |

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

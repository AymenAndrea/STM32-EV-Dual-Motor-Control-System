# Firmware

STM32CubeIDE project for the Nucleo-F446RE (HAL library).

## What's in this folder

| Item | What it is |
|---|---|
| `Core/Inc`, `Core/Src` | Application code (motor, encoder, PID, sensors, bring-up) + CubeMX-generated code |
| `Drivers/` | ST HAL + CMSIS libraries |
| `DualMotor.ioc` | CubeMX configuration (clocks, timers, pins) |
| `.project`, `.cproject`, `.mxproject` | CubeIDE project settings |
| `STM32F446RETX_FLASH.ld`, `STM32F446RETX_RAM.ld` | Linker scripts (memory map) |
| `.settings/`, `DualMotor Debug.launch` | IDE and debugger settings |

`Debug/` (build output) is excluded by `.gitignore` and is recreated on every build.

## Application modules

| File | Job |
|---|---|
| `config.h` | All tunable settings: `STAGE`, PID gains, limits, I2C addresses, invert flags |
| `board.c/.h` | Every hardware instance (motors, encoders, PIDs, sensors) in one place |
| `motor.c/.h` | Signed duty (−1…+1) → direction pins + 20 kHz PWM |
| `encoder.c/.h` | Quadrature count delta (16-bit rollover safe) → RPM |
| `pid.c/.h` | PI + feed-forward controller with anti-windup (no HAL calls) |
| `ina219.c/.h` | I2C current / bus-voltage driver |
| `lm35.c/.h` | ADC temperature driver (16-sample average) |
| `bringup.c/.h` | Staged hardware tests selected by `STAGE` |
| `app.c/.h` | Top level, `printf` → UART, 100 Hz control-loop callback |

## Build and flash

1. STM32CubeIDE → **File → Import → General → Existing Projects into Workspace** → select this `firmware` folder
   (leave "Copy projects into workspace" unticked).
2. Set `STAGE` in `Core/Inc/config.h` (1 = heartbeat, 2 = open-loop + RPM, 3 = closed-loop PID + sensors).
3. Build (hammer) → Run. Serial terminal on the ST-LINK COM port at **115200 baud**.

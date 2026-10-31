# Development Log

## [Aug 28, 2026] — Project definition & component research

**Status:** Defining scope and selecting components before any code or
wiring.

**What I did:**
- Defined the project as an EV-style, closed-loop dual-motor speed
  controller, scoped deliberately to brushed DC + PID (not BLDC/FOC) for
  this build
- Researched and finalized component choices: STM32 Nucleo-F446RE,
  TB6612FNG (over L298N), INA219 (over ACS712), LM35, N20 encoder motors
- Verified NUCLEO-F446RE authenticity by sourcing from an official
  distributor rather than an unverified third-party listing

**Problems encountered:**
- None yet — planning stage

**Next:**
- Finalize pin assignments
- Begin PID control simulation in Python ahead of hardware arrival

---

## [Sept 15, 2026] — Simulation & toolchain setup

**Status:** Preparing firmware structure and planning the control approach.

**What I did:**
- Installed STM32CubeIDE, explored the CubeMX pin-configuration tool
- Planned a Python PID simulation to test control-gain behavior before
  the hardware arrived (not built at this point; see later entry)
- Planned pin assignments: PWM timers, encoder-mode timers, I2C bus, ADC
  input, UART — documented in `system-architecture.md`

**Problems encountered:**
- None significant — this stage was research/planning based

**Next:**
- Write initial firmware skeleton in CubeIDE
- Order remaining components

---

## [Sept 18, 2026] — Firmware skeleton

**Status:** Core control logic structured in code, not yet tested on
hardware.

**What I did:**
- Wrote initial firmware structure in CubeIDE: PID loop logic and
  fault-detection state machine skeleton
- Structured the project for later hardware bring-up, matching the
  Core/Src, Core/Inc, Drivers layout CubeIDE generates natively

**Problems encountered:**
- None yet — no hardware to test against at this stage

**Next:**
- Wait for parts to arrive, then begin incremental hardware bring-up

---

## [Sept 20, 2026] — Hardware arrived, bring-up begins

**Status:** All components delivered. Beginning incremental bring-up:
bare board → UART → single motor open-loop → encoder → closed-loop PID →
second motor → sensors → fault detection → telemetry.

**What I did:**
- Verified Nucleo board detected correctly in STM32CubeIDE
- Flashed a basic LED-blink test to confirm board, cable, ST-LINK, and
  toolchain are all functioning before wiring any external components

**What I verified:**
- Onboard LED blinks as expected — board/ST-LINK/toolchain confirmed working

**Problems encountered:**
- None yet at this stage

**Next:**
- Bring up UART over the ST-LINK virtual COM port for debug output
- Begin single-motor open-loop PWM testing (no encoder/closed-loop yet)

- None — board was detected and flashed without issues

**Next:**
- Wire the full circuit on a breadboard
- Bring up UART so the board can report status over serial

---

## [Sept 28, 2026] — Full circuit wired on breadboard

**Status:** All components wired on a breadboard. Not yet verified with a
multimeter.

**What I did:**
- Set up a 3.3 V logic rail from the Nucleo and a separate motor rail
  fed from a bench power supply, with all grounds tied to a common GND
- Wired the TB6612FNG: PWMA → PA8, PWMB → PA9 (TIM1 CH1/CH2),
  AIN1 → PB5, AIN2 → PB4, BIN1 → PB10, BIN2 → PA10, STBY → PC7,
  VCC → 3.3 V, VM → motor rail
- Wired both N20 encoders on 3.3 V: encoder A → PA6/PA7 (TIM3),
  encoder B → PB6/PB7 (TIM4)
- Wired both INA219s into the motor power paths on a shared I2C bus
  (SCL → PB8, SDA → PB9); bridged A0 on the second board so the
  addresses are 0x40 (motor A) and 0x41 (motor B)
- Wired the LM35: +Vs → 3.3 V, Vout → PA0 (ADC1 channel 0), GND → GND

**What I verified:**
- Nothing electrical yet — wiring complete but untested

**Problems encountered:**
- None yet — continuity, rail voltages, and short checks still pending
- Bench supply voltage for the motor rail not set yet

**Next:**
- Multimeter check of rails and continuity before powering motors
- Configure all peripherals in CubeMX to match this pin map

---

## [Oct 2, 2026] — Peripheral configuration & UART bring-up

**Status:** All peripherals configured; board reporting over serial.

**What I did:**
- Configured clocks and peripherals in CubeMX:
  - System clock: 180 MHz
  - TIM1: PWM at 20 kHz (PSC 0, ARR 8999), above the audible range
    so the motors don't whine
  - TIM3/TIM4: encoder mode
  - TIM6: 100 Hz control tick (PSC 8999, ARR 99) with its interrupt
    enabled
  - USART2: 115200 baud
  - I2C1 on PB8/PB9; ADC1 channel 0 on PA0
- Added user labels to the GPIO pins (AIN1, AIN2, BIN1, BIN2, STBY) so
  the code uses names instead of raw pin numbers
- Restructured the firmware into modules:
  - `app.c`: top level
  - `bringup.c`: staged hardware tests
  - `config.h`: all tunable settings in one place
- Kept `main.c` changes to 3 lines inside the USER CODE blocks, so
  CubeMX can regenerate it safely
- Redirected `printf` to USART2 by implementing `_write()`
- Added a `STAGE` switch in `config.h` to bring up one subsystem at a
  time
- Used non-blocking timing (`HAL_GetTick()` comparisons) instead of
  `HAL_Delay()`, so several tasks can run in parallel

**What I verified:**
- Stage 1 running on hardware: the LED blinks every 500 ms and the
  serial terminal prints `alive 0, alive 1, alive 2...` every second
- This confirms the clock config, UART, and printf are working

**Problems encountered:**
- None — learned why `HAL_Delay()` is avoided: it blocks the entire
  program

**Next:**
- Write the motor driver module

---

## [Oct 5, 2026] — Motor driver module

**Status:** Motor driver written and compiling; not yet tested on
hardware.

**What I did:**
- Created `motor.h`/`motor.c` with a `Motor` struct: timer handle, PWM
  channel, direction pins, invert flag, last duty
- `Motor_Set()` takes a signed duty (−1.0 to +1.0):
  - Clamps it to ±MAX_DUTY
  - Applies the invert flag
  - Sets direction from the sign (IN1/IN2)
  - Sets speed from the magnitude (CCR = |duty| × 9000)
- Added `Motor_Coast()`, `Motor_Init()`, and `Motor_Standby()` (drives
  the TB6612 STBY pin)
- Created `board.h`/`board.c` to define `motorA`/`motorB` in one place
  and share them using `extern`
- Added an invert flag per motor in `config.h`, so a motor mounted
  backwards is fixed in software instead of by rewiring

**What I verified:**
- Builds with 0 errors

**Problems encountered:**
- Variable scope: used variables in `Motor_Coast()` that only existed
  inside `Motor_Set()` — replaced them with constants
- Wrong HAL names: `GPIO_PIN_High/Low` → `GPIO_PIN_SET/RESET`
- Pointers: the struct stores `&htim1` (the address of the shared timer
  handle, not a copy); inside functions that already receive a pointer,
  pass `m`, not `&m`

**Next:**
- Write an automated motor test sequence
- Add encoder reading

---

## [Oct 8, 2026] — Motor test sequence & encoder module

**Status:** Open-loop test sequence and encoder driver written;
compiling.

**What I did:**
- Wrote a Stage 2 test sequence that steps every 2 s: A forward 30% →
  A reverse 50% → A stop → B forward 30% → B reverse 50% → B stop →
  repeat
- Created `encoder.h`/`encoder.c`:
  - Delta = new count − old count; the sign gives the direction
  - Handles 16-bit counter rollover with
    `(int16_t)(uint16_t)(now - last)`
  - Keeps a running total and computes RPM
- `Encoder_Init()` stores the current counter value, so the first
  reading isn't garbage
- RPM = delta / 4200 × 100 × 60 (counts → revolutions → per second →
  per minute)

**What I verified:**
- Builds with 0 errors

**Problems encountered:**
- Off-by-one: the step counter reset at 5 and skipped the last step —
  fixed to reset at 6
- Resolution limit: 1 count ≈ 1.4 RPM at 100 Hz, so the speed reading
  will jitter slightly — noted for the controller design

**Next:**
- Sample the encoders at a fixed rate from a timer interrupt

---

## [Oct 10, 2026] — 100 Hz control tick & live RPM

**Status:** Open-loop stage complete in firmware; awaiting hardware
test.

**What I did:**
- Implemented `HAL_TIM_PeriodElapsedCallback()`:
  - Checks `htim->Instance == TIM6`, because the callback is shared by
    all timers
  - Updates both encoders every 10 ms
- Started TIM6 last during init, so the interrupt never runs before the
  motors and encoders are ready
- Added a live RPM printout every 500 ms
- `%f` is disabled in this toolchain, so values are cast to `(long)`
  and printed with `%ld`
- Synced and reviewed all project files

**What I verified:**
- Stage 2 firmware builds with 0 errors

**Problems encountered:**
- None — learned to keep interrupt callbacks short and to start
  interrupts only after everything they touch is initialized

**Next:**
- Implement a PID controller so each wheel holds a target RPM on its own

---

## [Oct 13, 2026] — PID controller module

**Status:** PID controller written and compiling.

**What I did:**
- Created `pid.h`/`pid.c` with a `PID` struct holding the gains (kp, ki,
  kd, kff) and state (integral, previous measurement)
- Built `PID_Update()` one term at a time:
  - Error = setpoint − measured
  - P = kp × error
  - Feed-forward = kff × setpoint (a starting guess for the duty)
  - I accumulates ki × error × dt to remove steady-state error
  - D is computed on the measurement rather than the error, to avoid
    "derivative kick" when the setpoint jumps
  - Output clamped to ±1.0
- Anti-windup: the integral is only saved when the output isn't
  saturated, plus a ±1.0 hard cap on the integral
- `PID_Reset()` clears the integral whenever a motor stops
- Starting gains in `config.h`: Kp 0.008, Ki 0.12, Kff 0.003, Kd 0
  (D is off because of the ±1.4 RPM encoder jitter; the controller runs
  as PI + feed-forward)

**What I verified:**
- Builds with 0 errors
- Hand-traced one step: target 60, measured 57 → output 0.225 duty

**Problems encountered:**
- Line order matters: `prev_meas` must be updated after it's used in the
  D term, otherwise D is always 0

**Next:**
- Close the loop in the 10 ms interrupt
- Add current and temperature sensing

---

## [Oct 15, 2026] — Closed-loop control & sensors

**Status:** Closed-loop speed control and sensors integrated; compiling,
awaiting hardware test.

**What I did:**
- Created `pidA`/`pidB` and `targetA`/`targetB`
  - The targets are `volatile`, because the main loop writes them and
    the interrupt reads them
- Closed the loop in the TIM6 interrupt: read encoders → PID →
  `Motor_Set()` every 10 ms
- A target of 0 coasts the motor and resets its PID, so the integral
  doesn't build up while stopped
- Stage 3 test: the targets step 40 → 80 → 0 RPM every 3 s, with the
  target, RPM, and duty % printed every 200 ms
- Integrated the INA219 driver over I2C:
  - Current = shunt voltage (10 µV per count) ÷ 0.1 Ω, signed
  - Bus voltage = raw value shifted right 3 bits × 4 mV
  - A failed-read counter for later fault detection
- Integrated the LM35 driver over the ADC: average of 16 samples,
  mV = raw × 3300 / 4095, °C = mV / 10
- Sensors are read every 100 ms in the main loop, not the interrupt,
  because I2C and ADC reads are too slow for the 10 ms control tick

**What I verified:**
- Stage 3 builds with 0 errors

**Problems encountered:**
- Linker error "multiple definition of PID_Init/PID_Reset/PID_Update",
  caused by `#include "pid.c"` in `app.c`, which compiled the file
  twice — removed it. Only `.h` files should be included

**Next:**
- Hardware verification:
  - Multimeter checks, then set the bench supply voltage
  - Run Stage 2 (directions, encoder signs)
  - Run Stage 3 (holds 40/80 RPM?)
  - Check the sensor readings against the multimeter
- Add a state machine (IDLE / RUN / FAULT) with fault detection:
  overcurrent, overtemperature, sensor failure, stalled wheel

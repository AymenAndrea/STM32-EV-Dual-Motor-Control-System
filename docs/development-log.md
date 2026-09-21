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

**Status:** Preparing firmware structure and validating control approach
while hardware ships.

**What I did:**
- Installed STM32CubeIDE, explored the CubeMX pin-configuration tool
- Wrote a Python PID simulation (`simulation/pid_simulation.py`) to test
  and tune control-gain behavior before real hardware was available
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

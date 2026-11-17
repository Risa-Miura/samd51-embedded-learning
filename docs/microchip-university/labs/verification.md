# Verification and remaining bench actions

## What software validation can establish

These are independently written register-level C implementations checked against the actual ATSAMD51P20A DFP. Build checks establish syntax, types, symbol availability, target/ABI and memory-layout consistency. They do not prove correct soldering, actual oscillator startup, peripheral bus transactions or a timing result.

The initial validation record is provided with this delivery and in the repository's lab README. No source image includes user-row configuration words. ELF load segments and BIN contents are checked before a programming command is allowed. The protected-region read/compare procedure itself must still be exercised with hardware.

## Software checks completed for this delivery

- All five open-loop applications and all four Lab1 open/closed, autonomous/button-gated combinations compiled with GCC9.2.1 and warnings as errors (eight distinct images).
- Each image passed the0x4000 load boundary, initial stack/Thumb reset-vector and exact ELF/BIN checks; no user-row/configuration image is present.
- Five negative/positive image/programming-guard unit tests passed.
- Native MPLAB attach settings for all five labs were checked against the installed Debug Adapter 2.2.0 schema (device/tool/SWD, ELF, DFP and speed units). Native target operation remains pending.
- The prepared GDB executable starts; OpenOCD and the default/HID configurations parse without initializing an adapter.
- Strict MkDocs build, rendered local-link/anchor check and A–K section checks passed. No browser interaction or physical target operation is implied by these checks.

[Detailed source/build validation record](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/VALIDATION.md).

## Lab 1 bring-up checklist

- [ ] Confirm exact board revision, Atmel-ICE kit/cable, AD2 harness, USB data cables and resistors.
- [ ] Confirm the X2 pin 1/key mapping, SAM port, grounds, VTref and reset connections.
- [ ] Measure target3.3 V; keep AD2 sources/Supplies disabled initially.
- [ ] Build the open-loop Lab 1 application; inspect target,0x4000 vectors, stack and load regions.
- [ ] Connect/read/halt through ICE; record firmware/driver/serial, device ID and protection state.
- [ ] Archive any existing application needed before replacing it.
- [ ] Program the application with the explicit script; record verify success and unchanged bootloader/user-row comparisons.
- [ ] Select MPLAB native attach in VS Code; reach application main through the retained bootloader, step C code, hit one temporary timer breakpoint, inspect variables/registers, then remove breakpoints. Record any fallback to GDB.
- [ ] Observe D13/D12, D35 timer reference, D34 ISR pulse and D37 divided clock.
- [ ] Run the held-button variant and record short/long input behavior.
- [ ] Run closed loop; verify crystal readiness/fine/coarse lock and repeat timing measurements.
- [ ] Verify normal reset/double-tap bootloader entry and retain backup/log evidence.

## Later labs

- [ ] Lab 2: RTC compare/NVIC/flag behavior and measured toggle interval, with internal oscillator uncertainty.
- [ ] Lab 3: safe verified stimulus, PWM duty response, DMA single-beat behavior, suspend/resume and overrun/error monitoring.
- [ ] Lab 4: correct event generator/user routing, seeded feedback acquisition, RTC-driven PORT toggle, qualified EIC edge and cadence change.
- [ ] Lab 5: exact1,000-halfword buffer, sentinel checks, stable CPU ownership, export length and repeat-capture behavior.
- [ ] Record raw captures, tool/firmware versions, source commit, configuration and limitations before updating any hardware status.

## Measurements not supplied

Actual LED periods, DFLL frequency, RTC error, interrupt latency/jitter, PWM error, acquisition throughput, DMA CPU cost, ADC noise/accuracy and sample-loss performance are all **not measured**. Predictions in the lab pages are not benchmarks. No Miniscope Zero firmware behavior or optimization is inferred from these exercises.

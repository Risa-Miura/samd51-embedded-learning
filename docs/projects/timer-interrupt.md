# 02 · Timer interrupt LED

**Status:** Planned — no firmware implementation or hardware results yet.

## Goal

Replace foreground blocking delays with a hardware timer interrupt while keeping foreground work responsive.

## Hardware requirements

Grand Central M4, USB data cable; logic analyzer or oscilloscope recommended for timing evidence.

## Implementation specification

Select an unused TC or TCC instance after inspecting Arduino core ownership. Trace its bus and generic clocks. Specify a 500 ms toggle interval and derive the compare value for the chosen mode. Clear only the intended interrupt flag with documented semantics. Keep the ISR bounded; defer logging and analysis to foreground code.

## Build and flashing instructions

No executable firmware is supplied for this project yet. First complete [the blink baseline](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/01-led-blink). Choose and record Arduino versus a standalone toolchain before implementation. For Arduino, put the eventual sketch in `src/<sketch-name>/<sketch-name>.ino`, compile for `adafruit:samd:adafruit_grandcentral_m4`, then upload to the detected board port. Record exact commands, board package/compiler versions, and the source commit before changing this status.

## Expected behavior and acceptance criteria

A periodic LED toggle continues while foreground code performs a bounded task. Define an acceptable period tolerance before running; it must be justified by the clock source and measurement tool.

## Debugging

Missing interrupts: verify clocks, peripheral enable, NVIC setup, handler name, and flag acknowledgement. Wrong period: check prescaler, count mode, compare semantics, and measured timer clock.

## Results and performance measurements

**Not measured.** Toggle period distribution, ISR execution time, interrupt latency with an explicit reference event, and any missed events. A timer-to-LED period alone does not measure interrupt latency.

| Run / commit | Configuration | Raw evidence | Observation |
| --- | --- | --- | --- |
| Pending | To define | None | Not run |

## Before calling this verified

- [ ] Source compiles for the selected device and exact toolchain.
- [ ] Upload and observed behavior are documented on the Grand Central M4.
- [ ] Timing boundaries, units, and instrument limits are explicit.
- [ ] Raw evidence and negative results are preserved.
- [ ] Documentation distinguishes predictions from measurements.

[Project directory and future source](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/02-timer-interrupt)

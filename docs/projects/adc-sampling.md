# 03 · Periodic ADC sampling

**Status:** Planned — no firmware implementation or hardware results yet.

## Goal

Capture a finite, periodically acquired buffer and export it with enough metadata to reproduce the run.

## Hardware requirements

Grand Central M4, USB data cable, known safe analog source (start with ground), common ground; optional potentiometer connected only between 3.3 V and ground with its wiper on a verified analog pin.

## Implementation specification

Select and verify an analog pin/channel. Specify reference, resolution, sample acquisition time, target sample interval, and finite sample count. Separate acquisition from CSV output. Define buffer ownership and overrun behavior. Begin with a simple implementation, then evaluate timer triggering. Do not apply 5 V to analog inputs.

## Build and flashing instructions

No executable firmware is supplied for this project yet. First complete [the blink baseline](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/01-led-blink). Choose and record Arduino versus a standalone toolchain before implementation. For Arduino, put the eventual sketch in `src/<sketch-name>/<sketch-name>.ino`, compile for `adafruit:samd:adafruit_grandcentral_m4`, then upload to the detected board port. Record exact commands, board package/compiler versions, and the source commit before changing this status.

## Expected behavior and acceptance criteria

The requested number of samples is stored without overrun. A grounded input should be near the low end subject to noise/offset; define a tolerance from the chosen setup rather than inventing a numeric result. Record actual cadence.

## Debugging

Unexpected values: check ground, reference, pin mux, channel, calibration, source impedance, and input limits. Irregular cadence: remove serial output from acquisition and inspect timer/conversion timing.

## Results and performance measurements

**Not measured.** Sample count, target and observed interval, reference/resolution, input condition, dropped samples, buffer size, and raw data. Distinguish ADC conversion time from end-to-end logging time.

| Run / commit | Configuration | Raw evidence | Observation |
| --- | --- | --- | --- |
| Pending | To define | None | Not run |

## Before calling this verified

- [ ] Source compiles for the selected device and exact toolchain.
- [ ] Upload and observed behavior are documented on the Grand Central M4.
- [ ] Timing boundaries, units, and instrument limits are explicit.
- [ ] Raw evidence and negative results are preserved.
- [ ] Documentation distinguishes predictions from measurements.

[Project directory and future source](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/03-adc-sampling)

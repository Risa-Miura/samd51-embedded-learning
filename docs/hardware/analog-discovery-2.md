# Analog Discovery 2 measurement guide

Use **Digilent WaveForms**, select **Analog Discovery 2** in Device Manager, and record the WaveForms version/device configuration used. Close other software using the instrument. These procedures describe expected signals; they do not report measurements made while preparing the manual.

## Voltage and connection rules

Digilent specifies 3.3 V LVCMOS digital I/O and up to 100 MS/s acquisition for AD2. Its waveform generators can produce bipolar voltages up to approximately ±5 V; its wider input/protection ranges do not authorize applying those voltages to a 3.3 V MCU. See [Digilent's hardware-specific WaveForms introduction](https://files.digilent.com/manuals/WaveForms/3.25.1/start3.html) and the [AD2 reference manual](https://digilent.com/reference/_media/reference/test-and-measurement/analog-discovery-2/ad2_rm.pdf), §§2, 3, 5 and 9. Flying leads limit analog fidelity, so these labs divide the DFLL to about 1 MHz instead of trying to characterize a 48 MHz edge.

- Connect a black AD2 **GND** lead to board **GND** for every stimulus or logic capture.
- For single-ended scope measurement, connect **1+** to the signal, **1−** to board GND, and AD2 GND to board GND. Use **2+ / 2−** similarly for channel 2. The minus input is not a power-supply output.
- Do not connect AD2 V+, V− or W2 for these labs. Power the board by USB and keep Supplies disabled.
- Use short leads. Configure channels as **inputs** except the explicitly assigned DIO0/W1 source. Do not generate a pattern on a measurement channel.
- Stop or disconnect sources while the board is unpowered; a 1 kΩ resistor limits accidental contention but is not a voltage-level converter.

## Capture presets

Actual rate/buffer choices depend on the selected AD2 device configuration. Use the nearest supported settings while preserving a long enough record and the stated time resolution. The timebase controls the displayed window; the configured acquisition buffer may contain more data.

| Experiment | Connections | Suggested acquisition | Trigger and expected trace |
| --- | --- | --- | --- |
| Lab 1 blink | Scope1+ → D13, 1−/GND → GND; optional Logic DIO1 → D13, DIO2 → D12 | Scope 2 kS/s, 8,192 samples; 200 ms/div, DC coupling, ~1 V/div | Rising at 1.65 V; about 500 ms per level / 1 s full cycle; D12 complementary |
| Lab 1 clock | Scope1+ or Logic DIO5 → D37 | Scope 50 MS/s; 0.2–1 µs/div; ≥4,096 samples | Rising at 1.65 V; approximately 1 MHz square wave. Do not interpret a low-rate alias as the clock |
| Lab 1 timer/ISR | Scope1+ → D35, Scope2+ → D34; both minus inputs/GND → GND; or Logic DIO4/DIO3 | 50–100 MS/s, ≥4,096 samples; 1–5 µs/div around a reference edge | Trigger D35 rising at 1.65 V; D34 ISR pulse follows the timer reference. D35 full cycle ~2 ms; ISR invocation ~1 ms |
| Lab 2 RTC | Scope1+ or DIO1 → D13 | 2 kS/s, 8,192 samples; 500 ms/div | Rising at 1.65 V; nominal 1 s toggle interval / 2 s full cycle, with ULP32K error |
| Labs 3/4 PWM | Scope1+ or Logic DIO1 → D2 | Scope 50 MS/s, ≥4,096 samples; 1 µs/div | Rising at 1.65 V; nominal 187.5 kHz (~5.333 µs period), duty follows ADC voltage |
| Labs 3/4 block completion | Logic DIO3 or Scope2+ → D34 | 10–50 MS/s; trigger on D34 rising; record long enough for the observed block rate | Short pulse once per 1,000 DMA beats; period depends on conversion/sample settings, not an invented throughput |
| Lab 4 button qualification | Logic DIO0 observes its stimulus, DIO1 → D13; optional DIO3 → D34 for DMA | 10 kS/s, 16,384 samples for ~1.6 s; choose slower streaming capture if following multiple seconds | Falling DIO0; only stable low inputs qualify, then RTC toggle rate changes after foreground processing |
| Lab 5 completed buffer | Scope1+ or DIO1 → D13; DIO3 → D34 | Trigger D34 rising, 10–50 MS/s | One completion pulse and LED toggle per requested finite capture; use debugger/counters for data correctness |

For nanosecond/microsecond timing, save the actual sample rate and do not infer precision beyond sample spacing and analog threshold uncertainty. For slow captures, a 2 kS/s record resolves timing only to about 0.5 ms. Use separate slow and fast captures rather than one low-rate record for all signals.

## Generate a safe analog input

1. Disconnect W1 from A7. With common ground connected, put Scope1+ on W1 and 1− on GND.
2. Open **Wavegen → W1**. Start with a DC level of **1.25 V**. Confirm the output with Scope/DC measurement, stop W1, and connect W1 through **1 kΩ** to **A7**.
3. For a slowly changing input, select a **1 Hz triangle**, **1.2 V offset**, **1.0 V peak amplitude (2.0 V peak-to-peak)**. Verify the actual minimum/maximum are **0.2/2.2 V** before attaching it to A7. If your UI presents amplitude differently, adjust based on that measured range.
4. Start W1 only after confirming board power and wiring. Keep A7 between 0 V and the 2.5 V ADC reference; the narrower 0.2–2.2 V range leaves margin.
5. For reproducible comparisons use DC levels **0.25, 1.25 and 2.25 V**, verified before connection. These should give approximately low/middle/high ADC values and ~10/50/90% PWM duty. They are predictions, not calibrated limits.

Do not connect W1 to the physical AREF pin. The firmware selects the internal 2.5 V ADC reference; the board's AREF jumper remains unchanged.

## Digital input and debounce test

Set **Static I/O DIO0** to output high/low through 1 kΩ to D48. For repeated tests, use **Patterns DIO0**, 0.5 Hz clock, 50% duty, push-pull output. AD2's digital output level is 3.3 V; do not add an external 5 V pull-up. Keep all other used DIO pins in input mode.

Lab 1's integrator uses nominal 50 ms low/high qualification. Lab 4 uses synchronous EIC debounce with nominal 8 ms ticks and eight stable ticks (~64 ms, plus sampling phase). Test both long pulses (≥250 ms) and short pulses (e.g. 10 ms). A single EIC detection is a qualified edge, not a measurement of mechanical contact bounce. Use a real switch to study actual bounce.

## Interpret latency and jitter honestly

Measure D35 reference edge → D34 rising and D34 high width in Lab 1. The former includes peripheral flag/event phase, NVIC/exception entry and instructions before the GPIO write; the latter includes instrumentation and ISR instructions, and is not the complete exception entry/exit cost. Capture repeated edges, export timestamps, calculate the distribution, and retain clock/sample/threshold settings. Change one variable at a time. Do not use single-step or breakpoints while collecting free-running timing evidence.

In Labs 3–5, do not keep `ADC_RESULT` in a live-refresh watch: reading it consumes the ready condition and can interfere with DMA. Inspect configuration, flags, descriptors and completed buffers instead.

## Evidence record

Save: date, firmware commit and `build-record.json`, open/closed clock option, actual wiring, WaveForms version/device configuration, sample rate/buffer/timebase/trigger/threshold, input voltage/source, exported raw capture, measured mean/min/max/standard deviation, and instrument limitations. All fields start **not measured**.

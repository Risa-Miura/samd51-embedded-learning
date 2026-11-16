# Lab 1 · DFLL, GPIO, timer and interrupts

**Software status:** Source supplied; build/image checks recorded in [verification](verification.md). **Hardware status:** Not run.

## A. Original objective

PDF pp.17–52 teach open/closed-loop DFLL setup, port direction/output/input access, a TC3 timer interrupt, NVIC control and a debounced switch that gates alternating LEDs. This adaptation retains those exercises, adding measurable hardware and ISR reference signals.

## B. Differences from the original hardware

The onboard Grand Central LED is **D13/PB01**, active high, not the original PC18/PB08 LED pair. **D12/PB00** supplies a complementary signal for AD2 (or an optional LED/resistor). There is no SW0 user switch: use **D48/PC04** with a button or AD2. The divided clock output is **D37/PA16**, not a 48 MHz output on PB14. [Full differences and corrections](compatibility.md).

The default build blinks without a button so the first upload has an obvious expected response. The `--button-gated` build performs the original held-button experiment. Both use the same direct register-level GPIO, timer and interrupt implementation.

## C. Required equipment

Board and USB data cable; Atmel-ICE, its SAM-compatible 10-pin 1.27 mm cable and USB cable; AD2 and flywire harness; ground/signal jumpers. For the input exercise, add one 1 kΩ series resistor and either a momentary button or AD2 DIO0. An extra LED plus its own1 kΩ resistor is optional. Confirm these items against the [equipment checklist](../../hardware/wiring-reference.md).

## D. Wiring

1. With power off, connect the ICE **SAM** port to board **X2** using the [verified contact map](../../hardware/atmel-ice.md#connector-and-power). VTref is board3.3 V sense, never target power.
2. Connect AD2 GND → board GND. For the first observation, Scope1+ → **D13**, Scope1− → GND. No LED wiring is necessary.
3. For the switch exercise, connect **DIO0 →1 kΩ→D48**, or D48→1 kΩ→button→GND. The internal pull-up supplies the released state.
4. For timing, use D34 as the ISR marker, D35 as the TC3 hardware reference, and D37 as the divided DFLL clock. All are outputs: AD2 channels on them are inputs only.
5. Power the board and probe through their USB cables. Keep AD2 Supplies/Wavegen disabled unless performing an explicitly configured input experiment.

| Probe signal | Grand Central | AD2 channel suggestion | Expected |
| --- | --- | --- | --- |
| LED state | D13 / PB01 | Scope1+ or Logic DIO1 |0–3.3 V, nominal 500 ms per level |
| Complement | D12 / PB00 | Logic DIO2 |Inverse of D13 |
| ISR marker | D34 / PA19 | Scope2+ or DIO3 |Short positive pulse every nominal 1 ms |
| Timer reference | D35 / PA18/E | Scope1+ or DIO4 |TC3 WO0 toggles every1 ms;500 Hz full cycle |
| Clock reference | D37 / PA16/M | Scope1+ or DIO5 |DFLL/48, approximately 1 MHz |
| Switch stimulus | D48 / PC04 |DIO0 output through1 kΩ |0–3.3 V; low=pressed |

## E. Firmware and register walkthrough

Canonical complete files: [Lab 1 main](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/lab-1/main.c), [shared register setup](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/workbench.c), [header](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/workbench.h), [startup](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/startup.c), [linker](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/bootloader-app.ld), and [build script](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/build.py). Build the supplied files together; the main alone is not a complete linked image.

```c
--8<-- "projects/mu-labs/lab-1/main.c"
```

### Startup and clocks

`Reset_Handler` masks/clears inherited interrupts, stops SysTick, installs the 0x4000 vector table, disables and invalidates CMCC, copies `.data`, clears `.bss`, and enters `main`. It does not call the old Arduino or XC32 startup. `clock_init` enables bus access, keeps conservative Flash wait states, selects enabled open-loop DFLL through GCLK0 and divides the CPU clock by 1.

For closed loop, GCLK3 receives XOSC32K, PCHCTRL0 feeds that generator into DFLL reference, DFLLMUL uses multiplier1,465, CSTEP5/FSTEP1, and MODE selects closed loop. Readiness and both lock flags are checked with bounded waits. A timeout leaves `g_fault_stage` visible for debugging rather than silently claiming the clock is configured.

`32768 ×1465 =48,005,120 Hz` is the nominal closed-loop average before crystal error; the timer constants use the course's nominal 48 MHz baseline. At this frequency the expected500 ms half-period is approximately 499.947 ms. Open-loop error is larger and must be measured. Do not infer clock accuracy from an LED's appearance.

### GPIO and TC3

`board_init` sets output latches before direction, enables PC04 input/pull-up, and enables continuous input sampling. `pin_mux` preserves the adjacent pin's nibble. TC3 has both its APBB bus clock and generic-clock channel 26 enabled, is reset while disabled, and uses COUNT16/MFRQ/prescaler 16/CC0=2,999. The nominal interval is:

$$T_{tick} = \frac{16(2999+1)}{48\,000\,000} = 1\text{ ms}.$$

The source explicitly enables and clears **MC0**, routes **TC3_IRQn**, then clears PRIMASK with `__enable_irq`. The ISR advances counters, qualifies the input with a saturating50-sample integrator, and changes complementary output levels every500 timer matches. All state shared with a debugger/foreground is visible as `volatile`; this does not imply a generic lock/atomicity mechanism.

### Original switch exercise

Build with `--button-gated`: after nominal 50 ms low qualification, the LED alternates while the input remains pressed. After nominal 50 ms high qualification it returns to the defined idle state: D13 high and D12 low. This is a hysteretic integrator, so bounce/history can affect exact qualification time; it is not a mechanical-switch timing claim.

Datasheet: DS60001507N §§13–15,28.6.4,29.6.2,32.6.3,48.6.2.6 and TC interrupt/synchronization sections. Original code references: PDF pp.18–29,30–45,48–51.

## F. Build and flash

From the repository root, enter `projects/mu-labs` and run:

```powershell
python build.py lab-1
```

Inspect `build/lab-1/open/build-record.json` and the memory map, then follow the [Atmel-ICE identity, power, backup and connection checks](../../hardware/atmel-ice.md). Only after those checks:

```powershell
python program.py build/lab-1/open/firmware.elf --replace-application
```

This replaces the existing application, not the bootloader. Enter the offset application's reset handler using its own stack, as documented in the [debug guide](../../hardware/atmel-ice.md#attach-gdb-and-enter-the-offset-application). No physical programming was performed while writing this adaptation.

For the original switch and closed-loop experiments, build each variant explicitly:

```powershell
python build.py lab-1 --button-gated
python build.py lab-1 --closed-loop
python build.py lab-1 --closed-loop --button-gated
```

Their ELF paths are `build/lab-1/open-button/`, `closed/` and `closed-button/` respectively. Program only the selected image. The source is compiled and checked; the ICE connection and actual bootloader preservation check must still be performed.

## G. Debug with Atmel-ICE

At `main`, inspect `SCB->VTOR` (expected0x4000), the clock source, and `g_fault_stage` (expected0). Step through `board_init`, `clock_init` and `tc3_init`. Inspect:

- `GCLK_REGS->GCLK_GENCTRL[0]`, `[2]`, optional `[3]`, and `GCLK_PCHCTRL[26]`.
- `OSCCTRL_DFLLCTRLA/B`, `DFLLMUL`, `DFLLSYNC`, and `STATUS`; in a closed build verify DFLLLCKC/F.
- `PORT_REGS->GROUP[1]` DIR/OUT for PB01/PB00 and GROUP2 PINCFG4/IN for the input.
- `TC3_REGS->COUNT16` CTRLA/WAVE/CC0/INTENSET/INTFLAG/SYNCBUSY.
- `g_ms`, `g_led_toggles`, `g_button_pressed`; `TC3_Handler` as a temporary breakpoint.

Peripherals may freeze or accumulate flags while halted. Confirm one interrupt reaches the handler, then remove breakpoints and run freely for AD2 measurements. Do not claim1 ms from counting interrupts while single-stepping.

## H. AD2 procedure

1. Capture D13 at 2 kS/s,8,192 samples,200 ms/div, rising trigger1.65 V. Measure at least three high/low intervals; save the capture. D12 should be complementary.
2. In a separate fast capture, measure D37 at 50 MS/s,0.2–1 µs/div. Confirm approximately 1 MHz before interpreting timer error. A low-rate alias is not evidence of a slower clock.
3. Put Scope1 on D35 and Scope2 on D34, both minus inputs/GND to GND. Use50–100 MS/s,1–5 µs/div, rising trigger on D35. Measure reference-edge→ISR-marker delay and marker width across repeated acquisitions. Include event/flag phase and instrumentation in the interpretation.
4. Test the button-gated build with DIO0 Static I/O or a 0.5 Hz pattern. Check low/high qualification and the defined idle output. Compare short and long inputs; record observations rather than assuming every pulse is accepted.
5. Rebuild closed loop and repeat clock/LED captures with identical instrument settings. Separate the deterministic multiplier error from reference/timing uncertainty.

[Complete AD2 settings and limitations](../../hardware/analog-discovery-2.md).

## I. Expected results

Default build: periodic D13 blink, inverse D12, nominal 1 kHz ISR invocation,500 Hz D35 hardware reference, and approximately 1 MHz D37 clock. Closed loop changes the clock source/lock state and accuracy model, not the educational behavior. Button-gated mode requires a qualified low input before alternating outputs.

The exact interrupt latency, pulse width, clock frequency and timing jitter are **not measured**. No numeric maximum-latency or jitter guarantee is supplied.

## J. Troubleshooting

| Symptom | Diagnosis |
| --- | --- |
| ELF/Flash image starts at 0 | Wrong project or linker; use the new lab build and its image validator |
| No target connection | Recheck SAM/AVR choice, keyed orientation, VTref, ground, power, reset and SWD speed |
| Stuck before `main` | Check stack/reset entry, VTOR, initialization and Default_Handler; do not use an address-zero image |
| Nonzero `g_fault_stage` |1–4 DFLL/GCLK0;5–9 crystal/reference/lock;10 clock probe;11 unexpected buck regulator mode;20–23 TC3;100+/200+/300+ channel lock/disable/enable |
| Timer counts but no ISR | MC0 peripheral enable/flag, NVIC TC3_IRQn, correct vector table/handler, PRIMASK |
| Input always pressed/released | D48 is PC04, not PB31; check internal pull-up, ground and the selected input source |
| Clock/ISR trace looks wrong | Separate slow/fast sample rates, verify trigger and output channel, remove breakpoints, check aliasing |
| Clock mode changes corrupt USB serial | The lab has no application USB stack; AD2/debugger are the observation channels |

## K. What I learned


- Which clock source actually feeds TC3 and the CPU, and what evidence confirms it?
- Why does the timer need both a bus clock and a generic clock?
- What changes if CC0 is3,000 instead of 2,999?
- Which three interrupt enables must agree?
- What is included in the measured edge-to-marker interval?
- How did software debounce behave with short inputs or real switch bounce?
- What must be true before applying these measurement habits to Miniscope Zero firmware?


| Date / firmware commit | Setup and raw capture | Actual observations / measurements | Unresolved issue |
| --- | --- | --- | --- |
| Not performed | Not recorded | Not measured | Hardware verification pending |

Keep expected and actual results separate. Check off the [bench verification list](verification.md) only after recording evidence for Lab 1.

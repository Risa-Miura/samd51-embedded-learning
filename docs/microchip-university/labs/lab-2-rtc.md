# Lab 2 · RTC counter interrupts

**Status:** Source supplied; software checks recorded separately; no hardware run.

## A. Original objective

PDF pp.53–65 configure the RTC and implement a compare interrupt that toggles LEDs. The manual uses **MODE0,32-bit counter**, not MODE2 calendar timekeeping. Preserve that distinction when interpreting RTCC terminology.

## B. Hardware differences

PB01/D13 replaces the original two LEDs. The internal **OSCULP32K** remains the RTC clock, so no external crystal or expansion board is needed. Even if the CPU is rebuilt in closed-loop mode, RTC accuracy is still determined by its own selected internal oscillator.

## C. Required equipment

Board, Atmel-ICE/SAM cable, target/probe USB cables; AD2 harness and ground/signal leads for timing. No button, analog source, resistor or extra LED is needed for this lab.

## D. Wiring

Keep the verified SWD/power connection. AD2 GND→board GND; Scope1+→D13,1−→GND. Optionally put Logic DIO3 on D34 to observe the RTC ISR marker. All AD2 outputs remain disabled.

## E. Firmware

[Complete Lab 2 main](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/lab-2/main.c) uses shared `rtc_init` and `rtc_set_period` in [workbench.c](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/workbench.c).

`rtc_init` enables the APBA clock, disables/resets RTC with synchronization checks, enables/selects ULP32K through OSC32KCTRL.RTCCTRL, sets MODE0/MATCHCLR/prescaler 1, writes COMP0=32,767, and enables CMP0 plus RTC_IRQn. The compare flag is set and COUNT clears on the following clock cycle (§21.6.2.3);32,768 nominal RTC ticks give a nominal 1 s toggle interval. The handler clears CMP0/OVF, increments `g_rtc_matches` and toggles PB01.

The manual's p.61 says two seconds while p.65 says one; this implementation defines one second per toggle explicitly. The full LED cycle is two toggle intervals. No calendar, battery backup, long-term accuracy or low-power behavior is claimed.

Datasheet DS60001507N §§21.6.2.1–3,21.6.4,21.6.7,21.8 and29.6.6. Original function names: `init_rtcc`/`init_RTCC`, `RTC_Handler`, `enable_interrupts`, `main`.

## F. Build and flash

From the repository root, enter `projects/mu-labs` and run:

```powershell
python build.py lab-2
```

Inspect `build/lab-2/open/build-record.json` and the memory map, then follow the [Atmel-ICE identity, power, backup and connection checks](../../hardware/atmel-ice.md). Only after those checks:

```powershell
python program.py build/lab-2/open/firmware.elf --replace-application
```

This replaces the existing application, not the bootloader. Close OpenOCD, then select **MPLAB: Attach Lab 2 (already programmed)** in VS Code. Follow the [MPLAB native attach and application-entry procedure](mplab-vscode.md#program-then-attach-with-mplab). An [explicit GDB startup alternative](../../hardware/atmel-ice.md#attach-gdb-and-enter-the-offset-application) is available if native reset does not reach main. No physical programming was performed while writing this adaptation.

## G. Debug with Atmel-ICE

Use the **MPLAB Debugger in VS Code** configuration above. Variables, Watch, CPU Registers and the supported device/memory views provide the inspections below; use the native debugger console only for its own commands.

Inspect `OSC32KCTRL_RTCCTRL`, RTC MODE0 CTRLA/COMP0/INTENSET/INTFLAG/SYNCBUSY, `g_rtc_matches`, and PB01 OUT. Break temporarily in `RTC_Handler`, verify CMP0 is acknowledged by a write-one-to-clear assignment, then run without breakpoints. COUNT is read-synchronized; consult COUNTSYNC/read synchronization instead of assuming a debugger refresh shows the current counter.

## H. AD2 measurements

Scope1 on D13, DC coupling, rising trigger1.65 V,2 kS/s,8,192 samples,500 ms/div. Save at least several toggle intervals, using a longer streaming/record capture if necessary. Measure a mean interval and variation; log that the reference is internal ULP32K. D34 produces a brief marker per RTC ISR and requires a separate fast acquisition to measure pulse width.

## I. Expected results

D13 toggles about once per second, making an approximately 0.5 Hz complete waveform. Internal oscillator error is expected; “about one second” is not a precision timekeeping specification. `g_rtc_matches` advances once per handled compare.

## J. Troubleshooting

Check RTC source selection independently of GCLK0, ensure the module was disabled before reset, wait for synchronization, enable CMP0 and RTC_IRQn, and use the application vector table. A very long delay after changing COMP may indicate COUNT was above the new compare value; `rtc_set_period` disables and clears COUNT before updating COMP. A frozen trace during a breakpoint is not proof that the oscillator stopped in normal operation.

## K. What I learned


- Why does the RTC use a different source than the CPU?
- Why does a 1 s toggle interval produce a 2 s waveform period?
- What does MATCHCLR change, and when is the compare flag set?
- How would MODE2 calendar operation require a 1 Hz clock and different registers?
- What measured evidence distinguishes oscillator error from ISR latency?


| Date / firmware commit | Setup and raw capture | Actual observations / measurements | Unresolved issue |
| --- | --- | --- | --- |
| Not performed | Not recorded | Not measured | Hardware verification pending |

Keep expected and actual results separate. Check off the [bench verification list](verification.md) only after recording evidence for Lab 2.

# Lab software validation record

**Hardware status: not tested.** None of the validation commands connected to a probe, read a physical target, programmed Flash, changed fuses, or collected a waveform.

## Inputs and preservation

The entire supplied 126-page course manual was text-extracted and read, with selected original board, RTC, EIC and memory figures rendered for inspection. Original objectives and page coverage are mapped in the documentation. The original course PDF is not redistributed.

Source PDF SHA-256: `40a8a577209d138564f26757ca02264176fd3779755c46e04158abb2cf4c992c`.

Existing SAME54 `Lab1.X`, Arduino blink, standalone XC32 blink, root VS Code/MPLAB settings and personal lesson notes were retained. The pre-existing uncommitted MCU basics note was not included in the lab commit.

## Firmware build results

Toolchain: Arm GNU Embedded 9-2019-q4-major, GCC 9.2.1, Microchip SAMD51_DFP 3.10.283 and ARM CMSIS 6.4.0. Warnings are errors. All images passed ARM/ELF32, 0x4000 application-origin, stack/Thumb reset vector, load-segment and exact ELF/BIN-content checks.

| Application / configuration | BIN bytes (build artifact size) | Initial SP | Build / image check |
| --- | --- | --- | --- |
| lab-1 / closed | 1904 | 0x2003fff8 | Passed |
| lab-1 / closed-button | 1924 | 0x2003fff8 | Passed |
| lab-1 / open | 1780 | 0x2003fff8 | Passed |
| lab-1 / open-button | 1800 | 0x2003fff8 | Passed |
| lab-2 / open | 1660 | 0x2003fff8 | Passed |
| lab-3 / open | 2708 | 0x2003fff8 | Passed |
| lab-4 / open | 2992 | 0x2003fff8 | Passed |
| lab-5 / open | 2212 | 0x2003fff8 | Passed |

Build artifact sizes are not hardware performance measurements. Full source hashes, compiler version, load regions and BIN hashes are in each ignored `build-record.json`.

## Defensive image tests

Five unit tests passed: a valid application is accepted; a segment at address0 is rejected; a segment in the user row is rejected; a modified/stale BIN is rejected; and the programming CLI refuses to run without explicit application-replacement acknowledgement.

Run after building Lab1: `python -m unittest discover -s tests -v`.

## Register and debugger checks

- Compiled pin/mux assertions for PB01, PC04/EXTINT4, PB04/ADC1 AIN6, PA16/GCLK2, PC18/TCC0 WO2 and PA18/TC3 WO0.
- Compiled assertions for TC3/ADC1/TCC0 clock IDs, ADC1 DMA request70, DMAC event34, ADC1 event user57, and descriptor size/layout.
- Compared numeric common register offsets with the installed SAME54_DFP3.5.87: no differences among the common offset symbols for the eleven peripherals listed in the compatibility report. This is not a blanket claim that devices/errata are identical.
- GDB8.3 executable starts successfully. xPack OpenOCD0.12.0-7 archive SHA-256 was verified; the executable starts, and both the default target configuration and optional CMSIS-DAP HID backend parse with `noinit` and shutdown. No adapter initialization occurred.
- The supplied optional Windows setup helper's existing-tool retention path was exercised. Fresh download/extraction is specified with pinned archive hashes; actual target USB driver/firmware operation remains untested.

## Documentation checks

The strict MkDocs build passed locally. The rendered-site checker passed 38 HTML pages and 3,090 local links/assets/fragments. All five lab pages render Aâ€“K sections; Lab 1 includes its canonical main source. Navigation, source URLs, SVD paths and startup/linker files were inspected. Deployment is verified separately by the GitHub Actions run.

## Remaining bench work

Confirm actual board revision/cable mapping/3.3 V reference; connect and identify the target; retain any needed old application; exercise the explicit Flash verify and unchanged-bootloader/user-row comparisons; reach/step main and the selected ISR; measure the clock, LED, timer/reference/ISR signals; verify ADC/PWM DMA and events; capture a complete stable sample buffer; record all actual tool settings, raw captures and limitations.

See [the full checklist](../../docs/microchip-university/labs/verification.md). No real measurement files, claimed course completion, or inferred Miniscope Zero performance are included.

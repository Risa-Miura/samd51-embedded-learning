# SAMD51 bare-metal blink (MPLAB for VS Code)

Standalone project for ATSAMD51P20A / Adafruit Grand Central M4 Express.
The existing Arduino example in ../src/blink/ is retained separately.

## Build

1. Open this samd51-blink folder in VS Code with the MPLAB extensions.
2. Select samd51-blink / default, then run MPLAB CMake: Build
   (older extensions call this Full Build), or Clean and build.
3. Use XC32 v2.50, Microchip SAMD51_DFP 3.10.283 and ARM CMSIS 6.4.0.
4. Output: out/samd51-blink/default.elf and default.hex.

.vscode/samd51-blink.mplab.json is the source of the MPLAB configuration.
Its file set lists only src/main.c. There are no loadable firmware images.
Allow MPLAB to generate cmake/ and _build/; do not hand-edit those files.

The program uses PB01 (ordinary onboard D13 LED), the internal open-loop
48 MHz DFLL and a polled SysTick delay, nominally 500 ms per LED state.
Timing and LED behavior require hardware verification.

## Before flashing

This is a reset-entry application linked at flash address 0, using XC32's
device startup. It is NOT an Arduino/UF2 bootloader-offset application.
Programming it can replace the Grand Central USB bootloader. Preserve a
recoverable bootloader image and decide on the bootloader/memory layout
before programming. Do not copy this HEX to the UF2 drive.

An ATSAMD51-capable SWD programmer/debugger, correct SWDIO/SWCLK/GND/
target-voltage-reference wiring, board power, and a matching MPLAB tool
configuration are needed. Do not reuse the SAME54 board's EDBG selection;
choose the probe actually connected. Check boot protection and user-row
programming settings. This source intentionally supplies no config pragmas.
If retaining USB upload, first create and verify a bootloader-compatible
link layout and upload format; the default address-zero image is unsuitable.

No board has been flashed or hardware-validated by this repair.

## References

- https://developerhelp.microchip.com/xwiki/bin/view/software-tools/ides/extensions/workflow/
- https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.cpp

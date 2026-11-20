# SAMD51 firmware journal 🔧

I joined UCLA's Aharoni Lab as an undergrad research assistant in November 2026, and I'm learning microcontroller firmware so I can help optimize and migrate the firmware for the wireless Miniscope Zero. This repo is where I keep my notes and experiments while I work through Microchip University's course on an Adafruit Grand Central M4 (ATSAMD51P20A, ARM Cortex-M4F at 120 MHz).

<img src="images/blink-grand-central.gif" alt="Grand Central M4 on a desk with its red L LED blinking" width="420">

## What I've done so far

- [Lesson 01: microcontroller basics](lessons/01-microcontroller-basics.md). My notes on clock domains, device headers, clock sync (`SYNCBUSY`), the DFLL, GPIO registers, the NVIC and timers.
- [LED blink](projects/01-led-blink/README.md). I got the red `L` LED blinking with Arduino calls first, then by writing the `PORT` registers directly with `OUTTGL`. The gif above is the register version.

## What's next

I haven't measured anything yet. Next I want to check the blink timing on an oscilloscope and step through the code with an Atmel-ICE, then keep going with lesson 02.

## Run the blink

1. In Arduino IDE 2.x, add `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` as a board manager URL and install Adafruit SAMD Boards.
2. Pick Adafruit Grand Central M4 (SAMD51) and your board's port.
3. Open `projects/01-led-blink/src/blink/blink.ino` and upload it.

The `L` LED (D13 / PB01) should blink about every 500 ms. CLI commands and troubleshooting are in the [blink README](projects/01-led-blink/README.md).

## Sources

- [Microchip University: Getting Started with Writing Code for Microchip Arm Cortex MCUs](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers)
- [Adafruit Grand Central M4 guide](https://learn.adafruit.com/adafruit-grand-central)
- [SAM D5x/E5x datasheet](https://www.microchip.com/content/dam/mchp/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf)

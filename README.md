# SAMD51 Embedded Systems Learning Journal

My notes and experiments while learning microcontroller firmware from the registers up, on an ARM Cortex-M4F board (Adafruit Grand Central M4 Express, ATSAMD51P20A).

**Who:** Undergraduate Research Assistant in UCLA's Aharoni Lab (since Nov 2026), learning microcontroller fundamentals to prepare for optimizing and migrating firmware for the wireless Miniscope Zero.

**[Read lesson 01 · Microcontroller basics →](lessons/01-microcontroller-basics.md)**

**Current status (Nov 2026):** Lesson 01 is complete. The LED blink runs on the board, both with Arduino calls and with direct `PORT` register writes (`OUTTGL`). Nothing has been measured yet; oscilloscope timing and Atmel-ICE stepping are next.

## What's here

| Path | What it is | Status |
| --- | --- | --- |
| [`lessons/01-microcontroller-basics.md`](lessons/01-microcontroller-basics.md) | Notes on clock domains, device headers, clock-domain synchronization (`SYNCBUSY`), the DFLL, GPIO registers, the NVIC and timer counters, with diagrams and code | **Complete** |
| [`projects/01-led-blink/`](projects/01-led-blink/README.md) | Blink the onboard LED with Arduino calls, then with direct `PORT` register writes | **Runs on the board**; period not measured |

New lessons and experiments are added here as I start them.

## Hardware

| Board | MCU | Core | Nominal clock | Memory | Logic |
| --- | --- | --- | --- | --- | --- |
| Adafruit Grand Central M4 Express | ATSAMD51P20A | ARM Cortex-M4F | 120 MHz | 1 MB Flash, 256 KB SRAM | 3.3 V |

For the next steps: Atmel-ICE (SWD debugger) and an oscilloscope.

## Run the blink

1. In Arduino IDE 2.x, add `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` as a board manager URL and install **Adafruit SAMD Boards**.
2. Select **Adafruit Grand Central M4 (SAMD51)** and the board's port.
3. Open [`projects/01-led-blink/src/blink/blink.ino`](projects/01-led-blink/src/blink/blink.ino), then **Verify** and **Upload**.
4. **Check:** the red LED labeled `L` (D13 / PB01) turns on and off about every 500 ms. That is the requested delay, not a measured period.

Arduino CLI commands, the register version and troubleshooting are in the [blink README](projects/01-led-blink/README.md).

## Next (planned)

- Measure the blink period on an oscilloscope.
- Step through the blink with the Atmel-ICE.
- Continue the Microchip University course with lesson 02.

Questions about Miniscope Zero are things to investigate later, not claims about its firmware.

## Sources

[Microchip University: Getting Started with Writing Code for Microchip Arm Cortex MCUs](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers) · [Grand Central M4 guide](https://learn.adafruit.com/adafruit-grand-central) · [SAM D5x/E5x datasheet](https://www.microchip.com/content/dam/mchp/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf)

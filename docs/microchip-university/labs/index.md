# Microchip labs on the SAMD51 workbench

This is a hands-on adaptation of the supplied **Getting Started with Cortex M0+/M23/M4 Microcontrollers Lab Manual**, all 126 PDF pages. It preserves the five labs' learning objectives while targeting the **Grand Central M4 / ATSAMD51P20A**, **Atmel-ICE**, and **Analog Discovery 2**.

**Status:** Source provided and software checks performed; physical board, probe, wiring, and measurements are **not verified**. Expected waveforms are predictions to test. No course completion or hardware results are claimed.

## Start with Lab 1

1. Read the [compatibility report](compatibility.md), [workbench setup](workbench-setup.md), and [MPLAB in VS Code guide](mplab-vscode.md).
2. Check the [board pin map](../../hardware/grand-central-m4.md), [Atmel-ICE wiring](../../hardware/atmel-ice.md), and [AD2 connections](../../hardware/analog-discovery-2.md).
3. Compile [Lab 1](lab-1-gpio-timers.md), inspect its application-only memory map, then perform the explicit programming/debug steps on your hardware.
4. Complete the Lab 1 evidence checklist before progressing to RTC, DMA, and events.

| Lab | Original PDF pages | Educational result | Adaptation |
| --- | --- | --- | --- |
| [1 · DFLL, GPIO, timer and NVIC](lab-1-gpio-timers.md) | 17–52 | Route clocks, access pins, debounce an input, run a timer ISR | Onboard D13, D48 input, TC3, measured reference/ISR signals |
| [2 · RTC counter interrupts](lab-2-rtc.md) | 53–65 | Configure RTC and compare interrupts | MODE0 counter, internal ULP32K, onboard LED |
| [3 · Peripheral-to-peripheral DMA](lab-3-dma.md) | 66–104 | ADC, TCC PWM, DMA descriptors and suspend/resume | AD2 W1 → A7; ADC1 RESULT → TCC0 CCBUF2; D2 PWM |
| [4 · Event routing and EIC debounce](lab-4-event-system.md) | 105–121 | Replace CPU control with peripheral events; hardware debounce | DMA → ADC start; RTC → PORT; D48/EXTINT4 |
| [5 · ADC-to-memory DMA](lab-5-memory-dma.md) | 122–126 | Increment a DMA destination and inspect a sample buffer | 1,000 halfword samples, 8-sample averaging, stable one-shot capture |

## Files and evidence

New firmware lives under [projects/mu-labs](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/README.md). Each lab has its own `main.c`; common register setup, startup, and the linker script are shared. Existing `projects/Lab1.X`, both older blink implementations, VS Code/MPLAB settings, and personal notes are retained.

The [manual reading map](manual-reading-map.md) separates original procedures from adaptation decisions. The [verification and remaining actions](verification.md) record what software checks establish and what must still be done on the bench.

Source manual: supplied course PDF, 126 pages. Obtain the original through the [Microchip University course](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers); it is not redistributed in this repository. Primary adaptation sources are the [SAM D5x/E5x datasheet](https://onlinedocs.microchip.com/oxy/GUID-F5813793-E016-46F5-A9E2-718D8BCED496-en-US-15/index.html), device packs, [Adafruit schematic](https://github.com/adafruit/Adafruit-Grand-Central-PCB/blob/master/Adafruit%20Grand%20Central%20M4%20Express%20rev%20B.sch), [Atmel-ICE guide](https://www.microchip.com/content/dam/mchp/documents/MCU08/ProductDocuments/UserGuides/Atmel-ICE-Programmers-Debuggers-DS50002999A.pdf), and [Digilent documentation](https://files.digilent.com/manuals/WaveForms/3.25.1/start3.html).

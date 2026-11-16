# Microchip University notebook

**Starting course:** [Getting Started with Writing Code for the Microchip Arm Cortex Microcontrollers](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers).

The [Microchip SAM D5x/E5x training catalog](https://onlinedocs.microchip.com/oxy/GUID-62CB236E-26C5-4CE9-A182-B1D33FE1512C-en-US-3/GUID-AB68F86C-5EE6-4358-B52F-2AB31E9D9EAD.html) is a source of follow-on study. Some examples target other devices; adapt clocks, registers, pin assignments, startup code, and build configuration for the ATSAMD51P20A before running them.

These pages organize my planned study by topic. They are not official course transcripts or exact module titles. All entries start as **Not started**.

| Topic | Status | Experiment direction |
| --- | --- | --- |
| [01 · MCU basics](01-microcontroller-basics.md) | Not started | Trace reset to `main` |
| [02 · Clock system](02-clock-system.md) | Not started | Draw the clock path |
| [03 · GPIO](03-gpio.md) | Not started | Explain `LED_BUILTIN` mapping |
| [04 · Timers](04-timers.md) | Not started | Plan a periodic compare |
| [05 · Interrupts](05-interrupts.md) | Not started | Bound interrupt work |
| [06 · DMA](06-dma.md) | Not started | Plan buffer ownership |
| [07 · ADC](07-adc.md) | Not started | Define a sampling contract |
| [08 · Event System](08-event-system.md) | Not started | Map an event route |

[How to write the first lesson](../contributing.md#add-the-first-microchip-university-lesson)

## Adapted hands-on course labs

The [five-lab SAMD51 workbench manual](labs/index.md) follows the supplied course PDF using the Grand Central M4, Atmel-ICE and Analog Discovery 2. It includes independent bare-metal source, verified pin/target definitions, build/image checks, and explicit remaining hardware tests. Personal topic notes above remain separate.

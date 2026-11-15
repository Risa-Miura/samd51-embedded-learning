# SAMD51 and board reference

| Item | Baseline |
| --- | --- |
| Board | Adafruit Grand Central M4 Express |
| Device | ATSAMD51P20A |
| CPU | 32-bit ARM Cortex-M4 with single-precision FPU |
| Nominal clock | 120 MHz; keep the standard board option for comparisons |
| Internal memory | 1 MB Flash, 256 KB SRAM |
| Logic | 3.3 V; do not connect 5 V logic to MCU inputs |
| Arduino FQBN | `adafruit:samd:adafruit_grandcentral_m4` |
| First LED | Use `LED_BUILTIN`; the variant maps it to Arduino pin 13 |

These are platform facts, not measurements. See the [Microchip device page](https://www.microchip.com/en-us/product/ATSAMD51P20A), [Adafruit board guide](https://learn.adafruit.com/adafruit-grand-central/overview), [board definitions](https://github.com/adafruit/ArduinoCore-samd/blob/master/boards.txt), and [pin variant](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.h). Pin 13 is an Arduino index; resolve its actual PORT group/bit before register-level work.

## Datasheet reading map

Use the [SAM D5x/E5x family datasheet](https://www.microchip.com/content/dam/mchp/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf) and the device's current errata from the product page. Section numbering changes between revisions, so cite chapter titles and the exact revision actually consulted.

| Question | Chapters / structures to inspect |
| --- | --- |
| Is this resource present on my package? | Device selection, pinout, multiplexing tables |
| Why is a peripheral idle? | MCLK, GCLK, peripheral enable/reset and synchronization |
| Which pin changes? | PORT, PMUX/PINCFG, board schematic and variant |
| What causes periodic work? | TC/TCC, Cortex-M NVIC, peripheral interrupts |
| What moves a sample? | ADC, DMAC descriptors, Event System |
| What are the limits? | Electrical characteristics, timing, errata |

## Record for each experiment

Board revision, board core/compiler versions, nominal and verified clocks, peripheral instance, pin/channel mapping, reference voltage, bootloader and memory assumptions when relevant, source commit, and measurement method.

Arduino provides startup code, USB support, and timing facilities. Before direct register writes, inspect the core's ownership of clocks, timers, and interrupts; register-level code can disrupt those services.

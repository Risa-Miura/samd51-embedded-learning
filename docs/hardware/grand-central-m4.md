# Grand Central M4: lab pin and memory reference

The board is **ATSAMD51P20A**, Cortex-M4F, with 1 MiB Flash and 256 KiB SRAM. Its normal Arduino setup can run at 120 MHz; these course adaptations deliberately use the nominal **48 MHz DFLL** to match the course calculations. The compiler emits software floating-point ABI code; the exercises use integer arithmetic and do not enable or benchmark the FPU.

## Verified pad assignments

| Board label | MCU pad | Lab function | Direction / voltage |
| --- | --- | --- | --- |
| D13 / ordinary LED | PB01 | LED / RTC event output | Output, 0–3.3 V, active high |
| D12 | PB00 | Complementary Lab 1 signal | Output, 0–3.3 V |
| D48 | PC04 | Button / EIC EXTINT4, mux A | Input with pull-up; drive only 0–3.3 V |
| D34 | PA19 | ISR or DMA block marker | Output, 0–3.3 V |
| D35 | PA18 | TC3 WO0, mux E | 500 Hz nominal hardware reference in Lab 1 |
| D37 | PA16 | GCLK_IO2, mux M | DFLL/48, approximately 1 MHz |
| A7 | PB04 | ADC1 AIN6, mux B | Analog input; lab stimulus restricted to 0.2–2.2 V |
| D2 | PC18 | TCC0 WO2, mux F | PWM output, nominal 187.5 kHz |
| X2 SWDCLK | PA30 | Debug clock | Probe-controlled; never repurpose |
| X2 SWDIO | PA31 | Debug data | Probe-controlled; never repurpose |

These assignments were checked against the [Adafruit variant](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.cpp), [rev B schematic](https://github.com/adafruit/Adafruit-Grand-Central-PCB/blob/master/Adafruit%20Grand%20Central%20M4%20Express%20rev%20B.sch) and SAMD51 DFP PIO/ATDF signals. The firmware also asserts the key mux values at compile time. Do not confuse the printed D/A labels with PORT bit numbers.

## Clock and power baseline

X4 is a 32.768 kHz crystal on PA00/PA01 in the rev B schematic, so a crystal-referenced DFLL exercise is feasible without adding a crystal. The default open-loop build needs no crystal lock. The RTC deliberately uses **OSCULP32K**, matching the manual rather than automatically becoming crystal-accurate when the CPU uses closed loop.

Power the board through its USB data connector for these exercises. The MCU GPIO uses **3.3 V logic**. Do not use 5 V signals, AD2 V+ or the Atmel-ICE reference pin to power it. Do not connect an external voltage source to AREF or cut its board jumper for these labs; ADC INTREF is selected internally. See [Adafruit power/pin guidance](https://learn.adafruit.com/adafruit-grand-central/pinouts).

## Memory contract

The DFLL open-loop specification requires the MCU's **LDO regulator mode** (DS60001507N, DFLL electrical characteristics). Startup checks `SUPC.VREG.SEL` and stops with `g_fault_stage=11` if it finds buck mode; it does not silently change a live regulator configuration. Begin with the documented hardware-reset/debug entry and inspect the supply configuration if that guard triggers.

| Region | New lab use |
| --- | --- |
| Flash `0x00000000–0x00003fff` | Existing 16 KiB UF2 bootloader, excluded from the image |
| Flash `0x00004000–0x000fffff` | Exactly one new application at a time |
| SRAM `0x20000000–0x2003fff7` | Data, BSS, aligned DMA descriptors/buffer, and stack |
| Final eight SRAM bytes | Reserved for bootloader double-tap state and stack alignment |
| User row `0x00804000` | Read/compare backup only; never programmed by the lab scripts |

The application offset matches the Grand Central board core's [bootloader linker script](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/linker_scripts/gcc/flash_with_bootloader.ld). This is a GNU application ELF/BIN, **not a UF2 file**: do not copy either file to the bootloader USB drive. SWD replaces the existing application, so preserve any application you want before programming.

No new code writes user fuses, BOOTPROT, security bits, or region-lock settings. Existing protection may still prevent a write; stop and investigate rather than clearing it.

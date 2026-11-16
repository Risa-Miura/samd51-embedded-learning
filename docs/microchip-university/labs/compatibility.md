# Compatibility and hardware differences

## Platform comparison

| Original manual | This workbench | Consequence |
| --- | --- | --- |
| SAM E54 Xplained Pro, ATSAME54P20A | Grand Central M4 Express, ATSAMD51P20A | Select the exact device and its own headers, vectors and memory map |
| Onboard EDBG via debug USB | External Atmel-ICE on the SAM port via SWD | Separate probe USB, target USB power and 10-pin cable |
| I/O Xplained on EXT1 | No expansion board | AD2 stimulus/measurement replaces sensor and second LED observation |
| Board LED PC18, expansion LED PB08 | Onboard D13/PB01; D2/PC18 remains available as PWM output | Onboard LED polarity is active high; PWM is measured electrically |
| SW0 on PB31 | No general-purpose onboard button | D48/PC04 input using a button to GND or AD2 digital output; reset is not SW0 |
| Expansion light sensor PB04/ADC1 AIN6 | A7/PB04/ADC1 AIN6 | Same ADC instance/channel; use a verified 0.2–2.2 V AD2 signal |
| Raw 48 MHz GCLK0 on PB14 | DFLL/48 on D37/PA16/GCLK_IO2 | Approximately 1 MHz is much more suitable for AD2 flying leads |
| Whole-device reset image and configuration pragmas | Application at `0x4000`, existing bootloader/user row retained | New startup/vector table and constrained linker/flashing checks |
| MPLAB X 5.15 / XC32 2.15 | VS Code + Arm GNU + Microchip packs + OpenOCD/GDB | Build/debug commands are transparent; existing MPLAB projects remain intact |

The [Adafruit rev B schematic](https://github.com/adafruit/Adafruit-Grand-Central-PCB/blob/master/Adafruit%20Grand%20Central%20M4%20Express%20rev%20B.sch) identifies X4 as a 32.768 kHz crystal connected to PA00/PA01, X2 as the 10-pin SWD header, and the ordinary LED on PB01. The [Adafruit Arduino variant](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.cpp) resolves header labels to MCU pads. These are wiring references only; the new firmware uses no Arduino runtime.

## Device and register verification

Local checks use **Microchip SAMD51_DFP 3.10.283** and **ARM CMSIS 6.4.0**. The original imported project uses SAME54_DFP 3.5.87. Comparing common `_REG_OFST` definitions found no numeric offset differences in TC, TCC, RTC, ADC, DMAC, EIC, EVSYS, GCLK, OSCCTRL, OSC32KCTRL and PORT. This comparison does **not** prove every bit, package signal, erratum or peripheral is interchangeable. The new code is compiled with warnings as errors against the SAMD51 headers and has compile-time assertions for the used pad/mux/clock/DMA/event IDs.

| Used resource | SAMD51 definition / verified value | Initialization requirement |
| --- | --- | --- |
| DFLL reference | `OSCCTRL_GCLK_ID_DFLL48 = 0` | Open-loop ready first; external 32 kHz via GCLK3 for optional closed-loop; wait fine/coarse lock |
| TC3 | `TC3_GCLK_ID = 26`, `TC3_IRQn = 110` | APBB clock, COUNT16 reset/configure, synchronized CC0, peripheral flag and NVIC enable |
| TCC0 | `TCC0_GCLK_ID = 25` | APBB clock; NPWM/PER/CC2 synchronization; PC18 function F |
| ADC1 | `ADC1_GCLK_ID = 41`, AIN6 on PB04/B | APBD clock; factory BIAS fields; 2.5 V internal reference; enable-protected configuration |
| ADC1 result DMA | `ADC1_DMAC_ID_RESRDY = 70` (`0x46`) | One-beat burst per result; reading RESULT consumes the ready condition |
| DMAC channel 0 interrupt | `DMAC_0_IRQn = 31` | Aligned descriptor/write-back tables in SRAM; valid descriptor; priority level0 and global enable |
| DMAC → ADC event | Generator34, user57 | Async EVSYS channel 0; USER value 1; ADC STARTEI and DMAC EVOE/EVOSEL |
| RTC → PORT event | Generator12, PORT user1 | Async channel 1; USER value 2; MODE0 CMPEO0; PORT EVACT0/PID0/PORTEI0 |
| EIC input | PC04/A EXTINT4, `EIC_EXTINT_4_IRQn = 16`, GCLK channel 4 | Input buffer/pull-up; CONFIG0 SENSE4; debounce group 0; NVIC |
| Clock probe | PA16/M GCLK_IO2 | GENCTRL2 DFLL/48, OE, PMUX and output direction |

The source names interrupts exactly as the selected DFP does: `EIC_EXTINT_4_Handler`, including the underscore. Names in older course headers may differ. The vector table routes the handlers explicitly and is relocated through `SCB->VTOR`.

## Manual inconsistencies corrected

| PDF page(s) | Issue in the original material | Adaptation decision |
| --- | --- | --- |
| 13–16 | Configuration pragmas can carry fuse/boot-protection changes into the image | No config words; no user-row load segment; no fuse modification |
| 17, 50 | NVIC is called “Non-Volatile”; `NVIC_EnableIRQ` is described as global interrupt enable | Use Nested Vectored Interrupt Controller; enable peripheral, NVIC, and PRIMASK independently |
| 28 | Integer multiplier 1,465 is described approximately as 48 MHz | State `32768 × 1465 = 48,005,120 Hz` nominal, before crystal error; do not claim exactly 48 MHz |
| 29 | DFLLRDY is treated as proof of closed-loop stabilization | Check `DFLLLCKC` and `DFLLLCKF`, separately from DFLLRDY |
| 34 | Pull and input sampling wording is imprecise | PULLEN enables pull; OUT selects pull-up/down; set the chosen input's continuous sampling bit |
| 35–37 | `PORT_WRCONFIG_PINMASK(14)` is a value, not `1 << 14` | Configure explicit PMUX nibble and PINCFG; preserve the neighbor nibble |
| 39–44 | CC0=3,000 is used for the nominal 1 ms interval | Use CC0=2,999 with prescaler 16, and explicitly handle MC0; measure the actual clock/error |
| 61, 65 | RTC text alternates between two-second and one-second interrupts | Configure COMP0=32,767 at nominal 32,768 Hz, giving a nominal 1 s toggle interval |
| 92–94 | Absolute descriptor addresses and literal offsets make layout fragile | Use linker-allocated 16-byte-aligned objects and typed register addresses |
| 98 | Shown CHCTRLA omits explicit burst trigger action | Set `TRIGACT_BURST` and `BURSTLEN_SINGLE` so each ADC result request transfers one beat |
| 107, 110 | PCHCTRL22/21 are labelled EVSYS channels0/1 | On SAMD51 those channel clocks are 11/12; asynchronous paths used here require neither |
| 111 | A PORT-event explanation mentions EIC, although this route is generated by RTC CMP0 | Keep RTC → PORT distinct from the button's EIC interrupt |
| 118 | DIV128 and eight samples are described with inconsistent debounce arithmetic | Select a clear 1 kHz / 8 × 8-tick synchronous qualification (~64 ms plus phase), not the copied formula |
| 119 | `ASYNCH(15)` selects bits0–3, not EXTINT15 | Use synchronous stability debounce and the correct EXTINT4 bit mask |
| 123 | `uint8_t[1000]` conflicts with 1,000 halfword DMA beats; prose mixes 100 and 1,000 samples | Reserve `uint16_t[1000]` (2,000 bytes); one-past-end destination is `g_samples + 1000` |
| 124 | “8X oversampling” could be read as increased output bit depth | Eight conversions averaged with ADJRES3 retain a 12-bit result; no extra ENOB is claimed |
| 126 | Fixed example RAM addresses are shown | Use `g_samples` and the ELF/map file; its address can change with the build |

## Explicit behavioral differences

Lab 1 defaults to autonomous blink for initial bring-up; `--button-gated` restores the original held-switch exercise. Its second output is a complementary signal on D12; an extra LED is optional and requires a series resistor. Lab 3 retains the original 187.5 kHz nominal PWM, but D2 has no onboard LED, so AD2 measures duty directly. Lab 5 defaults to a one-shot finite capture rather than repeatedly overwriting the buffer; a debugger variable requests another block.

The educational DMA byte transfer from the 8-bit ADC result to the low byte of TCC CCBUF2 follows the original peripheral-to-peripheral exercise; upper duty bits start at zero. Physical transfer behavior, suspend/resume timing, overruns and duty updates must be checked on hardware. Compilation is not evidence that the bus/peripheral transaction was observed.

Datasheet reference: [DS60001507N](https://onlinedocs.microchip.com/oxy/GUID-F5813793-E016-46F5-A9E2-718D8BCED496-en-US-15/index.html), §§6, 11.6, 13–15, 19.6.2, 21.6, 22.6, 23.6.4.3, 28.6.4, 29.6, 31.5, 32.6, 45.5.10/45.6, 48.6 and 49.6. The downloaded reference is 2,002 pages; section numbers, rather than copied page offsets from older revisions, are the stable lookup keys.

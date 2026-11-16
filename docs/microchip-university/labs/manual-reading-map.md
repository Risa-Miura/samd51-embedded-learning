# Original manual: reading and source map

Page references below are **one-based PDF page numbers**, which match the printed page numbers in the supplied 126-page manual. The document's repeated 2016/2017 footer dates are retained in the source; they are not the date of this adaptation.

## Complete coverage

| PDF pages | Original content | Source files / functions / operations | Where adapted |
| --- | --- | --- | --- |
| 1–3 | Title, contents, SAME54 board and I/O Xplained on EXT1; debug USB/EDBG | Complete reference projects, expansion LED/light sensor, onboard SW0 | [Compatibility](compatibility.md), [wiring](../../hardware/wiring-reference.md) |
| 4–9 | XC32 device/component/PIO headers; MPLAB navigation | `xc.h`, `same54p20a.h`, `component/*.h`, `pio/*.h`; original tools MPLAB X 5.15 / XC32 2.15 | [Setup](workbench-setup.md#inspect-the-device-headers) |
| 10–12 | Open existing MPLAB projects and source organization | `system_init.c`, `constants.h`, `globals.c`, `main.c`, extern headers | Shared `workbench.c/.h`, per-lab `main.c` |
| 13–16 | Read configuration words and generate `#pragma config` | User words, BOOTPROT/region locks, old XC32 erratum | Omitted deliberately: preserve existing user row and bootloader |
| 17–22 | Lab 1 tasks, open-loop DFLL and GCLK0 | `DFLL48M_open_loop_init`, DFLLCTRLA/DFLLSYNC/STATUS, GENCTRL0 | `clock_init`, open build |
| 23–29 | External-crystal closed-loop DFLL | XOSC32K, GENCTRL3, PCHCTRL0, DFLLMUL, DFLLCTRLB | `clock_init`, `--closed-loop` build |
| 30–38 | GPIO and pin multiplexing | `init_ports`; PB08 expansion LED, PB31 SW0, PB14 GCLK0 output, PC18 board LED; DIR/OUT/PINCFG/PMUX/WRCONFIG | `board_init`, `pin_mux`; D13/PB01, D48/PC04, divided clock on D37/PA16 |
| 39–45 | TC3, clocks and 1 ms timer setup | `init_TC3`, APBBMASK, PCHCTRL26, COUNT16, prescaler 16, MFRQ, CC0, OVF | `tc3_init`; explicit MC0 flag; 2,999 compare value |
| 46–51 | Globals, constants, debounce ISR, NVIC and foreground LEDs | `TC3_Handler`, `enable_interrupts`, `main`; 500 ms LED half-period | Lab 1 counters, bounded ISR and complementary outputs |
| 52 | Run/program/debug | SW0 held → two LEDs alternate | [Lab 1 procedure](lab-1-gpio-timers.md), Atmel-ICE replacing EDBG |
| 53–60 | Lab 2 RTC initialization | `init_rtcc`/`init_RTCC`; OSCULP32K, RTCCTRL, MODE0, MATCHCLR, COMP0, CMP interrupt | `rtc_init`, `rtc_set_period` |
| 61–65 | RTC ISR, NVIC, empty main loop and LED observation | `RTC_Handler`, `enable_interrupts`, `main`; text inconsistently says one/two seconds | [Lab 2](lab-2-rtc.md): defined nominal 1 s toggle interval |
| 66–69 | Lab 3 tasks and ADC/PWM pin mux | PB04/B ADC1 AIN6; PC18/F TCC0 WO2 | Same MCU pads exposed as A7 and D2 |
| 70–77 | TCC normal PWM, channel 2 | `init_TCC0`, PCHCTRL25, WAVE, PER=255, CC2=128 | `pwm_init`, D2 waveform replaces board LED brightness |
| 77–88 | ADC clock, calibration, reference, free-run and resolution | `init_ADC`, PCHCTRL41, factory calibration, 2.5 V INTREF, AIN6, DIV256, 8 bit, SAMPCTRL | `adc_init`; AD2 replaces the light sensor |
| 89–99 | DMAC descriptor and channel initialization | `init_dmac`, BASEADDR/WRBADDR, BTCNT=1,000, ADC1 RESULT → TCC0 CCBUF2, circular descriptor, SUSP | `dma_init`, linker-managed aligned descriptors, explicit single-beat burst trigger |
| 100–104 | Suspend/resume ISR, RTC heartbeat, seed conversion, light response | `DMAC_0_Handler`, `RTC_Handler`, `main`, SWTRIG.START | [Lab 3](lab-3-dma.md), expected voltage-to-duty relationship |
| 105–113 | Lab 4 events and changes to prior ADC/DMA code | `init_event`; DMAC CH0 → ADC1 START; RTC CMP0 → PORT event; FREERUN removed; EVOSEL_BURST, CHEVCTRL | `events_init`, symbolic event IDs and channel+1 user encoding |
| 114–119 | EIC setup and debounce | `init_external_interrupts`; PB31/EXTINT15, 1 kHz GCLK1, CONFIG, DEBOUNCEN, DPRESCALER, ASYNCH | `eic_init`; PC04/EXTINT4, synchronous stability debounce |
| 120–121 | EIC changes RTC compare period and runs | `EIC_EXTINT15_Handler`; COUNT cleared before COMP changed | `EIC_EXTINT_4_Handler` defers period change to foreground; 1 s/0.25 s toggles |
| 122–124 | Lab 5 destination increments and averaged ADC values | `u8ADCdata[1000]` example, HWORD beats, DSTINC, end address, 12 bit, AVGCTRL 8/ADJRES3 | `uint16_t g_samples[1000]`, one-past-end address and one-shot chain |
| 125–126 | Pause and inspect data memory | Absolute example addresses near `0x20002534` | Inspect `g_samples` symbol and map file, not copied addresses |

## Reading the original alongside the adaptation

The PDF is the primary record of the **educational exercise**. It is not a safe flashing recipe for the Grand Central. The imported `projects/Lab1.X` remains an original SAME54 reference; do not flash it to this board. The new code is an independently written implementation of the relevant behavior, with [explicit corrections](compatibility.md#manual-inconsistencies-corrected).

Original Lab 2 teaches the RTC's 32-bit counter mode, despite the course's RTCC terminology. It does not implement a calendar. Original Lab 5 is ADC-to-memory acquisition, despite its short “memory data transfer” title; it is not a memory-to-memory `memcpy` exercise.

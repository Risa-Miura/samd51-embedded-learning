# Workbench setup: edit, build, inspect, debug

## Selected workflow and alternatives

| Approach | Evidence / strengths | Limitations and decision |
| --- | --- | --- |
| MPLAB X / XC32 | Original course workflow; ATSAMD51 DFP and Atmel-ICE are supported by Microchip; XC32 v2.50 and the selected packs are present locally | IDE programming/reset options must be checked before preserving an offset bootloader application; existing address-zero project is retained, not reused for flashing |
| VS Code + Arm GNU + Microchip headers + Python build script | Final labs are register-level C, with explicit vectors/linker layout, ELF/BIN checks, reproducible flags and `compile_commands.json` | Requires Arm GNU tools; no Arduino framework/startup/USB convenience layer |
| OpenOCD / GDB + Cortex-Debug | CMSIS-DAP supports Atmel-ICE; official OpenOCD D51 target/flash driver; standard ELF symbols and SVD inspection | Offline executable/config checks are complete; actual ICE driver/firmware/connection still needs a bench check |

Use **VS Code + GNU + OpenOCD/GDB** for these new applications. Continue using your established MPLAB setup for the separate existing projects. Do not merge their generated build files or loadable images. If choosing MPLAB for the new code later, preserve the supplied startup/linker contract, omit config words, inspect the image, and review erase/program-range behavior first.

## Local tool baseline

- Python 3.10+ for `build.py` and `program.py`; no Python packages are needed for firmware builds.
- Arm GNU Embedded **9-2019-q4-major / GCC 9.2.1**, GDB 8.3, already prepared locally in ignored `.tools/arm-gnu`.
- Microchip **SAMD51_DFP3.10.283** at `%USERPROFILE%/.mchp_packs/Microchip/SAMD51_DFP/3.10.283`.
- ARM **CMSIS 6.4.0** at `%USERPROFILE%/.mchp_packs/ARM/CMSIS/6.4.0`.
- xPack **OpenOCD 0.12.0-7** in ignored `.tools/xpack-openocd-0.12.0-7`. Its version string identifies a packaged `0.12.0+dev` build; the exact release/archive is locked, rather than assuming it is the unmodified0.12.0 tag.
- VS Code; Microsoft C/C++ and Cortex-Debug are optional editor/debugger interfaces.

Tool binaries and packs are **not committed**. For another clone, install the pinned packs with Microchip's Pack Manager / MPLAB support and install Arm GNU from [Arm's official toolchain distribution](https://developer.arm.com/downloads/-/gnu-rm). The tested legacy Windows GCC archive is also distributed in [Adafruit's board tool packages](https://github.com/adafruit/arduino-board-index/releases/tag/build-tools). Obtain OpenOCD from the [maintainer's pinned Windows release](https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.12.0-7), verifying its published SHA-256. A newer compiler may be usable but has not been claimed as tested here. The [tool lock](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/toolchain-lock.json) records the target and pack versions.

## Build Lab 1 first

On a fresh Windows clone, the optional pinned download helper prepares the two ignored tool folders:

```powershell
Set-Location projects/mu-labs
python setup_windows_tools.py
```

It verifies SHA-256 before extracting the compiler/OpenOCD archives and retains existing tool folders. Install the pinned Microchip/CMSIS packs separately. The tools are already prepared on this workbench, so rerunning the helper retains them. It does not install drivers, change PATH, connect a probe or program hardware.

From the repository root:

```powershell
Set-Location projects/mu-labs
python build.py lab-1
```

Output: `build/lab-1/open/firmware.elf`, `firmware.bin`, `firmware.map` and `build-record.json`. The script prints sizes and rejects load segments below0x4000 or outside the 1 MiB Flash. It also checks stack/reset vectors and that the BIN exactly matches the ELF. It does not invoke any programming tool.

Alternative paths:

```powershell
python build.py lab-1 --gcc "C:/path/to/arm-none-eabi-gcc.exe"
python build.py lab-1 --packs "C:/path/to/.mchp_packs"
python build.py lab-1 --closed-loop
python build.py lab-1 --closed-loop --button-gated
python build.py all
```

Build variants have separate output folders: `open`, `closed`, `open-button`, or `closed-button`. For another lab use `lab-2` through `lab-5`. Choose its corresponding ELF for programming/debugging. Record the exact variant; do not debug an old ELF while flashing a new BIN.

## Build details to understand

The compiler targets `cortex-m4`, Thumb, software floating-point ABI, C11, `-Og -g3`, freestanding operation, section garbage collection, and **warnings as errors**. The target macro is `__SAMD51P20A__`; the source includes `samd51p20a.h`, not a renamed SAME54 header. Startup is deliberately small and provides RAM initialization and a complete DFP-shaped vector table. No C library, heap, Arduino API or vendor-generated configuration pragmas are linked. This is an educational runtime, not a general C++/RTOS runtime.

The code disables and invalidates CMCC before initializing shared RAM, leaving cache off so CPU/DMA ownership is easier to inspect. This is a controlled teaching choice, not a firmware performance optimization. A measurement made with this setup cannot be presented as the default120 MHz board's performance.

## Inspect the device headers

Open the DFP's `samd51a/include/samd51p20a.h`, `component/`, `instance/` and `pio/samd51p20a.h`. Compare these exact definitions:

- `TC3_GCLK_ID`, `TC3_IRQn`, `TC3_REGS`, `TC_SYNCBUSY_CC0`.
- `ADC1_GCLK_ID`, `ADC1_DMAC_ID_RESRDY`, `ADC_CALIB_*` and `FUSES_SW0_WORD_0_ADC1_*`.
- `EVENT_ID_GEN_DMAC_CH_0`, `EVENT_ID_USER_ADC1_START`, `EVENT_ID_USER_PORT_EV_0`.
- `PINMUX_PC04A_EIC_EXTINT4`, `PINMUX_PB04B_ADC1_AIN6`, `PINMUX_PC18F_TCC0_WO2`.

`build.py` writes an ignored `compile_commands.json`. Open **projects/mu-labs** as its own VS Code folder to use the scoped C/C++ settings/tasks; these do not replace your root XC32 IntelliSense or MPLAB files. Source navigation must reflect the same header paths and target macro as the build.

## Inspect the image before a physical flash

```powershell
& .\.tools\arm-gnu\bin\arm-none-eabi-readelf.exe -l build/lab-1/open/firmware.elf
& .\.tools\arm-gnu\bin\arm-none-eabi-objdump.exe -h build/lab-1/open/firmware.elf
Get-Content build/lab-1/open/build-record.json
```

Expect the application vector table at **0x4000**, stack **0x2003fff8**, and no file-backed load segment at address0 or the user row. BSS is reserved in SRAM and initialized by startup; its zero-file-size ELF segment is not a Flash write. [Follow the Atmel-ICE guide](../../hardware/atmel-ice.md) for the separate, explicit connection/programming steps.

## Documentation preview

The existing MkDocs dependencies/tasks are preserved. From the repository root with its documentation environment selected:

```powershell
python -m pip install -r requirements-lock.txt
python -m mkdocs serve
```

Record actual observations in each lab's **K. What I learned** section and evidence table. A build pass or debugger connection does not complete a timing experiment.

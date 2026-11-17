# Workbench setup: edit, build, inspect, debug

## MPLAB in VS Code

Your primary Microchip interface is **MPLAB Tools for VS Code**. Follow the [MPLAB extension workflow](mplab-vscode.md) for the installed extensions, existing XC32 project, lab build tasks, native Atmel-ICE attach configuration, and stepping/register inspection. The desktop MPLAB X IDE is not needed for these instructions.

| Part of the workflow | Selection for the adapted labs | Validation / limit |
| --- | --- | --- |
| Editor and Microchip services | VS Code + installed MPLAB Extension Pack | Local installed versions identified in the extension guide |
| Firmware build | Arm GNU + Microchip headers + `build.py`, run through VS Code tasks | Eight image variants compiled and checked; the build is not a generated MPLAB/XC32 project |
| Explicit application programming | `program.py` through OpenOCD | Application range/ELF checks and protected-region comparisons; actual target operation pending |
| Primary debugging interface | Debug Adapter for MPLAB, Atmel-ICE/SWD, direct ELF `attach` | Supplied settings checked against installed 2.2.0 schema; hardware session pending |
| Alternative debugging interface | OpenOCD/GDB + optional Cortex-Debug in VS Code | Offline executable/config checks; explicit offset startup sequence |

Your separate native MPLAB blink project continues to use XC32 v2.50 and MPLAB CMake Runner in VS Code. It retains its original startup and address-zero layout. The new labs retain their tested custom startup/linker and 0x4000 layout. Review the image and programming ranges before using a generic MPLAB Program or Debug Launch action with either project.

## Local tool baseline

- Python 3.10+ for `build.py` and `program.py`; no Python packages are needed for firmware builds.
- Arm GNU Embedded **9-2019-q4-major / GCC 9.2.1**, GDB 8.3, already prepared locally in ignored `.tools/arm-gnu`.
- Microchip **SAMD51_DFP3.10.283** at `%USERPROFILE%/.mchp_packs/Microchip/SAMD51_DFP/3.10.283`.
- ARM **CMSIS 6.4.0** at `%USERPROFILE%/.mchp_packs/ARM/CMSIS/6.4.0`.
- xPack **OpenOCD 0.12.0-7** in ignored `.tools/xpack-openocd-0.12.0-7`. Its version string identifies a packaged `0.12.0+dev` build; the exact release/archive is locked, rather than assuming it is the unmodified0.12.0 tag.
- VS Code with the installed **MPLAB Extension Pack**, including MPLAB clangd and Debug Adapter for MPLAB. Cortex-Debug is an optional alternative.

Tool binaries and packs are **not committed**. For another clone, install the pinned packs with MPLAB's pack support in VS Code and install Arm GNU from [Arm's official toolchain distribution](https://developer.arm.com/downloads/-/gnu-rm). The tested legacy Windows GCC archive is also distributed in [Adafruit's board tool packages](https://github.com/adafruit/arduino-board-index/releases/tag/build-tools). Obtain OpenOCD from the [maintainer's pinned Windows release](https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.12.0-7), verifying its published SHA-256. A newer compiler may be usable but has not been claimed as tested here. The [tool lock](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/toolchain-lock.json) records the target and pack versions.

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

`build.py` writes an ignored `compile_commands.json`. Open **projects/mu-labs** as its own VS Code folder to use the scoped tasks and MPLAB clangd compile-command path. The optional C/C++ setting is retained for users of that language service. Your root XC32 IntelliSense and existing MPLAB project files are preserved. Source navigation must reflect the same headers and target macro as the actual build.

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

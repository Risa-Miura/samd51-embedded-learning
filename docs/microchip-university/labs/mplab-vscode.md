# MPLAB extension workflow in VS Code

Use **MPLAB Tools for VS Code** as your Microchip development interface. The desktop MPLAB X IDE is not required for the steps below. The extension can install its own MPLAB debugger components; their names may refer to MPLAB X even though the interface is VS Code.

## Your installed setup

The local extension inventory contains MPLAB Extension Pack **1.2.9**, Services for MPLAB **2.4.2**, Debug Adapter for MPLAB **2.2.0**, MPLAB UI **2.2.3**, MPLAB CMake Runner **1.11.0**, MPLAB Toolchain Support **1.11.3**, Project Importer **2.2.0**, and MPLAB clangd **2.1.0**. XC32 **v2.50** and the SAMD51/CMSIS packs are already present.

The installed debugger schema and [Microchip's Debug Adapter documentation](https://marketplace.visualstudio.com/items?itemName=Microchip.mplab-core-da) support `Atmel-ICE`, `SWD`, direct ELF files, and `attach`. The supplied configuration uses those documented fields. This is a configuration check; a real ICE connection and source-level debug session remain to be tested.

## Choose the folder and build route

| Work | Folder to open in VS Code | Build action |
| --- | --- | --- |
| Adapted Labs 1–5 | `projects/mu-labs` | **Terminal → Run Task → Lab 1: build open loop**, or the corresponding lab task |
| Your existing native MPLAB/XC32 blink project | `projects/01-led-blink/samd51-blink` | Select its configuration in MPLAB, generate CMake, then build using MPLAB CMake Runner |
| Original course project for comparison | `projects/Lab1.X` | Review/import with MPLAB Project Importer; it still targets SAME54 and is not a Grand Central image |

**The adapted labs still compile with the tested Arm GNU compiler through `build.py`.** Using the MPLAB debugger does not require changing that compiler. The supplied tasks are standard VS Code tasks, not generated MPLAB CMake projects. XC32 remains the compiler for your separate native MPLAB project. No claim is made that these new labs have been compiled with XC32.

For a new native MPLAB project, use **MPLAB: Create new project** in the Command Palette. Review settings through MPLAB UI or **MPLAB: Edit Project Properties (JSON)**; select **ATSAMD51P20A**, your XC32 installation, **SAMD51_DFP 3.10.283**, and **CMSIS 6.4.0** as appropriate. Use the CMake Runner to generate/build. The existing blink project links at address zero, so its Program/Debug action can replace the bootloader. Use the offset lab images and the preservation procedure for this course adaptation.

Opening a folder containing the old `Lab1.X` may offer an import. Importing project metadata does not retarget its device, rewrite its firmware, or establish bootloader preservation. You can complete these labs without importing it.

## Build Lab 1 in VS Code

1. Use **File → Open Folder** and open `projects\mu-labs`.
2. Keep the installed MPLAB Extension Pack enabled. Allow its normal Services/Debugger setup to finish if prompted.
3. Select **Terminal → Run Task → Lab 1: build open loop**. The equivalent terminal command is `python build.py lab-1`.
4. Read `build/lab-1/open/build-record.json`. The image must start at `0x4000`; the task checks its load regions, stack/reset vector, and matching BIN.
5. For source navigation, MPLAB clangd uses the generated `compile_commands.json` in this folder. If you also use Microsoft C/C++, choose one language service for this folder to avoid duplicate diagnostics. The repository root's existing XC32 settings are retained.

The build task does not connect to hardware. When you select a different Lab 1 variant, use its matching ELF for both programming and debugging.

## Program, then attach with MPLAB

1. Complete the [SWD wiring, target power, identity and backup checks](../../hardware/atmel-ice.md). Use the explicit `program.py` procedure there to program and verify the selected application and compare the retained bootloader/user row. MPLAB's general Program/Erase/Debug Launch actions are not covered by that script's range and readback checks.
2. Close any OpenOCD process before MPLAB takes the Atmel-ICE. One process can own the probe at a time.
3. Optionally run **MPLAB: List connected tools** and record the ICE serial number. Add a `serial` field to the selected configuration if several Atmel-ICE probes are attached.
4. Open **Run and Debug**, select **MPLAB: Attach Lab 1 (already programmed)**, then press **F5**. The checked-in configuration specifies `mplab-core-da`, `attach`, `ATSAMD51P20A`, `Atmel-ICE`, `SWD`, and the exact ELF/DFP. `interfaceSpeed: 0.5` means **0.5 MHz / 500 kHz** in this extension.
5. `attach` connects to an already programmed target and requests a halt. It does not use the extension's programming `launch` mode. There is no automatic build/program task on F5: keep the ELF identical to the image you programmed.

Configurations for Labs 2–5 are also supplied. For a Lab 1 variant, edit the configuration's `program` path from `open` to `closed`, `open-button`, or `closed-button` to match the programmed build.

### Reach main through the retained bootloader

Set a breakpoint at `main` in the selected lab source. While the native MPLAB session is active, use **MPLAB: Reset and Halt at main**, when offered for the connected target, and inspect the resulting stop. A hardware reset starts the retained bootloader first; let its normal application timeout expire. Avoid double-tapping reset during this step, because that deliberately requests bootloader mode.

At the application `main`, confirm the call stack/source belongs to the selected lab, `SCB->VTOR == 0x4000`, and the stack is in SRAM. Do not assume an initial halt in the bootloader has initialized the application's `.data` or `.bss`. If the native reset action does not reach the application, inspect the PC and record the failure. The [alternative GDB entry sequence](../../hardware/atmel-ice.md#attach-gdb-and-enter-the-offset-application) explicitly sets the application's stack and reset entry; do not paste its GDB commands into MPLAB's native Debug Console.

### Inspect and step

- Use the source gutter for a breakpoint at `main` or `TC3_Handler`; **F10** steps over, **F11** steps into, **Shift+F11** steps out, and **F5** continues.
- In **Run and Debug**, use Variables, Watch, Call Stack and CPU Registers. Add `g_fault_stage`, `g_ms`, `g_led_toggles`, and the expressions listed in each lab's section G.
- Use MPLAB's device/I/O views when available for the selected device and the supported memory viewer for SRAM. Inspect `g_samples` only after Lab 5 reports a complete buffer. Avoid repeated reads of ADC RESULT while DMA owns it.
- MPLAB's native Debug Console has its own commands; `help -?` lists them. GDB commands and Cortex-Debug's `-exec` prefix belong to the alternative configuration.
- Remove breakpoints and run continuously before capturing AD2 timing. A paused session changes the experiment.

The desktop MPLAB X IDE is not part of this procedure. OpenOCD/GDB and Cortex-Debug remain an alternative within VS Code for explicit startup control and diagnosis.

## Remaining physical checks

Record the actual debugger backend/firmware/driver versions, ICE serial, target identity/voltage, unchanged protected-region comparisons, first stop at main, stepping/ISR behavior, and reset-to-application behavior. The extension configuration has been inspected against the installed schema; none of these target operations was performed while preparing this guide.

References: [Microchip's MPLAB Tools for VS Code overview](https://www.microchip.com/en-us/development-tool/MPLAB-EXTENSIONS), [Services for MPLAB](https://marketplace.visualstudio.com/items?itemName=Microchip.mplab-extensions-core), [Debug Adapter for MPLAB](https://marketplace.visualstudio.com/items?itemName=Microchip.mplab-core-da), and [Project Importer for MPLAB](https://marketplace.visualstudio.com/items?itemName=Microchip.mplabx-importer). Installed extension manifests and readmes supplied the local command names and version-specific field checks.

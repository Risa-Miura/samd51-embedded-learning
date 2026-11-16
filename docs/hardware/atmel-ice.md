# Atmel-ICE: SWD programming and debugging

## Connector and power

Use the **SAM** connector on the Atmel-ICE, not the AVR connector. The Grand Central rev B schematic calls its standard 2×5, 1.27 mm (0.05 inch) debug header **X2**. Use the official SAM-compatible probe ribbon from the kit or an adapter whose signal mapping is verified. Do not use an AVR ISP lead by appearance alone.

| Atmel-ICE SAM signal / pin | Grand Central X2 contact | Target connection |
| --- | --- | --- |
| VTG / 1 | 1 | Board 3.3 V reference, **sense input to probe**, not target power |
| SWDIO / 2 | 2 | PA31 / SWDIO |
| GND / 3 | 3 | Board GND |
| SWCLK / 4 | 4 | PA30 / SWCLK |
| GND / 5 | 5 | Board GND |
| SWO / 6 | 6 | Optional trace, not used by these labs |
| Key / 7 | 7 | Key/not connected |
| Unused for SWD / 8 | 8 | Leave as supplied; not used |
| GND / 9 | 9 | **X2 GND-detect is not connected in the rev B schematic**; use contacts3/5 for target ground |
| nSRST / 10 | 10 | Board active-low reset |

References: [Atmel-ICE DS50002999A](https://www.microchip.com/content/dam/mchp/documents/MCU08/ProductDocuments/UserGuides/Atmel-ICE-Programmers-Debuggers-DS50002999A.pdf), §2.6 and §§3.9/4.1.2, tables3-8/4-4; [Adafruit rev B X2 schematic](https://github.com/adafruit/Adafruit-Grand-Central-PCB/blob/master/Adafruit%20Grand%20Central%20M4%20Express%20rev%20B.sch). A 20-pin SAM-ICE/J-Link pin table is a different connector, and is not the table above.

### Orientation

With the **board header's mating face** facing you, normalize the diagram so its identified **contact 1** is upper-left:

```text
pin-1 mark / square pad
          v
    1 VTref    2 SWDIO
    3 GND      4 SWCLK
    5 GND      6 SWO (unused)
    7 KEY      8 unused
    9 NC      10 nRESET
```

This is a pin-number diagram, not a claim that pin 1 always points toward a particular board edge. A cable/socket viewed from its wire-entry side is mirrored. On the ICE enclosure, the red pin 1 ribbon conductor aligns with the triangle on the blue belt per the official guide. On the target, align that conductor with the board header's pin 1 indicator/key. If either indicator or the cable mapping is ambiguous, stop and check against the actual keyed adapter/schematic; do not guess from wire colors or rotate a connector until communication works. If using individual wires, map every contact by signal, including VTref and reset.

The board must be powered independently, normally through its USB connector. The Atmel-ICE does **not** power the target through VTG. Confirm approximately 3.3 V on the board rail before connection; AD2 Scope/DC measurement can verify voltage at the accessible 3.3 V/GND headers. Do not short fine-pitch contacts with loose probes. Unplug power before continuity checks.

## Host setup and compatibility

- Install Microchip's official tool/USB support if using MPLAB X or Microchip Studio. Use those tools for any required Atmel-ICE firmware update; do not guess an image or interrupt an update.
- The selected open workflow uses **OpenOCD + CMSIS-DAP + GDB**. OpenOCD's `target/atsame5x.cfg` explicitly supports D51 despite its name, and its `atsame5` driver identifies `SAMD51P20A` with 1 MiB Flash/256 KiB SRAM. See the [official target configuration](https://github.com/openocd-org/openocd/blob/v0.12.0/tcl/target/atsame5x.cfg) and [driver](https://github.com/openocd-org/openocd/blob/v0.12.0/src/flash/nor/atsame5.c).
- CMSIS-DAP HID can use the Windows HID driver. Do not replace the entire ICE composite-device driver with Zadig. If discovery fails, close MPLAB/Studio, inspect Device Manager, and try the documented `cmsis-dap backend hid` selection with the OpenOCD build used here.
- Keep only one debug server/IDE connected to the probe. If several CMSIS-DAP probes are attached, add `adapter serial <actual ICE serial>` to the local config before connection.

The bundled local xPack OpenOCD 0.12.0-7 executable and configuration were checked offline; the probe's firmware, USB interface and actual device connection have **not** been exercised. Verify these during first bring-up. Initial SWD speed is a conservative 500 kHz.

## First connection: identify and read, without programming

From `projects/mu-labs` in PowerShell, after following the wiring checks:

```powershell
& .\.tools\xpack-openocd-0.12.0-7\bin\openocd.exe -f debug/atmel-ice.cfg -c "init; reset halt; flash probe 0; check_grand_central_target; shutdown"
```

This resets and halts the board and reads its identity/protection; it does not erase or program Flash. Expect the log to identify **ATSAMD51P20A** and the target voltage/probe as supported by that backend. The helper checks processor/family/series/device selection and refuses a BOOTPROT region extending beyond `0x4000`. A read failure, secured device, wrong part or unexpected protection is a reason to stop, not to unlock or change fuses.

## Program the application, preserve the bootloader

First compile and inspect the build record. Then run explicitly:

```powershell
python build.py lab-1
python program.py build/lab-1/open/firmware.elf --replace-application
```

`program.py` checks the ELF's ARM type, vector/stack addresses and every file-backed load segment; it verifies that the BIN exactly matches those segments. It backs up the first 16 KiB and the 512-byte user row under an ignored timestamped `backups/` folder, writes/verifies the application BIN at **0x4000**, reads the same protected regions again, and compares them byte-for-byte. It leaves the target halted. The script does not issue chip erase, unlock, protection changes or user-row writes. Existing application data in the application's erased Flash blocks is replaced; archive anything important first.

Never substitute the original `Lab1.X` image or the older address-zero `samd51-blink` ELF. Do not use `atsame5 chip-erase`, `atsame5 bootloader <size>`, mass erase, unlock or copied `#pragma config` settings. If a sector is protected or programming fails, retain backups/logs and stop before considering protection changes. Restoring a bootloader/fuse image is a separate operation requiring an explicit decision.

## Attach GDB and enter the offset application

Start the debug server in one terminal:

```powershell
& .\.tools\xpack-openocd-0.12.0-7\bin\openocd.exe -f debug/atmel-ice.cfg
```

In a second terminal:

```powershell
& .\.tools\arm-gnu\bin\arm-none-eabi-gdb.exe build/lab-1/open/firmware.elf
```

At the GDB prompt:

```text
target extended-remote localhost:3333
monitor reset halt
set $sp = *(unsigned int *)0x4000
set $pc = Reset_Handler
set $primask = 1
tbreak main
continue
```

The hardware reset initially points at the retained bootloader, not this ELF's reset vector. Setting the application stack and reset entry deliberately starts its C runtime; `Reset_Handler` relocates VTOR, disables inherited interrupts/SysTick, initializes RAM and calls `main`. This attach sequence performs **no GDB `load`** and makes no additional Flash writes.

Use `break TC3_Handler`, `next`, `step`, `continue`, `info registers`, `print g_ms`, and `print/x *TC3_REGS` as appropriate. At `main`, source-level stepping is meaningful; at reset/startup, instruction stepping (`stepi`) may be clearer. Set a temporary breakpoint and remove it before timing captures. Breakpoint resources are finite; prefer a few hardware breakpoints rather than software patches in Flash.

## VS Code debugging

Open the **projects/mu-labs** folder, install **Cortex-Debug** and Microsoft C/C++ if needed, build the selected lab, then use the supplied **Attach Lab 1 (already programmed)** launch configuration. It attaches to the server already running on port3333, loads symbols/SVD, and does not program. In the Debug Console use GDB commands with Cortex-Debug's `-exec` prefix, for example `-exec monitor reset halt` and the entry sequence above. Change the ELF path to debug another lab/build variant.

Variables show C state; the Peripherals view uses the installed ATSAMD51P20A SVD. Watch `g_fault_stage`, `g_ms`, `g_led_toggles` and configuration registers. Reading an ADC RESULT consumes data, and some registers have read side effects: do not live-refresh RESULT while DMA owns it. Lab 5's completed buffer is the safe place to inspect captured samples.

## Recovery checks

| Failure | Check before taking another action |
| --- | --- |
| Probe not found / busy | Correct USB data cable; official firmware/driver; only one IDE/server; CMSIS-DAP HID backend |
| Target voltage missing | Board USB power; VTref on SAM pin 1; GND3/5; connector orientation |
| Cannot halt / read device | SWDIO2, SWCLK4, reset10; short cable; lower `adapter speed` to 100 kHz; inspect reset wiring |
| Program blocked by protection | Check logs/BOOTPROT/region state; no automatic unprotect or user-row change |
| Stops at address0 or bootloader | Application is at 0x4000; enter its reset handler with its own stack as shown |
| Clock wait never completes | Read `g_fault_stage`; inspect source/reference/channel and optional crystal lock; return to open-loop build |
| LED freezes at a breakpoint | Timers/peripherals may halt in debug; remove breakpoints and run continuously for measurements |
| Bootloader USB not visible | The bare-metal app has no USB stack; double-tap reset and check existing bootloader mode. Do not erase the device as a discovery step |

Physical connection, readback, Flash preservation comparison, breakpoint/step behavior and autonomous reset boot must all be recorded on the actual hardware before claiming this workflow is bench-verified.

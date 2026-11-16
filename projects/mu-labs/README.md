# Microchip University labs — Grand Central M4

Independent bare-metal C adaptations for ATSAMD51P20A. Existing SAME54 and blink projects remain separate.

- [Start with the manual](../../docs/microchip-university/labs/index.md)
- [Workbench setup](../../docs/microchip-university/labs/workbench-setup.md)
- [Atmel-ICE guide](../../docs/hardware/atmel-ice.md)
- [AD2 guide](../../docs/hardware/analog-discovery-2.md)

From this folder run `python build.py lab-1` before attempting a hardware connection. Local ignored tools are prepared in `.tools`; packs remain in the installed `.mchp_packs` directory. `python build.py all` builds five separate applications. Use `--closed-loop` and `--button-gated` for the Lab1 variants.

The default layout starts at0x4000 and reserves the first16 KiB for the existing UF2 bootloader. `build.py` checks all load segments, vectors and matching BIN contents. `program.py` is a separate explicit hardware operation, requires `--replace-application`, backs up/compares the bootloader and user row, and never issues a chip erase/unlock/fuse command. Read the wiring and preservation guide before using it.

`shared/` supplies the target-specific vector table, small C startup, register initialization and linker script. `lab-1/` through `lab-5/` each supply a separate `main.c`. `debug/` holds the CMSIS-DAP/D51 configuration. `.vscode/` is scoped to this folder, preserving the repository's existing XC32/MPLAB configuration. Build outputs, local tools, captures/backups and compile commands are ignored.

**Hardware status: untested.** No flashing, probe connection, actual waveforms or measurements were performed while preparing these files. Software validation is recorded in the delivery and lab verification guide. Do not treat compile success as a completed experiment.

[Detailed build, image-guard and offline debugger validation](VALIDATION.md).

/* ATSAMD51P20A, 1 MiB Flash, 256 KiB SRAM.
 * Reserve the first 16 KiB for the existing Grand Central UF2 bootloader.
 * Reserve the final 8 SRAM bytes for bootloader double-tap state/alignment.
 */
ENTRY(Reset_Handler)
MEMORY {
  FLASH (rx) : ORIGIN = 0x00004000, LENGTH = 1M - 16K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 256K - 8
}
_estack = ORIGIN(RAM) + LENGTH(RAM);
PHDRS {
  flash PT_LOAD FLAGS(5);
  ram PT_LOAD FLAGS(6);
  zero PT_LOAD FLAGS(6);
}
SECTIONS {
  .vectors : { KEEP(*(.vectors)) } > FLASH :flash
  .text : { *(.text*) *(.rodata*) . = ALIGN(4); } > FLASH :flash
  .ARM.extab : { *(.ARM.extab*) } > FLASH :flash
  .ARM.exidx : { *(.ARM.exidx*) } > FLASH :flash
  .data : {
    . = ALIGN(4); _sdata = .; *(.data*) . = ALIGN(4); _edata = .;
  } > RAM AT> FLASH :ram
  _sidata = LOADADDR(.data);
  .bss (NOLOAD) : {
    . = ALIGN(16); _sbss = .; *(.bss*) *(COMMON) . = ALIGN(16); _ebss = .;
  } > RAM :zero
  /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) }
  ASSERT(ADDR(.vectors) == 0x4000, "Bootloader application origin moved")
  ASSERT((_estack & 7) == 0, "Stack must be 8-byte aligned")
  ASSERT(_ebss + 8192 < _estack, "Reserve at least 8 KiB for the stack")
}

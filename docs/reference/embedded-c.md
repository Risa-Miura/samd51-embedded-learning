# Embedded C working reference

This page is a study aid, not a record of completed exercises. Use the device's CMSIS headers instead of handwritten addresses and masks.

## Integer widths and masks

Prefer fixed-width types when a peripheral format or serialized record requires a specific width. Keep shifts unsigned and within the width of the left operand.

```c
#include <stdint.h>

/* Pure C illustration, not a peripheral write. */
uint32_t set_bit(uint32_t value, unsigned bit) {
    return bit < 32u ? value | (UINT32_C(1) << bit) : value;
}
```

## Volatile and concurrency

`volatile` tells the compiler that accesses are observable. It does not make a multi-step update atomic, provide a lock, transfer buffer ownership, or synchronize CPU and DMA. Record the single-writer/reader contract and use a target-appropriate critical section or synchronization method where needed.

## Register access semantics

A normal read-modify-write can lose a concurrent update. Some registers provide SET/CLR aliases; others clear flags when a one is written. Read the register's access semantics before choosing an operation. A reserved bit or synchronized register may require a particular sequence.

## Keep interrupt work bounded

Prefer capturing a small fact and returning promptly. Avoid blocking output, heap allocation, and long processing in an ISR. Buffer indices and event counters require explicit overflow handling. Measure the instrumented implementation and describe the added instrumentation overhead.

## Inspect the generated code

Record the compiler and optimization flags. Inspect the map file for memory usage and the disassembly when evaluating a specific operation. ARM Cortex-M uses Thumb instructions; distinguish the compiled code from illustrative assembly.

```asm
/* Illustrative Thumb sequence only; not a complete firmware program. */
movs r0, #0
adds r0, r0, #1
```

## Questions to practice

- What happens if an index wraps while an ISR produces data?
- Which buffer can foreground code read while DMA fills another?
- Does an optimization change observable behavior or just reduce work?

Consult [the resource list](resources.md) for the device and ARM reference material.

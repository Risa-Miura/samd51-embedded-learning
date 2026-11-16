# Lab 5 · DMA into an ADC sample buffer

**Status:** Complete source supplied and software-checked; no captured hardware samples.

## A. Original objective

PDF pp.122–126 teach a DMA transfer with sequential destination increments and debugger inspection of memory. The source remains **ADC1 RESULT**; this is not a memory-to-memory copy. The ADC changes to 12-bit results with eight conversions averaged per result.

## B. Hardware differences

Use AD2 W1→A7 as the voltage source. Remove the PWM requirement: the onboard D13 LED marks a completed block. Replace hardcoded RAM addresses with a linker-allocated `uint16_t g_samples[1000]`. Default one-shot ownership makes the debugger view stable instead of allowing continuous overwrite. Request another capture through `g_capture_request` only after the buffer is complete.

## C. Required equipment

Board/ICE/USB setup, AD2/harness, one 1 kΩ series resistor for W1→A7 and ground/signal leads. A stable DC source through the verified W1 configuration is enough; no button, extra LED or external memory is needed.

## D. Wiring

AD2 GND→board GND; verified W1→1 kΩ→A7. Optional Scope1+ monitors A7,1−→GND. D13 and D34 can be observed as completion signals. D2 PWM is not configured in this lab. Disable all unneeded outputs; never apply the generator to AREF.

## E. Firmware

[Lab 5 main](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/lab-5/main.c) uses shared [ADC/DMA setup](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/projects/mu-labs/shared/workbench.c). `g_capture_request` starts at 1 to request the first finite block.

- ADC1 selects12-bit resolution, free-run and AVGCTRL.SAMPLENUM8/ADJRES3. Eight conversions are averaged back to a 12-bit value; this is not a guaranteed increase in effective bit depth.
- Source remains fixed at the ADC RESULT register, beat size is **HWORD**, BTCNT=1,000 and destination increment is enabled with default one-beat steps.
- Initial DSTADDR is **`g_samples + 1000`**, exactly 2,000 bytes past the first sample. The DMAC decrements its working end address before accessing the first beat; do not use the final element's address or multiply the pointer twice.
- The buffer is exactly 1,000 `uint16_t` elements (2,000 bytes), initially0xffff as an obvious unwritten sentinel. The descriptor has DESCADDR=0 and completion interrupt action, giving a finite chain.
- On TCMPL, the handler stops ADC enable, executes a memory barrier, counts the block, sets `g_buffer_ready` and toggles D13. The DMA has finished owning the buffer. The main loop will not refill it until a subsequent request after completion.

The original p.123 byte array conflicts with halfword beats and mixed100/1,000-sample prose. This implementation uses consistent sample counts/types and symbol addresses. It does not write a descriptor to an arbitrary fixed RAM location.

Datasheet DS60001507N §§22.6.2.3/22.6.2.7,22.10,45.6.2.9–11/45.6.4. Original `init_dmac` destination/BTCTRL changes and `init_adc` averaging are retained conceptually; the overwrite policy is deliberately changed for reliable inspection.

## F. Build and flash

From the repository root, enter `projects/mu-labs` and run:

```powershell
python build.py lab-5
```

Inspect `build/lab-5/open/build-record.json` and the memory map, then follow the [Atmel-ICE identity, power, backup and connection checks](../../hardware/atmel-ice.md). Only after those checks:

```powershell
python program.py build/lab-5/open/firmware.elf --replace-application
```

This replaces the existing application, not the bootloader. Enter the offset application's reset handler using its own stack, as documented in the [debug guide](../../hardware/atmel-ice.md#attach-gdb-and-enter-the-offset-application). No physical programming was performed while writing this adaptation.

## G. Debug with Atmel-ICE

Watch `g_buffer_ready` until it becomes1, then halt and inspect `g_samples`, `g_descriptor`, `g_writeback`, `g_dma_blocks`, `g_dma_errors` and `g_adc_overruns`. Do not pause early and mistake initial0xffff sentinel values for ADC results. Expected completed values are0–4095; untouched0xffff entries require investigation.

GDB examples after completion:

```text
print g_buffer_ready
print g_dma_errors
print/x &g_samples
x/32uh g_samples
print g_samples[999]
dump binary memory capture.bin (char *)&g_samples[0] (char *)&g_samples[1000]
```

`capture.bin` contains exactly 2,000 bytes in target little-endian halfword order. Give each real capture a dated run record; the repository ships no example hardware data. To acquire another block after completion:

```text
set var g_capture_request = 1
continue
```

Do not request a new capture while viewing the prior buffer as stable evidence. If a transfer error occurred, inspect the cause and reset/rebuild rather than silently accepting a partial buffer.

## H. AD2 measurements

Begin with a scope-verified1.25 V DC input, acquire a complete block and inspect its distribution. Repeat at 0.25 and2.25 V; ideal12-bit results are roughly410,2,048 and3,686, subject to actual reference/input/ADC errors. These are expected values, not acceptance tolerances. Define tolerances after consulting electrical specifications and instrument uncertainty.

Then use the verified1 Hz0.2–2.2 V triangle. A complete buffer should vary with its sampled phase, but do not infer a uniform fixed sampling rate without measuring it. D34 produces one completion pulse per requested block. To measure total acquisition time, use an explicitly added start marker paired with completion, account for averaging/startup, and retain both edges; this source does not supply a fabricated block duration.

Optional Python import of a **real** exported buffer:

```python
from pathlib import Path
import numpy as np
raw = Path("capture.bin").read_bytes()
assert len(raw) == 2000
samples = np.frombuffer(raw, dtype="<u2")
assert samples.size == 1000 and np.all(samples <= 4095)
print(samples.min(), samples.max(), samples.mean())
```

## I. Expected results

Exactly1,000 sequential12-bit averaged values, no unwritten sentinel, one completion count/LED toggle per request, and a stable buffer after completion. The ADC-to-memory timing, noise reduction, voltage accuracy and absence of dropped data remain to be established on hardware. A zero overrun-incident counter alone is not a complete proof of data integrity.

## J. Troubleshooting

- Sentinel values: wait for completion; inspect BTCNT, halfword beat size, DSTINC and the correct **one-past-end** destination.
- Neighbor memory corruption: compare reserved2,000-byte buffer with actual beat count/width; inspect the map and descriptor; do not copy the manual's absolute addresses.
- All zero/saturated: inspect actual A7 voltage, reference, channel and calibration.
- Values move while paused: wrong/older continuous-overwrite implementation, a new request, or debugger/peripheral run settings; establish ownership before trusting snapshots.
- No completion: verify request/start, DMAC TCMPL enable/vector, DESCADDR=0, flags and `g_fault_stage`.

## K. What I learned


- Why is destination size2,000 bytes rather than1,000?
- Why does the descriptor contain a one-past-end address?
- What establishes CPU ownership of the finished buffer?
- How does averaging eight samples differ from proving extra effective resolution?
- What raw evidence would support a claim about throughput or missed samples?


| Date / firmware commit | Setup and raw capture | Actual observations / measurements | Unresolved issue |
| --- | --- | --- | --- |
| Not performed | Not recorded | Not measured | Hardware verification pending |

Keep expected and actual results separate. Check off the [bench verification list](verification.md) only after recording evidence for Lab 5.

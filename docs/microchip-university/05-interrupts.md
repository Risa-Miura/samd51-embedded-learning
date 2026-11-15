# Interrupts

**Status:** Not started

**Study date:** To record

**Course module / timestamp:** To record

**Board / core version / commit:** To record

!!! note "Editable learning template"
    The prompts below are preparation for study. Replace them with my own explanations and evidence. No lesson completion, hardware implementation, or results are claimed.

## 1. Overview

What question does this lesson answer? Summarize the relevant module and its scope.

## 2. Learning Objectives

- Explain peripheral flags, enables, NVIC priority, vector handlers, and communication with foreground code.
- Describe one observation that would demonstrate understanding.

## 3. Key Concepts

Separate interrupt latency, ISR execution time, and event period. Explain why volatile alone does not provide atomicity or queue ownership.

Add a diagram, describe assumptions, and distinguish datasheet facts from hypotheses.

## 4. Important Registers

**Lookup prompts:** Peripheral INTFLAG and INTENSET/INTENCLR; NVIC enable, pending, and priority via CMSIS. Confirm write-one-to-clear semantics before acknowledging a flag.

| Register / field | Purpose and access semantics | Reset value / synchronization | Datasheet revision & section |
| --- | --- | --- | --- |
| To verify | To explain | To verify | To cite |

Do not write guessed addresses or bit positions. Use the matching device header; distinguish read-only, read/write, and write-one-to-clear behavior.

## 5. Code Examples

Add a small C example after verifying the target device and toolchain. Explain each relevant operation and link to the full source under `projects/`. Label incomplete snippets as illustrative and record whether they compile.

```c
/* Learning placeholder: add a verified example after completing this section. */
```

## 6. Hands-On Experiment

**Proposed experiment:** Add a counter and a bounded foreground reader to a timer experiment; record how pending events and shared state are handled.

- Hypothesis and expected behavior: to define.
- Hardware, wiring, clock configuration, and instrument: to record.
- Build / upload command and source commit: to record.
- Procedure and acceptance criterion: to define before running.
- Actual implementation, observations, raw files: not yet recorded.

## 7. Debugging Notes

| Symptom | Evidence / hypothesis | Change made | Retest outcome |
| --- | --- | --- | --- |
| No experiment run yet | — | — | — |

## 8. Connection to Miniscope Zero

**Question to investigate:** Which work can be deferred outside an ISR to preserve real-time responsiveness?

Confirm relevance by inspecting the actual project with advisor guidance. Record a public source or describe the boundary of what can be shared.

## 9. Key Takeaways

To write after study: what can I now explain, what evidence supports it, and what remains uncertain?

## 10. References

- [Starting Microchip University course](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers) — record the relevant module and timestamp.
- [SAM D5x/E5x family datasheet](https://www.microchip.com/content/dam/mchp/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf) — record revision, chapter title, and table/section identifiers actually consulted.
- [Grand Central M4 guide](https://learn.adafruit.com/adafruit-grand-central) — confirm board-specific assumptions.

# Clock system

**Status:** Not started

**Study date:** To record

**Course module / timestamp:** To record

**Board / core version / commit:** To record

!!! note "Editable learning template"
    The prompts below are preparation for study. Replace them with my own explanations and evidence. No lesson completion, hardware implementation, or results are claimed.

## 1. Overview

What question does this lesson answer? Summarize the relevant module and its scope.

## 2. Learning Objectives

- Distinguish oscillators, generic clock generators, peripheral clock channels, and bus clocks.
- Describe one observation that would demonstrate understanding.

## 3. Key Concepts

Draw the source → generator → channel → peripheral path. Explain why a 120 MHz CPU does not imply every peripheral runs at 120 MHz.

Add a diagram, describe assumptions, and distinguish datasheet facts from hypotheses.

## 4. Important Registers

**Lookup prompts:** MCLK bus masks; GCLK GENCTRL and PCHCTRL; relevant OSCCTRL/OSC32KCTRL fields. Look up the exact peripheral ID and synchronization rules.

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

**Proposed experiment:** Inspect the Arduino core clock configuration, then plan a timer clock with explicit source and divider. Preserve USB and system timekeeping clocks.

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

**Question to investigate:** Which timing assumptions must be verified before trusting timestamps or comparing firmware performance?

Confirm relevance by inspecting the actual project with advisor guidance. Record a public source or describe the boundary of what can be shared.

## 9. Key Takeaways

To write after study: what can I now explain, what evidence supports it, and what remains uncertain?

## 10. References

- [Starting Microchip University course](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers) — record the relevant module and timestamp.
- [SAM D5x/E5x family datasheet](https://www.microchip.com/content/dam/mchp/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf) — record revision, chapter title, and table/section identifiers actually consulted.
- [Grand Central M4 guide](https://learn.adafruit.com/adafruit-grand-central) — confirm board-specific assumptions.

<p class="journal-kicker">Engineering notebook · ARM Cortex-M4F</p>

# SAMD51 Embedded Systems Learning Journal

<p class="journal-lead">This repository documents my journey learning embedded systems development, ARM Cortex-M4 programming, and low-level microcontroller firmware using the Adafruit Grand Central M4 Express.</p>

<div class="journal-meta" markdown>
**Research direction** · UCLA undergraduate research in the Aharoni Lab. My long-term goal is to understand, profile, and optimize firmware for the fully wireless Miniscope Zero project.
</div>

## From first blink to measurable firmware

This journal connects course study to small, reproducible experiments. Each entry will separate the hardware model, the implementation, and the evidence gathered on the board.

<div class="grid cards" markdown>

- **01 · Set up the workbench**

    Get the documentation running and establish a first board check.

    [Start locally →](getting-started.md)

- **02 · Study the fundamentals**

    Build understanding with eight topic templates for Microchip University study.

    [Open the course notebook →](microchip-university/index.md)

- **03 · Build one experiment at a time**

    Progress from an Arduino blink sketch toward timed acquisition and DMA.

    [Explore the projects →](projects/index.md)

- **04 · Measure before optimizing**

    Plan fair comparisons of execution time, throughput, and timing jitter.

    [Read the benchmark specification →](projects/dma-benchmark.md)

</div>

## Hardware

| Platform | Device | Core | Nominal CPU clock |
| --- | --- | --- | --- |
| Adafruit Grand Central M4 Express | ATSAMD51P20A | ARM Cortex-M4F | 120 MHz |

The board uses 3.3 V logic. Pin mapping, voltage limits, and clock assumptions belong in each experiment's setup. See the [board reference](reference/samd51.md) and its primary sources.

## Learning goals

- Understand microcontroller architecture and the ARM Cortex-M4 execution model.
- Learn embedded C and work confidently with device headers and registers.
- Configure GPIO, clocks, timers, interrupts, ADC, DMA, and serial communication.
- Develop debugging and profiling habits that produce repeatable evidence.
- Investigate ways to optimize real-time firmware for predictable behavior.

[Follow the learning roadmap](learning-roadmap.md){ .md-button .md-button--primary }
[Run the first blink](projects/led-blink.md){ .md-button }

## Current status

!!! info "Notebook scaffold — hardware experiments pending"
    The course pages are editable prompts. The LED blink sketch is supplied for board verification; hardware upload and timing measurements remain to be performed. Timer, ADC, and DMA projects are specifications. No completed lessons or measured hardware results are claimed.

Proposed connections to Miniscope Zero are research questions to investigate, not descriptions of its current firmware or hardware.

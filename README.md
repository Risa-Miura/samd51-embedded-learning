# SAMD51 Embedded Systems Learning Journal

An engineering notebook for learning embedded systems, ARM Cortex-M4 programming, and low-level firmware on the **Adafruit Grand Central M4 Express**.

**[Read the journal](https://risa-miura.github.io/samd51-embedded-learning/)** · [Start locally](docs/getting-started.md) · [Roadmap](docs/learning-roadmap.md) · [Project guides](docs/projects/index.md)

## Purpose

I am a UCLA undergraduate research assistant in the Aharoni Lab, working toward understanding, profiling, and optimizing firmware for the fully wireless Miniscope Zero project. This repository connects Microchip University study to small experiments and reproducible measurements. It is a personal notebook and an engineering portfolio in progress.

## Current status

**Notebook scaffold.** Eight editable topic templates, one Arduino blink sketch, three future-project specifications, and an optional benchmark analysis notebook are provided. Lessons have not been marked complete. Hardware upload, observations, and performance measurements remain pending. Research connections are questions to investigate, not claims about the current Miniscope Zero implementation.

See [validation and evidence](docs/validation.md) for software checks versus hardware work still needed. GitHub Actions builds and publishes the journal to GitHub Pages whenever `main` changes.

## Hardware

| Board | MCU | CPU | Nominal clock | Logic |
| --- | --- | --- | --- | --- |
| Adafruit Grand Central M4 Express | ATSAMD51P20A | ARM Cortex-M4F | 120 MHz | 3.3 V |

The device provides 1 MB Flash and 256 KB SRAM. Follow the [board and device reference](docs/reference/samd51.md) before wiring peripherals.

## Learning roadmap

1. Board bring-up and an Arduino blink baseline.
2. Architecture, embedded C, startup, and memory.
3. Clock routing, GPIO, and register access.
4. Timers, interrupts, and predictable scheduling.
5. ADC capture, buffering, and serial data export.
6. DMA, Event System, and fair performance comparisons.
7. Profiling habits and verified questions for Miniscope Zero firmware.

Track evidence in [the detailed roadmap](docs/learning-roadmap.md).

## Projects

| Project | Firmware / specification | Guide | Status |
| --- | --- | --- | --- |
| 01 · LED blink | [Source](projects/01-led-blink/src/blink/blink.ino) | [Run it](docs/projects/led-blink.md) | Example supplied; hardware test pending |
| 02 · Timer interrupt LED | [Specification](projects/02-timer-interrupt/README.md) | [Guide](docs/projects/timer-interrupt.md) | Planned |
| 03 · ADC sampling | [Specification](projects/03-adc-sampling/README.md) | [Guide](docs/projects/adc-sampling.md) | Planned |
| 04 · DMA benchmark | [Specification](projects/04-dma-benchmark/README.md) | [Guide](docs/projects/dma-benchmark.md) | Planned; analysis template supplied |

## Quick start

```sh
git clone https://github.com/Risa-Miura/samd51-embedded-learning.git
cd samd51-embedded-learning
python -m venv .venv
```

Windows PowerShell (no activation required):

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements-lock.txt
.\.venv\Scripts\python.exe -m mkdocs serve
```

macOS / Linux:

```sh
.venv/bin/python -m pip install -r requirements-lock.txt
.venv/bin/python -m mkdocs serve
```

Use Python 3.13 for the documented baseline. Open [localhost:8000](http://127.0.0.1:8000), edit `docs/`, and save to preview changes. Open the repository in VS Code, select the `.venv` interpreter, and use the supplied documentation tasks. If using the delivered local folder, skip cloning.

Before a push on Windows, run `.\.venv\Scripts\python.exe -m mkdocs build --strict --clean` followed by `.\.venv\Scripts\python.exe scripts/check_links.py` from the repository root. These explicit paths work without terminal activation. The **Docs** tasks also use the local environment directly. See [local setup](docs/getting-started.md) for other platforms and `No module named mkdocs` troubleshooting.

## Repository structure

```text
.github/workflows/       Strict build, link check, and Pages deployment
.vscode/                 Editor recommendations and documentation tasks
docs/                    Website pages, course templates, and references
  microchip-university/  Eight planned study topics
  projects/              Project reading guides
  reference/             Board, C, and primary learning resources
  assets/                Styles, logo, images, and math configuration
projects/                Firmware source and per-project specifications
benchmarks/              Optional Python notebook and empty CSV schema
templates/               Reusable lesson and project templates
scripts/                 Rendered-site internal link checker
mkdocs.yml               Navigation, theme, search, and extensions
requirements*.txt        Minimal direct dependencies and full docs lock
```

## Add the first lesson

Open `docs/microchip-university/01-microcontroller-basics.md`, change its status to **In progress**, and fill the ten prompted sections using actual study notes. Record the course module, date, references, and experiment evidence. Update the topic index when appropriate, validate, commit, and push. [Step-by-step writing guide](docs/contributing.md#add-the-first-microchip-university-lesson).

For another topic, copy [the lesson template](templates/lesson-template.md) into `docs/microchip-university/`, then add it to `mkdocs.yml` and the topic index. Keep original explanations separate from course quotations and expected behavior separate from observations.

## Run the blink example

Install **Adafruit SAMD Boards** in Arduino IDE, select **Adafruit Grand Central M4 (SAMD51)** at **120 MHz**, open `projects/01-led-blink/src/blink/blink.ino`, choose the connected port, then Verify and Upload. The builtin LED is expected to toggle every 500 ms. [Full setup, CLI commands, and troubleshooting](projects/01-led-blink/README.md).

## Optional benchmark analysis

Install `benchmarks/requirements.txt` into the selected Python environment and run:

```sh
python -m jupyter lab benchmarks/analysis-template.ipynb
```

The default input is an empty CSV schema. The notebook can import real CSVs or explicitly labelled synthetic demonstration data. It compares elapsed time, throughput, and timing jitter without treating them as CPU utilization. Jupyter is not required to write or build the documentation. See [the benchmark protocol](benchmarks/README.md).

## Learning resources

- [Primary Microchip University course](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers).
- [Microchip SAM D5x/E5x training resources](https://onlinedocs.microchip.com/oxy/GUID-62CB236E-26C5-4CE9-A182-B1D33FE1512C-en-US-3/GUID-AB68F86C-5EE6-4358-B52F-2AB31E9D9EAD.html).
- [Grand Central M4 guide](https://learn.adafruit.com/adafruit-grand-central).
- [SAMD51 product documentation](https://www.microchip.com/en-us/product/ATSAMD51P20A).
- [Full reference list](docs/reference/resources.md).

## Publishing and contributing notes

Pushes to `main` build and deploy via GitHub Actions after Pages is configured to **GitHub Actions**. Pull requests build and check links without deploying. See [publication instructions](docs/publishing.md).

Add concise, dated notes with source citations, source-code links, configuration, and raw evidence. Use [the contribution guide](docs/contributing.md) for new topics, projects, images, Mermaid diagrams, and math. Publish only cleared research material. No software license has been chosen for this personal notebook; add an appropriate license intentionally before inviting reuse.

## Adapted Microchip University workbench labs

The [complete five-lab adaptation](docs/microchip-university/labs/index.md) targets the Grand Central M4 / ATSAMD51P20A, Atmel-ICE SWD and Analog Discovery 2. Start with the [workbench setup](docs/microchip-university/labs/workbench-setup.md) and [Lab 1](docs/microchip-university/labs/lab-1-gpio-timers.md). New register-level C lives in [projects/mu-labs](projects/mu-labs/README.md), separate from the original SAME54 and existing blink projects. The new application layout begins at `0x4000` to retain the UF2 bootloader. Software-checked source is supplied; physical flashing, behavior and all measurements remain unverified.

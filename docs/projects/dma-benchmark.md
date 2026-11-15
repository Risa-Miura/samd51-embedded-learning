# 04 · Polling, interrupt, and DMA benchmark

**Status:** Planned — no firmware implementation or hardware results yet.

## Goal

Compare equivalent acquisition workloads using repeated measurements of elapsed time, effective throughput, and sample timing jitter.

## Hardware requirements

Grand Central M4, stable signal/source, USB data cable, and a timing instrument or validated on-device timebase. All modes must use the same sample count, data width, clock settings, and peripheral configuration.

## Implementation specification

First implement and verify one workload such as finite ADC capture. Then implement polling, interrupt-driven, and DMA variants. Define start/stop boundaries, warm-up policy, sample interval, overflow detection, and repeated independent runs. Keep CSV transmission outside timed regions. Account for DMA descriptor alignment, address semantics, completion, and memory/cache ownership per the SAMD51 datasheet.

## Build and flashing instructions

No executable firmware is supplied for this project yet. First complete [the blink baseline](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/01-led-blink). Choose and record Arduino versus a standalone toolchain before implementation. For Arduino, put the eventual sketch in `src/<sketch-name>/<sketch-name>.ino`, compile for `adafruit:samd:adafruit_grandcentral_m4`, then upload to the detected board port. Record exact commands, board package/compiler versions, and the source commit before changing this status.

## Expected behavior and acceptance criteria

Each mode acquires equivalent valid data with no unreported drops. Report repeat count and variation. Elapsed acquisition time may be dominated by the sample rate; measure CPU service time separately before claiming lower CPU utilization.

## Debugging

DMA failures: inspect descriptor/write-back addresses, alignment, trigger source, transfer width/increment mode, end addresses, and enable order. Inconsistent comparisons: inspect clocks, compiler optimization, logging overhead, and acquisition boundaries.

## Results and performance measurements

**Not measured.** For each run record elapsed_us, sample_count, bytes_per_sample, target_interval_us, and the population standard deviation of observed inter-sample intervals as jitter_us. Compare acquisition throughput separately from CPU occupancy. Never infer CPU utilization from elapsed_us alone.

| Run / commit | Configuration | Raw evidence | Observation |
| --- | --- | --- | --- |
| Pending | To define | None | Not run |

## Before calling this verified

- [ ] Source compiles for the selected device and exact toolchain.
- [ ] Upload and observed behavior are documented on the Grand Central M4.
- [ ] Timing boundaries, units, and instrument limits are explicit.
- [ ] Raw evidence and negative results are preserved.
- [ ] Documentation distinguishes predictions from measurements.

## Optional Python analysis

The [analysis notebook](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/benchmarks/analysis-template.ipynb) uses pandas and matplotlib. Documentation builds do not need Jupyter or these packages.

```sh
python -m pip install -r benchmarks/requirements.txt
python -m jupyter lab benchmarks/analysis-template.ipynb
```

Select the environment where the optional dependencies were installed. Run cells from the top. The default input is an empty CSV schema and produces no measurements. Set `USE_SYNTHETIC = True` only to demonstrate the plotting pipeline; all resulting plots are labelled synthetic.

Place a real CSV under `benchmarks/results/`, then change `CSV_PATH` in the notebook. See the [benchmark protocol](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/benchmarks/README.md) for field definitions and measurement boundaries. Raw result files are ignored by default; intentionally add reviewed public data with `git add -f benchmarks/results/<file>.csv`.

[Project directory and future source](https://github.com/Risa-Miura/samd51-embedded-learning/tree/main/projects/04-dma-benchmark)

# Benchmark analysis workspace

**Status:** Template only; no hardware measurements supplied.

Open `analysis-template.ipynb` after installing the optional dependencies:

```sh
python -m pip install -r benchmarks/requirements.txt
python -m jupyter lab benchmarks/analysis-template.ipynb
```

Use the selected Python environment as the notebook kernel. Docs builds do not use these packages. `data/measurements-template.csv` contains a header only. Leave `USE_SYNTHETIC = False` for actual analysis. Empty inputs are handled without creating or suggesting results. Set it to `True` solely to exercise the analysis pipeline; figures and tables explicitly identify synthetic data.

## Measurement contract

One row is one independently repeated run of the same finite acquisition workload. Keep board/core/compiler version, optimization flags, CPU/peripheral clocks, input, resolution, sample count, transfer width, buffer size, and warm-up policy constant across methods.

| CSV field | Unit / meaning |
| --- | --- |
| `method` | Exactly `polling`, `interrupt`, or `dma` |
| `run` | Unique non-empty run ID within each method |
| `elapsed_us` | Microseconds between defined acquisition start and completion, > 0 |
| `sample_count` | Integer number of acquired samples, at least 2 |
| `bytes_per_sample` | Integer storage/transfer bytes per sample, > 0 |
| `target_interval_us` | Intended sample interval in microseconds, > 0 |
| `jitter_us` | Population standard deviation of observed inter-sample intervals within this run, >= 0 |

Store raw per-sample timestamps alongside the CSV. For timestamps `t_us`, compute intervals as `numpy.diff(t_us)` and jitter as `intervals.std(ddof=0)`. This definition measures interval variation, not sample time error relative to an ideal schedule. Use enough intervals for a meaningful estimate; two samples alone provide no useful variability estimate.

`elapsed_us` includes acquisition and the selected completion boundary, but excludes CSV/USB export and Python analysis. Define whether initial sample latency is included and keep that choice constant. Record the clock/instrument resolution and instrumentation overhead. Throughput is the effective acquired samples or bytes divided by this elapsed time; it is not necessarily ADC maximum throughput or CPU occupancy.

Use a separate CPU busy-time or cycle measurement before making CPU utilization claims. An acquisition running at a fixed sample rate can have the same elapsed time across all methods despite different CPU costs. Verify every buffer for correctness, overrun, and dropped samples before comparing speed.

## Run record

Copy this record for real data: date; source commit; board revision; core/compiler/tool versions; optimization; clock tree; ADC reference/resolution/sample time; transfer width/count; trigger/interrupt priorities; buffer layout/ownership; start/stop boundary; warm-up/repeat policy; instrument and resolution; raw timestamp file; overflow/missing-data checks; observed limitations.

Put CSVs in `results/`, set `CSV_PATH`, and run all cells. Unknown methods, missing/duplicate run IDs, invalid values, and unequal workload settings are rejected to prevent misleading plots. The summary reports mean elapsed time, between-run standard deviation, mean effective throughput, and mean within-run interval jitter. The error bars show between-run elapsed-time standard deviation, not confidence intervals or within-run jitter.

Raw results and generated figures under `results/` are ignored by default. To publish reviewed public results, add chosen files explicitly with `git add -f benchmarks/results/<file>.csv` and document provenance. Clear notebook outputs before committing so synthetic demonstrations never masquerade as project results.

[DMA project specification](../projects/04-dma-benchmark/README.md)

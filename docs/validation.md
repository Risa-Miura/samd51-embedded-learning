# Validation and project status

## Evidence policy

Software validation confirms that the notebook builds and its examples are structurally sound. Hardware verification requires an actual upload and observations on the Grand Central M4. Keep those two statuses separate.

No Microchip University lessons have been marked complete. Timer, ADC, DMA, and Event System notes are editable templates. The later firmware projects are specifications, and the benchmark CSV is empty by default. Synthetic notebook data is opt-in and explicitly labelled.

## Software validation

The repository is checked using:

```sh
python -m mkdocs build --strict --clean
python scripts/check_links.py
```

The first command validates the MkDocs configuration and navigation and fails on warnings. The second checks rendered internal links, assets, and fragment targets. External references are not part of that offline check.

The optional notebook supports both an empty real-data input and a labelled synthetic demonstration. Its outputs are cleared in the committed template. The blink sketch targets `adafruit:samd:adafruit_grandcentral_m4` and rejects other target macros at compile time.

See the GitHub Actions run for the exact deployment commit and hosted build outcome. The delivered validation report records checks performed during initial setup.

## Hardware verification still required

- [ ] Compile in the local Arduino toolchain and record its version.
- [ ] Upload blink to the physical Grand Central M4 and record the selected port.
- [ ] Observe the ordinary builtin LED and record actual behavior.
- [ ] Measure its period if making timing claims.
- [ ] Implement and verify timer, ADC, and DMA projects before changing their status.
- [ ] Collect real benchmark data with a defined protocol and raw evidence.

No device was physically tested while preparing this notebook. No hardware measurements are included.

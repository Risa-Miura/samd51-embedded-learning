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

## Initial software checks — November 14, 2026

- MkDocs 1.6.1 / Material 9.7.0: strict clean build passed locally on Python 3.14.6. The deployment workflow uses Python 3.13.
- Rendered site: 24 HTML pages and 1,469 internal links/assets/fragments passed the offline checker.
- Workflow structure: main/PR triggers, build dependency, and Pages deployment permissions checked. See the GitHub Actions run for the exact deployed commit and hosted outcome.
- Notebook: schema valid; empty CSV, labelled synthetic plotting, CSV import, and invalid elapsed-time rejection checked. Committed outputs remain empty.
- Blink: compiled successfully using Arduino CLI 1.6.0, Adafruit SAMD Boards 1.7.17, and ARM GCC 9.2.1 for `adafruit:samd:adafruit_grandcentral_m4`. The compiler reported 12,364 bytes of program storage. This is a build artifact size, not a hardware performance measurement.

The local Windows ARM compiler initially could not find a deeply nested C++ header. Copying the compiler to a shorter workspace path and overriding `compiler.path` resolved the legacy path-length issue. No firmware source change was needed. If this occurs locally, install the toolchain in a shorter path or use the standard Arduino IDE installation location.

Interactive browser inspection was unavailable because browser control could not verify the Windows browser URL. Navigation and anchors were checked in generated HTML; responsive layout, search interaction, copy buttons, Mermaid/math rendering, and theme switching still benefit from a manual browser check.

## Hardware verification still required

- [ ] Upload blink to the physical Grand Central M4 and record the selected port.
- [ ] Observe the ordinary builtin LED and record actual behavior.
- [ ] Measure its period if making timing claims.
- [ ] Implement and verify timer, ADC, and DMA projects before changing their status.
- [ ] Collect real benchmark data with a defined protocol and raw evidence.

No device was physically tested while preparing this notebook. No hardware measurements are included.

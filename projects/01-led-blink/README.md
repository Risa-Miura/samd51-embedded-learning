# 01 · LED blink

**Status:** Runs on the board (LED seen blinking). Period not measured yet.

Blinks the red LED labeled `L` on the Grand Central M4 (Arduino pin D13, SAMD51 pin PB01). Two versions:

- **Arduino calls:** [`src/blink/blink.ino`](src/blink/blink.ino) uses `pinMode` and `digitalWrite`.
- **Direct register writes:** sets `PORT->Group[1].DIRSET` once, then toggles with `PORT->Group[1].OUTTGL`. The code and explanation are in [lesson 01](../../lessons/01-microcontroller-basics.md#controlling-the-registers-directly).

## Flash with Arduino IDE

1. Add `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` under **File → Preferences → Additional boards manager URLs**.
2. In **Boards Manager**, install **Adafruit SAMD Boards** (not Arduino SAMD Boards).
3. Select **Adafruit Grand Central M4 (SAMD51)**, keep **CPU Speed: 120 MHz**, and choose the board's port under **Tools → Port**.
4. Open `src/blink/blink.ino` (Arduino needs the folder and sketch to share the name `blink`), then **Verify** and **Upload**.

## Flash with Arduino CLI

From the repository root, after installing [Arduino CLI](https://arduino.github.io/arduino-cli/latest/installation/):

```sh
arduino-cli core update-index --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
arduino-cli core install adafruit:samd@1.7.17 --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
arduino-cli compile --fqbn adafruit:samd:adafruit_grandcentral_m4 projects/01-led-blink/src/blink
arduino-cli board list
arduino-cli upload --port COM7 --fqbn adafruit:samd:adafruit_grandcentral_m4 projects/01-led-blink/src/blink
```

Replace `COM7` with the port `board list` reports (`/dev/...` on macOS/Linux).

## Expected behavior

The LED is on for about 500 ms, then off for about 500 ms. That is the delay the code requests, not a measured period. `delay()` blocks the sketch, but core interrupts keep running.

## Results

| Date | Version | Observation | Measured period |
| --- | --- | --- | --- |
| Nov 2026 | Arduino calls | LED `L` blinked | Not measured |
| Nov 2026 | Register writes (`OUTTGL`) | LED `L` blinked | Not measured |

**Next (planned):** measure the period with an oscilloscope and step through the code with the Atmel-ICE.

## Troubleshooting

| Symptom | Check |
| --- | --- |
| Board not listed | Use a data cable (not charge-only) and check the selected board package. |
| Upload port disappeared | Double-tap reset to enter the UF2 bootloader, select the new port, and retry. |
| `#error "Select Adafruit Grand Central M4"` | The sketch rejects other boards at compile time. Pick Grand Central M4 under Adafruit SAMD Boards. |

## References

- [Adafruit Grand Central M4: Arduino IDE setup](https://learn.adafruit.com/adafruit-grand-central/using-with-arduino-ide)
- [Grand Central variant pin definitions](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.h)

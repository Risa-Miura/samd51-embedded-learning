# 01 · LED blink

**Status:** Example supplied; hardware verification pending.

[Source](src/blink/blink.ino) · [Website guide](../../docs/projects/led-blink.md)

## Hardware requirements

- Adafruit Grand Central M4 Express and a USB data cable.
- A computer with Arduino IDE 2.x or Arduino CLI.
- The onboard `LED_BUILTIN`; no external wiring is needed. This is the ordinary LED, not the NeoPixel.

## Build and flash with Arduino IDE

1. Add `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` under **File → Preferences → Additional boards manager URLs**.
2. In **Boards Manager**, install **Adafruit SAMD Boards** (not Arduino SAMD Boards). Record the installed version; the CLI baseline below uses 1.7.17.
3. Select **Adafruit Grand Central M4 (SAMD51)**. Keep **CPU Speed: 120 MHz** and other board options at their defaults.
4. Open `projects/01-led-blink/src/blink/blink.ino`. Arduino requires the sketch file and containing folder to share the name `blink`.
5. Connect the board, select its port under **Tools → Port**, and click **Verify**.
6. Click **Upload**. After upload, observe the onboard LED and record the actual behavior.

## Build and flash with Arduino CLI

Run from the repository root after installing [Arduino CLI](https://arduino.github.io/arduino-cli/latest/installation/):

```sh
arduino-cli core update-index --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
arduino-cli core install adafruit:samd@1.7.17 --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
arduino-cli compile --fqbn adafruit:samd:adafruit_grandcentral_m4 projects/01-led-blink/src/blink
arduino-cli board list
```

Replace `COM7` with the port returned for the connected board (Linux/macOS use the returned `/dev/...` path):

```sh
arduino-cli upload --port COM7 --fqbn adafruit:samd:adafruit_grandcentral_m4 projects/01-led-blink/src/blink
```

The sketch checks the target macros at compile time. The board package maps `LED_BUILTIN` and configures clocks/startup; this is Arduino C++, not a bare-metal example.

## Expected behavior

The ordinary onboard LED should be high for about 500 ms and low for about 500 ms: approximately one full on/off cycle per second. `delay()` blocks foreground sketch execution, while core interrupts may still run. The requested delays are not a measured timing guarantee.

## Debugging

| Symptom | Check |
| --- | --- |
| Board not listed | Use a data cable, check USB connection, then check the selected board package. |
| Upload port disappeared | Double-tap reset to enter the UF2 bootloader, select the new bootloader port, and retry upload. |
| Target compile error | Choose Grand Central M4 under Adafruit SAMD Boards, not Mega/AVR or another SAMD board. |
| NeoPixel stays off | This sketch drives the ordinary builtin LED. It does not use a NeoPixel library. |
| Timing differs | Record CPU speed, core version, actual waveform, and the measurement method. |

## Results and performance measurements

**Hardware status: not run.** Record date, board/core/compiler versions, source commit, verify/upload output, observed LED behavior, and any measured period here. Leave timing measurements blank until observed on hardware.

| Run | Observation | Measured period | Instrument / raw file |
| --- | --- | --- | --- |
| Pending | Not recorded | Not measured | Not recorded |

## MPLAB bare-metal example

A separate [SAMD51 blink project](samd51-blink/README.md) now lives in `samd51-blink/`. Open that folder in VS Code using the MPLAB profile. It targets ATSAMD51P20A with XC32 v2.50 and builds independently of the original SAME54 `Lab1.X` project. Its ordinary onboard LED is PB01/D13.

The MPLAB image uses the default address-zero reset layout. Read its flashing prerequisites before programming: it requires an appropriate SWD probe and can overwrite the USB bootloader. The Arduino baseline above remains the USB-upload example. Hardware verification of the new MPLAB example is pending.

## Later bootloader-aware implementation

Retain this Arduino baseline. A future MPLAB version should implement and verify a bootloader-aware memory layout before using USB upload. Do not replace the bootloader as part of the Arduino exercise.

## References

- [Adafruit Arduino guide](https://learn.adafruit.com/adafruit-grand-central/using-with-arduino-ide)
- [Board definitions](https://github.com/adafruit/ArduinoCore-samd/blob/master/boards.txt)
- [Grand Central variant](https://github.com/adafruit/ArduinoCore-samd/blob/master/variants/grand_central_m4/variant.h)

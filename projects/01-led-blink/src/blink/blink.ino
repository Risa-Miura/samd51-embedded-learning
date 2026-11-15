// Grand Central M4 Express / ATSAMD51P20A.
// Example for first board verification; physical behavior must be checked on hardware.
#include <Arduino.h>

#if !defined(ARDUINO_GRAND_CENTRAL_M4) || !defined(__SAMD51P20A__)
#error "Select Adafruit Grand Central M4 (SAMD51) before compiling."
#endif

constexpr unsigned long kHalfPeriodMs = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(kHalfPeriodMs);
  digitalWrite(LED_BUILTIN, LOW);
  delay(kHalfPeriodMs);
}

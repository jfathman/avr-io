## avr-io

Bare metal software to support interactive and script
driven microcontroller I/O, using technologies selected
for long life with minimal churn.

Provides:

  * Core library
  * Interactive monitor
  * Lua API to access monitor
  * Lua scripts to operate monitor

Supports:

  * ATmega328P (Arduino UNO R3)
  * ATmega2560 (Arduino Mega 2560)
  * ATMega4809 (Arduino Nano Every)

Design goals:

  * Simple
  * Robust
  * Self-contained
  * Minimal dependencies
  * PROGMEM to conserve sram
  * Develop on Alpine Linux
  * Command line tools (avr-gcc, make, avrdude)

Anti-goals:

  * Arduino API compatible
  * Backward compatible
  * Windows or Mac host
  * IDE support

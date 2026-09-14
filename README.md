## avr-io

Bare metal software to support interactive and script
driven microcontroller I/O, using technologies selected
for long life with minimal churn.

**Supports:**

| MCU        | Board              | SRAM | Flash  |
| ---------- | ------------------ | ---: | -----: |
| ATmega328P | Arduino Uno R3     | 2 KB | 32 KB  |
| ATmega2560 | Arduino Mega 2560  | 8 KB | 256 KB |
| ATmega4809 | Arduino Nano Every | 6 KB | 48 KB  |

**Provides:**

  * Core library
  * Interactive monitor
  * Lua API to access monitor
  * Lua scripts to operate monitor

**Design goals:**

  * Simple
  * Robust
  * Self-contained
  * Minimal dependencies
  * PROGMEM to conserve sram
  * Develop on Alpine Linux
  * Command line tools (avr-gcc, make, avrdude)

**Anti-goals:**

  * Arduino API compatible
  * Backward compatible
  * Windows or Mac host
  * IDE support

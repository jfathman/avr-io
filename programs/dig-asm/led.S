; led.S

#define __SFR_OFFSET 0

#include "avr/io.h"

.global led_init
.global led_on
.global led_off

#if defined(__AVR_ATmega328P__)

#define LED 5 // PB5 = Port B Bit 5 (Pin 13)

led_init:
    sbi DDRB, LED
    ret

led_on:
    sbi PORTB, LED
    ret

led_off:
    cbi PORTB, LED
    ret

#endif

#if defined(__AVR_ATmega2560__)

#define LED 7 // PB7 = Port B Bit 7 (Pin 13)

led_init:
    sbi DDRB, LED
    ret

led_on:
    sbi PORTB, LED
    ret

led_off:
    cbi PORTB, LED
    ret

#endif

#if defined(__AVR_ATmega4809__)

#define LED 2 // PE2 = Port E Bit 2 (Pin 13)

led_init:
    sbi _SFR_IO_ADDR(VPORTE_DIR), LED
    ret

led_on:
    sbi _SFR_IO_ADDR(VPORTE_OUT), LED
    ret

led_off:
    cbi _SFR_IO_ADDR(VPORTE_OUT), LED
    ret

#endif

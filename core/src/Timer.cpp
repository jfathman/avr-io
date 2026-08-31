// Timer.cpp

#include <avr/interrupt.h>
#include <avr/io.h>
#include "Timer.h"

static volatile unsigned long msec_counter = 0;

#if defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega2560__)
ISR(TIMER0_COMPA_vect)
{
    msec_counter++;
}
#endif

#if defined(__AVR_ATmega4809__)
ISR(TCB3_INT_vect) {
    msec_counter++;
    TCB3.INTFLAGS = TCB_CAPT_bm;
}
#endif

void Timer::init()
{
    cli();

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TCCR0A = _BV(WGM01); // CTC mode

        TCCR0B = _BV(CS01) | _BV(CS00); // Prescaler 64

        // 1 msec compare value: (16 MHz / (64 * 1000 Hz)) - 1 = 249
        OCR0A = 249;

        TIMSK0 = _BV(OCIE0A); // Compare match interrupt.
    #endif

    #if defined(__AVR_ATmega4809__)
        // Set compare count for 1 msec interval.
        // 16 MHz / 2 * 0.001 seconds = 8000
        TCB3.CCMP = 8000;

        // Enable periodic interrupt.
        TCB3.INTCTRL = TCB_CAPT_bm;

        // Configure prescaler DIV2 and enable timer.
        TCB3.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm;
    #endif

    sei();
}

uint32_t Timer::millis()
{
    uint8_t entrySREG = SREG;

    cli();

    uint32_t value = msec_counter;

    SREG = entrySREG;

    return value;
}

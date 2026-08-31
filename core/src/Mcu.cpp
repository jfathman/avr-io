// Mcu.cpp

#include <avr/wdt.h>
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Mcu.h"

void Mcu::init()
{
    #if defined(__AVR_ATmega4809__)
        // Use internal oscillator.
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc);
        
        // Disable clock prescaler so MCU runs at 16 MHz.
        _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);
    #endif
}

void Mcu::reset()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        wdt_enable(WDTO_15MS);
        for (;;)
            ;
    #endif

    #if defined(__AVR_ATmega4809__)
        _delay_ms(15);
        _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
    #endif
}

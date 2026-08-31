// Watchdog.cpp

#include <avr/wdt.h>
#include <util/delay.h>
#include "Watchdog.h"

// Run disable_wdt() at mcu startup before main() is called.
void disable_wdt(void) __attribute__((naked)) __attribute__((section(".init3")));

void disable_wdt(void)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        // Clear WDRF in MCUSR -before- disabling watchdog.
        // MCUSR = MCU Status Register
        // WDRF  = Watchdog Reset Flag

        MCUSR &= ~_BV(WDRF);
    #endif

    wdt_disable();
}

void Watchdog::reset()
{
    wdt_enable(WDTO_15MS);

    while (true)
        ;
}

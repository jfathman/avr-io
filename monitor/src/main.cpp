// main.cpp

#include <avr/wdt.h>
#include <delay.h>
#include <Digital.h>
#include <Mcu.h>
#include <memory.h>
#include "commands.h"
#include "globals.h"

void wdtInit();
void blink();

int main(void)
{
    mcu.init();

    wdtInit();

    sramInit();

    stackReserve(384);

    timer.init();

    serial.init(115200);

    blink();

    process_commands();
}

void wdtInit()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        wdt_enable(WDTO_4S);
    #endif

    #if defined(__AVR_ATmega4809__)
        wdt_enable(WDT_PERIOD_4KCLK_gc);
    #endif
}

void blink()
{
    Digital digital;

    digital.mode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 3; i++)
    {
        digital.on(LED_BUILTIN);
        delay(50);
        digital.off(LED_BUILTIN);
        delay(50);
    }
}

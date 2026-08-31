// getchar.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <delay.h>
#include <Digital.h>
#include <Mcu.h>
#include <Serial.h>
#include <Timer.h>

static Digital digital;
static Mcu     mcu;
static Serial  serial;
static Timer   timer;

int main(void)
{
    mcu.init();
    timer.init();
    serial.init(115200);
    digital.mode(LED_BUILTIN, OUTPUT);

    digital.off(LED_BUILTIN);

    printf_P(PSTR("enter keyboard characters:\n"));

    while (true)
    {
        char c;

        while ((c = getchar()) == EOF)
            ;

        digital.on(LED_BUILTIN);

        putchar(c);

        if (c == 0x0D)
        {
            putchar(0x0A);
        }
            
        digital.off(LED_BUILTIN);
    }

    return 0;
}

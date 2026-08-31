// dig.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <delay.h>
#include <Digital.h>
#include <Mcu.h>
#include <Serial.h>

int main(void)
{
    Digital digital;
    Mcu     mcu;
    Serial  serial;

    mcu.init();
    serial.init(115200);
    digital.mode(LED_BUILTIN, OUTPUT);

    uint32_t counter = 0;

    while (true)
    {
        digital.on(LED_BUILTIN);
        printf_P(PSTR("[%lu] LED On\n"), counter);
        delay(1000);

        digital.off(LED_BUILTIN);
        printf_P(PSTR("[%lu] LED Off\n"), counter);
        delay(1000);

        counter++;
    }

    return 0;
}

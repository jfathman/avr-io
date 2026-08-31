// dig-asm.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <delay.h>
#include <Mcu.h>
#include <Serial.h>
#include <Timer.h>

#include "led.h"

int main(void)
{
    Mcu    mcu;
    Serial serial;
//    Timer  timer;

    mcu.init();
//    timer.init();
    serial.init(115200);

    led_init();

    uint32_t counter = 0;

    while (true)
    {
        led_on();
        printf_P(PSTR("[%lu] LED On\n"), counter);
        delay(500);

        led_off();
        printf_P(PSTR("[%lu] LED Off\n"), counter);
        delay(500);

        counter++;
    }

    return 0;
}

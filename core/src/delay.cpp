// delay.cpp

#include <util/delay.h>
#include "delay.h"

void delay(uint16_t msec)
{
    while (msec--)
    {
        _delay_ms(1);
    }
}

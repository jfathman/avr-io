// adc.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <Analog.h>
#include <delay.h>
#include <Digital.h>
#include <Mcu.h>
#include <Serial.h>
#include <Interval.h>
#include <Timer.h>

static Analog  analog;
static Digital digital;
static Mcu     mcu;
static Serial  serial;
static Timer   timer;

static Interval interval(timer);

int main(void)
{
    mcu.init();
    timer.init();
    serial.init(115200);
    analog.init(VREF_VCC);
    digital.mode(LED_BUILTIN, OUTPUT);
    digital.on(LED_BUILTIN);

    interval.start(PERIODIC, 500);

    uint32_t counter = 0;

    while (true)
    {
        if (interval.isExpired())
        {
            digital.toggle(LED_BUILTIN);
        }

        uint16_t value = analog.read(A0);

        double voltage = (value / 1024.0) * 5.0;

        printf_P(PSTR("[%04lu] A0 = %.2f v\n"), counter, voltage);

        delay(100);

        counter++;
    }

    return 0;
}

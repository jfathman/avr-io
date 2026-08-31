// pwm.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <delay.h>
#include <Digital.h>
#include <Interval.h>
#include <Mcu.h>
#include <Serial.h>
#include <Timer.h>

static Digital digital;
static Mcu     mcu;
static Serial  serial;
static Timer   timer;

static Interval interval(timer);

void pwmUpdate(uint32_t counter, uint8_t pin, uint16_t dutyCycle);

int main(void)
{
    mcu.init();
    timer.init();
    serial.init(115200);
    digital.mode(LED_BUILTIN, OUTPUT);
    digital.on(LED_BUILTIN);

    interval.start(PERIODIC, 500);

    uint8_t pin = 3;

    uint32_t counter = 0;

    while (true)
    {
        // ramp up duty cycle from 0 to 255 on pwm pin

        for (int16_t dutyCycle = 0; dutyCycle <= 255; dutyCycle += 4)
        {
            if (interval.isExpired())
            {
                digital.toggle(LED_BUILTIN);
            }

            pwmUpdate(counter, pin, dutyCycle);

            delay(10);
        }

        // ramp down duty cycle from 255 to 0 on pwm pin

        for (int16_t dutyCycle = 255; dutyCycle >= 0; dutyCycle -= 4)
        {
            if (interval.isExpired())
            {
                digital.toggle(LED_BUILTIN);
            }

            pwmUpdate(counter, pin, dutyCycle);

            delay(10);
        }

        counter++;
    }

    return 0;
}

void pwmUpdate(uint32_t counter, uint8_t pin, uint16_t dutyCycle)
{
    bool ledState = digital.read(LED_BUILTIN);

    digital.pwmWrite(pin, dutyCycle);

    printf_P(PSTR( "[%.4lu] LED %s D%u: %3u\n"), counter, ledState ? "On " : "Off", pin, dutyCycle);
}

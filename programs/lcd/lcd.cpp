// lcd.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <delay.h>
#include <Digital.h>
#include <Lcd.h>
#include <Mcu.h>
#include <Serial.h>

int main(void)
{
    Digital digital;
    Lcd     lcd;
    Mcu     mcu;
    Serial  serial;

    mcu.init();
    serial.init(115200);
    digital.mode(LED_BUILTIN, OUTPUT);

    uint32_t counter = 0;

    while (true)
    {
        digital.toggle(LED_BUILTIN);
        delay(1000);
        counter++;

        // lcd init

        if (!lcd.init())
        {
            printf_P(PSTR("[%lu] Error: lcd init\n"), counter);
            continue;
        }

        printf_P(PSTR("[%lu] lcd init\n"), counter);

        // lcd clear

        if (!lcd.clear())
        {
            printf_P(PSTR("[%lu] Error: lcd clear\n"), counter);
            continue;
        }

        printf_P(PSTR("[%lu] lcd clear\n"), counter);

        // lcd displayOn

        if (!lcd.displayOn())
        {
            printf_P(PSTR("[%lu] Error: lcd displayOn\n"), counter);
            continue;
        }

        printf_P(PSTR("[%lu] lcd displayOn\n"), counter);

        // lcd backlightOn

        if (!lcd.backlightOn())
        {
            printf_P(PSTR("[%lu] Error: lcd backlightOn\n"), counter);
            continue;
        }

        printf_P(PSTR("[%lu] lcd backlightOn\n"), counter);
            
        while (true)
        {
            char buffer[LCD_COLS] = { 0 };

            digital.toggle(LED_BUILTIN);
            counter++;

            // lcd moveCursor to 1st row

            if (!lcd.moveCursor(0))
            {
                printf_P(PSTR("[%lu] Error: lcd moveCursor\n"), counter);
                break;
            }

            // lcd write (to 1st row)

            snprintf_P(buffer, sizeof(buffer), PSTR("[%lu] Row 1"), counter);

            if (!lcd.write(buffer))
            {
                printf_P(PSTR("[%lu] Error: lcd write\n"), counter);
                break;
            }

            printf_P(PSTR("%s\n"), buffer);

            // lcd moveCursor to end of 1st row

            if (!lcd.moveCursor(0, 15))
            {
                printf_P(PSTR("[%lu] Error: lcd moveCursor\n"), counter);
                break;
            }

            // lcd write (to end of 1st row)

            if (!lcd.write(digital.read(LED_BUILTIN, ACTUAL) ? "*" : "_"))
            {
                printf_P(PSTR("[%lu] Error: lcd write\n"), counter);
                break;
            }

            // lcd moveCursor to 2nd row

            if (!lcd.moveCursor(1))
            {
                printf_P(PSTR("[%lu] Error: lcd moveCursor\n"), counter);
                break;
            }

            // lcd write (to 2nd row)

            snprintf_P(buffer, sizeof(buffer), PSTR("[%lu] Row 2"), counter);

            if (!lcd.write(buffer))
            {
                printf_P(PSTR("[%lu] Error: lcd write\n"), counter);
                break;
            }

            printf_P(PSTR("%s\n"), buffer);

            delay(1000);
        }
    }

    return 0;
}

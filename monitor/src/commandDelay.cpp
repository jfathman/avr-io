// commandDelay.cpp

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "commands.h"
#include "globals.h"
#include "utils.h"

void cmd_delay(const char *args, int argc)
{
    // delay 0-10000 (msec)

    if (argc == 1 && isdigits(args))
    {
        unsigned int msec = atoi(args);

        if (msec <= 10000)
        {
            while (msec--)
            {
                if (serial.available())
                {
                    while (serial.available())
                    {
                        getchar();
                    }
                    printf_P(PSTR("delay aborted\n"));
                    return;
                }
                _delay_ms(1);
            }

            printf_P(PSTR("ok\n"));
            return;
        }
    }
        
    printf_P(PSTR("invalid args\n"));
}

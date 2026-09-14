// commandLcd.cpp

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <Lcd.h>
#include "commands.h"
#include "globals.h"
#include "utils.h"

void cmd_lcd(const char *args, int argc)
{
    (void) argc;

    char arg1[10] = {0};
    char arg2[4]  = {0};
    char arg3[4]  = {0};

    // lcd - init
    // lcd - display on|off
    // lcd - backlight on|off
    // lcd - clear <1|2>
    // lcd - cursor 1|2 <1-16>
    // lcd - write <text>

    const uint8_t count = sscanf_P(args, PSTR("%9s %3s %3s"), arg1, arg2, arg3);

    static bool init = false;

    if (count == 1 && match_P(arg1, PSTR("init")))
    {
        // explicit init

        if (!lcd.init())
        {
            printf_P(PSTR("device error\n"));
            return;
        }

        init = true;
        printf_P(PSTR("ok\n"));
        return;
    }

    if (!init)
    {
        // auto init

        if (!lcd.init())
        {
            printf_P(PSTR("device error\n"));
            return;
        }

        init = true;
    }

    if (count == 2 && match_P(arg1, PSTR("display")))
    {
        if (match_P(arg2, PSTR("on")))
        {
            if (!lcd.displayOn())
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        if (match_P(arg2, PSTR("off")))
        {
            if (!lcd.displayOff())
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        printf_P(PSTR("invalid args\n"));

        return;
    }

    if (count == 2 && match_P(arg1, PSTR("backlight")))
    {
        if (match_P(arg2, PSTR("on")))
        {
            if (!lcd.backlightOn())
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        if (match_P(arg2, PSTR("off")))
        {
            if (!lcd.backlightOff())
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        printf_P(PSTR("invalid args\n"));

        return;
    }

    if (count == 1 && match_P(arg1, PSTR("clear")))
    {
        if (!lcd.clear())
        {
            printf_P(PSTR("device error\n"));
            return;
        }

        printf_P(PSTR("ok\n"));
        return;
    }

    if (count == 2 && match_P(arg1, PSTR("clear")))
    {
        uint8_t row = atoi(arg2);

        if (row == 1 || row == 2)
        {
            if (!lcd.clear(row - 1))
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        printf_P(PSTR("invalid args\n"));
        return;
    }

    if (count == 2 && match_P(arg1, PSTR("cursor")))
    {
        uint8_t row = atoi(arg2);

        if (row == 1 || row == 2)
        {
            if (!lcd.moveCursor(row -1, 0))
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        printf_P(PSTR("invalid args\n"));
    }

    if (count == 3 && match_P(arg1, PSTR("cursor")))
    {
        uint8_t row = atoi(arg2);
        uint8_t col = atoi(arg3);

        if ((row == 1 || row == 2) && (col >= 1 && col <= 16))
        {
            if (!lcd.moveCursor(row -1, col -1))
            {
                printf_P(PSTR("device error\n"));
                return;
            }

            printf_P(PSTR("ok\n"));
            return;
        }

        printf_P(PSTR("invalid args\n"));
    }

    if (count >= 2 && match_P(arg1, PSTR("write")))
    {
        const char *s = args + strlen(arg1);

        // skip whitespace
        while (*s != 0 && isspace(*s))
        {
            s++;
        }

        if (!lcd.write(s))
        {
            printf_P(PSTR("device error\n"));
            return;
        }

        printf_P(PSTR("ok\n"));
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

// commandPwm.cpp

#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <Digital.h>
#include "commands.h"
#include "utils.h"

void cmd_pwm(const char *args, int argc)
{
    (void) argc;

    char arg1[8] = {0};
    char arg2[8] = {0};

    // pwm <pin> 0-255
    // pwm <pin> read

    if (sscanf_P(args, PSTR("%7s %7s"), arg1, arg2) != 2)
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    Digital digital;

    unsigned int pin = 0;
    unsigned int dutyCycle = 0;

    if (isdigits(arg1) && match_P(arg2, PSTR("read")))
    {
        pin = atoi(arg1);
        dutyCycle = digital.pwmRead(pin);

        if (coreError == ERR_NONE)
        {
            printf_P(PSTR("%u\n"), dutyCycle);
        }
        else
        {
            log_error();
        }

        return;
    }

    if (!isdigits(arg1) || !isdigits(arg2))
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    pin = atoi(arg1);
    dutyCycle = atoi(arg2);

    if (!isPwmPin(pin))
    {
        printf_P(PSTR("invalid pin\n"));
        return;
    }

    if (dutyCycle > 255)
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    digital.pwmWrite(pin, dutyCycle);
        
    log_result();
}

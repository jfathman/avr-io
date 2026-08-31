// commandDig.cpp

#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <Digital.h>
#include "commands.h"
#include "utils.h"

void cmd_dig(const char *args, int argc)
{
    (void) argc;

    char arg1[8] = {0};
    char arg2[8] = {0};

    // dig <pin> in|inpull|out
    // dig <pin> on|off|set|clear|1|0
    // dig <pin> toggle
    // dig <pin> rda|rdc

    if (sscanf_P(args, PSTR("%7s %7s"), arg1, arg2) != 2)
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    if (!isdigits(arg1))
    {
        printf_P(PSTR("invalid args\n"));
        return;
    }

    unsigned int pin = atoi(arg1);

    if (!isDigPin(pin))
    {
        printf_P(PSTR("invalid pin\n"));
        return;
    }

    Digital digital;

    // dig <pin> in
    if (match_P(arg2, PSTR("in")))
    {
        digital.mode(pin, INPUT);
        log_result();
        return;
    }

    // dig <pin> inpull
    if (match_P(arg2, PSTR("inpull")))
    {
        digital.mode(pin, INPUT_PULLUP);
        log_result();
        return;
    }

    // dig <pin> out
    if (match_P(arg2, PSTR("out")))
    {
        digital.mode(pin, OUTPUT);
        log_result();
        return;
    }

    // dig <pin> on|set|1

    if (match_P(arg2, PSTR("on"))  || 
        match_P(arg2, PSTR("set")) || 
        match_P(arg2, PSTR("1")))
    {
        digital.on(pin);
        log_result();
        return;
    }

    // dig <pin> off|clear|0
    if (match_P(arg2, PSTR("off"))   ||
        match_P(arg2, PSTR("clear")) ||
        match_P(arg2, PSTR("0")))
    {
        digital.off(pin);
        log_result();
        return;
    }

    // dig <pin> toggle
    if (match_P(arg2, PSTR("toggle")))
    {
        digital.toggle(pin);
        if (coreError != ERR_NONE)
        {
            log_error();
            return;
        }

        uint8_t pin_state = digital.read(pin, ACTUAL);
        if (coreError != ERR_NONE)
        {
            log_error();
            return;
        }

        printf_P(PSTR("%S\n"), pin_state ? PSTR("on") : PSTR("off"));
        return;
    }

    // dig <pin> rda
    if (match_P(arg2, PSTR("rda")))
    {
        uint8_t value = digital.read(pin, ACTUAL);
        log_result(value);
        return;
    }

    // dig <pin> rdc
    if (match_P(arg2, PSTR("rdc")))
    {
        uint8_t value = digital.read(pin, COMMANDED);
        log_result(value);
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

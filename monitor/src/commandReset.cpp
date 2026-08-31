// commandReset.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "globals.h"

void cmd_reset(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // reset - restart mcu

    printf_P(PSTR("ok\n"));

    mcu.reset();
}

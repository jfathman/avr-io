// commandUptime.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "globals.h"

void cmd_uptime(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // uptime - since startup

    unsigned long msecs = timer.millis();

    unsigned long seconds = msecs   / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours   = minutes / 60;
    unsigned long days    = hours   / 24;
    
    msecs   %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours   %= 24;
    
    printf_P(PSTR("DDDD:HH:MM:SS.sss\n"));
    printf_P(PSTR("%04lu:%02lu:%02lu:%02lu.%03lu\n"),
        days, hours, minutes, seconds, msecs);
}

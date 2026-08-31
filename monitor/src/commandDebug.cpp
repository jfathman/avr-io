// commandDebug.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "globals.h"
#include "utils.h"

static void print_state();

void cmd_debug(const char *args, int argc)
{
    // debug on|off

    if (argc == 0)
    {
        print_state();
        return;
    }

    if (argc == 1)
    {
        // debug on
        if (match_P(args, PSTR("on")))
        {
            debug.enable();
            print_state();
            return;
        }

        // debug off
        if (match_P(args, PSTR("off")))
        {
            debug.disable();
            print_state();
            return;
        }
    }
    
    printf_P(PSTR("invalid args\n"));
}

static void print_state()
{
    printf_P(PSTR("debug is %S\n"), debug.isEnabled() ? PSTR("on") : PSTR("off"));
}


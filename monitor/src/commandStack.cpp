// commandStack.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

#define PRINT_VAL(text,value) \
    printf_P(PSTR("%.4u %S\n"), value, PSTR(text));

void cmd_stack(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // stack - stack usage

    PRINT_VAL("stack size",   stackSize());
    PRINT_VAL("stack used",   stackUsed());
    PRINT_VAL("stack unused", stackUnused());
}

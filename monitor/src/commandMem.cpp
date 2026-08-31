// commandMem.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

#define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.4x %.4u %S\n"), value, value, PSTR(text));

void cmd_mem(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // mem - memory usage

    PRINT_VAL("sram size", sramSize());
    PRINT_VAL("data size", dataSize());
    PRINT_VAL("bss  size", bssSize());
    PRINT_VAL("heap size", heapSize());
    PRINT_VAL("heap used", heapUsed());
    PRINT_VAL("heap free", heapFree());
    PRINT_VAL("stack max", stackMaximum());
}

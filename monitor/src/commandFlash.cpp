// commandFlash.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

#if defined(__AVR_ATmega328P__)
    #define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.4lx %.5lu %S\n"), value, value, PSTR(text));
#endif

#if defined(__AVR_ATmega2560__) || \
    defined(__AVR_ATmega4809__)
    #define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.5lx %.6lu %S\n"), value, value, PSTR(text));
#endif

void cmd_flash(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // flash - flash usage

    PRINT_VAL("flash size", flashSize());
    PRINT_VAL("flash text", flashText());
    PRINT_VAL("flash data", flashData());
    #if defined(__AVR_ATmega4809__)
    PRINT_VAL("flash rodata", flashReadOnlyData());
    #endif
    PRINT_VAL("flash used", flashUsed());
    PRINT_VAL("flash free", flashFree());
}

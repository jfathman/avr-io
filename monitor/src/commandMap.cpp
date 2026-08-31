// commandMap.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

#if defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega2560__)
    #define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.4x %.4u %S\n"), value, value, PSTR(text));
#endif

#if defined(__AVR_ATmega4809__)
    #define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.4x %.5u %S\n"), value, value, PSTR(text));
#endif

void cmd_map(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // map - memory map

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
    PRINT_VAL("cpu regs",  0x0000);
    PRINT_VAL("i/o regs",  0x0020);
    PRINT_VAL("ext regs",  0x0060);
    #endif

    #if defined(__AVR_ATmega4809__)
    PRINT_VAL("i/o regs",  0x0000);
    PRINT_VAL("ext regs",  0x0040);
    PRINT_VAL("nvm regs",  0x1000);
    PRINT_VAL("eeprom",    0x1400);
    PRINT_VAL("reserved",  0x1500);
    #endif

    PRINT_VAL("sram",      sramStart());
    PRINT_VAL("data",      dataStart());
    PRINT_VAL("data end",  dataEnd());
    PRINT_VAL("bss",       bssStart());
    PRINT_VAL("bss end",   bssEnd());
    PRINT_VAL("heap",      mallocHeapStart());
    PRINT_VAL("brkval",    brkval());
    PRINT_VAL("heap end",  mallocHeapEnd());
    PRINT_VAL("stack ptr", stackPointer());
    PRINT_VAL("stack top", stackTop());
    PRINT_VAL("sram end",  sramEnd());

    #if defined(__AVR_ATmega4809__)
    PRINT_VAL("flash",     0x4000);
    PRINT_VAL("end",       0xFFFF);
    #endif
}

// commandEeprom.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"
#include "utils.h"

#define PRINT_VAL(text,value) \
    printf_P(PSTR("0x%.4x %.4u %S\n"), value, value, PSTR(text));

void cmd_eeprom(const char *args, int argc)
{
    // eeprom erase|usage

    if (argc == 0 || match_P(args, PSTR("usage")))
    {
        PRINT_VAL("eeprom size", eepromSize());
        PRINT_VAL("eeprom used", eepromUsed());
        PRINT_VAL("eeprom free", eepromFree());
        return;
    }

    if (match_P(args, PSTR("erase")))
    {
        eepromErase();
        printf_P(PSTR("ok\n"));
        return;
    }

    printf_P(PSTR("invalid args\n"));
}

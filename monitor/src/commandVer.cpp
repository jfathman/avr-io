// commandVer.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/version.h>
#include "commands.h"
#include "utils.h"

void cmd_ver(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // ver - version info

    printf_P(PSTR("%-8S %S\n"),  PSTR("version"),  PSTR(TO_STR(VERSION)));
    printf_P(PSTR("%-8S %S\n"),  PSTR("commit"),   PSTR(TO_STR(COMMIT_ID)));
    printf_P(PSTR("%-8S %s\n"),  PSTR("gcc"),      __VERSION__);
    printf_P(PSTR("%-8S %lu\n"), PSTR("c++"),      __cplusplus);
    printf_P(PSTR("%-8S %s\n"),  PSTR("avr libc"), __AVR_LIBC_VERSION_STRING__);
}

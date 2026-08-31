// commandSystem.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "utils.h"

void cmd_system(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // system - system info
        
    #if   __AVR_ARCH__ == 5       // atmega328p
        #define ARCH_NAME PSTR("avr5")
    #elif __AVR_ARCH__ == 6       // atmega2560
        #define ARCH_NAME PSTR("avr6")
    #elif __AVR_ARCH__ == 103     // atmega4809
        #define ARCH_NAME PSTR("avrxmega3")
    #else
        #error "unknown __AVR_ARCH__" 
    #endif

    printf_P(PSTR("board %S\n"), PSTR(TO_STR(BOARD)));
    printf_P(PSTR("mcu   %S\n"), PSTR(TO_STR(MCU))); 
    printf_P(PSTR("arch  %S\n"), ARCH_NAME);
    printf_P(PSTR("clock %lu MHz\n"), F_CPU/1000000L); 
}

// commandCrc.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

void cmd_crc(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // crc - flash crc-16

    uint16_t crc = 0xFFFF;

    for (uint16_t i = flashStart(); i < flashUsed(); i++)
    {
        uint8_t byte = pgm_read_byte(i); 

        crc ^= (uint16_t) byte;

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    printf_P(PSTR("%.4x\n"), crc);
}

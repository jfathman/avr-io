// commandI2c.cpp

#include <stdio.h>
#include <avr/pgmspace.h>
#include <I2c.h>
#include "commands.h"

static I2c i2c;

void cmd_i2c(const char *args, int argc)
{
    // i2c - <beg> <end>

    unsigned int addressBegin = 0;
    unsigned int addressEnd = 0;

    bool valid = false;

    switch (argc)  
    {
        case 0:
            addressBegin = I2C_ADDRESS_MIN;
            addressEnd = I2C_ADDRESS_MAX;
            valid = true;
            break;
        case 1:
            valid = (sscanf_P(args, PSTR("%x"), &addressBegin) == 1);
            addressEnd = addressBegin;
            break;
        case 2:
            valid = (sscanf_P(args, PSTR("%x %x"), &addressBegin, &addressEnd) == 2);
            break;
    }

    if (valid)
    {
        if (addressBegin <= addressEnd && addressEnd <= I2C_ADDRESS_MAX)
        {
            printf_P(PSTR("   "));

            for (uint8_t i = 0; i < 16; i++)
            {
                printf_P(PSTR("  %x"), i);
            }

            for (uint8_t address = I2C_ADDRESS_MIN; address <= I2C_ADDRESS_MAX; address++)
            {
                if (address % 16 == 0)
                {
                    printf_P(PSTR("\n"));
                    printf_P(PSTR("%.2x: "), address);
                }

                if (address >= addressBegin && address <= addressEnd)
                {
                    if (address < I2C_RESERVED_BELOW || address > I2C_RESERVED_ABOVE)
                    {
                        printf_P(PSTR("   "));
                    }
                    else if (i2c.detect(address))
                    {
                        printf_P(PSTR("%.2x "), address);
                    }
                    else
                    {
                        printf_P(PSTR("-- "));
                    }
                }
                else
                {
                    printf_P(PSTR("   "));
                }
            }

            printf_P(PSTR("\n"));
            return;
        }
    }

    printf_P(PSTR("invalid args\n"));
}

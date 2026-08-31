// memory.cpp

#include <avr/eeprom.h>
#include "memory.h"

void sramInit()
{
    char *p = (char *) heapStart();

    uint16_t sp = stackPointer();

    while (p < (char *) sp)
    {
        *p++ = 0xA5;
    }
}

uint16_t stackUnused()
{
    uint8_t *p = (uint8_t *) stackTop() - stackSize();

    uint16_t stackUnused = 0;

    // Note: 0xA5 data bytes will be misclassified
    // as stack unused bytes.

    for (uint16_t i = 0; i < stackSize(); i++, p++)
    {
        if (*p != 0xA5)
        {
            break;
        }

        stackUnused++;
    }

    return stackUnused;
}

uint16_t eepromUsed()
{
    eeprom_busy_wait();

    uint8_t *p = 0;

    uint16_t eepromUsed = 0;

    // Note: 0xFF data bytes will be misclassified
    // as erased free bytes.

    for (uint16_t i = 0; i < eepromSize(); i++, p++)
    {
        if (eeprom_read_byte(p) != 0xFF)
        {
            eepromUsed++;
        }
    }

    return eepromUsed;
}

void eepromErase()
{
    eeprom_busy_wait();

    uint8_t *p = 0;

    for (size_t i = 0; i < eepromSize(); i++, p++)
    {
        eeprom_update_byte(p, 0xFF);
    }
}

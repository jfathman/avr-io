// commandDump.cpp

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <memory.h>
#include "commands.h"

enum class DumpType : uint8_t
{
    None,
    SRAM,
    Flash,
    EEPROM,
    Heap
};

static uint32_t current_dump_address;
static DumpType current_dump_type = DumpType::None;

static void dump_memory(uint32_t address, DumpType dump_type);
static void dump_heap();

void cmd_dump(const char *args, int argc)
{
    // dump s|f|e|h <addr>

    uint32_t address = 0UL;

    char region[7] = { 0 };

    bool valid = false;

    if (argc == 1)
    {
        valid = (sscanf_P(args, PSTR("%6s"), region) == 1);
    }

    if (argc == 2)
    {
        valid = (sscanf_P(args, PSTR("%6s %lx"), region, &address) == 2);
    }

    if (valid)
    {
        // dump s <addr>
        if (strncmp_P(region, PSTR("sram"), strlen(region)) == 0)
        {
            if (argc == 1)
            {
                address = sramStart();
            }

            return dump_memory(address, DumpType::SRAM);
        }

        // dump f <addr>
        if (strncmp_P(region, PSTR("flash"), strlen(region)) == 0)
        {
            return dump_memory(address, DumpType::Flash);
        }

        // dump e <addr>
        if (strncmp_P(region, PSTR("eeprom"), strlen(region)) == 0)
        {
            return dump_memory(address, DumpType::EEPROM);
        }

        // dump h
        if (strncmp_P(region, PSTR("heap"), strlen(region)) == 0)
        {
            return dump_heap();
        }
    }

    printf_P(PSTR("invalid args\n"));
}

void cmd_cont(const char *args, int argc)
{
    (void) args;
    (void) argc;

    // cont - continue dump

    char buffer[16] = {0};

    char *memoryType = nullptr;

    switch (current_dump_type)
    {
        case DumpType::SRAM:   memoryType = (char *) "s"; break;
        case DumpType::Flash:  memoryType = (char *) "f"; break;
        case DumpType::EEPROM: memoryType = (char *) "e"; break;
        case DumpType::Heap:   return;
        default: return;
    }

    snprintf_P(buffer, sizeof(buffer), PSTR("%s %lx"),
        memoryType, current_dump_address + 1UL);

    cmd_dump(buffer, 2);
}

static void dump_memory(uint32_t address, DumpType dump_type)
{
    current_dump_type = dump_type;

    uint32_t maxAddress = 0;

    switch (dump_type)
    {
        case DumpType::SRAM:
            if (address < sramStart())
            {
                printf_P(PSTR("sram start is %.4x\n"), sramStart());
                return;
            }
            if (address >= (maxAddress = sramStart() + sramSize()))
            {
                printf_P(PSTR("sram end\n"));
                return;
            }
            break;
        case DumpType::Flash:
            if (address >= (maxAddress = flashSize()))
            {
                printf_P(PSTR("flash end\n"));
                return;
            }
            break;
        case DumpType::EEPROM:
            if (address >= (maxAddress = eepromSize()))
            {
                printf_P(PSTR("eeprom end\n"));
                return;
            }
            break;
        default:
            printf_P(PSTR("program error\n"));
            return;
    };

    char bufferHex[64];
    char bufferAscii[24];

    size_t len = 0;

    for (size_t i = 0; i < 8; i++)
    {
        memset(bufferHex, 0, sizeof(bufferHex));
        memset(bufferAscii, 0, sizeof(bufferAscii));

        if (address >= maxAddress)
        {
            break;
        }

        snprintf_P(bufferHex, sizeof(bufferHex), PSTR("%.5lx  "), address);

        for (size_t j = 0; j < 16; j++, address++)
        {
            current_dump_address = address;

            if (address >= maxAddress)
            {
                break;
            }

            unsigned char byte = 0;

            switch (dump_type)
            {
                case DumpType::SRAM:
                    byte = *((char *) address);
                    break;
                case DumpType::Flash:
                    byte = pgm_read_byte(address);
                    break;
                case DumpType::EEPROM:
                    byte = eeprom_read_byte((uint8_t *) address); 
                    break;
                default:
                    printf_P(PSTR("program error\n"));
                    return;
            };

            len = strlen(bufferHex);
            snprintf_P(bufferHex + len, sizeof(bufferHex) - len, PSTR("%.2x "), byte);

            byte = isprint(byte) ? byte : '.';
            byte = isspace(byte) ? '.' : byte;

            len = strlen(bufferAscii);
            snprintf_P(bufferAscii + len, sizeof(bufferAscii) - len, PSTR("%c"), byte);

            if (j == 7)
            {
                len = strlen(bufferHex);
                snprintf_P(bufferHex + len, sizeof(bufferHex) - len, PSTR(" "));

                len = strlen(bufferAscii);
                snprintf_P(bufferAscii + len, sizeof(bufferAscii) - len, PSTR(" "));
            }
        }

        const unsigned int padLength = 57;

        while (sizeof(bufferHex) > padLength && strlen(bufferHex) < padLength)
        {
            len = strlen(bufferHex);
            snprintf_P(bufferHex + len, sizeof(bufferHex) - len, PSTR(" "));
        }

        printf_P(PSTR("%s|%s|\n"), bufferHex, bufferAscii);
    }
}

static void dump_heap()
{
    current_dump_type = DumpType::Heap;
 
    heap_entry_t *p = (heap_entry_t *) mallocHeapStart();

    for (size_t i = 0; (char *) p < (char *) brkval(); i++)
    {
        printf_P(PSTR("entry: %2u addr: %.4x (%4u) len: %.4x (%3u)\n"),
            i, (size_t) p, (size_t) p, p->size, p->size);

        p = (heap_entry_t *) ((char *) p + sizeof(*p) + p->size);
    }
}

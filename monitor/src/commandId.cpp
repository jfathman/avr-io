// commandId.cpp

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "commands.h"
#include "eepromData.h"
#include "globals.h"
#include "utils.h"

static void id_read(char *id, const size_t len);
static void id_write(const char *id);
static void id_erase();
static bool is_valid(const char *s);
static void string_sanitize(char *p, const size_t len);

void cmd_id(const char *args, int argc)
{
    char arg1[6]  = {0};
    char arg2[10] = {0};

    // id erase|r|w <id>

    sscanf_P(args, PSTR("%5s %9s"), arg1, arg2);

    // id r
    if (argc == 0 || (argc == 1 && match_P(arg1, PSTR("r"))))
    {
        char id[id_len + 1] = {0};
        id_read(id, id_len);
        printf_P(PSTR("%s\n"), strlen(id) ? id : "none");
        return;
    }

    // id erase
    if (argc == 1 && match_P(arg1, PSTR("erase")))
    {
        id_erase();
        printf_P(PSTR("%S\n"), PSTR("ok"));
        return;
    }

    // id w <id>
    if (argc == 2 && match_P(arg1, PSTR("w")))
    {
        if (strlen(arg2) <= id_len && is_valid(arg2))
        {
            id_write(arg2);
            char id[id_len + 1] = {0};
            id_read(id, id_len);
            printf_P(PSTR("%S\n"), match(arg2, id) ? PSTR("ok") : PSTR("error"));
            return;
        }
    }
    
    printf_P(PSTR("invalid args\n"));
}

static void id_read(char *id, const size_t len)
{
    eeprom_busy_wait();
    const size_t size = MIN(id_len, len);
    eeprom_read_block((void *) id, (const void *) id_ptr, size);
    string_sanitize(id, size);
}

static void id_write(const char *id)
{
    eeprom_busy_wait();
    const size_t size = MIN(id_len, strlen(id));
    eeprom_update_block((const void *) id, (void *) id_ptr, size);

    uint8_t *p = (uint8_t *) id_ptr + size;

    for (size_t i = size; i < id_len; i++, p++)
    {
        eeprom_update_byte(p, 0);
    }
}

static void id_erase()
{
    eeprom_busy_wait();

    uint8_t *p = (uint8_t *) id_ptr;

    for (size_t i = 0; i < id_len; i++, p++)
    {
        eeprom_update_byte(p, 0xFF);
    }
}

static bool is_valid(const char *s)
{
    while (*s)
    {
        if (!isalnum(*s) && *s != '-')
        {
            return false;
        }

        s++;
    }

    return true;
}

static void string_sanitize(char *p, const size_t len)
{
    for (size_t i = 0; i < len; i++, p++)
    {
        if (!isprint(*p))
        {
            *p = 0;
        }
    }
}

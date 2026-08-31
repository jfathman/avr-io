// commands.cpp

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coreError.h>
#include "commands.h"
#include "globals.h"
#include "utils.h"

static void prompt();
static int  get_argc(const char *args);
static int  get_char();
static int  interpret_escape_sequence();
static void dispatch_command(char *cmd);
static void print_chars(uint8_t count, char c);
static void print_banner();

static void cmd_help (const char *args, int argc);
static void cmd_clear(const char *args, int argc);
static void cmd_nop  (const char *args, int argc);

#define ESC_SEQ_UP       1
#define ESC_SEQ_DOWN     2
#define ESC_SEQ_DELETE   3
#define ESC_SEQ_F1       4
#define ESC_SEQ_F2       5
#define ESC_SEQ_F3       6
#define ESC_SEQ_F4       7
#define ESC_SEQ_F5       8
#define ESC_SEQ_F6       9
#define ESC_SEQ_F7      10
#define ESC_SEQ_F8      11
#define ESC_SEQ_F9      12
#define ESC_SEQ_F10     13
#define ESC_SEQ_F11     14
#define ESC_SEQ_F12     15
#define ESC_SEQ_IGNORED 16
#define ESC_SEQ_UNKNOWN 17

typedef struct
{
    const char *cmd;
    void (*func)(const char *args, int argc);
    uint8_t argc_min;
    uint8_t argc_max;
    const char *desc;
} menu_t;

#define MENU_ITEM(var, name, desc) \
    static const char name_##var[] PROGMEM = name; \
    static const char desc_##var[] PROGMEM = desc;

MENU_ITEM( adc1,   "adc",    "init vcc|int|ext <v.v>"     );
#if defined(__AVR_ATmega2560__)
MENU_ITEM( adc2,   "adc",    "init int1v1|int2v56 <v.v>"  );
#endif 
#if defined(__AVR_ATmega4809__)
MENU_ITEM( adc2,   "adc",    "init int0v55|int1v1|int1v5" );
MENU_ITEM( adc3,   "adc",    "init int2v5|int4v34 <v.v>"  );
#endif 
MENU_ITEM( adc4,   "adc",    "<pin> read"                 );
MENU_ITEM( dig1,   "dig",    "<pin> in|inpull|out"        );
MENU_ITEM( dig2,   "dig",    "<pin> on|off|set|clear|1|0" );
MENU_ITEM( dig3,   "dig",    "<pin> toggle"               );
MENU_ITEM( dig4,   "dig",    "<pin> rda|rdc"              );
MENU_ITEM( pwm,    "pwm",    "<pin> 0-255|read"           );
MENU_ITEM( nop,    "nop",    "no operation"               );
MENU_ITEM( delay,  "delay",  "0-10000 (msec)"             );
MENU_ITEM( debug,  "debug",  "on|off"                     );
MENU_ITEM( map,    "map",    "memory map"                 );
MENU_ITEM( mem,    "mem",    "memory usage"               );
MENU_ITEM( heap,   "heap",   "heap usage"                 );
MENU_ITEM( stack,  "stack",  "stack usage"                );
MENU_ITEM( flash,  "flash",  "flash usage"                );
MENU_ITEM( crc,    "crc",    "flash crc-16"               );
MENU_ITEM( eeprom, "eeprom", "erase|usage"                );
MENU_ITEM( id,     "id",     "erase|r|w <id>"             );
MENU_ITEM( dump,   "dump",   "s|f|e|h <addr>"             );
MENU_ITEM( cont,   "cont",   "continue dump"              );
MENU_ITEM( uptime, "uptime", "since startup"              );
MENU_ITEM( clear,  "clear",  "clear screen"               );
MENU_ITEM( reset,  "reset",  "restart mcu"                );
MENU_ITEM( system, "system", "system info"                );
MENU_ITEM( ver,    "ver",    "version info"               );
MENU_ITEM( help1,  "help",   "command menu"               );
MENU_ITEM( help2,  "?",      "command menu"               );

static const menu_t menu[] PROGMEM = {
    { name_adc1,   cmd_adc,    2, 3, desc_adc1   },
    #if defined(__AVR_ATmega2560__)
    { name_adc2,   cmd_adc,    2, 3, desc_adc2   },
    #endif 
    #if defined(__AVR_ATmega4809__)
    { name_adc2,   cmd_adc,    2, 3, desc_adc2   },
    { name_adc3,   cmd_adc,    2, 3, desc_adc3   },
    #endif 
    { name_adc4,   cmd_adc,    2, 3, desc_adc4   },
    { name_dig1,   cmd_dig,    2, 2, desc_dig1   },
    { name_dig2,   cmd_dig,    2, 2, desc_dig2   },
    { name_dig3,   cmd_dig,    2, 2, desc_dig3   },
    { name_dig4,   cmd_dig,    2, 2, desc_dig4   },
    { name_pwm,    cmd_pwm,    2, 2, desc_pwm    },
    { name_nop,    cmd_nop,    0, 0, desc_nop    },
    { name_delay,  cmd_delay,  1, 1, desc_delay  },
    { name_debug,  cmd_debug,  0, 1, desc_debug  },
    { name_map,    cmd_map,    0, 0, desc_map    },
    { name_mem,    cmd_mem,    0, 0, desc_mem    },
    { name_heap,   cmd_heap,   0, 0, desc_heap   },
    { name_stack,  cmd_stack,  0, 0, desc_stack  },
    { name_flash,  cmd_flash,  0, 0, desc_flash  },
    { name_crc,    cmd_crc,    0, 0, desc_crc    },
    { name_eeprom, cmd_eeprom, 0, 1, desc_eeprom },
    { name_id,     cmd_id,     0, 2, desc_id     },
    { name_dump,   cmd_dump,   1, 2, desc_dump   },
    { name_cont,   cmd_cont,   0, 0, desc_cont   },
    { name_uptime, cmd_uptime, 0, 0, desc_uptime },
    { name_clear,  cmd_clear,  0, 0, desc_clear  },
    { name_reset,  cmd_reset,  0, 0, desc_reset  },
    { name_system, cmd_system, 0, 0, desc_system },
    { name_ver,    cmd_ver,    0, 0, desc_ver    },
    { name_help1,  cmd_help,   0, 1, desc_help1  },
    { name_help2,  cmd_help,   0, 1, desc_help2  },
};

void process_commands(void)
{
    char cmdbuf[24] = { 0 };
    char savbuf[24] = { 0 };

    uint8_t num_chars = 0;

    while (true)
    {
        char c = get_char();

        switch (c)
        {
            case '\b':
                if (num_chars > 0)
                {
                    printf_P(PSTR("\b \b"));
                    num_chars--;
                    cmdbuf[num_chars] = 0;
                }
                break;

            // picocom sends \r when enter is pressed,
            // emulating teletype carriage return key.

            case '\r':
                if (num_chars > 0)
                {
                    char *cmd = cmdbuf;

                    // skip leading whitespace

                    while (*cmd != 0 && isspace(*cmd))
                    {
                        cmd++;
                    }

                    // remove trailing whitespace

                    char *end = (char *) cmd + strlen(cmd) - 1;

                    while(end > cmd && isspace(*end))
                    {
                        *end-- = 0;
                    }

                    if (strlen(cmd) > 0)
                    {
                        dispatch_command(cmd);
                    }

                    if (strncmp_P(cmd, PSTR("dump"), 4) == 0)
                    {
                        memset(savbuf, 0, sizeof(savbuf));
                        strncpy_P(savbuf, PSTR("cont"), sizeof(savbuf) - 1);
                    }
                    else if (strcmp_P(cmd, PSTR("clear")) != 0)
                    {
                        memset(savbuf, 0, sizeof(savbuf));
                        strncpy(savbuf, cmdbuf, sizeof(savbuf) -1);
                    }
                }
                else
                {
                    putchar('\n');
                }
                memset(cmdbuf, 0, sizeof(cmdbuf));
                num_chars = 0;
                prompt();
                break;

            case 0x1B: // escape
                switch (interpret_escape_sequence())
                {
                    case ESC_SEQ_UP:     // up arrow key
                        while (num_chars > 0)
                        {
                            printf_P(PSTR("\b \b"));
                            num_chars--;
                        }
                        strncpy(cmdbuf, savbuf, sizeof(cmdbuf) -1);
                        printf_P(PSTR("%s"), cmdbuf);
                        num_chars = strlen(cmdbuf);
                        break;

                    case ESC_SEQ_DOWN:   // down arrow key
                        while (num_chars > 0)
                        {
                            printf_P(PSTR("\b \b"));
                            num_chars--;
                        }
                        memset(cmdbuf, 0, sizeof(cmdbuf));
                        break;

                    case ESC_SEQ_DELETE: // delete key
                        if (num_chars > 0)
                        {
                            printf_P(PSTR("\b \b"));
                            num_chars--;
                            cmdbuf[num_chars] = 0;
                        }
                        break;

                    // Future use:
                    // case ESC_SEQ_F1:  break;
                    // case ESC_SEQ_F2:  break;
                    // case ESC_SEQ_F3:  break;
                    // case ESC_SEQ_F4:  break;
                    // case ESC_SEQ_F5:  break;
                    // case ESC_SEQ_F6:  break;
                    // case ESC_SEQ_F7:  break;
                    // case ESC_SEQ_F8:  break;
                    // case ESC_SEQ_F9:  break;
                    // case ESC_SEQ_F10: break;
                    // case ESC_SEQ_F11: break;
                    // case ESC_SEQ_F12: break;

                    case ESC_SEQ_IGNORED:
                        break;

                    case ESC_SEQ_UNKNOWN:
                        break;
                }
                break;

            default:
                if (num_chars < sizeof(cmdbuf) - 1)
                {
                    if (isprint(c))
                    {
                        cmdbuf[num_chars++] = c;
                        putchar(c);
                    }
                }
        }
    }
}

static void prompt()
{
    printf_P(PSTR("W:> "));
}

static int get_argc(const char *args)
{
    int  argc = 0;
    bool in_arg = false;

    for (const char *s = args; *s != 0; s++)
    {
        if (!isspace(*s))
        {
            if (in_arg == 0)
            {
                in_arg = true;
                argc++;
            }
        }
        else if (in_arg)
        {
            in_arg = false;
        }
    }

    return argc;
}

static int get_char()
{
    char c;

    while ((c = getchar()) == EOF)
        ;

    return c;
}

static int interpret_escape_sequence()
{
    // To determine key codes: $ showkey -a

    int key;

    if (get_char() == '[')
    {
        char c = get_char();

        switch (c)
        {
            case 'A': // 0x1B[A  (up arrow key)
                return ESC_SEQ_UP;

            case 'B': // 0x1B[B  (down arrow key)
                return ESC_SEQ_DOWN;

            case '3': // 0x1B[3~ (delete key)
                return get_char() == '~' ? ESC_SEQ_DELETE : ESC_SEQ_UNKNOWN;

            case '1':
                switch (get_char())
                {
                    case '7': key = ESC_SEQ_F6; break; // F6 0x1B[17~
                    case '8': key = ESC_SEQ_F7; break; // F7 0x1B[18~
                    case '9': key = ESC_SEQ_F8; break; // F8 0x1B[19~
                    default:  key = ESC_SEQ_UNKNOWN;
                }
                return get_char() == '~' ? key : ESC_SEQ_UNKNOWN;

            case '2':
                switch (get_char())
                {
                    case '0': key = ESC_SEQ_F9;  break; // F9  0x1B[20~
                    case '1': key = ESC_SEQ_F10; break; // F10 0x1B[21~
                    case '3': key = ESC_SEQ_F11; break; // F11 0x1B[23~
                    case '4': key = ESC_SEQ_F12; break; // F12 0x1B[24~
                    default:  key = ESC_SEQ_UNKNOWN;
                }
                return get_char() == '~' ? key : ESC_SEQ_UNKNOWN;

            default:

                if (isupper(c))
                {
                    // 0x1B[x where x=C..Z
                    return ESC_SEQ_IGNORED;
                }

                if (c == '[')
                {
                    switch (get_char())
                    {
                        case 'A': return ESC_SEQ_F1; // F1 0x1B[[A
                        case 'B': return ESC_SEQ_F2; // F2 0x1B[[B
                        case 'C': return ESC_SEQ_F3; // F3 0x1B[[C
                        case 'D': return ESC_SEQ_F4; // F4 0x1B[[D
                        case 'E': return ESC_SEQ_F5; // F5 0x1B[[E
                    }
                    return ESC_SEQ_UNKNOWN;
                }
        }
    }

    return ESC_SEQ_UNKNOWN;
}

static void dispatch_command(char *cmd)
{
    const char *args = cmd;

    // skip command text

    while (*args != 0 && !isspace(*args))
    {
        args++;
    }

    const size_t cmdlen = args - cmd;

    // skip whitespace

    while (*args != 0 && isspace(*args))
    {
        args++;
    }

    menu_t buffer;

    menu_t *p = (menu_t *) &buffer;

    uint32_t pf = pgm_get_far_address(menu);

    for (size_t i = 0; i < ROWS(menu); i++, pf += sizeof(menu_t))
    {
        memcpy_PF(p, pf, sizeof(menu_t));

        if (cmdlen == strlen_PF((uint32_t) p->cmd) &&
            strncmp_PF(cmd, (uint32_t) p->cmd, cmdlen) == 0)
        {
            putchar('\n');

            int argc = get_argc(args);

            if (argc < p->argc_min || argc > p->argc_max)
            {
                printf_P(PSTR("invalid args\n"));
                return;
            }

            p->func(args, argc);

            return;
        }
    }

    putchar('\n');

    printf_P(PSTR("unknown command\n"));
}

static void print_chars(uint8_t count, char c)
{
    for (size_t i = 0; i < count; i++)
    {
        putchar(c);
    }
}

static void print_banner()
{
    print_chars(35, '*');
    printf_P(PSTR("\n"));

    printf_P(PSTR("**"));
    print_chars(8, ' ');
    printf_P(PSTR("AVR MCU Monitor"));
    print_chars(8, ' ');
    printf_P(PSTR("**"));
    printf_P(PSTR("\n"));

    print_chars(35, '*');
    printf_P(PSTR("\n"));
}

static void cmd_help(const char *args, int argc)
{
    size_t max_len = 0;

    for (size_t i = 0; i < ROWS(menu); i++)
    {
        uint32_t pf = (uint32_t) pgm_read_ptr_far((uint32_t) &menu[i].cmd);

        max_len = MAX(max_len, strlen_PF(pf));
    }

    if (argc == 0)
    {
        print_banner();
    }

    for (size_t i = 0; i < ROWS(menu); i++)
    {
        uint32_t pf = (uint32_t) pgm_read_ptr_far((uint32_t) &menu[i].cmd);

        if (argc == 1)
        {
            if (strncmp_PF(args, (uint32_t) pf, strlen(args)) != 0)
            {
                continue;
            }
        }

        printf_P(PSTR("  %S"), pf);

        for (size_t j = 0; j < max_len - strlen_PF(pf); j++)
        {
            printf_P(PSTR(" "));
        }

        pf = (uint32_t) pgm_read_ptr_far((uint32_t) &menu[i].desc);

        printf_P(PSTR(" %S\n"), pf);
    }
}

static void cmd_clear(const char *args, int argc)
{
    (void) args;
    (void) argc;

    printf_P(PSTR("\033[H\033[2J"));
}

static void cmd_nop(const char *args, int argc)
{
    (void) args;
    (void) argc;

    printf_P(PSTR("ok\n"));
}

void log_result()
{
    if (coreError == ERR_NONE)
    {
        printf_P(PSTR("ok\n"));
    }
    else
    {
        log_error();
    }
}

void log_result(uint8_t value)
{
    if (coreError == ERR_NONE)
    {
        printf_P(PSTR("%u\n"), value);
    }
    else
    {
        log_error();
    }
}

void log_error()
{
    const char *desc = NULL;

    switch (coreError)
    {
        case ERR_NONE:    desc = PSTR("none");    break;
        case ERR_PIN:     desc = PSTR("pin");     break;
        case ERR_VREF:    desc = PSTR("vref");    break;
        case ERR_CHANNEL: desc = PSTR("channel"); break;
        case ERR_MODE:    desc = PSTR("mode");    break;
        case ERR_OPTION:  desc = PSTR("option");  break;
        default:          desc = PSTR("unknown");
    }

    printf_P(PSTR("error: %S\n"), desc);
}

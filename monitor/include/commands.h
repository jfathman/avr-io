// commands.h

#pragma once

#include <stdint.h>

void process_commands(void);

void cmd_adc    (const char *args, int argc);
void cmd_dig    (const char *args, int argc);
void cmd_pwm    (const char *args, int argc);
void cmd_delay  (const char *args, int argc);
void cmd_debug  (const char *args, int argc);
void cmd_map    (const char *args, int argc);
void cmd_mem    (const char *args, int argc);
void cmd_heap   (const char *args, int argc);
void cmd_stack  (const char *args, int argc);
void cmd_flash  (const char *args, int argc);
void cmd_crc    (const char *args, int argc);
void cmd_eeprom (const char *args, int argc);
void cmd_id     (const char *args, int argc);
void cmd_dump   (const char *args, int argc);
void cmd_cont   (const char *args, int argc);
void cmd_uptime (const char *args, int argc);
void cmd_reset  (const char *args, int argc);
void cmd_system (const char *args, int argc);
void cmd_ver    (const char *args, int argc);

void log_result();
void log_result(uint8_t value);
void log_error();

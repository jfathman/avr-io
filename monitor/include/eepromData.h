// eepromData.h

#pragma once

#include <avr/eeprom.h>

#pragma pack(1)

typedef struct {
    uint8_t id[8];
} eeprom_data_t;

#pragma pack()

eeprom_data_t EEMEM eeprom_data;

#define CFG_OFFSET(field) (offsetof(eeprom_data_t, field))
#define CFG_PTR(field) (((uint8_t *) &eeprom_data) + CFG_OFFSET(field))
#define CFG_LEN(field) (sizeof(((eeprom_data_t *) 0)->field))

const void  *id_ptr = CFG_PTR(id);
const size_t id_len = CFG_LEN(id);

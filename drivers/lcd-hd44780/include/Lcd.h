// Lcd.h

#pragma once

#include <stdint.h>

#define LCD_I2C_ADDRESS 0x27

#define LCD_ROWS 2
#define LCD_COLS 16

class Lcd
{
  public:
    bool init();
    bool write(const char *str);
    bool moveCursor(uint8_t row, uint8_t col = 0);
    bool home();
    bool clear();
    bool clear(uint8_t row);
    bool backlightOn();
    bool backlightOff();
    bool displayOn();
    bool displayOff();
  private:
    bool _write(uint8_t data);
    bool _write4bit(uint8_t nibble, uint8_t control);
    bool _writeCommand(uint8_t cmd);
    bool _writeChar(uint8_t ch);
};

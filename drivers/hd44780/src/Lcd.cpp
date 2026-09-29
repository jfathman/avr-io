// Lcd.cpp

#include <avr/io.h>
#include <util/delay.h>
#include <I2c.h>
#include "Lcd.h"

// PCF8574 pin mapping to HD44780 LCD:

#define RS 0 // Register Select
#define RW 1 // Read/Write
#define EN 2 // Enable
#define BL 3 // Backlight Control Transistor
#define D4 4 // Data Line 4
#define D5 5 // Data Line 5
#define D6 6 // Data Line 6
#define D7 7 // Data Line 7

static uint8_t backlight_state = _BV(BL);

static I2c i2c;

bool Lcd::init()
{
    i2c.init();

    // HD44780 LCD operated by PCF8574 I2C expander
    // requires precise init timing per Hitachi doc.

    _delay_ms(50);
    
    // Reset and sync the LCD state machine by
    // sending 8-bit sequence (0x30) three times.

    if (!_write4bit(0x30, 0))
    {
        return false;
    }

    _delay_ms(5);     // 5 msec after 1st 0x30

    if (!_write4bit(0x30, 0))
    {
        return false;
    }

    _delay_us(150);   // 150 msec after 2nd 0x30

    if (!_write4bit(0x30, 0))
    {
        return false;
    }

    _delay_us(150);   // 150 msec after 3rd 0x30

    // Switch to 4-bit mode.

    if (!_write4bit(0x20, 0))
    {
        return false;
    }

    // Select 4-bit interface, two line mode, 5x8 dot font matrix.

    if (!_writeCommand(0x28))
    {
        return false;
    }

    // Select display on, cursor off, blink off.

    if (!_writeCommand(0x0C))
    {
        return false;
    }

    // Increment cursor each time char is written to LCD.

    if (!_writeCommand(0x06))
    {
        return false;
    }

    // Clear display and move cursor to home position.

    if (!_writeCommand(0x01))
    {
        return false;
    }

    // Delay (2 msec) is required after clear command.

    _delay_ms(2);

    return true;
}

bool Lcd::moveCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch (row)
    {
        case 0:  address = 0x00; break;
        case 1:  address = 0x40; break;
        default: return false;
    }

    return _writeCommand(0x80 | (address + col));
}
    
bool Lcd::home()
{
    return moveCursor(0, 0);
}

bool Lcd::clear()
{
    return clear(0) && clear(1) && home();
}

bool Lcd::clear(uint8_t row)
{
    if (!moveCursor(row, 0))
    {
        return false;
    }

    for (uint8_t i = 0; i < LCD_COLS; i++)
    {
        if (!_writeChar(' '))
        {
            return false;
        }
    }

    return moveCursor(row, 0);
}

bool Lcd::backlightOn()
{
    backlight_state |= _BV(BL);

    return _write(0);
}

bool Lcd::backlightOff()
{
    backlight_state &= ~_BV(BL);

    return _write(0);
}

bool Lcd::displayOn()
{
    // Select display on, cursor off, blink off.

    if (!_writeCommand(0x0C))
    {
        return false;
    }

    _delay_ms(2);

    return true;
}

bool Lcd::displayOff()
{
    // Select display off, cursor off, blink off.

    if (!_writeCommand(0x08))
    {
        return false;
    }

    _delay_ms(2);

    return true;
}

bool Lcd::write(const char *str)
{
    for (uint8_t i = 0; *str && i < LCD_COLS; i++)
    {
        if (!_writeChar(*str++))
        {
            return false;
        }
    }

    return true;
}

bool Lcd::_write(uint8_t data)
{
    bool result = i2c.write(LCD_I2C_ADDRESS, data | backlight_state);

    _delay_us(50);

    return result;
}

bool Lcd::_write4bit(uint8_t nibble, uint8_t control)
{
    uint8_t data = (nibble & 0xF0) | control;

    // Pulse the enable pin to command the
    // HD44780 controller to read the data.
    // Data is read on the trailing edge.

    if (!_write(data | _BV(EN)))
    {
        return false;
    }

    _delay_us(1); // pulse duration

    if (!_write(data & ~(_BV(EN))))
    {
        return false;
    }

    _delay_us(50);

    return true;
}

bool Lcd::_writeCommand(uint8_t command)
{
    uint8_t high_nibble = command & 0xF0;

    uint8_t low_nibble = (command << 4) & 0xF0;

    if (!_write4bit(high_nibble, 0))
    {
        return false;
    }

    if (!_write4bit(low_nibble, 0))
    {
        return false;
    }

    return true;
}

bool Lcd::_writeChar(uint8_t ch)
{
    uint8_t high_nibble = ch & 0xF0;

    uint8_t low_nibble = (ch << 4) & 0xF0;

    if (!_write4bit(high_nibble, _BV(RS)))
    {
        return false;
    }

    if (!_write4bit(low_nibble, _BV(RS)))
    {
        return false;
    }

    return true;
}

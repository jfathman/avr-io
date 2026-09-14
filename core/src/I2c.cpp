// I2c.cpp

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "I2c.h"

void I2c::init()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TWSR = 0;  // Prescaler 1
        TWBR = 72; // SCL 100 kHz
        TWCR = _BV(TWEN);
    #endif

    #if defined(__AVR_ATmega4809__)
        // SCL 100 kHz
        TWI0.MBAUD = (uint8_t)((F_CPU / (2 * 100000UL)) - 5);
        TWI0.MCTRLA = TWI_ENABLE_bm;
        TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    #endif
}

bool I2c::start()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT);

        if (_waitCompleteStart())
        {
            uint8_t status = TWSR & 0xF8;

            if (status == TW_START || status == TW_REP_START)
            {
                return true;
            }
        }

        stop();

        return false;
    #endif

    #if defined(__AVR_ATmega4809__)
        // I2C start is automatic.
        return true;
    #endif
}

bool I2c::stop()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT);
        return _waitCompleteStop();
    #endif

    #if defined(__AVR_ATmega4809__)
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return _waitCompleteStop();
    #endif
}

bool I2c::write(uint8_t address, uint8_t data)
{
    return start()
        && writeAddress(I2C_SLA_W(address))
        && writeData(data)
        && stop();
}

bool I2c::writeAddress(uint8_t address)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TWDR = address;
        TWCR = _BV(TWEN) | _BV(TWINT);

        if (_waitCompleteWrite())
        {
            uint8_t status = TWSR & 0xF8;

            if (status == TW_MT_SLA_ACK)
            {
                return true;
            }
        }

        stop();

        return false;
    #endif

    #if defined(__AVR_ATmega4809__)
        TWI0.MADDR = address;

        if (_waitCompleteWrite())
        {
            if (!(TWI0.MSTATUS & TWI_RXACK_bm))
            {
                return true;
            }
        }

        stop();

        return false;
    #endif
}

bool I2c::writeData(uint8_t data)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        TWDR = data;
        TWCR = _BV(TWEN) | _BV(TWINT);

        if (_waitCompleteWrite())
        {
            uint8_t status = TWSR & 0xF8;

            if (status == TW_MT_DATA_ACK)
            {
                return true;
            }
        }

        stop();

        return false;
    #endif

    #if defined(__AVR_ATmega4809__)
        TWI0.MDATA = data;

        if (_waitCompleteWrite())
        {
            if (!(TWI0.MSTATUS & TWI_RXACK_bm))
            {
                return true;
            }
        }

        stop();

        return false;
    #endif
}

bool I2c::detect(uint8_t address)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__) || \
        defined(__AVR_ATmega4809__)
        init();

        return start()
            && writeAddress(I2C_SLA_W(address))
            && stop();
    #endif
}

bool I2c::_waitCompleteStart()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        return _waitCondition(2000, []()
        {
            return TWCR & _BV(TWINT);
        });
    #endif

    #if defined(__AVR_ATmega4809__)
        // I2C start is automatic.
        return true;
    #endif
}

bool I2c::_waitCompleteWrite()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        return _waitCondition(2000, []()
        {
            return TWCR & _BV(TWINT);
        });
    #endif

    #if defined(__AVR_ATmega4809__)
        return _waitCondition(2000, []()
        {
            return TWI0.MSTATUS & TWI_WIF_bm;
        });
    #endif
}

bool I2c::_waitCompleteStop()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        return _waitCondition(2000, []()
        {
            return !(TWCR & _BV(TWSTO));
        });
    #endif

    #if defined(__AVR_ATmega4809__)
        return _waitCondition(2000, []()
        {
            return (TWI0.MSTATUS & TWI_BUSSTATE_gm) == TWI_BUSSTATE_IDLE_gc;
        });
    #endif
}

template <typename Func>
bool I2c::_waitCondition(uint16_t timeout_usec, Func condition)
{
    while (!condition())
    {
        _delay_us(1);

        if (--timeout_usec == 0)
        {
            return false;
        }
    }

    return true;
}

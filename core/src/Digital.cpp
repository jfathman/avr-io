// Digital.cpp

#include "coreError.h"
#include "coreUtils.h"
#include "Digital.h"

void Digital::mode(uint8_t pin, uint8_t mode)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return;
    }

    coreError = ERR_NONE;
        
    uint8_t entrySREG = SREG;

    cli();

    #if defined(BOARD_arduino_uno_r3) || \
        defined(BOARD_arduino_mega_2560)
        volatile uint8_t *port_reg = getPortReg(pin);
        volatile uint8_t *port_dir = getPortDir(pin);
        uint8_t port_bit = getPortBit(pin);

        // Enable the digital input buffer if this is an
        // analog capable pin, in case the digital input
        // buffer is disabled from a prior analog read.

        #if defined(BOARD_arduino_uno_r3)
            if (pin >= A0_PIN && pin <= A5_PIN)
            {
                DIDR0 &= ~_BV(pin - A0_PIN);
            }
        #endif

        #if defined(BOARD_arduino_mega_2560)
            if (pin >= A0_PIN && pin <= A7_PIN)
            {
                DIDR0 &= ~_BV(pin - A0_PIN);
            }

            if (pin >= A8_PIN && pin <= A15_PIN)
            {
                DIDR2 &= ~_BV(pin - A8_PIN);
            }
        #endif

        if (mode == OUTPUT)
        {
            *port_dir |= _BV(port_bit);
        }
        else if (mode == INPUT)
        {
            *port_dir &= ~_BV(port_bit);
        }
        else if (mode == INPUT_PULLUP)
        {
            *port_dir &= ~_BV(port_bit);
            *port_reg |=  _BV(port_bit);
        }
        else
        {
            coreError = ERR_MODE;
        }
    #endif

    #if defined(BOARD_arduino_nano_every)
        volatile uint8_t *port_dir = getPortDir(pin);
        volatile uint8_t *port_ctl = getPortCtl(pin);
        uint8_t port_bit = getPortBit(pin);

        // Enable digital input buffer.
        *port_ctl &= ~PORT_ISC_gm;

        if (mode == OUTPUT)
        {
            *port_dir |= _BV(port_bit);
        }
        else if (mode == INPUT)
        {
            *port_dir &= ~_BV(port_bit);
            *port_ctl &= ~PORT_PULLUPEN_bm;
        }
        else if (mode == INPUT_PULLUP)
        {
            *port_dir &= ~_BV(port_bit);
            *port_ctl |= PORT_PULLUPEN_bm;
        }
        else
        {
            coreError = ERR_MODE;
        }
    #endif
        
    SREG = entrySREG;
}

void Digital::set(uint8_t pin)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return;
    }

    coreError = ERR_NONE;

    uint8_t entrySREG = SREG;

    cli();

    #if defined(BOARD_arduino_uno_r3) || \
        defined(BOARD_arduino_mega_2560)
        volatile uint8_t *port_reg = getPortReg(pin);
        uint8_t port_bit = getPortBit(pin);

        if (isPwmPin(pin))
        {
            pwmDisable(pin);
        }

        *port_reg |= _BV(port_bit);
    #endif

    #if defined(BOARD_arduino_nano_every)
        volatile uint8_t *port_out = getPortOut(pin);
        uint8_t port_bit = getPortBit(pin);

        if (isPwmPin(pin))
        {
            pwmDisable(pin);
        }

        *port_out |= _BV(port_bit);
    #endif

    SREG = entrySREG;
}

void Digital::clear(uint8_t pin)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return;
    }

    coreError = ERR_NONE;

    uint8_t entrySREG = SREG;

    cli();

    #if defined(BOARD_arduino_uno_r3) || \
        defined(BOARD_arduino_mega_2560)
        volatile uint8_t *port_reg = getPortReg(pin);
        uint8_t port_bit = getPortBit(pin);

        if (isPwmPin(pin))
        {
            pwmDisable(pin);
        }

        *port_reg &= ~_BV(port_bit);
    #endif

    #if defined(BOARD_arduino_nano_every)
        volatile uint8_t *port_out = getPortOut(pin);
        uint8_t port_bit = getPortBit(pin);

        if (isPwmPin(pin))
        {
            pwmDisable(pin);
        }

        *port_out &= ~_BV(port_bit);
    #endif

    SREG = entrySREG;
}

void Digital::toggle(uint8_t pin, uint8_t option)
{
    uint8_t state = read(pin, option);

    if (coreError == ERR_NONE)
    {
        if (state)
        {
            clear(pin);
        }
        else
        {
            set(pin);
        }
    }
}

void Digital::on(uint8_t pin)
{
    set(pin);
}

void Digital::off(uint8_t pin)
{
    clear(pin);
}

uint8_t Digital::read(uint8_t pin, uint8_t option)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return 0;
    }

    coreError = ERR_NONE;

    uint8_t reading = 0;

    uint8_t entrySREG = SREG;

    cli();

    #if defined(BOARD_arduino_uno_r3) || \
        defined(BOARD_arduino_mega_2560)
        volatile uint8_t *port_reg = getPortReg(pin);
        volatile uint8_t *port_inp = getPortInp(pin);
        uint8_t port_bit = getPortBit(pin);

        if (option == ACTUAL)
        {
            reading = *port_inp & _BV(port_bit) ? 1 : 0;
        }
        else if (option == COMMANDED)
        {
            reading = *port_reg & _BV(port_bit) ? 1 : 0;
        }
        else
        {
            coreError = ERR_OPTION;
        }
    #endif

    #if defined(BOARD_arduino_nano_every)
        volatile uint8_t *port_out = getPortOut(pin);
        volatile uint8_t *port_inp = getPortInp(pin);
        uint8_t port_bit = getPortBit(pin);

        if (option == ACTUAL)
        {
            reading = *port_inp & _BV(port_bit) ? 1 : 0;
        }
        else if (option == COMMANDED)
        {
            reading = *port_out & _BV(port_bit) ? 1 : 0;
        }
        else
        {
            coreError = ERR_OPTION;
        }
    #endif

    SREG = entrySREG;

    return reading;
}

void Digital::pwmWrite(uint8_t pin, uint8_t dutyCycle)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return;
    }

    coreError = ERR_NONE;

    if (isPwmPin(pin))
    {
        uint8_t entrySREG = SREG;

        cli();

        if (dutyCycle == 0)
        {
            pwmSetDutyCycle(pin, 0);   // so pwmRead() will return 0
            clear(pin);                // prevent narrow glitch pulses
        }
        else if (dutyCycle == 255)
        {
            pwmSetDutyCycle(pin, 255); // so pwmRead() will return 255
            set(pin);                  // prevent narrow glitch pulses
        }
        else {
            pwmEnable(pin);

            if (coreError == ERR_NONE)
            {
                pwmSetDutyCycle(pin, dutyCycle);
            }
        }

        SREG = entrySREG;
    }
    else
    {
        coreError = ERR_PIN;
    }
}

uint8_t Digital::pwmRead(uint8_t pin)
{
    if (pin >= ROWS(pins))
    {
        coreError = ERR_PIN;
        return 0;
    }

    coreError = ERR_NONE;

    if (isPwmPin(pin))
    {
        if (coreError == ERR_NONE)
        {
            return pwmGetDutyCycle(pin);
        }
    }
    else
    {
        coreError = ERR_PIN;
    }

    return 0;
}

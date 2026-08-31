// pinDefs.h

#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "coreError.h"

#if defined(BOARD_arduino_uno_r3)

#define D0   0 // PD0
#define D1   1 // PD1
#define D2   2 // PD2
#define D3   3 // PD3
#define D4   4 // PD4
#define D5   5 // PD5
#define D6   6 // PD6
#define D7   7 // PD7
#define D8   8 // PB0
#define D9   9 // PB1
#define D10 10 // PB2
#define D11 11 // PB3
#define D12 12 // PB4
#define D13 13 // PB5
#define D14 14 // PC0
#define D15 15 // PC1
#define D16 16 // PC2
#define D17 17 // PC3
#define D18 18 // PC4
#define D19 19 // PC5

#define LED_BUILTIN D13

#define A0 0 // PC0
#define A1 1 // PC1
#define A2 2 // PC2
#define A3 3 // PC3
#define A4 4 // PC4
#define A5 5 // PC5

#define A0_PIN D14
#define A1_PIN D15
#define A2_PIN D16
#define A3_PIN D17
#define A4_PIN D18
#define A5_PIN D19

inline bool isDigPin(uint8_t pin)
{
    return pin <= D19;
}

inline bool isPwmPin(uint8_t pin)
{
    return pin == D3 || pin == D5  || pin == D6
        || pin == D9 || pin == D10 || pin == D11;
}

inline bool isAdcPin(uint8_t pin)
{
    return pin <= A5;
}

inline void pwmEnable(uint8_t pin)
{
    // From ATmega328P Datasheet:
    //
    // Timer/Counter Control Register A
    //
    //   COM = Compare Match Output Mode
    //
    //              7        6        5        4        3        2       1       0
    //         +--------+--------+--------+--------+--------+--------+-------+-------+
    // TCCR0A  | COM0A1 | COM0A0 | COM0B1 | COM0B0 |   --   |   --   | WGM01 | WGM00 |
    // TCCR1A  | COM1A1 | COM1A0 | COM1B1 | COM1B0 |   --   |   --   | WGM11 | WGM10 |
    // TCCR2A  | COM2A1 | COM2A0 | COM2B1 | COM2B0 |   --   |   --   | WGM21 | WGM20 |
    //         +--------+--------+--------+--------+--------+--------+-------+-------+
    //
    // Timer/Counter Control Register B
    //
    //   CS = Clock Select
    //
    //             7       6      5     4       3      2      1      0
    //         +-------+-------+----+-------+-------+------+------+------+
    //         |                    |               |      Clock/64      |
    //         +-------+-------+----+-------+-------+------+------+------+
    //         |                    |   0   |   0   |   0  |   1  |   1  |
    //         +-------+-------+----+-------+-------+------+------+------+
    // TCCR0B  | FOC0A | FOC0B | -- |   --  | WGM02 | CS02 | CS01 | CS00 |
    // TCCR1B  | ICNC1 | ICES1 | -- | WGM13 | WGM12 | CS12 | CS11 | CS10 |
    //         +-------+-------+----+-------+-------+------+------+------+
    //
    //             7       6      5     4       3      2      1      0
    //         +-------+-------+----+-------+-------+------+------+------+
    //         |                    |               |      Clock/64      |
    //         +-------+-------+----+-------+-------+------+------+------+
    //         |                    |   0   |   0   |   1  |   0  |   0  |
    //         +-------+-------+----+-------+-------+------+------+------+
    // TCCR2B  | FOC2A | FOC2B | -- |   --  | WGM22 | CS22 | CS21 | CS20 |
    //         +-------+-------+----+-------+-------+------+------+------+
    //
    // Waveform Generation Mode
    //
    //   +-------------------------------+
    //   |         8-bit Timers          |
    //   +---------------+---------------+
    //   +    TCCRnB     |    TCCRnA     |  <-- Register
    //   +-------+-------+-------+-------+
    //   |   4   |   3   |   1   |   0   |  <-- Register Pin
    //   +-------+-------+-------+-------+
    //   |   0   |   0   |   1   |   1   |  <-- Fast PWM Mode
    //   +-------+-------+-------+-------+
    //   |   --  | WGM02 | WGM01 | WGM00 |  Timer0
    //   |   --  | WGM22 | WGM21 | WGM20 |  Timer2
    //   +-------+-------+-------+-------+
    //
    //   +-------------------------------+
    //   |        16-bit Timers          |
    //   +---------------+---------------+
    //   +    TCCRnB     |    TCCRnA     |  <-- Register
    //   +-------+-------+-------+-------+
    //   |   4   |   3   |   1   |   0   |  <-- Register Pin
    //   +-------+-------+-------+-------+
    //   |   0   |   1   |   0   |   1   |  <-- Fast PWM Mode (8-bit, TOP 0x00FF)
    //   +-------+-------+-------+-------+
    //   | WGM13 | WGM12 | WGM11 | WGM10 |  Timer1
    //   +-------+-------+-------+-------+
    //
    // Compare Output Mode
    //
    //   OC = Output Compare
    //
    //   +-----+------+--------+-----+------+-----------------+
    //   | Pin | Port | Timer  |  Hz |  OC  |  Compare Mode   |
    //   +-----+------+--------+-----+------+-----------------+
    //   |     |      |        |     |      |    1   |    0   | <-- Non-Inverting Mode
    //   +-----+------+--------+-----+------+-----------------+
    //   | D6  |  PD6 | Timer0 | 980 | OC0A | COM0A1 | COM0A0 |
    //   | D5  |  PD5 | Timer0 | 980 | OC0B | COM0B1 | COM0B0 |
    //   | D9  |  PB1 | Timer1 | 980 | OC1A | COM1A1 | COM1A0 |
    //   | D10 |  PB2 | Timer1 | 980 | OC1B | COM1B1 | COM1B0 |
    //   | D11 |  PB3 | Timer2 | 980 | OC2A | COM2A1 | COM2A0 |
    //   | D3  |  PD3 | Timer2 | 980 | OC2B | COM2B1 | COM2B0 |
    //   +-----+------+--------+-----+------+-----------------+

    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D6:  // PD6 Timer0 OC0A (8-bit timer)
            DDRD   |= _BV(DDD6);               // Pin Mode Output
            TCCR0A |= _BV(WGM01) | _BV(WGM00); // Fast PWM mode
            TCCR0A |= _BV(COM0A1);             // Non-Inverting Mode
            TCCR0B |= _BV(CS01) | _BV(CS00);   // Clock/64
            break;

        case D5:  // PD5 Timer0 OC0B (8-bit timer)
            DDRD   |= _BV(DDD5);               // Pin Mode Output
            TCCR0A |= _BV(WGM01) | _BV(WGM00); // Fast PWM mode
            TCCR0A |= _BV(COM0B1);             // Non-Inverting Mode
            TCCR0B |= _BV(CS01) | _BV(CS00);   // Clock/64
            break;

        case D9:  // PB1 Timer1 OC1A (16-bit timer)
            DDRB   |= _BV(DDB1);               // Pin Mode Output
            TCCR1B |= _BV(WGM12);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(WGM10);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(COM1A1);             // Non-Inverting Mode
            TCCR1B |= _BV(CS11) | _BV(CS10);   // Clock/64
            break;

        case D10: // PB2 Timer1 OC1B (16-bit timer)
            DDRB   |= _BV(DDB2);               // Pin Mode Output
            TCCR1B |= _BV(WGM12);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(WGM10);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(COM1B1);             // Non-Inverting Mode
            TCCR1B |= _BV(CS11) | _BV(CS10);   // Clock/64
            break;

        case D11: // PB3 Timer2 OC2A (8-bit timer)
            DDRB   |= _BV(DDB3);               // Pin Mode Output
            TCCR2A |= _BV(WGM21) | _BV(WGM20); // Fast PWM mode
            TCCR2A |= _BV(COM2A1);             // Non-Inverting Mode
            TCCR2B |= _BV(CS22);               // Clock/64
            break;

        case D3:  // PD3 Timer2 OC2B (8-bit timer)
            DDRD   |= _BV(DDD3);               // Pin Mode Output
            TCCR2A |= _BV(WGM21) | _BV(WGM20); // Fast PWM mode
            TCCR2A |= _BV(COM2B1);             // Non-Inverting Mode
            TCCR2B |= _BV(CS22);               // Clock/64
            break;

        default:
            coreError = ERR_PIN;
    }

    SREG = entrySREG;
}

inline void pwmDisable(uint8_t pin)
{
    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D6:  TCCR0A &= ~_BV(COM0A1); break; // Timer0
        case D5:  TCCR0A &= ~_BV(COM0B1); break; // Timer0
        case D9:  TCCR1A &= ~_BV(COM1A1); break; // Timer1
        case D10: TCCR1A &= ~_BV(COM1B1); break; // Timer1
        case D11: TCCR2A &= ~_BV(COM2A1); break; // Timer2
        case D3:  TCCR2A &= ~_BV(COM2B1); break; // Timer2
        default:  coreError = ERR_PIN;
    }

    SREG = entrySREG;
}

inline void pwmSetDutyCycle(uint8_t pin, uint8_t dutyCycle)
{
    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D3:  OCR2B = dutyCycle; break;
        case D5:  OCR0B = dutyCycle; break;
        case D6:  OCR0A = dutyCycle; break;
        case D9:  OCR1A = dutyCycle; break;
        case D10: OCR1B = dutyCycle; break;
        case D11: OCR2A = dutyCycle; break;
        default:  coreError = ERR_PIN;
    }

    SREG = entrySREG;
}

inline uint8_t pwmGetDutyCycle(uint8_t pin)
{
    uint8_t dutyCycle = 0;
    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D3:  dutyCycle = OCR2B; break;
        case D5:  dutyCycle = OCR0B; break;
        case D6:  dutyCycle = OCR0A; break;
        case D9:  dutyCycle = OCR1A; break;
        case D10: dutyCycle = OCR1B; break;
        case D11: dutyCycle = OCR2A; break;
        default:  coreError = ERR_PIN;
    }

    SREG = entrySREG;

    return dutyCycle;
}

typedef struct {
    volatile uint8_t *port_reg;
    volatile uint8_t *port_dir;
    volatile uint8_t *port_inp;
} ports_t;

typedef struct {
    uint8_t port;
    uint8_t bit;
} pins_t;

enum class Port {
    B, C, D
};

// Define arrays as const (not static const) to specify
// external linkage.  Prevents duplicate definitions if
// header file is included by multiple source modules.

const ports_t PROGMEM ports[] = {
    { &PORTB, &DDRB, &PINB },
    { &PORTC, &DDRC, &PINC },
    { &PORTD, &DDRD, &PIND }
};

const pins_t PROGMEM pins[] = {
    { (uint8_t) Port::D, 0 }, // D0
    { (uint8_t) Port::D, 1 }, // D1
    { (uint8_t) Port::D, 2 }, // D2
    { (uint8_t) Port::D, 3 }, // D3
    { (uint8_t) Port::D, 4 }, // D4
    { (uint8_t) Port::D, 5 }, // D5
    { (uint8_t) Port::D, 6 }, // D6
    { (uint8_t) Port::D, 7 }, // D7
    { (uint8_t) Port::B, 0 }, // D8
    { (uint8_t) Port::B, 1 }, // D9
    { (uint8_t) Port::B, 2 }, // D10
    { (uint8_t) Port::B, 3 }, // D11
    { (uint8_t) Port::B, 4 }, // D12
    { (uint8_t) Port::B, 5 }, // D13
    { (uint8_t) Port::C, 0 }, // D14
    { (uint8_t) Port::C, 1 }, // D15
    { (uint8_t) Port::C, 2 }, // D16
    { (uint8_t) Port::C, 3 }, // D17
    { (uint8_t) Port::C, 4 }, // D18
    { (uint8_t) Port::C, 5 }, // D19
};

inline uint8_t getPortIndex(uint8_t pin)
{
    return pgm_read_byte(&pins[pin].port);
}

inline volatile uint8_t *getPortReg(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_reg);
}

inline volatile uint8_t *getPortDir(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_dir);
}

inline volatile uint8_t *getPortInp(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_inp);
}

inline uint8_t getPortBit(uint8_t pin)
{
    return pgm_read_byte(&pins[pin].bit);
}

#else
    #error "Unexpected MCU"
#endif

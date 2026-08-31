// pinDefs.h

#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "coreError.h"

#if defined(BOARD_arduino_mega_2560)

#define D0   0 // PE0
#define D1   1 // PE1
#define D2   2 // PE4
#define D3   3 // PE5
#define D4   4 // PG5
#define D5   5 // PE3
#define D6   6 // PH3
#define D7   7 // PH4
#define D8   8 // PH5
#define D9   9 // PH6
#define D10 10 // PB4
#define D11 11 // PB5
#define D12 12 // PB6
#define D13 13 // PB7
#define D14 14 // PJ1
#define D15 15 // PJ0
#define D16 16 // PH1
#define D17 17 // PH0
#define D18 18 // PD3
#define D19 19 // PD2
#define D20 20 // PD1
#define D21 21 // PD0
#define D22 22 // PA0
#define D23 23 // PA1
#define D24 24 // PA2
#define D25 25 // PA3
#define D26 26 // PA4
#define D27 27 // PA5
#define D28 28 // PA6
#define D29 29 // PA7
#define D30 30 // PC7
#define D31 31 // PC6 
#define D32 32 // PC5
#define D33 33 // PC4
#define D34 34 // PC3
#define D35 35 // PC2
#define D36 36 // PC1
#define D37 37 // PC0
#define D38 38 // PD7
#define D39 39 // PG2
#define D40 40 // PG1
#define D41 41 // PG0
#define D42 42 // PL7
#define D43 43 // PL6
#define D44 44 // PL5
#define D45 45 // PL4
#define D46 46 // PL3
#define D47 47 // PL2
#define D48 48 // PL1
#define D49 49 // PL0
#define D50 50 // PB3
#define D51 51 // PB2
#define D52 52 // PB1
#define D53 53 // PB0
#define D54 54 // PF0
#define D55 55 // PF1  
#define D56 56 // PF2  
#define D57 57 // PF3  
#define D58 58 // PF4  
#define D59 59 // PF5  
#define D60 60 // PF6  
#define D61 61 // PF7  
#define D62 62 // PK0  
#define D63 63 // PK1  
#define D64 64 // PK2  
#define D65 65 // PK3  
#define D66 66 // PK4  
#define D67 67 // PK5  
#define D68 68 // PK6  
#define D69 69 // PK7  

#define LED_BUILTIN D13

#define A0   0 // PF0
#define A1   1 // PF1
#define A2   2 // PF2
#define A3   3 // PF3
#define A4   4 // PF4
#define A5   5 // PF5
#define A6   6 // PF6
#define A7   7 // PF7
#define A8   8 // PK0
#define A9   9 // PK1
#define A10 10 // PK2
#define A11 11 // PK3
#define A12 12 // PK4
#define A13 13 // PK5
#define A14 14 // PK6
#define A15 15 // PK7

#define A0_PIN  D54
#define A1_PIN  D55
#define A2_PIN  D56
#define A3_PIN  D57
#define A4_PIN  D58
#define A5_PIN  D59
#define A6_PIN  D60
#define A7_PIN  D61
#define A8_PIN  D62
#define A9_PIN  D63
#define A10_PIN D64
#define A11_PIN D65
#define A12_PIN D66
#define A13_PIN D67
#define A14_PIN D68
#define A15_PIN D69

inline bool isDigPin(uint8_t pin)
{
    return pin <= D69;
}

inline bool isPwmPin(uint8_t pin)
{
    return (pin >= D2  && pin <= D13)
        || (pin >= D44 && pin <= D46);
}

inline bool isAdcPin(uint8_t pin)
{
    return pin <= A15;
}

inline void pwmEnable(uint8_t pin)
{
    // From ATmega2560 Datasheet:
    //
    // Timer/Counter Control Register A
    //
    //   COM = Compare Match Output Mode
    //
    //              7        6        5        4        3        2       1       0
    //         +--------+--------+--------+--------+--------+--------+-------+-------+
    // TCCR0A  | COM0A1 | COM0A0 | COM0B1 | COM0B0 |   --   |   --   | WGM01 | WGM00 |
    // TCCR1A  | COM1A1 | COM1A0 | COM1B1 | COM1B0 | COM1C1 | COM1C0 | WGM11 | WGM10 |
    // TCCR2A  | COM2A1 | COM2A0 | COM2B1 | COM2B0 |   --   |   --   | WGM21 | WGM20 |
    // TCCR3A  | COM3A1 | COM3A0 | COM3B1 | COM3B0 | COM3C1 | COM3C0 | WGM31 | WGM30 |
    // TCCR4A  | COM4A1 | COM4A0 | COM4B1 | COM4B0 | COM4C1 | COM4C0 | WGM41 | WGM40 |
    // TCCR5A  | COM5A1 | COM5A0 | COM5B1 | COM5B0 | COM5C1 | COM5C0 | WGM51 | WGM50 |
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
    // TCCR3B  | ICNC3 | ICES3 | -- | WGM33 | WGM32 | CS32 | CS31 | CS30 |
    // TCCR4B  | ICNC4 | ICES4 | -- | WGM43 | WGM42 | CS42 | CS41 | CS40 |
    // TCCR5B  | ICNC5 | ICES5 | -- | WGM53 | WGM52 | CS52 | CS51 | CS50 |
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
    //   | WGM33 | WGM32 | WGM31 | WGM30 |  Timer3
    //   | WGM43 | WGM42 | WGM41 | WGM40 |  Timer4
    //   | WGM53 | WGM52 | WGM51 | WGM50 |  Timer5
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
    //   | D13 |  PB7 | Timer0 | 980 | OC0A | COM0A1 | COM0A0 |
    //   | D4  |  PG5 | Timer0 | 980 | OC0B | COM0B1 | COM0B0 |
    //   | D11 |  PB5 | Timer1 | 980 | OC1A | COM1A1 | COM1A0 |
    //   | D12 |  PB6 | Timer1 | 980 | OC1B | COM1B1 | COM1B0 |
    //   | D10 |  PB4 | Timer2 | 980 | OC2A | COM2A1 | COM2A0 |
    //   | D9  |  PH6 | Timer2 | 980 | OC2B | COM2B1 | COM2B0 |
    //   | D5  |  PE3 | Timer3 | 980 | OC3A | COM3A1 | COM3A0 |
    //   | D2  |  PE4 | Timer3 | 980 | OC3B | COM3B1 | COM3B0 |
    //   | D3  |  PE5 | Timer3 | 980 | OC3C | COM3C1 | COM3C0 |
    //   | D6  |  PH3 | Timer4 | 980 | OC4A | COM4A1 | COM4A0 |
    //   | D7  |  PH4 | Timer4 | 980 | OC4B | COM4B1 | COM4B0 |
    //   | D8  |  PH5 | Timer4 | 980 | OC4C | COM4C1 | COM4C0 |
    //   | D46 |  PL3 | Timer5 | 980 | OC5A | COM5A1 | COM5A0 |
    //   | D45 |  PL4 | Timer5 | 980 | OC5B | COM5B1 | COM5B0 |
    //   | D44 |  PL5 | Timer5 | 980 | OC5C | COM5C1 | COM5C0 |
    //   +-----+------+--------+-----+------+-----------------+

    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D13: // PB7 Timer0 OC0A (8-bit timer)
            DDRB   |= _BV(DDB7);               // Pin Mode Output
            TCCR0A |= _BV(WGM01) | _BV(WGM00); // Fast PWM mode
            TCCR0A |= _BV(COM0A1);             // Non-Inverting Mode
            TCCR0B |= _BV(CS01) | _BV(CS00);   // Clock/64
            break;

        case D4:  // PG5 Timer0 OC0B (8-bit timer)
            DDRG   |= _BV(DDG5);               // Pin Mode Output
            TCCR0A |= _BV(WGM01) | _BV(WGM00); // Fast PWM mode
            TCCR0A |= _BV(COM0B1);             // Non-Inverting Mode
            TCCR0B |= _BV(CS01) | _BV(CS00);   // Clock/64
            break;

        case D11: // PB5 Timer1 OC1A (16-bit timer)
            DDRB   |= _BV(DDB5);               // Pin Mode Output
            TCCR1B |= _BV(WGM12);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(WGM10);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(COM1A1);             // Non-Inverting Mode
            TCCR1B |= _BV(CS11) | _BV(CS10);   // Clock/64
            break;

        case D12: // PB6 Timer1 OC1B (16-bit timer)
            DDRB   |= _BV(DDB6);               // Pin Mode Output
            TCCR1B |= _BV(WGM12);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(WGM10);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR1A |= _BV(COM1B1);             // Non-Inverting Mode
            TCCR1B |= _BV(CS11) | _BV(CS10);   // Clock/64
            break;

        case D10: // PB4 Timer2 OC2A (8-bit timer)
            DDRB   |= _BV(DDB4);               // Pin Mode Output
            TCCR2A |= _BV(WGM21) | _BV(WGM20); // Fast PWM mode
            TCCR2A |= _BV(COM2A1);             // Non-Inverting Mode
            TCCR2B |= _BV(CS22);               // Clock/64
            break;

        case D9:  // PH6 Timer2 OC2B (8-bit timer)
            DDRH   |= _BV(DDH6);               // Pin Mode Output
            TCCR2A |= _BV(WGM21) | _BV(WGM20); // Fast PWM mode
            TCCR2A |= _BV(COM2B1);             // Non-Inverting Mode
            TCCR2B |= _BV(CS22);               // Clock/64
            break;

        case D5:  // PE3 Timer3 OC3A (16-bit timer)
            DDRE   |= _BV(DDE3);               // Pin Mode Output
            TCCR3B |= _BV(WGM32);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(WGM30);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(COM3A1);             // Non-Inverting Mode
            TCCR3B |= _BV(CS31) | _BV(CS30);   // Clock/64
            break;

        case D3:  // PE5 Timer3 OC3C (16-bit timer)
            DDRE   |= _BV(DDE5);               // Pin Mode Output
            TCCR3B |= _BV(WGM32);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(WGM30);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(COM3C1);             // Non-Inverting Mode
            TCCR3B |= _BV(CS31) | _BV(CS30);   // Clock/64
            break;

        case D2:  // PE4 Timer3 OC3B (16-bit timer)
            DDRE   |= _BV(DDE4);               // Pin Mode Output
            TCCR3B |= _BV(WGM32);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(WGM30);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR3A |= _BV(COM3B1);             // Non-Inverting Mode
            TCCR3B |= _BV(CS31) | _BV(CS30);   // Clock/64
            break;

        case D6:  // PH3 Timer4 OC4A (16-bit timer)
            DDRH   |= _BV(DDH3);               // Pin Mode Output
            TCCR4B |= _BV(WGM42);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(WGM40);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(COM4A1);             // Non-Inverting Mode
            TCCR4B |= _BV(CS41) | _BV(CS40);   // Clock/64
            break;

        case D7:  // PH4 Timer4 OC4B (16-bit timer)
            DDRH   |= _BV(DDH4);               // Pin Mode Output
            TCCR4B |= _BV(WGM42);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(WGM40);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(COM4B1);             // Non-Inverting Mode
            TCCR4B |= _BV(CS41) | _BV(CS40);   // Clock/64
            break;

        case D8:  // PH5 Timer4 OC4C (16-bit timer)
            DDRH   |= _BV(DDH5);               // Pin Mode Output
            TCCR4B |= _BV(WGM42);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(WGM40);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR4A |= _BV(COM4C1);             // Non-Inverting Mode
            TCCR4B |= _BV(CS41) | _BV(CS40);   // Clock/64
            break;

        case D46: // PL3 Timer5 OC5A (16-bit timer)
            DDRL   |= _BV(DDL3);               // Pin Mode Output
            TCCR5B |= _BV(WGM52);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(WGM50);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(COM5A1);             // Non-Inverting Mode
            TCCR5B |= _BV(CS51) | _BV(CS50);   // Clock/64
            break;

        case D45: // PL4 Timer5 OC5B (16-bit timer)
            DDRL   |= _BV(DDL4);               // Pin Mode Output
            TCCR5B |= _BV(WGM52);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(WGM50);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(COM5B1);             // Non-Inverting Mode
            TCCR5B |= _BV(CS51) | _BV(CS50);   // Clock/64
            break;

        case D44: // PL5 Timer5 OC5C (16-bit timer)
            DDRL   |= _BV(DDL5);               // Pin Mode Output
            TCCR5B |= _BV(WGM52);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(WGM50);              // Fast PWM Mode (8-bit, TOP 0x00FF)
            TCCR5A |= _BV(COM5C1);             // Non-Inverting Mode
            TCCR5B |= _BV(CS51) | _BV(CS50);   // Clock/64
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
        case D13: TCCR0A &= ~_BV(COM0A1); break; // Timer0
        case D4:  TCCR0A &= ~_BV(COM0B1); break; // Timer0
        case D11: TCCR1A &= ~_BV(COM1A1); break; // Timer1
        case D12: TCCR1A &= ~_BV(COM1B1); break; // Timer1
        case D10: TCCR2A &= ~_BV(COM2A1); break; // Timer2
        case D9:  TCCR2A &= ~_BV(COM2B1); break; // Timer2
        case D5:  TCCR3A &= ~_BV(COM3A1); break; // Timer3
        case D3:  TCCR3A &= ~_BV(COM3C1); break; // Timer3
        case D2:  TCCR3A &= ~_BV(COM3B1); break; // Timer3
        case D6:  TCCR4A &= ~_BV(COM4A1); break; // Timer4
        case D7:  TCCR4A &= ~_BV(COM4B1); break; // Timer4
        case D8:  TCCR4A &= ~_BV(COM4C1); break; // Timer4
        case D46: TCCR5A &= ~_BV(COM5A1); break; // Timer5
        case D45: TCCR5A &= ~_BV(COM5B1); break; // Timer5
        case D44: TCCR5A &= ~_BV(COM5C1); break; // Timer5
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
        case D13: OCR0A = dutyCycle; break;
        case D4:  OCR0B = dutyCycle; break;
        case D11: OCR1A = dutyCycle; break;
        case D12: OCR1B = dutyCycle; break;
        case D10: OCR2A = dutyCycle; break;
        case D9:  OCR2B = dutyCycle; break;
        case D5:  OCR3A = dutyCycle; break;
        case D3:  OCR3C = dutyCycle; break;
        case D2:  OCR3B = dutyCycle; break;
        case D6:  OCR4A = dutyCycle; break;
        case D7:  OCR4B = dutyCycle; break;
        case D8:  OCR4C = dutyCycle; break;
        case D46: OCR5A = dutyCycle; break;
        case D45: OCR5B = dutyCycle; break;
        case D44: OCR5C = dutyCycle; break;
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
        case D13: dutyCycle = OCR0A; break;
        case D4:  dutyCycle = OCR0B; break;
        case D11: dutyCycle = OCR1A; break;
        case D12: dutyCycle = OCR1B; break;
        case D10: dutyCycle = OCR2A; break;
        case D9:  dutyCycle = OCR2B; break;
        case D5:  dutyCycle = OCR3A; break;
        case D3:  dutyCycle = OCR3C; break;
        case D2:  dutyCycle = OCR3B; break;
        case D6:  dutyCycle = OCR4A; break;
        case D7:  dutyCycle = OCR4B; break;
        case D8:  dutyCycle = OCR4C; break;
        case D46: dutyCycle = OCR5A; break;
        case D45: dutyCycle = OCR5B; break;
        case D44: dutyCycle = OCR5C; break;
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
    A, B, C, D, E, F, G, H, J, K, L
};

// Define arrays as const (not static const) to specify
// external linkage.  Prevents duplicate definitions if
// header file is included by multiple source modules.

const ports_t PROGMEM ports[] = {
    { &PORTA, &DDRA, &PINA },
    { &PORTB, &DDRB, &PINB },
    { &PORTC, &DDRC, &PINC },
    { &PORTD, &DDRD, &PIND },
    { &PORTE, &DDRE, &PINE },
    { &PORTF, &DDRF, &PINF },
    { &PORTG, &DDRG, &PING },
    { &PORTH, &DDRH, &PINH },
    { &PORTJ, &DDRJ, &PINJ },
    { &PORTK, &DDRK, &PINK },
    { &PORTL, &DDRL, &PINL }
};

const pins_t PROGMEM pins[] = {
    { (uint8_t) Port::E, 0 }, // D0
    { (uint8_t) Port::E, 1 }, // D1
    { (uint8_t) Port::E, 4 }, // D2
    { (uint8_t) Port::E, 5 }, // D3
    { (uint8_t) Port::G, 5 }, // D4
    { (uint8_t) Port::E, 3 }, // D5
    { (uint8_t) Port::H, 3 }, // D6
    { (uint8_t) Port::H, 4 }, // D7
    { (uint8_t) Port::H, 5 }, // D8
    { (uint8_t) Port::H, 6 }, // D9
    { (uint8_t) Port::B, 4 }, // D10
    { (uint8_t) Port::B, 5 }, // D11
    { (uint8_t) Port::B, 6 }, // D12
    { (uint8_t) Port::B, 7 }, // D13
    { (uint8_t) Port::J, 1 }, // D14
    { (uint8_t) Port::J, 0 }, // D15
    { (uint8_t) Port::H, 1 }, // D16
    { (uint8_t) Port::H, 0 }, // D17
    { (uint8_t) Port::D, 3 }, // D18
    { (uint8_t) Port::D, 2 }, // D19
    { (uint8_t) Port::D, 1 }, // D20
    { (uint8_t) Port::D, 0 }, // D21
    { (uint8_t) Port::A, 0 }, // D22
    { (uint8_t) Port::A, 1 }, // D23
    { (uint8_t) Port::A, 2 }, // D24
    { (uint8_t) Port::A, 3 }, // D25
    { (uint8_t) Port::A, 4 }, // D26
    { (uint8_t) Port::A, 5 }, // D27
    { (uint8_t) Port::A, 6 }, // D28
    { (uint8_t) Port::A, 7 }, // D29
    { (uint8_t) Port::C, 7 }, // D30
    { (uint8_t) Port::C, 6 }, // D31
    { (uint8_t) Port::C, 5 }, // D32
    { (uint8_t) Port::C, 4 }, // D33
    { (uint8_t) Port::C, 3 }, // D34
    { (uint8_t) Port::C, 2 }, // D35
    { (uint8_t) Port::C, 1 }, // D36
    { (uint8_t) Port::C, 0 }, // D37
    { (uint8_t) Port::D, 7 }, // D38
    { (uint8_t) Port::G, 2 }, // D39
    { (uint8_t) Port::G, 1 }, // D40
    { (uint8_t) Port::G, 0 }, // D41
    { (uint8_t) Port::L, 7 }, // D42
    { (uint8_t) Port::L, 6 }, // D43
    { (uint8_t) Port::L, 5 }, // D44
    { (uint8_t) Port::L, 4 }, // D45
    { (uint8_t) Port::L, 3 }, // D46
    { (uint8_t) Port::L, 2 }, // D47
    { (uint8_t) Port::L, 1 }, // D48
    { (uint8_t) Port::L, 0 }, // D49
    { (uint8_t) Port::B, 3 }, // D50
    { (uint8_t) Port::B, 2 }, // D51
    { (uint8_t) Port::B, 1 }, // D52
    { (uint8_t) Port::B, 0 }, // D53
    { (uint8_t) Port::F, 0 }, // D54
    { (uint8_t) Port::F, 1 }, // D55
    { (uint8_t) Port::F, 2 }, // D56
    { (uint8_t) Port::F, 3 }, // D57
    { (uint8_t) Port::F, 4 }, // D58
    { (uint8_t) Port::F, 5 }, // D59
    { (uint8_t) Port::F, 6 }, // D60
    { (uint8_t) Port::F, 7 }, // D61
    { (uint8_t) Port::K, 0 }, // D62
    { (uint8_t) Port::K, 1 }, // D63
    { (uint8_t) Port::K, 2 }, // D64
    { (uint8_t) Port::K, 3 }, // D65
    { (uint8_t) Port::K, 4 }, // D66
    { (uint8_t) Port::K, 5 }, // D67
    { (uint8_t) Port::K, 6 }, // D68
    { (uint8_t) Port::K, 7 }  // D69
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

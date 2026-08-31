// pinDefs.h

#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "coreError.h"

#if defined(BOARD_arduino_nano_every)

#define D0   0 // PC5 RX
#define D1   1 // PC4 TX
#define D2   2 // PA0
#define D3   3 // PF5
#define D4   4 // PC6
#define D5   5 // PB2
#define D6   6 // PF4
#define D7   7 // PA1
#define D8   8 // PE3
#define D9   9 // PB0
#define D10 10 // PB1
#define D11 11 // PE0
#define D12 12 // PE1
#define D13 13 // PE2
#define D14 14 // PD3
#define D15 15 // PD2
#define D16 16 // PD1
#define D17 17 // PD0
#define D18 18 // PA2/PF2 (Port A for I2C, Port F for Analog/Digital)
#define D19 19 // PA3/PF3 (Port A for I2C, Port F for Analog/Digital)
#define D20 20 // PD4
#define D21 21 // PD5

#define LED_BUILTIN D13

#define A0 0 // PD3
#define A1 1 // PD2
#define A2 2 // PD1
#define A3 3 // PD0
#define A4 4 // PA2/PF2 (Port A for I2C, Port F for Analog/Digital)
#define A5 5 // PA3/PF3 (Port A for I2C, Port F for Analog/Digital)
#define A6 6 // PD4
#define A7 7 // PD5

inline bool isDigPin(uint8_t pin)
{
    return pin <= D21;
}

inline bool isPwmPin(uint8_t pin)
{
    return pin == D3 ||
           pin == D5 ||
           pin == D6 ||
           pin == D9 ||
           pin == D10;
}

inline bool isAdcPin(uint8_t pin)
{
    return pin <= A7;
}

inline void pwmEnable(uint8_t pin)
{
    uint8_t entrySREG = SREG;

    cli();

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | // Clock Prescaler DIV64
                        TCA_SINGLE_ENABLE_bm;        // Enable Timer

    switch (pin)
    {
        case D9:  // PB0 Timer A TCA0 Compare Channel 0 (976.6 Hz)
            PORTB.DIRSET       |= PIN0_bm;               // Pin Mode Output
            PORTMUX.TCAROUTEA  |= PORTMUX_TCA0_PORTB_gc; // Route TCA0 to Port B
            TCA0.SINGLE.PERBUF  = 255;                   // PWM TOP 8-bit Resolution
            TCA0.SINGLE.CMP0BUF = 0;                     // Initial PWM Duty Cycle
            TCA0.SINGLE.CTRLB  |=
                TCA_SINGLE_WGMODE_SINGLESLOPE_gc |       // Single-Slope PWM Mode
                TCA_SINGLE_CMP0EN_bm;                    // Enable Compare Channel
            break;

        case D10: // PB1 Timer A TCA0 Compare Channel 1 (976.6 Hz)
            PORTB.DIRSET       |= PIN1_bm;               // Pin Mode Output
            PORTMUX.TCAROUTEA  |= PORTMUX_TCA0_PORTB_gc; // Route TCA0 to Port B
            TCA0.SINGLE.PERBUF  = 255;                   // PWM TOP 8-bit Resolution
            TCA0.SINGLE.CMP1BUF = 0;                     // Initial PWM Duty Cycle
            TCA0.SINGLE.CTRLB  |=
                TCA_SINGLE_WGMODE_SINGLESLOPE_gc |       // Single-Slope PWM Mode
                TCA_SINGLE_CMP1EN_bm;                    // Enable Compare Channel
            break;

        case D5:  // PB2 Timer A TCA0 Compare Channel 2 (976.6 Hz)
            PORTB.DIRSET       |= PIN2_bm;               // Pin Mode Output
            PORTMUX.TCAROUTEA  |= PORTMUX_TCA0_PORTB_gc; // Route TCA0 to Port B
            TCA0.SINGLE.PERBUF  = 255;                   // PWM TOP 8-bit Resolution
            TCA0.SINGLE.CMP2BUF = 0;                     // Initial PWM Duty Cycle
            TCA0.SINGLE.CTRLB  |=
                TCA_SINGLE_WGMODE_SINGLESLOPE_gc |       // Single-Slope PWM Mode
                TCA_SINGLE_CMP2EN_bm;                    // Enable Compare Channel
            break;

        case D6:  // PF4 Timer B TCB0 (976.6 Hz)
            PORTF.DIRSET |= PIN4_bm;              // Pin Mode Output
            PORTMUX.TCBROUTEA |= PORTMUX_TCB0_bm; // Route TCB0 to PF4
            TCB0.CTRLA = 0;                       // Disable Timer
            TCB0.CCMPL = 255;                     // PWM TOP 8-bit Resolution
            TCB0.CCMPH = 0;                       // Initial PWM Duty Cycle
            TCB0.CTRLB = TCB_CNTMODE_PWM8_gc |    // PWM Mode 8-bit
                         TCB_CCMPEN_bm;           // Enable Compare Channel
            TCB0.CTRLA = TCB_CLKSEL_CLKTCA_gc |   // Use TCA0 as Source Clock
                         TCB_ENABLE_bm;           // Enable Timer
            break;

        case D3:  // PF5 Timer B TCB1 (976.6 Hz)
            PORTF.DIRSET |= PIN5_bm;              // Pin Mode Output
            PORTMUX.TCBROUTEA |= PORTMUX_TCB1_bm; // Route TCB1 to PF5
            TCB1.CTRLA = 0;                       // Disable Timer
            TCB1.CCMPL = 255;                     // PWM TOP 8-bit Resolution
            TCB1.CCMPH = 0;                       // Initial PWM Duty Cycle
            TCB1.CTRLB = TCB_CNTMODE_PWM8_gc |    // PWM Mode 8-bit
                         TCB_CCMPEN_bm;           // Enable Compare Channel
            TCB1.CTRLA = TCB_CLKSEL_CLKTCA_gc |   // Use TCA0 as Source Clock
                         TCB_ENABLE_bm;           // Enable Timer
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
        case D9:  TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP0EN_bm; break;
        case D10: TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP1EN_bm; break;
        case D5:  TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP2EN_bm; break;
        case D6:  TCB0.CTRLB &= ~TCB_CCMPEN_bm; break;
        case D3:  TCB1.CTRLB &= ~TCB_CCMPEN_bm; break;
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
        case D9:  TCA0.SINGLE.CMP0BUF = dutyCycle; break;
        case D10: TCA0.SINGLE.CMP1BUF = dutyCycle; break;
        case D5:  TCA0.SINGLE.CMP2BUF = dutyCycle; break;
        case D6:  TCB0.CCMPH = dutyCycle; break;
        case D3:  TCB1.CCMPH = dutyCycle; break;
        default:  coreError = ERR_PIN;
    }

    SREG = entrySREG;
}

inline uint8_t pwmGetDutyCycle(uint8_t pin)
{
    uint8_t notUsed;
    (void)  notUsed;

    uint8_t dutyCycle = 0;
    uint8_t entrySREG = SREG;

    cli();

    switch (pin)
    {
        case D9:  dutyCycle = TCA0.SINGLE.CMP0BUF; break;
        case D10: dutyCycle = TCA0.SINGLE.CMP1BUF; break;
        case D5:  dutyCycle = TCA0.SINGLE.CMP2BUF; break;
        case D6:  // Must read CCMPL first for valid CCMPH.
                  notUsed   = TCB0.CCMPL;
                  dutyCycle = TCB0.CCMPH; break;
        case D3:  // Must read CCMPL first for valid CCMPH.
                  notUsed   = TCB1.CCMPL;
                  dutyCycle = TCB1.CCMPH; break;
        default:  coreError = ERR_PIN;
    }

    SREG = entrySREG;

    return dutyCycle;
}

typedef struct {
    volatile uint8_t *port_dir;
    volatile uint8_t *port_out;
    volatile uint8_t *port_inp;
} ports_t;

typedef struct {
    uint8_t port;
    uint8_t bit;
    volatile uint8_t *port_ctl;
} pins_t;

enum class Port {
    A, B, C, D, E, F
};

// Define arrays as const (not static const) to specify
// external linkage.  Prevents duplicate definitions if
// header file is included by multiple source modules.

const ports_t PROGMEM ports[] = {
    { &PORTA.DIR, &PORTA.OUT, &PORTA.IN },
    { &PORTB.DIR, &PORTB.OUT, &PORTB.IN },
    { &PORTC.DIR, &PORTC.OUT, &PORTC.IN },
    { &PORTD.DIR, &PORTD.OUT, &PORTD.IN },
    { &PORTE.DIR, &PORTE.OUT, &PORTE.IN },
    { &PORTF.DIR, &PORTF.OUT, &PORTF.IN },
};

const pins_t PROGMEM pins[] = {
    { (uint8_t) Port::C, 5, &PORTC.PIN5CTRL }, // D0  PC5 RX
    { (uint8_t) Port::C, 4, &PORTC.PIN4CTRL }, // D1  PC4 TX
    { (uint8_t) Port::A, 0, &PORTA.PIN0CTRL }, // D2  PA0
    { (uint8_t) Port::F, 5, &PORTF.PIN5CTRL }, // D3  PF5
    { (uint8_t) Port::C, 6, &PORTC.PIN6CTRL }, // D4  PC6
    { (uint8_t) Port::B, 2, &PORTB.PIN2CTRL }, // D5  PB2
    { (uint8_t) Port::F, 4, &PORTF.PIN4CTRL }, // D6  PF4
    { (uint8_t) Port::A, 1, &PORTA.PIN1CTRL }, // D7  PA1
    { (uint8_t) Port::E, 3, &PORTE.PIN3CTRL }, // D8  PE3
    { (uint8_t) Port::B, 0, &PORTB.PIN0CTRL }, // D9  PB0
    { (uint8_t) Port::B, 1, &PORTB.PIN1CTRL }, // D10 PB1
    { (uint8_t) Port::E, 0, &PORTE.PIN0CTRL }, // D11 PE0
    { (uint8_t) Port::E, 1, &PORTE.PIN1CTRL }, // D12 PE1
    { (uint8_t) Port::E, 2, &PORTE.PIN2CTRL }, // D13 PE2
    { (uint8_t) Port::D, 3, &PORTD.PIN3CTRL }, // D14 PD3
    { (uint8_t) Port::D, 2, &PORTD.PIN2CTRL }, // D15 PD2
    { (uint8_t) Port::D, 1, &PORTD.PIN1CTRL }, // D16 PD1
    { (uint8_t) Port::D, 0, &PORTD.PIN0CTRL }, // D17 PD0
    { (uint8_t) Port::F, 2, &PORTF.PIN2CTRL }, // D18 PF2
    { (uint8_t) Port::F, 3, &PORTF.PIN3CTRL }, // D19 PF3
    { (uint8_t) Port::D, 4, &PORTD.PIN4CTRL }, // D20 PD4
    { (uint8_t) Port::D, 5, &PORTD.PIN5CTRL }, // D21 PD5
};

inline uint8_t getPortIndex(uint8_t pin)
{
    return pgm_read_byte(&pins[pin].port);
}

inline volatile uint8_t *getPortDir(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_dir);
}

inline volatile uint8_t *getPortOut(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_out);
}

inline volatile uint8_t *getPortInp(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&ports[getPortIndex(pin)].port_inp);
}

inline volatile uint8_t *getPortCtl(uint8_t pin)
{
    return (uint8_t *) pgm_read_word(&pins[pin].port_ctl);
}

inline uint8_t getPortBit(uint8_t pin)
{
    return pgm_read_byte(&pins[pin].bit);
}

#else
    #error "Unexpected MCU"
#endif

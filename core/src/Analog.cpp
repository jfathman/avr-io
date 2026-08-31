// Analog.cpp

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Analog.h"
#include "coreError.h"

void Analog::init(uint8_t vref)
{
    coreError = ERR_NONE;

    uint8_t entrySREG = SREG;

    cli();

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        // Prescaler 16 Mhz / 128 = 125 kHz for ADC clock.
        ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));

        _reference(vref);

        if (coreError == ERR_NONE)
        {
            ADCSRA |= _BV(ADEN);
        }
    #endif

    #if defined(__AVR_ATmega4809__)
        // Prescaler 16 MHz / 32 = 500 kHz for ADC clock.
        ADC0.CTRLC = ADC_PRESC_DIV32_gc; 

        _reference(vref);

        if (coreError == ERR_NONE)
        {
            ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc; 
        }
    #endif

    SREG = entrySREG;
}

void Analog::_reference(uint8_t vref)
{
    uint8_t entrySREG = SREG;

    cli();

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        if (vref == VREF_EXTERNAL) // S1:S0 = 00
        {
            ADMUX &= ~(_BV(REFS1) | _BV(REFS0));
        }
        else if (vref == VREF_VCC) // S1:S0 = 01
        {
            ADMUX &= ~_BV(REFS1);
            ADMUX |=  _BV(REFS0);
        }
        #if defined(__AVR_ATmega328P__)
        else if (vref == VREF_INTERNAL) // S1:S0 = 11
        {
            ADMUX |= _BV(REFS1) | _BV(REFS0);
        }
        #elif defined(__AVR_ATmega2560__)
        else if (vref == VREF_INTERNAL  // S1:S0 = 10
              || vref == VREF_INT_1V1)
        {
            ADMUX |=  _BV(REFS1);
            ADMUX &= ~_BV(REFS0);
        }
        else if (vref == VREF_INT_2V56) // S1:S0 = 11
        {
            ADMUX |= _BV(REFS1) | _BV(REFS0);
        }
        #endif
        else
        {
            coreError = ERR_VREF;
        }
    #endif

    #if defined(__AVR_ATmega4809__)
        if (vref == VREF_EXTERNAL)
        {
            ADC0.CTRLC |= (ADC0.CTRLC & ~ADC_REFSEL_gm) | ADC_REFSEL_VREFA_gc;
        }
        else if (vref == VREF_VCC)
        {
            ADC0.CTRLC |= ADC_REFSEL_VDDREF_gc;
        }
        else
        {
            switch (vref)
            {
                case VREF_INTERNAL: VREF.CTRLA = VREF_ADC0REFSEL_1V1_gc;  break;
                case VREF_INT_0V55: VREF.CTRLA = VREF_ADC0REFSEL_0V55_gc; break;
                case VREF_INT_1V1:  VREF.CTRLA = VREF_ADC0REFSEL_1V1_gc;  break;
                case VREF_INT_1V5:  VREF.CTRLA = VREF_ADC0REFSEL_1V5_gc;  break;
                case VREF_INT_2V5:  VREF.CTRLA = VREF_ADC0REFSEL_2V5_gc;  break;
                case VREF_INT_4V34: VREF.CTRLA = VREF_ADC0REFSEL_4V34_gc; break;
                default:
                    coreError = ERR_VREF;
                    SREG = entrySREG;
                    return;
            }

            ADC0.CTRLC |= ADC_REFSEL_INTREF_gc;
        }
    #endif

    SREG = entrySREG;
}

uint16_t Analog::read(uint8_t channel)
{
    coreError = ERR_NONE;

    if (!isAdcPin(channel))
    {
        coreError = ERR_CHANNEL;
        return 0;
    }

    uint8_t entrySREG = SREG;

    cli();

    #if defined(BOARD_arduino_uno_r3)
        ADMUX &= 0xF0;
        ADMUX |= (channel & 0x0F);
    #endif

    #if defined(BOARD_arduino_mega_2560)
        ADMUX &= 0xF0;
        ADMUX |= (channel & 0x07);
        if (channel < A8)
        {
            ADCSRB &= ~_BV(MUX5);
        }
        else
        {
            ADCSRB |= _BV(MUX5);
        }
    #endif

    #if defined(BOARD_arduino_uno_r3)
        DDRC  &= ~_BV(channel);     // Data direction input
        PORTC &= ~_BV(channel);     // Disable internal pull-up
        DIDR0 |=  _BV(channel);     // Disable digital input buffer
    #endif

    #if defined(BOARD_arduino_mega_2560)
        if (channel < A8)
        {
            DDRF  &= ~_BV(channel); // Data direction input
            PORTF &= ~_BV(channel); // Disable internal pull-up
            DIDR0 |=  _BV(channel); // Disable digital input buffer
        }
        else
        {
            uint8_t ch = channel & 0x07;
            DDRK  &= ~_BV(ch);      // Data direction input
            PORTK &= ~_BV(ch);      // Disable internal pull-up
            DIDR2 |=  _BV(ch);      // Disable digital input buffer
        }
    #endif

    #if defined(BOARD_arduino_nano_every)
        const uint8_t pin = D14 + channel;
        uint8_t port_bit = getPortBit(pin);
        volatile uint8_t *port_dir = getPortDir(pin);
        volatile uint8_t *port_ctl = getPortCtl(pin);

        *port_dir &= ~_BV(port_bit);             // Data direction input
        *port_ctl &= ~PORT_PULLUPEN_bm;          // Disable internal pull-up
        *port_ctl &= ~PORT_ISC_gm;               // Disable digital input buffer
        *port_ctl |=  PORT_ISC_INPUT_DISABLE_gc; // Disable digital input buffer

        switch (channel)
        {
            case A0: ADC0.MUXPOS = ADC_MUXPOS_AIN3_gc;  break;
            case A1: ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;  break;
            case A2: ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc;  break;
            case A3: ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc;  break;
            case A4: ADC0.MUXPOS = ADC_MUXPOS_AIN12_gc; break;
            case A5: ADC0.MUXPOS = ADC_MUXPOS_AIN13_gc; break;
            case A6: ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;  break;
            case A7: ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;  break;
            default:
                 coreError = ERR_CHANNEL;
                 SREG = entrySREG;
                 return 0;
        }
    #endif

    SREG = entrySREG;

    #if defined(BOARD_arduino_uno_r3) || \
        defined(BOARD_arduino_mega_2560)
        // Start conversion
        ADCSRA |= _BV(ADSC);

        // Wait for conversion to complete (~104 uSec)
        while (ADCSRA & _BV(ADSC))
            ;
    
        return ADCW;
    #endif

    #if defined(BOARD_arduino_nano_every)
        // Start converion
        ADC0.COMMAND = ADC_STCONV_bm;
        
        // Wait for conversion to complete (~14 uSec)
        while (!(ADC0.INTFLAGS & ADC_RESRDY_bm))
            ;
        
        // Clear ready flag
        ADC0.INTFLAGS = ADC_RESRDY_bm;

        return ADC0.RES;
    #endif
}

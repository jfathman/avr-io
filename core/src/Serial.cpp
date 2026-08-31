// Serial.cpp

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "Serial.h"

#if defined(__AVR_ATmega4809__)
#define USART_BAUD_VALUE(baud) (((4UL * F_CPU) + ((baud) / 2)) / (baud))
#endif

static int uart_getchar(FILE *stream)
{
    (void) stream;

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        return (UCSR0A & _BV(RXC0)) ? UDR0 : EOF;
    #endif

    #if defined(__AVR_ATmega4809__)
        return (USART3.STATUS & USART_RXCIF_bm) ? USART3.RXDATAL : EOF;
    #endif
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
    {
        uart_putchar('\r', stream);
    }

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        while (!(UCSR0A & _BV(UDRE0)))
            ;
        UDR0 = c;
    #endif

    #if defined(__AVR_ATmega4809__)
        while (!(USART3.STATUS & USART_DREIF_bm))
            ;
        USART3.TXDATAL = c;
    #endif

    return 0;
}

static FILE uart_input  = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void Serial::init(uint32_t baudrate)
{
    uint8_t entrySREG = SREG;

    cli();
 
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        UBRR0 = ((F_CPU / 8 + baudrate / 2) / baudrate - 1);

        UCSR0A |= _BV(U2X0);
        UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
        UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
    #endif

    #if defined(__AVR_ATmega4809__)
        // Route USART3 ALT1 pins (PB4 = TX, PB5 = RX) to SAMD11 USB serial.
        PORTMUX.USARTROUTEA &= ~PORTMUX_USART3_gm;
        PORTMUX.USARTROUTEA |=  PORTMUX_USART3_ALT1_gc;

        // Must set the TX pin high (UART marking condition) before
        // configuring it as an output because an initial low (UART
        // space condition) acts as a serial data start bit and the
        // first byte transmit after startup or reset is corrupt.
        PORTB.OUTSET = PIN4_bm; // Set TX (PB4) to initial high.

        PORTB.DIRSET = PIN4_bm; // Configure TX (PB4) as output.
        PORTB.DIRCLR = PIN5_bm; // Configure RX (PB5) as input.

        USART3.BAUD = (uint16_t) USART_BAUD_VALUE(baudrate);

        USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc // Async
                     | USART_PMODE_DISABLED_gc     // No parity
                     | USART_CHSIZE_8BIT_gc        // 8 data bits
                     | USART_SBMODE_1BIT_gc;       // 1 stop bit

        /*
        // Currently using polled serial rx, not interrtupt driven.
        USART3.CTRLA = USART_RXCIE_bm; // Enable receive interrupt.
        */

        USART3.CTRLB = USART_TXEN_bm   // Enable transmit.
                     | USART_RXEN_bm;  // Enable receive.
    #endif

    stdin  = &uart_input;
    stdout = &uart_output;
    stderr = &uart_output;

    SREG = entrySREG;
}

bool Serial::available()
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega2560__)
        return UCSR0A & _BV(RXC0);
    #endif

    #if defined(__AVR_ATmega4809__)
        return USART3.STATUS & USART_RXCIF_bm;
    #endif
}

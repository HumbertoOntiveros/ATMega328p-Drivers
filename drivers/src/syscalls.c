#include <stdio.h>
#include <stddef.h>
#include <avr/io.h>

typedef long ssize_t;

void UART_Init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(char data)
{
    while (!(UCSR0A & (_BV(UDRE0))) )
        ;
    UDR0 = data;
}

ssize_t _write(char var, FILE *stream)
{
    // translate \n to \r for br@y++ terminal
    //if (var == '\n') usart_putchar('\r');
    UART_Transmit(var);
    return 0;
    
}

// Custom FILE structure to direct stdout to UART
FILE uart_stdout = FDEV_SETUP_STREAM(_write, NULL, _FDEV_SETUP_WRITE);

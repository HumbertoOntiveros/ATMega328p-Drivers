#include <stdio.h>
#include <unistd.h>
#include <avr/io.h>

// Configuración del UART (igual que antes)
#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

void UART_Init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

// Implementación de syscalls para write()
ssize_t _write(int file, const char *ptr, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        UART_Transmit(ptr[i]);
    }
    return len;
}

/*/ Implementaciones de otras syscalls si es necesario
int _close(int file) { return -1; }
int _fstat(int file, struct stat *st) { return -1; }
int _isatty(int file) { return 1; }
off_t _lseek(int file, off_t offset, int whence) { return (off_t)-1; }
ssize_t _read(int file, char *ptr, size_t len) { return -1; }
*/
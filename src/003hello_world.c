/*
 * 003hello_world.c
 *
 * Created: 08/09/2024 14:59:20
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates a simple UART application where a "Hello, World!" message
 * is printed to the serial console every second.
 * 
 */

#include <stdio.h>

// Configuración del UART (igual que antes)
#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

extern uart_stdout;

void Delay_ms(uint32_t ms)
{
    // Assuming the ATmega328P has a 16 MHz clock
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms

    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

void UART_Init(unsigned int ubrr);

int main(void)
{
     UART_Init(MY_UBRR);

     stdout = &uart_stdout;  // Redirige stdout a UART

    while (1)
    {
        printf("Hello, World!\r\n");
        Delay_ms(1000);
    }

    return 0;
}

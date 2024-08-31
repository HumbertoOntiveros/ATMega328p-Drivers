/*
 * prueba.c
 *
 * Created: 20/01/2019 22:48:13
 * Author : marlon
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

int main(void);

int main(void)
{
    /* Replace with your application code */

	DDRB |= (1<<DDB5);
	
    while (1) 
    {
		PORTB ^= (1<<PORTB5);
		_delay_ms(5000);
    }
}

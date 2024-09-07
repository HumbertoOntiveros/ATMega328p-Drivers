/*
 * 001led_toggle.c
 *
 * Created: 30/09/2024 21:58:10
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates a simple GPIO application where a LED connected to pin PB5
 * toggles its state (ON/OFF) at regular intervals. The LED is configured as an output, 
 * and a delay function is used to create a visible blinking effect.
 *
 */ 
 
#include "atmega328p_gpio.h"

void delay(void)
{
	for(volatile uint32_t i = 0 ; i < 50000 ; i ++);
}

int main(void)
{
    GPIO_t LED;

    LED.GPIOX           = GPIOB; 
    LED.GPIO_Pin.Number = PIN5;
    LED.GPIO_Pin.Mode   = MODE_OUT;
    LED.GPIO_Pin.PullUp = PULLUP_DISABLED;

    GPIO_Init(LED);

    while (1)
    {
        GPIO_TogglePin(LED);
        delay();
    }

    return 0;
}

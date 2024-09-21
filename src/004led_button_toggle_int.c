/*
 * 004led_button_toggle_int.c
 *
 * Created: 19/09/2024
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates how to use pin change interrupts to toggle an LED (PB5)
 * each time a button (PB4) is pressed. The button is configured with an internal pull-up resistor.
 * The program detects a falling edge interrupt on the button pin and toggles the LED state accordingly.
 * A software delay is added to prevent bouncing issues.
 */

#include "atmega328p_gpio.h"

GPIO_t Led, Button;

void Delay_ms(uint32_t ms)
{
    // Assuming the ATmega328P has a 16 MHz clock
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms

    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

int main(void)
{

    Led.GPIOX           = GPIOB; 
    Led.GPIO_Pin.Number = PIN5;
    Led.GPIO_Pin.Mode   = MODE_OUT;
    Led.GPIO_Pin.PullUp = PULLUP_DISABLED;
   
    GPIO_Init(Led);

    Button.GPIOX           = GPIOB; 
    Button.GPIO_Pin.Number = PIN4;
    Button.GPIO_Pin.Mode   = MODE_IN;
    Button.GPIO_Pin.PullUp = PULLUP_ENABLED;

    GPIO_Init(Button);

    // Enable pin change interrupts on PB4
    GPIO_EnableInterrupt(&Button);

    // Configure the interrupt for falling edge detection
    GPIO_ConfigInterrupt(&Button, INT_FALLING_EDGE);

    // Enable global interrupts
    IRQ_EN();
    
    while (1);

    return 0;
}

ISR(ISR_PCINT0)
{
    if (!GPIO_ReadPin(Button))  // Check if button is pressed (falling edge)
    {
        GPIO_TogglePin(Led);  // Toggle LED when button is pressed
    }
    Delay_ms(300);
}
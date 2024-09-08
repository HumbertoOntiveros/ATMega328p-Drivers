/*
 * 002led_button_toggle.c
 *
 * Created: 06/09/2024 21:54:45
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates a simple GPIO application where a LED connected to pin PB5
 * toggles its state (ON/OFF) each time a button connected to pin PB4 is pressed.
 * The button is configured with a pull-up resistor enabled, and the LED is initially off.
 */

#include "atmega328p_gpio.h"
-
#define LED_ON       1
#define LED_OFF      0
#define BUTTON_HIGH  1
#define BUTTOM_LOW   0

void delay(void)
{
	for(volatile uint32_t i = 0 ; i < 80000 ; i ++);
}

int main(void)
{
    GPIO_t Led, Button;
    uint8_t LedCurrentState = LED_OFF;
    uint8_t ButtonCurrentState, ButtonPreviousState;

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

    ButtonPreviousState = GPIO_ReadPin(Button);

    while (1)
    {
        ButtonCurrentState = GPIO_ReadPin(Button);

        // Check if the button has been pressed (falling edge detection)
        if(!ButtonCurrentState && ButtonPreviousState)
        {
            // Toggle the LED
            if(LedCurrentState == LED_OFF)
            {
                GPIO_WritePin(Led, 1);
                LedCurrentState = LED_ON;
            }
            else
            {
                GPIO_WritePin(Led, 0);
                LedCurrentState = LED_OFF;
            }
        }
        
        // Update the previous state
        ButtonPreviousState = ButtonCurrentState;

        delay();

    }

    return 0;
}

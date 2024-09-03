/*
 * @file              atmega328p_gpio.c
 *
 * @brief             GPIO driver implementation for ATmega328P microcontroller.
 *
 * @details           This file provides the implementation of functions to initialize, 
 *                    de-initialize, read, write, and toggle GPIO pins for the ATmega328P.
 *                    It defines all the APIs to manage GPIO peripherals including
 *                    initialization, data handling, and toggling operations.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              September 1, 2024
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "atmega328p_gpio.h"

/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - Initializes the specified GPIO pin with the given settings.
 *
 * @param[in]         - pGPIO: Pointer to a GPIO_t structure containing the pin configuration.
 *
 * @return            - None
 *
 * @Note              - This function should be called before using the GPIO pin for any operation.
 */
void GPIO_Init(GPIO_t PORTX)
{
    //set direction of the pin
    *PORTX.GPIOX.DDR &= ~(1 << PORTX.GPIO_Pin.Number);//clearing
    *PORTX.GPIOX.DDR |= (PORTX.GPIO_Pin.Mode << PORTX.GPIO_Pin.Number);//setting

    //set pullup if Mode=input else clear output pin
    *PORTX.GPIOX.PORT &= ~(1 << PORTX.GPIO_Pin.Number);//clearing
    *PORTX.GPIOX.PORT |= PORTX.GPIO_Pin.Mode ? \
                            ~(1 << PORTX.GPIO_Pin.Number): \
                            (PORTX.GPIO_Pin.PullUp << PORTX.GPIO_Pin.Number);
    /*
    //DDRB |= (1<<DDB5);
    *PORTX.GPIOX.DDR |= (1 << PORTX.GPIO_Pin.Number);
    */
}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - De-initializes the specified GPIO pin, resetting it to its default state.
 *
 * @param[in]         - pGPIO: Pointer to a GPIO_t structure containing the pin to be de-initialized.
 *
 * @return            - None
 *
 * @Note              - This function resets the GPIO pin to its default input state with no pull-up.
 */
void GPIO_DeInit(GPIO_t GPIOX)
{

}

/*********************************************************************
 * @fn      		  - GPIO_ReadPin
 *
 * @brief             - Reads the current state of the specified GPIO pin.
 *
 * @param[in]         - pGPIO: Pointer to a GPIO_t structure containing the pin to be read.
 *
 * @return            - uint8_t: Returns 1 if the pin is high, 0 if the pin is low.
 *
 * @Note              - Ensure the pin is configured correctly before reading.
 */
uint8_t GPIO_ReadPin(GPIO_t *pGPIO)
{
    return 0;
}

/*********************************************************************
 * @fn      		  - GPIO_WritePin
 *
 * @brief             - Sets or clears the specified GPIO pin.
 *
 * @param[in]         - pGPIO: Pointer to a GPIO_t structure containing the pin to be written to.
 * @param[in]         - Value: Value to write to the pin (1 for high, 0 for low).
 *
 * @return            - None
 *
 * @Note              - Make sure the pin is configured as output before writing.
 */
void GPIO_WritePin(GPIO_t *pGPIO, uint8_t Value)
{

}

/*********************************************************************
 * @fn      		  - GPIO_TogglePin
 *
 * @brief             - Toggles the current state of the specified GPIO pin.
 *
 * @param[in]         - pGPIO: Pointer to a GPIO_t structure containing the pin to be toggled.
 *
 * @return            - None
 *
 * @Note              - This function inverts the current state of the pin.
 */
void GPIO_TogglePin(GPIO_t PORTX)
{
    // Check if the pin is configured as an output
    ((*PORTX.GPIOX.DDR & (1 << PORTX.GPIO_Pin.Number)) != 0) ?
        // If it is an output, toggle the pin state using the PIN register
        (*PORTX.GPIOX.PIN |= (1 << PORTX.GPIO_Pin.Number)) :
        // If it is not an output, do nothing
        (void)0;
        
    /*
    *PORTX.GPIOX.PORT ^= (1<<PORTX.GPIO_Pin.Number);
    */
}

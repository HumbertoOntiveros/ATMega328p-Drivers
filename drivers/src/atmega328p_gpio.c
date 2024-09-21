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
 * @param[in]         - GPIO_t structure containing the pin configuration.
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

}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - De-initializes the specified GPIO pin, resetting it to its default state.
 *
 * @param[in]         - GPIO_t structure containing the pin to be de-initialized.
 *
 * @return            - None
 *
 * @Note              - This function resets the GPIO pin to its default input state with no pull-up.
 */
void GPIO_DeInit(GPIO_t GPIOX)
{
    // Reset the pin's direction to input
    *GPIOX.GPIOX.DDR &= ~(1 << GPIOX.GPIO_Pin.Number);

    // Disable pull-up resistors by setting the pin to low
    *GPIOX.GPIOX.PORT &= ~(1 << GPIOX.GPIO_Pin.Number);
}

/*********************************************************************
 * @fn      		  - GPIO_ReadPin
 *
 * @brief             - Reads the current state of the specified GPIO pin.
 *
 * @param[in]         - GPIO_t structure containing the pin to be read.
 *
 * @return            - uint8_t: Returns 1 if the pin is high, 0 if the pin is low.
 *
 * @Note              - Ensure the pin is configured correctly before reading.
 */
uint8_t GPIO_ReadPin(GPIO_t PORTX)
{
    // Check if the pin is configured as an input
    return (!(*PORTX.GPIOX.DDR & (1 << PORTX.GPIO_Pin.Number))) ?
        // If it is an input, return the pin state using the PIN register
        ((*PORTX.GPIOX.PIN & (1 << PORTX.GPIO_Pin.Number)) != 0) :
        // If it is not an input, return 0
        0;
}

/*********************************************************************
 * @fn      		  - GPIO_WritePin
 *
 * @brief             - Sets or clears the specified GPIO pin.
 *
 * @param[in]         - GPIO_t structure containing the pin to be written to.
 * @param[in]         - Value: Value to write to the pin (1 for high, 0 for low).
 *
 * @return            - None
 *
 * @Note              - Make sure the pin is configured as output before writing.
 */
void GPIO_WritePin(GPIO_t PORTX, uint8_t Value)
{
    // Check if the pin is configured as an output
    (*PORTX.GPIOX.DDR & (1 << PORTX.GPIO_Pin.Number)) ?
        // If it is an output, set the pin state to "Value" using the PORT register
        ((Value) ? 
            (*PORTX.GPIOX.PORT |= (1 << PORTX.GPIO_Pin.Number)) : 
            (*PORTX.GPIOX.PORT &= ~(1 << PORTX.GPIO_Pin.Number))
        ) :
        // If it is not an output, do nothing
        (void)0;
}

/*********************************************************************
 * @fn      		  - GPIO_TogglePin
 *
 * @brief             - Toggles the current state of the specified GPIO pin.
 *
 * @param[in]         - GPIO_t structure containing the pin to be toggled.
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
        
}

/********************************************************************* 
 * @fn      		  - GPIO_EnableInterrupt
 *
 * @brief             - Enables the interrupt for the specified GPIO pin.
 *
 * @param[in]         - GPIO_t structure containing the pin for which the interrupt is to be enabled.
 *
 * @return            - None
 *
 * @Note              - This function must be called to allow the specified pin to trigger interrupts.
 */
void GPIO_EnableInterrupt(GPIO_t* PORTX) {

    if (PORTX->GPIOX.DDR == GPIOD.DDR)
    {
        if (PORTX->GPIO_Pin.Number == PIN2) // INT0 (External Interrupt)
        {
            INT_EIMSK_REG |= (1 << EIMSK_INT0);  // Enable INT0 interrupt
        }
        else if (PORTX->GPIO_Pin.Number == PIN3) // INT1 (External Interrupt)
        {
            INT_EIMSK_REG |= (1 << EIMSK_INT1);  // Enable INT1 interrupt
        }
        else
        {
            // PD0, PD1, PD4-PD7 use pin change interrupts (PCINT)
            PCINT_PCICR_REG |= (1 << PCICR_PCIE2);  // Enable PCINT for Port D
            PCINT_PCMSK2_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt in Port D
        }
    }
    else if (PORTX->GPIOX.DDR == GPIOB.DDR)
    {
        PCINT_PCICR_REG |= (1 << PCICR_PCIE0);  // Enable PCINT for Port B
        PCINT_PCMSK0_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt in Port B
    }
    else if (PORTX->GPIOX.DDR == GPIOC.DDR)
    {
        PCINT_PCICR_REG |= (1 << PCICR_PCIE1);  // Enable PCINT for Port C
        PCINT_PCMSK1_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt in Port C
    }
}

/********************************************************************* 
 * @fn      		  - GPIO_DisableInterrupt
 *
 * @brief             - Disables the interrupt for the specified GPIO pin.
 *
 * @param[in]         - GPIO_t structure containing the pin for which the interrupt is to be disabled.
 *
 * @return            - None
 *
 * @Note              - This function stops the specified pin from triggering interrupts.
 */
void GPIO_DisableInterrupt(GPIO_t* PORTX) {

    if (PORTX->GPIOX.DDR == GPIOD.DDR)
    {
        if (PORTX->GPIO_Pin.Number == PIN2) // INT0 (External Interrupt)
        {
            INT_EIMSK_REG &= ~(1 << EIMSK_INT0);  // Disable INT0 interrupt
        }
        else if (PORTX->GPIO_Pin.Number == PIN3) // INT1 (External Interrupt)
        {
            INT_EIMSK_REG &= ~(1 << EIMSK_INT1);  // Disable INT1 interrupt
        }
        else
        {
            // Disable specific pin change interrupt for Port D
            PCINT_PCMSK2_REG &= ~(1 << PORTX->GPIO_Pin.Number);
        }
    }
    else if (PORTX->GPIOX.DDR == GPIOB.DDR)
    {
        PCINT_PCMSK0_REG &= ~(1 << PORTX->GPIO_Pin.Number);  // Disable specific pin interrupt in Port B
    }
    else if (PORTX->GPIOX.DDR == GPIOC.DDR)
    {
        PCINT_PCMSK1_REG &= ~(1 << PORTX->GPIO_Pin.Number);  // Disable specific pin interrupt in Port C
    }
}

/********************************************************************* 
 * @fn      		  - GPIO_ConfigInterrupt
 *
 * @brief             - Configures the trigger mode for the specified GPIO pin interrupt.
 *
 * @param[in]         - GPIO_t structure containing the pin to be configured.
 * @param[in]         - Trigger mode (e.g., rising edge, falling edge).
 *
 * @return            - None
 *
 * @Note              - This function sets the behavior of the interrupt for the specified pin.
 */
void GPIO_ConfigInterrupt(GPIO_t* PORTX, uint8_t trigger) {

    if (PORTX->GPIOX.DDR == GPIOD.DDR)
    {
        if (PORTX->GPIO_Pin.Number == PIN2) // INT0 (External Interrupt)
        {
            INT_EICRA_REG &= ~(3 << EICRA_ISC00);        // Clear the trigger bits for INT0
            INT_EICRA_REG |= (trigger << EICRA_ISC00);   // Set the trigger type for INT0
        }
        else if (PORTX->GPIO_Pin.Number == PIN3) // INT1 (External Interrupt)
        {
            INT_EICRA_REG &= ~(3 << EICRA_ISC10);        // Clear the trigger bits for INT1
            INT_EICRA_REG |= (trigger << EICRA_ISC10);   // Set the trigger type for INT1
        }
        else
        {
            // PD0, PD1, PD4-PD7 use pin change interrupts (PCINT)
            PCINT_PCICR_REG |= (1 << PCICR_PCIE2);       // Enable PCINT for Port D
            PCINT_PCMSK2_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt for Port D
        }
    }
    else if (PORTX->GPIOX.DDR == GPIOB.DDR)
    {
        // Port B pins use pin change interrupts (PCINT)
        PCINT_PCICR_REG |= (1 << PCICR_PCIE0);           // Enable PCINT for Port B
        PCINT_PCMSK0_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt for Port B
    }
    else if (PORTX->GPIOX.DDR == GPIOC.DDR)
    {
        // Port C pins use pin change interrupts (PCINT)
        PCINT_PCICR_REG |= (1 << PCICR_PCIE1);           // Enable PCINT for Port C
        PCINT_PCMSK1_REG |= (1 << PORTX->GPIO_Pin.Number);  // Enable specific pin interrupt for Port C
    }
}


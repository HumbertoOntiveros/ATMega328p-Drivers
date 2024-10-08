#ifndef __ATMEGA328P_GPIO_H__
#define __ATMEGA328P_GPIO_H__

#include "atmega328p.h"

/*
 * Configuration structure for a GPIO pin
 */
typedef struct
{
    uint8_t Number; /*!< possible values from @GPIO_PIN_NUMBERS         >*/
    uint8_t Mode;   /*!< possible values from @GPIO_PIN_MODES           >*/
    uint8_t PullUp; /*!< possible values from @GPIO_PULLUP_CONFIG       >*/
    uint8_t AltFun; /*!< possible values from @GPIO_ALTERNATE_FUNCTIONS >*/
} GPIO_Pin_t;

/*
 * Handle structure for a GPIO pin
 */
typedef struct
{
    GPIO_Regs_t GPIOX;
    GPIO_Pin_t  GPIO_Pin;
} GPIO_t;

/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define PIN0  				0
#define PIN1  				1
#define PIN2  				2
#define PIN3  				3
#define PIN4  				4
#define PIN5  				5
#define PIN6  				6
#define PIN7  				7

/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define MODE_IN 		    0
#define MODE_OUT 		    1
#define MODE_ALTFN 	        2

/*
 * @GPIO_PULLUP_CONFIG
 * GPIO pin possible modes
 */
#define PULLUP_DISABLED     0
#define PULLUP_ENABLED      1

/*
 * @INT_SENSE_CONTROL
 * Interrupt sense control configuration
 */
#define INT_LOW_LEVEL       0x00  // The low level generates an interrupt request.
#define INT_LOGICAL_CHANGE  0x01  // Any logical change generates an interrupt request.
#define INT_FALLING_EDGE    0x02  // The falling edge generates an interrupt request.
#define INT_RISING_EDGE     0x03  // The rising edge generates an interrupt request.

/*
 * @GPIO_ALTERNATE_FUNCTIONS
 * GPIO pin possible alternate functions
 */

/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_t PORTX);
void GPIO_DeInit(GPIO_t PORTX);

/*
 * Data read and write
 */
uint8_t GPIO_ReadPin(GPIO_t PORTX);
void GPIO_WritePin(GPIO_t PORTX, uint8_t Value);
void GPIO_TogglePin(GPIO_t PORTX);

/*
 * APIs for enabling, disabling, and configuring 
 * GPIO pin interrupts for ISR handling.
 */
void GPIO_EnableInterrupt(GPIO_t* GPIOX);
void GPIO_DisableInterrupt(GPIO_t* GPIOX);
void GPIO_ConfigInterrupt(GPIO_t* GPIOX, uint8_t trigger);

#endif // __ATMEGA328P_GPIO_H__

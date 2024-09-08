#ifndef __ATMEGA328P_H__
#define __ATMEGA328P_H__

#include<stddef.h>
#include<stdint.h>

/******************************************************************************************
 *                                  MCU Specific Details                                  *
 ******************************************************************************************/

/*
 * GPIO Registers for ATmega328P
 * Base addresses of GPIO peripherals.
 */

#define GPIO_MCUCR_REG_ADDR ((volatile uint8_t*)0x55)

/* 
 * PORT B
 */
#define GPIO_DDRB_REG_ADDR   ((volatile uint8_t *)0x24)  // Data Direction Register for Port B
#define GPIO_PORTB_REG_ADDR  ((volatile uint8_t *)0x25)  // Data Register for Port B
#define GPIO_PINB_REG_ADDR   ((volatile uint8_t *)0x23)  // Pin Input Register for Port B

/* 
 * PORT C
 */
#define GPIO_DDRC_REG_ADDR   ((volatile uint8_t *)0x27)  // Data Direction Register for Port C
#define GPIO_PORTC_REG_ADDR  ((volatile uint8_t *)0x28)  // Data Register for Port C
#define GPIO_PINC_REG_ADDR   ((volatile uint8_t *)0x26)  // Pin Input Register for Port C

/* 
 * PORT D
 */
#define GPIO_DDRD_REG_ADDR   ((volatile uint8_t *)0x2A)  // Data Direction Register for Port D
#define GPIO_PORTD_REG_ADDR  ((volatile uint8_t *)0x2B)  // Data Register for Port D
#define GPIO_PIND_REG_ADDR   ((volatile uint8_t *)0x29)  // Pin Input Register for Port D


/******************************************************************************************
 *                        Peripheral Register Definition Structures                       *
 ******************************************************************************************/

/*
 * peripheral register definition structure for GPIO
 */
typedef struct 
{
    volatile uint8_t *MCUCR;
    volatile uint8_t *DDR;
    volatile uint8_t *PORT;
    volatile uint8_t *PIN;
} GPIO_Regs_t;


/******************************************************************************************
 *                                  Peripheral Definitions                                *
 ******************************************************************************************/
#define GPIOB  ((GPIO_Regs_t){   \
    .MCUCR = GPIO_MCUCR_REG_ADDR,\
    .DDR   = GPIO_DDRB_REG_ADDR, \
    .PORT  = GPIO_PORTB_REG_ADDR,\
    .PIN   = GPIO_PINB_REG_ADDR,\
})

#define GPIOC ((GPIO_Regs_t){\
    .MCUCR = GPIO_MCUCR_REG_ADDR,\
    .DDR   = GPIO_DDRC_REG_ADDR, \
    .PORT  = GPIO_PORTC_REG_ADDR,\
    .PIN   = GPIO_PINC_REG_ADDR, \
})

#define GPIOD ((GPIO_Regs_t){\
    .MCUCR = GPIO_MCUCR_REG_ADDR,\
    .DDR   = GPIO_DDRD_REG_ADDR, \
    .PORT  = GPIO_PORTD_REG_ADDR,\
    .PIN   = GPIO_PIND_REG_ADDR, \
})

/******************************************************************************************
 *                         Bit position definitions of peripherals                        *
 ******************************************************************************************/
 
/*
 * Bit position definitions of GPIO peripherals
 */

/*
 * Bit position definitions GPIO_MCUCR
 */
#define MCUCR_IVCE  0
#define MCUCR_IVSEL 1
#define MCUCR_PUD   4
#define MCUCR_BODSE 5
#define MCUCR_BODS  6

/*
 * Generic Macros Definition
 */
#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET         RESET
#define FLAG_SET 			SET

#endif // __ATMEGA328P_H__

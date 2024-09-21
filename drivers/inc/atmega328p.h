#ifndef __ATMEGA328P_H__
#define __ATMEGA328P_H__

#include<stddef.h>
#include<stdint.h>

/******************************************************************************************
 *                                  MCU Specific Details                                  *
 ******************************************************************************************/

/*
 * Interrupt Vector Definitions for ATmega328P
 *
 * This section defines the interrupt vector numbers for the ATmega328P microcontroller.
 * These vectors are used to handle different types of interrupts and events.
 * Each vector corresponds to a specific interrupt source as outlined in the ATmega328P datasheet.
 * 
 * Use these definitions to associate interrupt service routines (ISRs) with their respective vectors.
 * Ensure that the ISR functions are declared with the `ISR` macro and match the vector definitions below.
 * 
 * Interrupt Vector Assignments:
 * - __vector_1: External Interrupt Request 0 (INT0)
 * - __vector_2: External Interrupt Request 1 (INT1)
 * - __vector_3: Pin Change Interrupt Request 0 (PCINT0)
 * - __vector_4: Pin Change Interrupt Request 1 (PCINT1)
 * - __vector_5: Pin Change Interrupt Request 2 (PCINT2)
 * - __vector_6: Watchdog Timer Interrupt (WDT)
 * - __vector_7: Timer/Counter2 Compare Match A (TIMER2_COMPA)
 * - __vector_8: Timer/Counter2 Compare Match B (TIMER2_COMPB)
 * - __vector_9: Timer/Counter2 Overflow (TIMER2_OVF)
 * - __vector_10: Timer/Counter1 Capture Event (TIMER1_CAPT)
 * - __vector_11: Timer/Counter1 Compare Match A (TIMER1_COMPA)
 * - __vector_12: Timer/Counter1 Compare Match B (TIMER1_COMPB)
 * - __vector_13: Timer/Counter1 Overflow (TIMER1_OVF)
 * - __vector_14: Timer/Counter0 Compare Match A (TIMER0_COMPA)
 * - __vector_15: Timer/Counter0 Compare Match B (TIMER0_COMPB)
 * - __vector_16: Timer/Counter0 Overflow (TIMER0_OVF)
 * - __vector_17: SPI Serial Transfer Complete (SPI_STC)
 * - __vector_18: USART Receive Complete (USART_RX)
 * - __vector_19: USART Data Register Empty (USART_UDRE)
 * - __vector_20: USART Transmit Complete (USART_TX)
 * - __vector_21: ADC Conversion Complete (ADC)
 * - __vector_22: EEPROM Ready (EE_READY)
 * - __vector_23: Analog Comparator (ANALOG_COMP)
 * - __vector_24: Two-Wire Serial Interface (TWI)
 * - __vector_25: Store Program Memory Read (SPM_READY)
 *
 * To define an ISR for an interrupt, use the ISR macro with the appropriate vector definition.
 * Example:
 *   ISR(ISR_INT0) {
 *       // Handle External Interrupt Request 0 here
 *   }
 *
 * Make sure to configure the microcontroller's interrupt system properly in your code
 * to enable and handle these interrupts as needed.
 */

#define ISR_INT0          __vector_1
#define ISR_INT1          __vector_2
#define ISR_PCINT0        __vector_3
#define ISR_PCINT1        __vector_4
#define ISR_PCINT2        __vector_5
#define ISR_WDT           __vector_6
#define ISR_TIMER2_COMPA  __vector_7
#define ISR_TIMER2_COMPB  __vector_8
#define ISR_TIMER2_OVF    __vector_9
#define ISR_TIMER1_CAPT   __vector_10
#define ISR_TIMER1_COMPA  __vector_11
#define ISR_TIMER1_COMPB  __vector_12
#define ISR_TIMER1_OVF    __vector_13
#define ISR_TIMER0_COMPA  __vector_14
#define ISR_TIMER0_COMPB  __vector_15
#define ISR_TIMER0_OVF    __vector_16
#define ISR_SPI_STC       __vector_17
#define ISR_USART_RX      __vector_18
#define ISR_USART_UDRE    __vector_19
#define ISR_USART_TX      __vector_20
#define ISR_ADC           __vector_21
#define ISR_EE_READY      __vector_22
#define ISR_ANALOG_COMP   __vector_23
#define ISR_TWI           __vector_24
#define ISR_SPM_READY     __vector_25

/*
 *ISR handler function with required attributes for interrupt handling
 */

#define ISR(handler)     __attribute__ ((signal, used, externally_visible)) void handler(void)

/*
 * Interrupt Control
 */
#define IRQ_EN()  __asm__ volatile ("sei")  // Enable global interrupts
#define IRQ_DIS() __asm__ volatile ("cli")  // Disable global interrupts

/******************************************************************************************
 *                         Peripheral Register Addresses                                  *
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


/* 
 * External Interrupt Control Registers
 */
#define INT_EICRA_REG        (*(volatile uint8_t *)0x69)  // External Interrupt Control Register A
#define INT_EIMSK_REG        (*(volatile uint8_t *)0x3D)  // External Interrupt Mask Register
#define PCINT_PCICR_REG      (*(volatile uint8_t *)0x68)  // Pin Change Interrupt Control Register
#define PCINT_PCMSK0_REG     (*(volatile uint8_t *)0x6B)  // Pin Change Mask Register 0
#define PCINT_PCMSK1_REG     (*(volatile uint8_t *)0x6C)  // Pin Change Mask Register 1
#define PCINT_PCMSK2_REG     (*(volatile uint8_t *)0x6D)  // Pin Change Mask Register 2

/* 
 * External Interrupt Flag Registers
 */
#define INT_EIFR_REG         (*(volatile uint8_t *)0x3C)  // External Interrupt Flag Register
#define PCINT_PCIFR_REG      (*(volatile uint8_t *)0x3B)  // Pin Change Interrupt Flag Register

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
 * Bit position definitions of External Interrupts
 */

/*
 * Bit position definitions for INT_EICRA_REG
 */
#define EICRA_ISC00   0  // Interrupt Sense Control 0 Bit 0
#define EICRA_ISC01   1  // Interrupt Sense Control 0 Bit 1
#define EICRA_ISC10   2  // Interrupt Sense Control 1 Bit 0
#define EICRA_ISC11   3  // Interrupt Sense Control 1 Bit 1

/*
 * Bit position definitions for INT_EIMSK_REG
 */
#define EIMSK_INT0    0  // External Interrupt Request 0 Enable
#define EIMSK_INT1    1  // External Interrupt Request 1 Enable

/*
 * Bit position definitions for PCINT_PCICR_REG
 */
#define PCICR_PCIE0   0  // Pin Change Interrupt Enable 0
#define PCICR_PCIE1   1  // Pin Change Interrupt Enable 1
#define PCICR_PCIE2   2  // Pin Change Interrupt Enable 2

/*
 * Bit position definitions for INT_EIFR_REG
 */
#define EIFR_INTF0     0  // External Interrupt Flag 0
#define EIFR_INTF1     1  // External Interrupt Flag 1

/*
 * Bit position definitions for PCINT_PCIFR_REG
 */
#define PCIFR_PCIF0    0  // Pin Change Interrupt Flag 0
#define PCIFR_PCIF1    1  // Pin Change Interrupt Flag 1
#define PCIFR_PCIF2    2  // Pin Change Interrupt Flag 2


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
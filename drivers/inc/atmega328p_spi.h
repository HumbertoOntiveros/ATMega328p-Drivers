/*
 * atmega328p_spi.c
 *
 * Created: 11/10/2024 19:34:00
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Driver for ATmega328P SPI peripheral. Supports initialization, data transfer, 
 * and configuration of mode, clock settings, and data order. Provides simple 
 * APIs for efficient SPI communication.
 *
 */

#ifndef __ATMEGA328P_SPI_H__
#define __ATMEGA328P_SPI_H__

#include "atmega328p.h"
#include "atmega328p_gpio.h"

/******************************************************************************************
 *                                  Driver's Specific Details                             *
 ******************************************************************************************/
/*
 * Configuration structure for SPI peripheral
 */
typedef struct
{
    uint8_t Mode;
    uint8_t DataOrder;
    uint8_t CPOL;
    uint8_t CPHA;
    uint8_t SCKSpeed;
}SPI_Config_t;

/*
 * Handle structure for a SPI peripheral
 */
typedef struct
{
    SPI_Regs_t    *pReg;
    SPI_Config_t  Config;
   	uint8_t 	  *pTxBuffer;   /* !< To store the app. Tx buffer address > */
	uint8_t 	  *pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 	  TxLen;		/* !< To store Tx len > */
	uint32_t 	  RxLen;		/* !< To store Tx len > */
	uint8_t 	  TxState;	    /* !< To store Tx state > */
	uint8_t 	  RxState;	    /* !< To store Rx state > */
}SPI_t;

/*
 * SPI possible modes
 */
#define SPI_MODE_SLAVE 		    0
#define SPI_MODE_MASTER		    1

/*
 * SPI data order options
 */
 #define SPI_ORDER_MSB          0
 #define SPI_ORDER_LSB          1

 /*
 * SPI Clock Polarity(CPOL) options
 */
 #define SPI_CPOL_LOW           0                 
 #define SPI_CPOL_HIGH          1

/*
 * SPI Clock Phase (CPHA) options
 */
#define SPI_CPHA_LEADING        0
#define SPI_CPHA_TRAILING       1 

/*
 * SPI Clock Rate Selection options
 */
#define SPI_SCLK_FOSC_DIV4            0 //SPI2X = 0
#define SPI_SCLK_FOSC_DIV16           1 //SPI2X = 0
#define SPI_SCLK_FOSC_DIV64           2 //SPI2X = 0
#define SPI_SCLK_FOSC_DIV128          3 //SPI2X = 0
#define SPI_SCLK_FOSC_DIV2            4 //SPI2X = 1
#define SPI_SCLK_FOSC_DIV8            5 //SPI2X = 1
#define SPI_SCLK_FOSC_DIV32           6 //SPI2X = 1
#define SPI_SCLK_FOSC_DIV64_SPI2X     7 //SPI2X = 1

/*
 * SPI pins definition for the ATmega328P
 */
#define SPI_MOSI_PIN    PIN3  // PB3
#define SPI_MISO_PIN    PIN4  // PB4
#define SPI_SCK_PIN     PIN5  // PB5
#define SPI_SS_PIN      PIN2  // PB2

/*
 * SPI port pins
 */
#define SPI_GPIO_PORT   GPIOB

/*
 * SPI Pins definition
 */
#define SPI_MOSI {                 \
    .GPIOX = SPI_GPIO_PORT,        \
     .GPIO_Pin = {                 \
        .Number = SPI_MOSI_PIN,    \
        .Mode = MODE_OUT,          \
        .PullUp = PULLUP_DISABLED, \
        .AltFun = MODE_ALTFN       \
        }                          \
    }

#define SPI_MISO {                 \
    .GPIOX = SPI_GPIO_PORT,        \
    .GPIO_Pin = {                  \
        .Number = SPI_MISO_PIN,    \
        .Mode = MODE_OUT,          \
        .PullUp = PULLUP_DISABLED, \
        .AltFun = MODE_ALTFN       \
        }                          \
    }

#define SPI_SCK {                  \
    .GPIOX = SPI_GPIO_PORT,        \
    .GPIO_Pin = {                  \
        .Number = SPI_SCK_PIN,     \
        .Mode = MODE_OUT,          \
        .PullUp = PULLUP_DISABLED, \
        .AltFun = MODE_ALTFN       \
        }                          \
    }

#define SPI_SS {                   \
    .GPIOX = SPI_GPIO_PORT,        \
    .GPIO_Pin = {                  \
        .Number = SPI_SS_PIN,      \
        .Mode = MODE_OUT,          \
        .PullUp = PULLUP_DISABLED, \
        .AltFun = MODE_ALTFN       \
        }                          \
    }

/*
 * SPI application states
 */
#define SPI_READY 					0
#define SPI_BUSY_IN_RX 				1
#define SPI_BUSY_IN_TX 				2

/*
 * Possible SPI Application events
 */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2

/*
 * Generic Macros Definition
 */
 #define SPI_SPI2X_DIS_MASK        0X03
 
/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/

/*
 * Init and De-init
 */
 void SPI_Init(SPI_t   *pSPIInst);
 void SPI_Deinit(SPI_t *pSPIInst);

 /*
 * Data Send and Receive
 */
void SPI_SendData(SPI_t   *pSPIInst,uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_t   *pSPIInst, uint8_t *pRxBuffer, uint32_t Len);

 /*
 * Data Send and Receive with interruption
 */
uint8_t SPI_SendDataIT(SPI_t   *pSPIInst,uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_t   *pSPIInst, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(SPI_t *pSPIInst, uint8_t EnorDi);
void SPI_IRQHandling(SPI_t *pSPIInst);
static void  spi_txe_interrupt_handle(SPI_t *pSPIInst);
static void  spi_rxne_interrupt_handle(SPI_t *pSPIInst);

/*
 * Other Peripheral Control APIs and Macro control definition
 */
void SPI_Control(SPI_t *pSPIInst, uint8_t state);
#define SPI_SlaveControl(gpio, state) GPIO_WritePin((gpio), (state))

#endif // __ATMEGA328P_SPI_H__

/*
 * MIT License
 *
 * Copyright (c) 2024 HumbertoOntiveros
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Happy coding!
 */
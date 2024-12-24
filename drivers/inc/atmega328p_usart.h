/*
 * atmega328p_usart.c
 *
 * Created: 21/12/2024 21:16:06
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Driver for configuring and using the USART peripheral on ATmega328P. 
 * Supports initialization, data transfer (blocking and interrupt-based), 
 * and customizable settings for baud rate, parity, word length, and stop bits.
 * Includes APIs for error handling and application callbacks.
 *
 */

#ifndef __ATMEGA328P_USART_H__
#define __ATMEGA328P_USART_H__

#include "atmega328p.h"

/******************************************************************************************
 *                                  Driver's Specific Details                                  *
 ******************************************************************************************/
/*
 * Configuration structure for USART peripheral
 */
typedef struct 
{
    uint8_t  USART_Mode;
    uint8_t  USART_NoOfStopBits;
    uint8_t  USART_WordLength;
    uint8_t  USART_ParityControl;
    uint16_t USART_Baud;
}USART_Config_t;

/*
 * Handle structure for a USART peripheral
 */
typedef struct
{
    USART_Regs_t    *pReg;
    USART_Config_t   Config;
    uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 	0
#define USART_MODE_ONLY_RX 	1
#define USART_MODE_TXRX  	2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */			
#define USART_STD_BAUD_2400     0x01A0  // UBRRn: 416 (2400 bps)
#define USART_STD_BAUD_4800     0x00CF  // UBRRn: 207 (4800 bps)
#define USART_STD_BAUD_9600     0x0067  // UBRRn: 103 (9600 bps)
#define USART_STD_BAUD_14400    0x0044  // UBRRn: 68  (14400 bps)
#define USART_STD_BAUD_19200    0x0033  // UBRRn: 51  (19200 bps)
#define USART_STD_BAUD_28800    0x0022  // UBRRn: 34  (28800 bps)
#define USART_STD_BAUD_38400    0x0019  // UBRRn: 25  (38400 bps)
#define USART_STD_BAUD_57600    0x0010  // UBRRn: 16  (57600 bps)
#define USART_STD_BAUD_76800    0x000C  // UBRRn: 12  (76800 bps)
#define USART_STD_BAUD_115200   0x0008  // UBRRn: 8   (115200 bps)
#define USART_STD_BAUD_230400   0x0003  // UBRRn: 3   (230400 bps)
#define USART_STD_BAUD_250000   0x0003  // UBRRn: 3   (250000 bps)
#define USART_STD_BAUD_500000   0x0001  // UBRRn: 1   (500000 bps)
#define USART_STD_BAUD_1M       0x0000  // UBRRn: 0   (1M bps)

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD     3
#define USART_PARITY_EN_EVEN    2
#define USART_PARITY_DISABLE    0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_5BITS     0
#define USART_WORDLEN_6BITS     1
#define USART_WORDLEN_7BITS     2
#define USART_WORDLEN_8BITS     3
#define USART_WORDLEN_9BITS     4

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1        0
#define USART_STOPBITS_2        1

/*
 * USART flags
 */
#define USART_FLAG_TXE 			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		( 1 << USART_SR_RXNE)
#define USART_FLAG_TC 			( 1 << USART_SR_TC)

/*
 * Application states
 */
#define USART_BUSY_IN_RX       1
#define USART_BUSY_IN_TX       2
#define USART_READY            0


#define 	USART_EVENT_TX_CMPLT   0
#define		USART_EVENT_RX_CMPLT   1
#define		USART_EVENT_IDLE       2
#define		USART_EVENT_CTS        3
#define		USART_EVENT_PE         4
#define		USART_ERR_FE     	   5
#define		USART_ERR_NE    	   6
#define		USART_ERR_ORE    	   7

/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/
/*
 * Init and De-init
 */
void USART_Init(USART_t *pUSARTInst);
void USART_DeInit(USART_t *pUSARTInst);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_t *pUSARTInst, uint8_t *pTxBuffer, uint32_t Len);
void  USART_ReceiveData(USART_t *pUSARTInst,uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_t *pUSARTInst,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_t *pUSARTInst,uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQHandling(USART_t *pUSARTInst);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_Regs_t *pUSARTRegs, uint8_t EnOrDi);

/*
 * Application Callbacks
 */
void USART_ApplicationEventCallback(USART_t *pUSARTInst,uint8_t ApEv);

#endif // __ATMEGA328P_USART_H__
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
/*
 * @file              atmega328p_usart.c
 *
 * @brief             USART driver implementation for ATmega328P microcontroller.
 *
 * @details           Driver for configuring and using the USART peripheral on ATmega328P. 
 *                    Supports initialization, data transfer (blocking and interrupt-based), 
 *                    and customizable settings for baud rate, parity, word length, and stop bits.
 *                    Includes APIs for error handling and application callbacks.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              21/12/2024 21:16:06
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "atmega328p_usart.h"

/*********************************************************************
 * @fn            - USART_Init
 *
 * @brief         - Initializes the USART peripheral with the specified configurations.
 *                 This includes setting the mode (e.g., TX, RX), number of stop bits,
 *                 word length, parity control, and baud rate as defined in the configuration structure.
 *                 
 *                 The baud rate is configured considering a system clock of 16 MHz.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 *
 * @return        - None
 *
 * @Note          - Ensure that the USART clock is enabled before calling this function.
 */
void USART_Init(USART_t *pUSARTInst)
{
    // Configure the USART mode (RX, TX, or RX/TX).
    uint8_t ucsr0b = 0;
    if (pUSARTInst->Config.USART_Mode != USART_MODE_ONLY_TX)
        ucsr0b |= (1 << USART_UCSR0B_RXEN0);  // Enable RX if needed.
    if (pUSARTInst->Config.USART_Mode != USART_MODE_ONLY_RX)
        ucsr0b |= (1 << USART_UCSR0B_TXEN0);  // Enable TX if needed.
    pUSARTInst->pReg->UCSR0B = ucsr0b;

    // Configure the USART word length (5, 6, 7, or 8 bits).
    pUSARTInst->pReg->UCSR0C = (pUSARTInst->Config.USART_WordLength & 0x03) << USART_UCSR0C_UCSZ00;

    // If 9 bits word length, set the UCSZ02 bit.
    if (pUSARTInst->Config.USART_WordLength == USART_WORDLEN_9BITS)
        pUSARTInst->pReg->UCSR0B |= (1 << USART_UCSR0B_UCSZ02);
    else
        pUSARTInst->pReg->UCSR0B &= ~(1 << USART_UCSR0B_UCSZ02);

    // Configure the number of stop bits.
    if (pUSARTInst->Config.USART_NoOfStopBits == USART_STOPBITS_2)
        pUSARTInst->pReg->UCSR0C |= (1 << USART_UCSR0C_USBS0);
    else
        pUSARTInst->pReg->UCSR0C &= ~(1 << USART_UCSR0C_USBS0);

    // Configure the parity control (even/odd/no parity).
    pUSARTInst->pReg->UCSR0C |= (pUSARTInst->Config.USART_ParityControl & 0x30) << USART_UCSR0C_UPM00;

    // Configure the USART baud rate.
    // The baud rate is calculated based on the system clock (16 MHz).
    uint16_t baud = pUSARTInst->Config.USART_Baud;
    pUSARTInst->pReg->UBRR0L = (uint8_t)(baud & 0x00FF);
    pUSARTInst->pReg->UBRR0H = (uint8_t)((baud >> 8) & 0x0F);
}


/*********************************************************************
 * @fn            - USART_DeInit
 *
 * @brief         - Deinitializes the USART peripheral and resets its registers.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 *
 * @return        - None
 *
 * @Note          - This function resets the USART to its default state.
 */
void USART_DeInit(USART_t *pUSARTInst)
{
    // Reset all USART registers to zero
    pUSARTInst->pReg->UCSR0B = 0x00;
    pUSARTInst->pReg->UCSR0C = 0x00;
    pUSARTInst->pReg->UBRR0L = 0x00;
    pUSARTInst->pReg->UBRR0H = 0x00;
}

/*********************************************************************
 * @fn            - USART_SendData
 *
 * @brief         - Sends data via USART in blocking mode.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 * @param[in]     - pTxBuffer: Pointer to the transmission buffer.
 * @param[in]     - Len: Length of the data to be sent.
 *
 * @return        - None
 *
 * @Note          - Blocks until all data is sent.
 */
void USART_SendData(USART_t *pUSARTInst, uint8_t *pTxBuffer, uint32_t Len)
{
    // Loop through the data to be sent
    for(uint32_t i = 0; i < Len; i++)
    {
        // Wait until the transmit buffer is ready for new data
        while(!(pUSARTInst->pReg->UCSR0A & (1 << USART_UCSR0A_UDRE0)));

        // Handle 9-bit data transmission
        if(pUSARTInst->Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            // Set TXB80 bit based on the 9th data bit (MSB)
            pUSARTInst->pReg->UCSR0B = (pUSARTInst->pReg->UCSR0B & ~(1 << USART_UCSR0B_TXB80)) |
                                       ((*pTxBuffer & 0x0100) >> 8); // TXB80 is the MSB of the data
        }

        // Transmit the lower 8 bits of the data
        pUSARTInst->pReg->UDR0 = (uint8_t)(*pTxBuffer & 0xFF);

        // Move to the next byte in the buffer
        pTxBuffer++;
    }
}

/*********************************************************************
 * @fn            - USART_ReceiveData
 *
 * @brief         - Receives data via USART in blocking mode.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 * @param[in]     - pRxBuffer: Pointer to the reception buffer.
 * @param[in]     - Len: Length of the data to be received.
 *
 * @return        - None
 *
 * @Note          - Blocks until all data is received.
 */
void USART_ReceiveData(USART_t *pUSARTInst, uint8_t *pRxBuffer, uint32_t Len)
{
    // Implementation here
}

/*********************************************************************
 * @fn            - USART_SendDataIT
 *
 * @brief         - Sends data via USART in interrupt mode.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 * @param[in]     - pTxBuffer: Pointer to the transmission buffer.
 * @param[in]     - Len: Length of the data to be sent.
 *
 * @return        - USART_BUSY_IN_TX if transmission is ongoing, USART_READY otherwise.
 *
 * @Note          - This function uses interrupts for data transmission.
 */
uint8_t USART_SendDataIT(USART_t *pUSARTInst, uint8_t *pTxBuffer, uint32_t Len)
{
    // Implementation here
    return USART_READY;
}

/*********************************************************************
 * @fn            - USART_ReceiveDataIT
 *
 * @brief         - Receives data via USART in interrupt mode.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 * @param[in]     - pRxBuffer: Pointer to the reception buffer.
 * @param[in]     - Len: Length of the data to be received.
 *
 * @return        - USART_BUSY_IN_RX if reception is ongoing, USART_READY otherwise.
 *
 * @Note          - This function uses interrupts for data reception.
 */
uint8_t USART_ReceiveDataIT(USART_t *pUSARTInst, uint8_t *pRxBuffer, uint32_t Len)
{
    // Implementation here
    return USART_READY;
}

/*********************************************************************
 * @fn            - USART_IRQHandling
 *
 * @brief         - Handles USART interrupts.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 *
 * @return        - None
 *
 * @Note          - This function should be called inside the ISR.
 */
void USART_IRQHandling(USART_t *pUSARTInst)
{
    // Implementation here
}

/*********************************************************************
 * @fn            - USART_GetFlagStatus
 *
 * @brief         - Checks the status of a specified USART flag.
 *
 * @param[in]     - pUSARTRegs: Pointer to the USART registers.
 * @param[in]     - StatusFlagName: Name of the flag to check.
 *
 * @return        - Flag status (SET or RESET).
 *
 * @Note          - None
 */
uint8_t USART_GetFlagStatus(USART_Regs_t *pUSARTRegs, uint8_t StatusFlagName)
{
    // Implementation here
    return 0;
}

/*********************************************************************
 * @fn            - USART_ClearFlag
 *
 * @brief         - Clears a specified USART flag.
 *
 * @param[in]     - pUSARTRegs: Pointer to the USART registers.
 * @param[in]     - StatusFlagName: Name of the flag to clear.
 *
 * @return        - None
 *
 * @Note          - Ensure the flag supports clearing by software.
 */
void USART_ClearFlag(USART_Regs_t *pUSARTRegs, uint16_t StatusFlagName)
{
    // Implementation here
}

/*********************************************************************
 * @fn            - USART_PeripheralControl
 *
 * @brief         - Enables or disables the USART peripheral.
 *
 * @param[in]     - pUSARTRegs: Pointer to the USART registers.
 * @param[in]     - EnOrDi: ENABLE or DISABLE macros.
 *
 * @return        - None
 *
 * @Note          - Ensure proper configuration before enabling the peripheral.
 */
void USART_PeripheralControl(USART_Regs_t *pUSARTRegs, uint8_t EnOrDi)
{
    // Implementation here
}

/*********************************************************************
 * @fn            - USART_SetBaudRate
 *
 * @brief         - Configures the baud rate for USART communication.
 *
 * @param[in]     - pUSARTRegs: Pointer to the USART registers.
 * @param[in]     - BaudRate: Desired baud rate value.
 *
 * @return        - None
 *
 * @Note          - Ensure the baud rate value is valid.
 */
void USART_SetBaudRate(USART_Regs_t *pUSARTRegs, uint32_t BaudRate)
{
    // Implementation here
}

/*********************************************************************
 * @fn            - USART_ApplicationEventCallback
 *
 * @brief         - Weak implementation of the application event callback.
 *
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 * @param[in]     - ApEv: Event ID.
 *
 * @return        - None
 *
 * @Note          - The user application should override this function if required.
 */
__attribute__((weak)) void USART_ApplicationEventCallback(USART_t *pUSARTInst, uint8_t ApEv)
{
    // Implementation here
}

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
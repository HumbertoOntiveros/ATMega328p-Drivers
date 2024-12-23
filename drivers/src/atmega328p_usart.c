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
 * @param[in]     - pUSARTInst: Pointer to the USART handle structure.
 *
 * @return        - None
 *
 * @Note          - Ensure that the USART clock is enabled before calling this function.
 */
void USART_Init(USART_t *pUSARTInst)
{
    // Implementation here
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
    // Implementation here
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
    // Implementation here
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
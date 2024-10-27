/*
 * @file              atmega328p_spi.c
 *
 * @brief             SPI driver implementation for ATmega328P microcontroller.
 *
 * @details           This file provides the implementation of functions to initialize, 
 *                    de-initialize, transmit, and receive data using the SPI peripheral
 *                    of the ATmega328P. It defines all the APIs to manage SPI peripherals, 
 *                    including initialization, data handling, and control operations.
 *                    This driver offers easy-to-use functions for interfacing with SPI 
 *                    devices in master or slave mode, supporting various configurations 
 *                    such as clock polarity, phase, and data order.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              11/10/2024 19:34:00
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "atmega328p_spi.h"

/*********************************************************************
 * @fn          - SPI_Init
 *
 * @brief       - Initializes the SPI peripheral according to the 
 *                specified parameters in the SPI_t structure.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI handle structure that contains
 *                the configuration information for the SPI peripheral.
 *
 * @return      - None
 *
 * @note        - Make sure the SPI peripheral is correctly configured 
 *                before calling this function.
 */
void SPI_Init(SPI_t *pSPIInst)
{
    // Initialization code will be added here
    uint8_t spcr_temp = 0;
    uint8_t spsr_temp = 0;
    
    //SPI Pins configuration initialized
    GPIO_t spi_pins[] = {
        SPI_MOSI,
        SPI_MISO,
        SPI_SCK,
        SPI_SS
    };

    //Mode configuration
    spcr_temp |= pSPIInst->Config.Mode << SPI_SPCR_MSTR;

    //Data order configuration
    spcr_temp |= pSPIInst->Config.DataOrder << SPI_SPCR_DORD;

    //Clock polarity configuration
    spcr_temp |= pSPIInst->Config.CPOL << SPI_SPCR_CPOL;

    //Clock phase configuration
    spcr_temp |= pSPIInst->Config.CPHA << SPI_SPCR_CPHA;

    // SCK baud rate configuration
    if(pSPIInst->Config.SCKSpeed > SPI_SCLK_FOSC_DIV128)
    {
        //SCKSpeed formating to 2 bits
        pSPIInst->Config.SCKSpeed &= SPI_SPI2X_DIS_MASK;

        //SPI2X = 1
        spcr_temp |= pSPIInst->Config.SCKSpeed << SPI_SPCR_SPR0;
        spsr_temp |= 1 << SPI_SPSR_SPI2X;
    }
    else
    {
        //SPI = 0
        spcr_temp |= pSPIInst->Config.SCKSpeed << SPI_SPCR_SPR0;
    }

    //SPI Pins configuration
    for (int i = 0; i < sizeof(spi_pins) / sizeof(GPIO_t); i++) {
        GPIO_Init(spi_pins[i]);
    }

    //Enable SPI
    spcr_temp |= (1<<SPI_SPCR_SPE);

    //Write SPI registers SPSR, SPCR
    pSPIInst->pReg->SPCR = spcr_temp;
    pSPIInst->pReg->SPSR = spsr_temp;


}

/*********************************************************************
 * @fn          - SPI_Deinit
 *
 * @brief       - Deinitializes the SPI peripheral, resetting all 
 *                registers to their default values.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI handle structure.
 *
 * @return      - None
 *
 * @note        - This function should be called when the SPI peripheral 
 *                is no longer needed to save power or reconfigure.
 */
void SPI_Deinit(SPI_t *pSPIInst)
{
    // Deinitialization code will be added here
    pSPIInst->pReg->SPCR = 0x00;
}

/*********************************************************************
 * @fn          - SPI_SendData
 *
 * @brief       - Sends data through the SPI peripheral.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI handle structure.
 * @param[in]   - pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param[in]   - Len: Length of the data to be transmitted.
 *
 * @return      - None
 *
 * @note        - Ensure that the SPI is correctly configured for transmission.
 */
void SPI_SendData(SPI_t *pSPIInst, uint8_t *pTxBuffer, uint32_t Len)
{
    // Data transmission
    while(Len > 0)
    {
        pSPIInst->pReg->SPDR = *pTxBuffer;
        while(!(pSPIInst->pReg->SPSR & (1<<SPI_SPSR_SPIF)));
        Len--;
        pTxBuffer++;
    }
}

/*********************************************************************
 * @fn          - SPI_ReceiveData
 *
 * @brief       - Receives data through the SPI peripheral.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI handle structure.
 * @param[in]   - pRxBuffer: Pointer to the buffer where received data will be stored.
 * @param[in]   - Len: Length of the data to be received.
 *
 * @return      - None
 *
 * @note        - Ensure that the SPI is correctly configured for reception.
 */
void SPI_ReceiveData(SPI_t *pSPIInst, uint8_t *pRxBuffer, uint32_t Len)
{
    // Data reception
    while(Len > 0)
    {
        while(!(pSPIInst->pReg->SPSR & (1<<SPI_SPSR_SPIF)));
        *pRxBuffer = pSPIInst->pReg->SPDR;
        Len--;
        pRxBuffer++;
    }
}

/*********************************************************************
 * @fn          - SPI_SendDataIT
 *
 * @brief       - Sends data through the SPI peripheral using interrupts.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 * @param[in]   - pTxBuffer: Pointer to the buffer containing data to be sent.
 * @param[in]   - Len: Length of the data to be sent.
 *
 * @return      - Status indicating if the transmission was successful.
 *
 * @note        - Ensure that the SPI is correctly configured for transmission.
 */
uint8_t SPI_SendDataIT(SPI_t *pSPIInst, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t state = pSPIInst->TxState;

    if (state != SPI_BUSY_IN_TX) {
        // Save Tx buffer and lenght in SPI instance
        pSPIInst->pTxBuffer = pTxBuffer;
        pSPIInst->TxLen = Len;
        pSPIInst->TxState = SPI_BUSY_IN_TX;

        // Enable interruption
        pSPIInst->pReg->SPCR |= (1 << SPI_SPCR_SPIE);
    }
    return state;
}

/*********************************************************************
 * @fn          - SPI_ReceiveDataIT
 *
 * @brief       - Receives data through the SPI peripheral using interrupts.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 * @param[in]   - pRxBuffer: Pointer to the buffer where received data will be stored.
 * @param[in]   - Len: Length of the data to be received.
 *
 * @return      - Status indicating if the reception was successful.
 *
 * @note        - Ensure that the SPI is correctly configured for reception.
 */
uint8_t SPI_ReceiveDataIT(SPI_t *pSPIInst, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t state = pSPIInst->RxState;

    if (state != SPI_BUSY_IN_RX) {
        // Save Rx buffer and lenght in SPI instance
        pSPIInst->pRxBuffer = pRxBuffer;
        pSPIInst->RxLen = Len;
        pSPIInst->RxState = SPI_BUSY_IN_RX;

        // Enable interruption
        pSPIInst->pReg->SPCR |= (1 << SPI_SPCR_SPIE);
    }
    return state;
}

/*********************************************************************
 * @fn          - SPI_Control
 *
 * @brief       - Enable and dishable SPI peripheral.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI handle structure.
 * @param[in]   - state: state to be write.
 *
 * @return      - None
 *
 * @note        - Ensure that the SPI is correctly configured for reception.
 */
void SPI_Control(SPI_t *pSPIInst, uint8_t state)
{
    if(state)
    {
        // Enable SPI peripheral
        pSPIInst->pReg->SPCR |= (1<<SPI_SPCR_SPE);
    }else
    {
        // Disable SPI peripheral
        pSPIInst->pReg->SPCR &= ~(1<<SPI_SPCR_SPE);
    }
}

/*********************************************************************
 * @fn          - SPI_IRQInterruptConfig
 *
 * @brief       - Configures or deconfigures the SPI interrupt.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 * @param[in]   - EnorDi: Enable or Disable the interrupt (1 to enable, 0 to disable).
 *
 * @return      - None
 *
 * @note        - Use this function to enable or disable SPI interrupts as required.
 */
void SPI_IRQInterruptConfig(SPI_t *pSPIInst, uint8_t EnorDi)
{
    if (EnorDi)
    {
        // Enable interruption
        pSPIInst->pReg->SPCR |= (1 << SPI_SPCR_SPIE);
    }else
    {
        // Disable interruption
        pSPIInst->pReg->SPCR &= ~(1 << SPI_SPCR_SPIE);
    }
    
}

/*********************************************************************
 * @fn          - SPI_IRQHandling
 *
 * @brief       - Handles the SPI interrupt when it is triggered.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 *
 * @return      - None
 *
 * @note        - This function should handle the SPI interrupt and manage any pending flags.
 */
void SPI_IRQHandling(SPI_t *pSPIInst)
{
    //Check if SPI interrupt is active and if SPI is bussy Tx.
    if ((pSPIInst->pReg->SPSR & (1 << SPI_SPSR_SPIF)) && (pSPIInst->TxState == SPI_BUSY_IN_TX)) {
        spi_txe_interrupt_handle(pSPIInst);
    }
    //Check if SPI interrupt is active and if SPI is bussy Rx.
    if ((pSPIInst->pReg->SPSR & (1 << SPI_SPSR_SPIF)) && (pSPIInst->RxState == SPI_BUSY_IN_RX)) {
        spi_rxne_interrupt_handle(pSPIInst);
    }
}

/*********************************************************************
 * @fn          - spi_txe_interrupt_handle
 *
 * @brief       - Handles the SPI transmit interrupt event.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 *
 * @return      - None
 *
 * @note        - This function should manage the transmission of data when the transmit interrupt is triggered.
 */
static void  spi_txe_interrupt_handle(SPI_t *pSPIInst)
{
    if (pSPIInst->TxLen > 0) {
        // Write register SPDR
        pSPIInst->pReg->SPDR = *(pSPIInst->pTxBuffer);
        pSPIInst->pTxBuffer++;
        pSPIInst->TxLen--;
    }

    if (pSPIInst->TxLen == 0) {
        // Transmission completed
        pSPIInst->TxState = SPI_READY;
        pSPIInst->pReg->SPCR &= ~(1 << SPI_SPCR_SPIE); // Disable the interrupt

        // Call the callback
        SPI_ApplicationEventCallback(pSPIInst, SPI_EVENT_TX_CMPLT);
    }
}

/*********************************************************************
 * @fn          - spi_rxne_interrupt_handle
 *
 * @brief       - Handles the SPI receive not empty interrupt event.
 *
 * @param[in]   - pSPIInst: Pointer to the SPI instance structure.
 *
 * @return      - None
 *
 * @note        - This function should process incoming data when the receive not empty interrupt is triggered.
 */
static void  spi_rxne_interrupt_handle(SPI_t *pSPIInst)
{
    if (pSPIInst->RxLen > 0) {
        // Write register SPDR
        *(pSPIInst->pRxBuffer) = pSPIInst->pReg->SPDR;
        pSPIInst->pRxBuffer++;
        pSPIInst->RxLen--;
    }

    if (pSPIInst->RxLen == 0) {
        // Reception completed
        pSPIInst->RxState = SPI_READY;
        pSPIInst->pReg->SPCR &= ~(1 << SPI_SPCR_SPIE); // Disable the interrupt

        // Call the callback
        SPI_ApplicationEventCallback(pSPIInst, SPI_EVENT_RX_CMPLT);
    }
}

/*********************************************************************
 * @fn          - SPI_ApplicationEventCallback
 *
 * @brief       - Callback function to handle application-specific events for SPI.
 *
 * @param[in]   - pSPIHandle: Pointer to the SPI handle structure.
 * @param[in]   - AppEv: Application event type (e.g., transmission complete, error).
 *
 * @return      - None
 *
 * @note        - This is a weak function that can be overridden by the user 
 *                to implement custom event handling for SPI operations.
 */
__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_t *pSPIInst, uint8_t AppEv)
{
    // TODO: Implement custom application event handling.
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
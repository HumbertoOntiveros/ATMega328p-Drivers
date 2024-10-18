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
    spcr_temp |= pSPIInst->Config.Mode << SPCR_MSTR;

    //Data order configuration
    spcr_temp |= pSPIInst->Config.DataOrder << SPCR_DORD;

    //Clock polarity configuration
    spcr_temp |= pSPIInst->Config.CPOL << SPCR_CPOL;

    //Clock phase configuration
    spcr_temp |= pSPIInst->Config.CPHA << SPCR_CPHA;

    // SCK baud rate configuration
    if(pSPIInst->Config.SCKSpeed > SCLK_FOSC_DIV128)
    {
        //SCKSpeed formating to 2 bits
        pSPIInst->Config.SCKSpeed &= SPI_SPI2X_DIS_MASK;

        //SPI2X = 1
        spcr_temp |= pSPIInst->Config.SCKSpeed << SPCR_SPR0;
        spsr_temp |= 1 << SPSR_SPI2X;
    }
    else
    {
        //SPI = 0
        spcr_temp |= pSPIInst->Config.SCKSpeed << SPCR_SPR0;
    }

    //SPI Pins configuration
    for (int i = 0; i < sizeof(spi_pins) / sizeof(GPIO_t); i++) {
        GPIO_Init(spi_pins[i]);
    }

    //Enable SPI
    spcr_temp |= (1<<SPCR_SPE);

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
        while(!(pSPIInst->pReg->SPSR & (1<<SPSR_SPIF)));
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
        while(!(pSPIInst->pReg->SPSR & (1<<SPSR_SPIF)));
        *pRxBuffer = pSPIInst->pReg->SPDR;
        Len--;
        pRxBuffer++;
    }
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
        //enabled(state=1)
        pSPIInst->pReg->SPCR |= (1<<SPCR_SPE);
    }
    else
    {
        //disabled(state=0)
        pSPIInst->pReg->SPCR &= ~(1<<SPCR_SPE);
    }
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
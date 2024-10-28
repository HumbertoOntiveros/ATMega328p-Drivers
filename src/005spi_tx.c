/*
 * 005spi_tx.c
 *
 * Created: 13/10/2024 17:23:39
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates basic SPI transmission using the ATmega328P as a master.
 * It repeatedly sends a string message to an SPI slave device every second.
 * Configurations include MSB-first data order, clock polarity/phase (CPOL=0, CPHA=0),
 * and a clock speed of Fosc/16. The delay function creates a 1-second interval between transmissions.
 *
 */

#include<string.h>
#include "atmega328p_gpio.h"
#include "atmega328p_spi.h"

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

int main(void) {

    // Initialize necessary components
    SPI_t spi_device;
    GPIO_t ss0 = SPI_SS;
    char data_msg[] = "Hello World, Mother Fuckers. ";

    spi_device.pReg             = SPI;
    spi_device.Config.Mode      = SPI_MODE_MASTER;
    spi_device.Config.DataOrder = SPI_ORDER_MSB;
    spi_device.Config.CPOL      = SPI_CPOL_LOW;
    spi_device.Config.CPHA      = SPI_CPHA_LEADING;
    spi_device.Config.SCKSpeed  = SPI_SCLK_FOSC_DIV16;

    SPI_Init(&spi_device);
    SPI_SlaveControl(ss0, 1);

    while (1) {
        // Main loop
        SPI_SlaveControl(ss0, 0);
        SPI_SendData(&spi_device, (uint8_t*)data_msg, strlen(data_msg));
        SPI_SlaveControl(ss0, 1);
        Delay_ms(1000);
    }
    return 0;
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
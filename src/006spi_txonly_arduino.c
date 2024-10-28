/*
 * 006spi_txonly_arduino.c
 *
 * Created: 16/10/2024 20:16:18
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates SPI transmission using the ATmega328P as a master device.
 * It sends a message to an SPI slave device when a button is pressed, including message length.
 * Configurations include MSB-first data order, clock polarity/phase (CPOL=0, CPHA=0), 
 * and a clock speed of Fosc/16. 
 * The delay function debounces the button for reliable operation.
 *
 */
#include <string.h>
#include "atmega328p_gpio.h"
#include "atmega328p_spi.h"

#define USER_MESG "\
    An Arduino Uno board is best suited for beginners\n\
    who have just started using microcontrollers, on the other hand,\n\
    Arduino Mega board is for enthusiasts who require a lot of I/O\n\
    pins for their projects\n"

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

int main(void) {
    // Initialize necessary components
    char user_data[] = USER_MESG;
    GPIO_t button;
    GPIO_t ss0 = SPI_SS;
    SPI_t spi_device;

    //Button configuration
    button.GPIOX           = GPIOD;
    button.GPIO_Pin.Number = PIN7;
    button.GPIO_Pin.Mode   = MODE_IN;
    button.GPIO_Pin.PullUp = PULLUP_ENABLED;

    //Button init
    GPIO_Init(button);

    //SPI configuration
    spi_device.pReg             = SPI;
    spi_device.Config.Mode      = SPI_MODE_MASTER;
    spi_device.Config.DataOrder = SPI_ORDER_MSB;
    spi_device.Config.CPOL      = SPI_CPOL_LOW;
    spi_device.Config.CPHA      = SPI_CPHA_LEADING;
    spi_device.Config.SCKSpeed  = SPI_SCLK_FOSC_DIV16;

    //SPI init
    SPI_Init(&spi_device);
    SPI_SlaveControl(ss0, 1);

    // Main loop
    while (1) {
        
        //Wait till button is pressed
        while(GPIO_ReadPin(button));

        //to avoid button de-bouncing related issues 200ms of delay
        Delay_ms(500);

        //Enabled SPI Peripheral
        SPI_Control(&spi_device, 1);

        //Application: fist send length infomration
        uint8_t datalen = strlen(user_data); 
        SPI_SlaveControl(ss0, 0);
        SPI_SendData(&spi_device, &datalen, 1);
        SPI_SlaveControl(ss0, 1);

        //Application: to send data
        SPI_SlaveControl(ss0, 0);
        SPI_SendData(&spi_device, (uint8_t*)user_data, strlen(user_data));
        SPI_SlaveControl(ss0, 1);

        //Dishabled SPI Peripheral
        SPI_Control(&spi_device, 0);

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
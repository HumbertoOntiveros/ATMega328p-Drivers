/*
 * 009i2c_master_tx_testing.c
 *
 * Created: 25/11/2024 14:57:09
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates the use of the ATmega328P I2C peripheral in Master mode 
 * to transmit data to a slave device. The example initializes the I2C interface, configures 
 * a GPIO pin as an input for a button, and sends a predefined data packet over I2C 
 * when the button is pressed.
 *
 */

#include <stdio.h>
#include <string.h>
#include "atmega328p_i2c.h"
#include "atmega328p_gpio.h"

#define MY_ADDR 0x61;
#define SLAVE_ADDR  0x68

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

// I2C Handler
I2C_t i2c_device;

// Some data
uint8_t some_data[] = "We are testing I2C master Tx\n";

// I2C peripheral initialization.
void I2C_Inits(void)
{
    i2c_device.pReg = I2C;
    i2c_device.Config.DeviceAddress = MY_ADDR;
    i2c_device.Config.Mode = I2C_MODE_MASTER;
    i2c_device.Config.SCLSpeed= I2C_SCL_SPEED_100k;

    I2C_Init(&i2c_device);
}

void GPIO_ButtonInit(GPIO_t *button)
{

    button->GPIOX           = GPIOD;
    button->GPIO_Pin.Number = PIN7;
    button->GPIO_Pin.Mode   = MODE_IN;
    button->GPIO_Pin.PullUp = PULLUP_ENABLED;
	
    GPIO_Init(*button);
}

int main(void) {
    // Initialize necessary components
    GPIO_t button;

    // Button Init
    GPIO_ButtonInit(&button);

    // I2C Init
    I2C_Inits();

    // Enable the i2c peripheral
    I2C_PeripheralControl(&i2c_device, ENABLE);

    while (1) {
		//wait till button is pressed
		while(GPIO_ReadPin(button));

		//to avoid button de-bouncing related issues 200ms of delay
		Delay_ms(200);

		//send some data to the slave
		I2C_MasterSendData(&i2c_device, some_data, strlen((char*)some_data), SLAVE_ADDR, 0);
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
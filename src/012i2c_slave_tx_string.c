/*
 * 012i2c_slave_tx_string.c
 *
 * Created: 18/12/2024 19:29:33
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Demonstrates I2C slave mode functionality by transmitting a predefined 
 * string to the master based on received commands. Configures GPIO for a 
 * button and initializes I2C in slave mode with callback-based event handling.
 *
 */

#include<stdio.h>
#include<string.h>
#include "atmega328p_i2c.h"
#include "atmega328p_gpio.h"

#define SLAVE_ADDR  0x69
#define MY_ADDR SLAVE_ADDR

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

// I2C Handler
I2C_t i2c_device;

//Tx buffer
uint8_t Tx_buf[32]  = "I2C Slave mode testing..";

// I2C peripheral initialization.
void I2C_Inits(void)
{
    i2c_device.pReg = I2C;
    i2c_device.Config.DeviceAddress = MY_ADDR;
    i2c_device.Config.Mode = I2C_MODE_SLAVE;
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

    I2C_SlaveEnableDisableCallbackEvents(i2c_device.pReg, ENABLE);

    // Enable the I2C peripheral
    I2C_PeripheralControl(i2c_device.pReg, ENABLE);

    // Enable global interrupts
    IRQ_EN();
    
    // Waits 300ms.
    Delay_ms(300);

    // Main loop
    while (1);

    return 0;
}

// I2C Interruption Service Routine 
ISR(ISR_TWI)
{
    // I2C IRQ Handling
    I2C_IRQHandling(&i2c_device);
}

// I2C Aplication Event Callback
void I2C_ApplicationEventCallback(I2C_t *pI2CInst, uint8_t AppEv)
{
    static uint8_t commandCode = 0;
	static  uint8_t Cnt = 0;

	if(AppEv == I2C_EV_DATA_REQ)
	{
		//Master wants some data. slave has to send it
		if(commandCode == 0x51)
		{
			//send the length information to the master
			I2C_SlaveSendData(pI2CInst->pReg,strlen((char*)Tx_buf));
		}else if (commandCode == 0x52)
		{
			//Send the contents of Tx_buf
			I2C_SlaveSendData(pI2CInst->pReg,Tx_buf[Cnt++]);

		}
	}else if (AppEv == I2C_EV_DATA_RCV)
	{
		//Data is waiting for the slave to read . slave has to read it
		commandCode = I2C_SlaveReceiveData(pI2CInst->pReg);

	}else if (AppEv == I2C_ERROR_AF)
	{
        //This happens only during slave txing .
        //Master has sent the NACK. so slave should understand that master doesnt need
        //more data.
        if((Cnt) == strlen((char*)Tx_buf))
        {
            commandCode = 0xff;
            Cnt = 0;
        }
	}
	else if (AppEv == I2C_EV_STOP)
	{
		//This happens only during slave reception .
		//Master has ended the I2C communication with the slave.
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
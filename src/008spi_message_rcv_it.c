/*
 * 008spi_message_rcv_it.c
 *
 * Created: 27/10/2024 19:48:57
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This application receives and prints the user message received from the Arduino peripheral in SPI interrupt mode
 * User sends the message through Arduino IDE's serial monitor tool
 * Monitor the message received in the SWV itm data console.
 *
 */
#include<stdio.h>
#include<string.h>
#include "atmega328p_gpio.h"
#include "atmega328p_spi.h"

// UART Configuration
#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

extern uart_stdout;

GPIO_t IntPin;
SPI_t spi_device;

#define MAX_LEN 500

char RcvBuff[MAX_LEN];

volatile char ReadByte;


volatile uint8_t rcvStop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

void UART_Init(unsigned int ubrr);

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

void SPI_Inits(void)
{
    spi_device.pReg             = SPI;
    spi_device.Config.Mode      = SPI_MODE_MASTER;
    spi_device.Config.DataOrder = SPI_ORDER_MSB;
    spi_device.Config.CPOL      = SPI_CPOL_LOW;
    spi_device.Config.CPHA      = SPI_CPHA_LEADING;
    spi_device.Config.SCKSpeed  = SPI_SCLK_FOSC_DIV32;

    SPI_Init(&spi_device);
}

void Slave_GPIO_InterrupPinInit(void)
{

    //fix this shit
    IntPin.GPIOX           = GPIOD;
    IntPin.GPIO_Pin.Number = PIN2;
    IntPin.GPIO_Pin.Mode   = MODE_IN;
    IntPin.GPIO_Pin.PullUp = PULLUP_ENABLED;

    GPIO_Init(IntPin);

    GPIO_ConfigInterrupt(&IntPin, INT_FALLING_EDGE); 

    GPIO_EnableInterrupt(&IntPin);

}
int main(void) {
    // Initialize necessary components
    uint8_t dummy = 0xff;
    GPIO_t ss0 = SPI_SS;

    UART_Init(MY_UBRR);

    stdout = &uart_stdout;  // Redirige stdout a UART

	Slave_GPIO_InterrupPinInit();
    printf("Interrup Pin Init\n");

	//This function is used to initialize the SPI peripheral parameters
	SPI_Inits();
    printf("SPI Init\n");

    SPI_SlaveControl(ss0, 1);

    IRQ_EN();
    SPI_IRQInterruptConfig(&spi_device, ENABLE);

    // Main loop
    while (1) {

        rcvStop = 0;

		while(!dataAvailable); //wait till data available interrupt from transmitter device(slave)

	    GPIO_DisableInterrupt(&IntPin);
		
        //enable the SPI peripheral
		SPI_Control(&spi_device, ENABLE);

        while(!rcvStop)
		{
			/* fetch the data from the SPI peripheral byte by byte in interrupt mode */
			while ( SPI_SendDataIT(&spi_device, &dummy, 1) == SPI_BUSY_IN_TX);
			while ( SPI_ReceiveDataIT(&spi_device, (uint8_t*)&ReadByte, 1) == SPI_BUSY_IN_RX );
		}

		// confirm SPI is not busy
		//while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );
        Delay_ms(200);

		//Disable the SPI peripheral
		SPI_Control(&spi_device, DISABLE);

		printf("Rcvd data = %s\n",RcvBuff);

		dataAvailable = 0;

		GPIO_EnableInterrupt(&IntPin);
    }

    return 0;
}

ISR(ISR_SPI_STC)
{
    SPI_IRQHandling(&spi_device);
}

void SPI_ApplicationEventCallback(SPI_t *pSPIInst, uint8_t AppEv)
{
	static uint32_t i = 0;
	/* In the RX complete event , copy data in to rcv buffer . '\0' indicates end of message(rcvStop = 1) */
	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
				RcvBuff[i++] = ReadByte;
				if(ReadByte == '\0' || ( i == MAX_LEN)){
					rcvStop = 1;
					RcvBuff[i-1] = '\0';
					i = 0;
				}
	}

}

ISR(ISR_INT0)
{
    printf("Pin interrup triggered\n");
    dataAvailable = 1;
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
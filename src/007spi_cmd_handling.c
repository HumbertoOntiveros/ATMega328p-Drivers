/*
 * 007spi_cmd_handling.c
 *
 * Created: 18/10/2024 20:03:28
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates SPI-based command handling between a master 
 * (ATmega328P) and a slave device. It sends various commands—like LED 
 * control, sensor data request, LED status read, message print, and 
 * ID read—to the slave and interprets responses. Key SPI configurations 
 * include MSB-first data, low CPOL, leading CPHA, and clock speed division. 
 * Button-triggered command sends and debouncing are implemented.
 *
 */

#include "atmega328p_gpio.h"
#include "atmega328p_spi.h"
#include <stdio.h>
#include <string.h>

//command codes
#define COMMAND_LED_CTRL      		0x50
#define COMMAND_SENSOR_READ      	0x51
#define COMMAND_LED_READ      		0x52
#define COMMAND_PRINT      			0x53
#define COMMAND_ID_READ      		0x54

#define LED_ON     1
#define LED_OFF    0

//arduino analog pins
#define ANALOG_PIN0 	0
#define ANALOG_PIN1 	1
#define ANALOG_PIN2 	2
#define ANALOG_PIN3 	3
#define ANALOG_PIN4 	4

//arduino led

#define LED_PIN  13

// Configuración del UART (igual que antes)
#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

extern uart_stdout;

void UART_Init(unsigned int ubrr);

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

void SPI_Inits(SPI_t *pSPIInst)
{
    pSPIInst->pReg             = SPI;
    pSPIInst->Config.Mode      = SPI_MODE_MASTER;
    pSPIInst->Config.DataOrder = SPI_ORDER_MSB;
    pSPIInst->Config.CPOL      = SPI_CPOL_LOW;
    pSPIInst->Config.CPHA      = SPI_CPHA_LEADING;
    pSPIInst->Config.SCKSpeed  = SPI_SCLK_FOSC_DIV32;

	printf("SPI MODE : %u \n", pSPIInst->Config.Mode);
	printf("SPI ORDER: %u \n", pSPIInst->Config.DataOrder);
	printf("SPI CPOL : %u \n", pSPIInst->Config.CPOL);
	printf("SPI CPHA : %u \n", pSPIInst->Config.CPHA);
	printf("SPI SCKL : %u \n", pSPIInst->Config.SCKSpeed);

    SPI_Init(pSPIInst);
}

void GPIO_ButtonInit(GPIO_t *button)
{

    button->GPIOX           = GPIOD;
    button->GPIO_Pin.Number = PIN7;
    button->GPIO_Pin.Mode   = MODE_IN;
    button->GPIO_Pin.PullUp = PULLUP_ENABLED;
	
    GPIO_Init(*button);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{

	if(ackbyte == (uint8_t)0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}

int main(void) {
    // Initialize necessary components
    GPIO_t button;
    GPIO_t ss0 = SPI_SS;
    SPI_t spi_device;
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

    //printf init
    UART_Init(MY_UBRR);
    stdout = &uart_stdout;  // Redirige stdout a UART
    
    printf("Application is running\n");
    
    //GPIO Button initialization
    GPIO_ButtonInit(&button);

    //SPI initialization
    SPI_Inits(&spi_device);
    SPI_SlaveControl(ss0, 1);

	printf("SPI Init. done\n");

    // Main loop
    while (1) {
        
        //Wait till button is pressed
        while(GPIO_ReadPin(button));

		printf("Button presed <CMD_LED_CTRL>.\n");

        //to avoid button de-bouncing related issues 200ms of delay
        Delay_ms(400);

        //Enabled SPI Peripheral
        SPI_Control(&spi_device, 1);

	    //1. CMD_LED_CTRL  	<pin no(1)>     <value(1)>
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

        //send command
        SPI_SlaveControl(ss0, 0);
		Delay_ms(100);
        SPI_SendData(&spi_device, &commandcode, 1);

        //do dummy read to clear off the SPIF
		SPI_ReceiveData(&spi_device, &dummy_read, 1);

        //Send some dummy bits (1 byte) fetch the response from the slave
        SPI_SendData(&spi_device, &dummy_write, 1);

        //read the ack byte received
		SPI_ReceiveData(&spi_device, &ackbyte, 1);

        if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			args[1] = LED_ON;

			//send arguments
			SPI_SendData(&spi_device, args, 2);
			// dummy read
			//SPI_ReceiveData(&spi_device, args, 2);
			printf("COMMAND_LED_CTRL Executed\n");
		}

        SPI_SlaveControl(ss0, 1);
		
        //end of COMMAND_LED_CTRL

        //2. CMD_SENOSR_READ   <analog pin number(1) >

		//wait till button is pressed
		while(GPIO_ReadPin(button));
		printf("Button presed <CMD_SENOSR_READ>.\n");

		//to avoid button de-bouncing related issues 200ms of delay
		Delay_ms(400);

		commandcode = COMMAND_SENSOR_READ;

		//send command
		SPI_SlaveControl(ss0, 0);
		SPI_SendData(&spi_device, &commandcode, 1);

		//do dummy read to clear off the SPIF
		SPI_ReceiveData(&spi_device, &dummy_read, 1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(&spi_device, &dummy_write, 1);

		//read the ack byte received
		SPI_ReceiveData(&spi_device, &ackbyte, 1);

		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = ANALOG_PIN0;

			//send arguments
			SPI_SendData(&spi_device, args, 1); //sending one byte of

			//do dummy read to clear off the SPIF
			SPI_ReceiveData(&spi_device, &dummy_read, 1);

			//insert some delay so that slave can ready with the data
			Delay_ms(400);

			//Send some dummy bits (1 byte) fetch the response from the slave
			SPI_SendData(&spi_device, &dummy_write, 1);

			uint8_t analog_read;
			SPI_ReceiveData(&spi_device, &analog_read, 1);
			printf("COMMAND_SENSOR_READ %d\n",analog_read);
		}

		SPI_SlaveControl(ss0, 1);
        
		//3.  CMD_LED_READ 	 <pin no(1) >

		//wait till button is pressed
		while(GPIO_ReadPin(button));
		printf("Button presed <CMD_LED_READ>.\n");

		//to avoid button de-bouncing related issues 200ms of delay
		Delay_ms(400);

		commandcode = COMMAND_LED_READ;

		//send command
		SPI_SlaveControl(ss0, 0);
		SPI_SendData(&spi_device, &commandcode, 1);

		//do dummy read to clear off the SPIF
		SPI_ReceiveData(&spi_device, &dummy_read, 1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(&spi_device, &dummy_write, 1);

		//read the ack byte received
		SPI_ReceiveData(&spi_device, &ackbyte, 1);

		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;

			//send arguments
			SPI_SendData(&spi_device, args, 1); //sending one byte of

			//do dummy read to clear off the SPIF
			SPI_ReceiveData(&spi_device, &dummy_read, 1);

			//insert some delay so that slave can ready with the data
			Delay_ms(400);

			//Send some dummy bits (1 byte) fetch the response from the slave
			SPI_SendData(&spi_device, &dummy_write, 1);

			uint8_t led_status;
			SPI_ReceiveData(&spi_device, &led_status, 1);
			printf("COMMAND_READ_LED %d\n",led_status);

		}

		SPI_SlaveControl(ss0, 1);

        //4. CMD_PRINT 		<len(2)>  <message(len) >

		//wait till button is pressed
		while(GPIO_ReadPin(button));
		printf("Button presed <CMD_PRINT>.\n");

		//to avoid button de-bouncing related issues 200ms of delay
		Delay_ms(400);

		commandcode = COMMAND_PRINT;

		//send command
		SPI_SlaveControl(ss0, 0);
		SPI_SendData(&spi_device, &commandcode, 1);

		//do dummy read to clear off the SPIF
		SPI_ReceiveData(&spi_device, &dummy_read, 1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(&spi_device, &dummy_write, 1);

		//read the ack byte received
		SPI_ReceiveData(&spi_device, &ackbyte, 1);

		uint8_t message[] = "Hello ! How are you ??";
		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = strlen((char*)message);

			//send arguments
			SPI_SendData(&spi_device, args, 1); //sending length

			//do dummy read to clear off the SPIF
			SPI_ReceiveData(&spi_device, &dummy_read, 1);

			Delay_ms(400);

			//send message
			for(int i = 0 ; i < args[0] ; i++){
				SPI_SendData(&spi_device, &message[i], 1);
				SPI_ReceiveData(&spi_device, &dummy_read, 1);
			}

			printf("COMMAND_PRINT Executed \n");

		}

		SPI_SlaveControl(ss0, 1);

		//5. CMD_ID_READ
		//wait till button is pressed
		while(GPIO_ReadPin(button));
		printf("Button presed <CMD_ID_READ>.\n");

		//to avoid button de-bouncing related issues 200ms of delay
		Delay_ms(400);

		commandcode = COMMAND_ID_READ;

		//send command
		SPI_SlaveControl(ss0, 0);
		SPI_SendData(&spi_device, &commandcode, 1);

		//do dummy read to clear off the SPIF
		SPI_ReceiveData(&spi_device, &dummy_read, 1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(&spi_device, &dummy_write, 1);

		//read the ack byte received
		SPI_ReceiveData(&spi_device, &ackbyte, 1);

		uint8_t id[16];
		uint32_t i=0;
		if( SPI_VerifyResponse(ackbyte))
		{
			//read 10 bytes id from the slave
			for(  i = 0 ; i < 15 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_SendData(&spi_device, &dummy_write, 1);
				SPI_ReceiveData(&spi_device, &id[i], 1);
			}

			id[15] = '\0';

			printf("COMMAND_ID : %s \n",id);

		}

		//lets confirm SPI is not busy
		Delay_ms(400);

		SPI_SlaveControl(ss0, 1);

		//Disable the SPI peripheral
		SPI_Control(&spi_device, 0);

		printf("SPI Communication Closed\n");

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
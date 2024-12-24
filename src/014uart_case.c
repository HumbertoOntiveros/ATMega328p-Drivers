/*
 * 014uart_case.c
 *
 * Created: 23/12/2024 22:16:18
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * This example demonstrates UART communication where messages are sent to an Arduino
 * on button press, and the responses are received and printed. It handles message 
 * transmission, reception, and debouncing with a simple delay.
 *
 */
#include "stdio.h"
#include "string.h"
#include "atmega328p_gpio.h"
#include "atmega328p_usart.h"

//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024];

USART_t uart_device;

//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

void UART_Inits(void)
{
    uart_device.pReg = USART;
    uart_device.Config.USART_Baud = USART_STD_BAUD_115200;
    uart_device.Config.USART_Mode = USART_MODE_TXRX;
    uart_device.Config.USART_NoOfStopBits = USART_STOPBITS_1;
    uart_device.Config.USART_ParityControl = USART_PARITY_DISABLE;
    uart_device.Config.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&uart_device);
}

void GPIO_ButtonInit(GPIO_t *button)
{

    button->GPIOX           = GPIOD;
    button->GPIO_Pin.Number = PIN7;
    button->GPIO_Pin.Mode   = MODE_IN;
    button->GPIO_Pin.PullUp = PULLUP_ENABLED;
	
    GPIO_Init(*button);
}

void Delay_ms(uint32_t ms) {
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

int main(void) {
    GPIO_t button;
	uint32_t cnt = 0;

    // Button Init
    GPIO_ButtonInit(&button);

    // UART Init
    UART_Inits();

    // Enable global interrupts
    IRQ_EN();

    printf("Application is running\n");

    //do forever
    while(1)
    {
        // wait till button is pressed
        while(GPIO_ReadPin(button));

        // to avoid button de-bouncing related issues 200ms of delay
        Delay_ms(200);

        // Next message index ; make sure that cnt value doesn't cross 2
        cnt = cnt % 3;

        //First lets enable the reception in interrupt mode
        //this code enables the receive interrupt
        while ( USART_ReceiveDataIT(&uart_device, (uint8_t *)rx_buf, strlen(msg[cnt])) != USART_READY);

        //Send the msg indexed by cnt in blocking mode
        USART_SendData(&uart_device, (uint8_t*)msg[cnt], strlen(msg[cnt]));

        printf("Transmitted : %s\n",msg[cnt]);


        //Now lets wait until all the bytes are received from the arduino .
        //When all the bytes are received rxCmplt will be SET in application callback
        while(rxCmplt != SET);

        //just make sure that last byte should be null otherwise %s fails while printing
        rx_buf[strlen(msg[cnt])+ 1] = '\0';

        //Print what we received from the arduino
        printf("Received    : %s\n",rx_buf);

        //invalidate the flag
        rxCmplt = RESET;

        //move on to next message indexed in msg[]
        cnt ++;
    }

    return 0;
}

ISR(ISR_USART_RX)
{
	USART_IRQHandling(&uart_device);
}

void USART_ApplicationEventCallback(USART_t *pUSARTInst, uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
			rxCmplt = SET;

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
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
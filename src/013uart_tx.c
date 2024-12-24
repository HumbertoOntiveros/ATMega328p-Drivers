/*
 * 013uart_tx.c
 *
 * Created: 23/12/2024 18:49:43
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Briefly describe the example's functionality, key configurations,
 * and expected outcomes for user clarity.
 *
 */
#include "string.h"
#include "atmega328p_gpio.h"
#include "atmega328p_usart.h"

char msg[1024] = "UART Tx testing...\n\r";

USART_t uart_device;

void UART_Inits(void)
{
    uart_device.pReg = USART;
    uart_device.Config.USART_Baud = USART_STD_BAUD_115200;
    uart_device.Config.USART_Mode = USART_MODE_ONLY_TX;
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
    // Initialize necessary components
    GPIO_t button;

    // Button Init
    GPIO_ButtonInit(&button);

    // UART Init
    UART_Inits();

    // Enable the USART peripheral
    USART_PeripheralControl(uart_device.pReg, ENABLE);

    while (1) {

		// wait till button is pressed
		while(GPIO_ReadPin(button));

        // to avoid button de-bouncing related issues 200ms of delay
        Delay_ms(200);

        // Send data
        USART_SendData(&uart_device, (uint8_t*)msg, strlen(msg));
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
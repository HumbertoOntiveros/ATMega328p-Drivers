/*
 * @file              lcd.c
 *
 * @brief             XXXXX driver implementation for ATmega328P microcontroller.
 *
 * @details           This file provides the implementation of functions to initialize, 
 *                    de-initialize, etc, etc, XXXX for the ATmega328P.
 *                    It defines all the APIs to manage XXXX peripherals including
 *                    initialization, data handling, and XXXXX operations.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              26/12/2024 11:24:35
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "lcd.h"

static GPIO_t lcd_signal;
/*********************************************************************
 * @fn            - mdelay
 *
 * @brief         - Creates a delay in milliseconds.
 *
 * @param[in]     - cnt: Number of milliseconds to delay.
 *
 * @return        - None
 *
 * @Note          - This is a blocking delay, implemented using a busy-wait loop.
 */
static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}

/*********************************************************************
 * @fn            - udelay
 *
 * @brief         - Creates a delay in microseconds.
 *
 * @param[in]     - cnt: Number of microseconds to delay.
 *
 * @return        - None
 *
 * @Note          - This is a blocking delay, implemented using a busy-wait loop.
 *                  Assumes ATmega328P running at 16 MHz.
 */
static void udelay(uint32_t cnt)
{
    // Each iteration of this loop takes 4 clock cycles on a 16 MHz CPU
    // Calculate the total number of iterations required for the delay
    uint32_t iterations = (cnt * 16) / 4;

    while (iterations--) {
        __asm__ __volatile__("nop"); // Assembly NOP for precise timing
    }
}

/*********************************************************************
 * @fn            - lcd_enable
 *
 * @brief         - Generates an enable pulse for the LCD to latch the data/command.
 *
 * @param[in]     - None
 *
 * @return        - None
 *
 * @Note          - Ensure proper timing with microsecond delays between GPIO operations.
 */
static void lcd_enable(void)
{
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_EN;
	GPIO_WritePin(lcd_signal, GPIO_PIN_SET);
	udelay(10);
	GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);
	udelay(100);/* execution time > 37 micro seconds */
}

/*********************************************************************
 * @fn            - write_4_bits
 *
 * @brief         - Writes 4 bits of data or command to the LCD data lines (D4-D7).
 *
 * @param[in]     - value: 4-bit value to be written to the LCD.
 *
 * @return        - None
 *
 * @Note          - Ensures proper timing by triggering the LCD enable signal 
 *                  after setting each data bit.
 */
static void write_4_bits(uint8_t value)
{
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_D4;
	GPIO_WritePin(lcd_signal, ((value >> 0) & 0x1) );
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_D5;
	GPIO_WritePin(lcd_signal, ((value >> 1) & 0x1) );
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_D6;
	GPIO_WritePin(lcd_signal, ((value >> 2) & 0x1) );
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_D7;
	GPIO_WritePin(lcd_signal, ((value >> 3) & 0x1) );

	lcd_enable();
}

/*********************************************************************
 * @fn            - lcd_send_command
 *
 * @brief         - Sends a command to the LCD to control its operation.
 *
 * @param[in]     - cmd: Command to be sent to the LCD.
 *
 * @return        - None
 *
 * @Note          - Use this function for low-level operations such as cursor control and display settings.
 */
void lcd_send_command(uint8_t cmd)
{
	/* RS=0 for LCD command */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RS;
	GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	/*R/nW = 0, for write */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RW;
	GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	write_4_bits(cmd >> 4);
	write_4_bits(cmd & 0x0F);

}

/*********************************************************************
 * @fn            - lcd_print_char
 *
 * @brief         - Sends a character to the LCD for display using 4-bit parallel 
 *                  data transmission. First, the higher nibble of the data is 
 *                  transmitted, followed by the lower nibble.
 *
 * @param[in]     - data: 8-bit ASCII character to be displayed on the LCD.
 *
 * @return        - None
 *
 * @Note          - Ensure the LCD is in write mode and RS is set for data before
 *                  sending the nibbles. Timing constraints must be observed.
 */
void lcd_print_char(uint8_t data)
{
	/* RS=1 for LCD user data */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RS;
	GPIO_WritePin(lcd_signal, GPIO_PIN_SET);

	/*R/nW = 0, for write */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RW;
	GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	write_4_bits(data >> 4);  /*Higher nibble*/
	write_4_bits(data & 0x0F); /*Lower nibble*/

}

/*********************************************************************
 * @fn            - lcd_print_string
 *
 * @brief         - Displays a null-terminated string on the LCD starting from
 *                  the current cursor position.
 *
 * @param[in]     - str: Pointer to the null-terminated string to be displayed.
 *
 * @return        - None
 *
 * @Note          - Ensure the string length does not exceed the LCD's display capacity.
 */
void lcd_print_string(char *message)
{

      do
      {
          lcd_print_char((uint8_t)*message++);
      }
      while (*message != '\0');

}

/*********************************************************************
 * @fn            - lcd_init
 *
 * @brief         - Initializes the LCD by configuring the GPIO pins and sending 
 *                  the necessary initialization commands.
 *
 * @param[in]     - None
 *
 * @return        - None
 *
 * @Note          - Ensure the proper delay timings during initialization.
 */
void lcd_init(void)
{

	//1. Configure the gpio pins which are used for lcd connections
	lcd_signal.GPIOX = LCD_GPIO_PORT;
	lcd_signal.GPIO_Pin.Mode = MODE_OUT;
	lcd_signal.GPIO_Pin.Number = LCD_GPIO_RS;
	lcd_signal.GPIO_Pin.PullUp = PULLUP_DISABLED;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	lcd_signal.GPIO_Pin.Number = LCD_GPIO_RW;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);
	
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_EN;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	lcd_signal.GPIO_Pin.Number = LCD_GPIO_D4;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	lcd_signal.GPIO_Pin.Number = LCD_GPIO_D5;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	lcd_signal.GPIO_Pin.Number = LCD_GPIO_D6;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	lcd_signal.GPIO_Pin.Number = LCD_GPIO_D7;
	GPIO_Init(lcd_signal);
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	//2. Do the LCD initialization
	mdelay(40);

	/*RS = 0 , For LCD command */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RS;
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	/* RnW = 0, Writing to LCD */
    lcd_signal.GPIO_Pin.Number = LCD_GPIO_RW;
    GPIO_WritePin(lcd_signal, GPIO_PIN_RESET);

	write_4_bits(0x3);

	mdelay(5);

	write_4_bits(0x3);

	udelay(150);

	write_4_bits(0x3);
	write_4_bits(0x2);

	//function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//disply ON and cursor ON
	lcd_send_command(LCD_CMD_DON_CURON);

	lcd_display_clear();

	//entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}

/*********************************************************************
 * @fn            - lcd_display_clear
 *
 * @brief         - Clears the LCD display and resets the cursor to the home position.
 *
 * @param[in]     - None
 *
 * @return        - None
 *
 * @Note          - Ensure to follow the LCD timing for the clear display command.
 */
void lcd_display_clear(void)
{
	//Display clear
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	/*
	 * check page number 24 of datasheet.
	 * display clear command execution wait time is around 2ms
	 */

	mdelay(2);
}

/*********************************************************************
 * @fn            - lcd_display_return_home
 *
 * @brief         - Returns the cursor to the home position on the LCD without
 *                  clearing the display.
 *
 * @param[in]     - None
 *
 * @return        - None
 *
 * @Note          - Timing constraints must be respected.
 */
void lcd_display_return_home(void)
{

	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */
	mdelay(2);
}

/*********************************************************************
 * @fn            - lcd_set_cursor
 *
 * @brief         - Sets the cursor position on the LCD screen based on the given 
 *                  row and column. Supports a 2x16 character display.
 *
 * @param[in]     - row: Row number (1 or 2)
 *                  column: Column number (1 to 16)
 *
 * @return        - None
 *
 * @Note          - Column input is adjusted to match the LCD's address map. The 
 *                  row input selects between the first and second rows.
 */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      lcd_send_command((column |= 0x80));
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
        lcd_send_command((column |= 0xC0));
      break;
    default:
      break;
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
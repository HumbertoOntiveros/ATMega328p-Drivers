/*
 * lcd.c
 *
 * Created: 26/12/2024 11:24:35
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Briefly describe the driver's functionality, key configurations,
 * and expected outcomes for user clarity.
 *
 */

#ifndef __LCD_H__
#define __LCD_H__

#include "atmega328p_gpio.h"

/******************************************************************************************
 *                                  BSP Specific Details                                  *
 ******************************************************************************************/
/*
 * Pin Configuration
 * Hardware connections and pin definitions for LCD interface 
 */
#define LCD_GPIO_PORT  GPIOD
#define LCD_GPIO_RS	   GPIO_PIN_NO_0
#define LCD_GPIO_RW	   GPIO_PIN_NO_1
#define LCD_GPIO_EN	   GPIO_PIN_NO_2
#define LCD_GPIO_D4	   GPIO_PIN_NO_3
#define LCD_GPIO_D5	   GPIO_PIN_NO_4
#define LCD_GPIO_D6	   GPIO_PIN_NO_5
#define LCD_GPIO_D7	   GPIO_PIN_NO_6

/*
 * LCD Control Commands
 * Basic instructions for display initialization and control
 */
#define LCD_CMD_4DL_2N_5X8F  		0x28
#define LCD_CMD_DON_CURON    		0x0E
#define LCD_CMD_INCADD       		0x06
#define LCD_CMD_DIS_CLEAR    		0X01
#define LCD_CMD_DIS_RETURN_HOME  	0x02

/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/
/*
 * LCD Initialization
 * Basic setup and configuration functions for display startup
 */
void lcd_init(void);

/*
 * LCD Control Operations
 * Low-level command handling and display control functions
 */
void lcd_send_command(uint8_t cmd);
void lcd_display_clear(void);
void lcd_display_return_home(void);

/*
 * LCD Writing Operations
 * Character and string display functions with cursor management
 */
void lcd_print_char(uint8_t data);
void lcd_print_string(char*);
void lcd_set_cursor(uint8_t row, uint8_t column);

#endif // __LCD_H__

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
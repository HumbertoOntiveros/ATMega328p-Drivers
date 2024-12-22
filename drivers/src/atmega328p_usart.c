/*
 * @file              atmega328p_usart.c
 *
 * @brief             USART driver implementation for ATmega328P microcontroller.
 *
 * @details           Driver for configuring and using the USART peripheral on ATmega328P. 
 *                    Supports initialization, data transfer (blocking and interrupt-based), 
 *                    and customizable settings for baud rate, parity, word length, and stop bits.
 *                    Includes APIs for error handling and application callbacks.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              21/12/2024 21:16:06
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "atmega328p_usart.h"

/*********************************************************************
 * @fn            - 
 *
 * @brief             - 
 *
 * @param[in]         - 
 *
 * @return            - 
 *
 * @Note              -
 */


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
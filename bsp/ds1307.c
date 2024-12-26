/*
 * @file              ds1307.c
 *
 * @brief             ds1307 driver implementation for ATmega328P microcontroller.
 *
 * @details            Driver for DS1307 RTC module, providing I2C communication for setting and reading time and date.
 *                     Configurable time format (12-hour, 24-hour) and I2C speed. Supports initialization, time, 
 *                     and date management for easy integration into embedded systems.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              25/12/2024 22:12:36
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "ds1307.h"

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
/*
 * ds1307.c
 *
 * Created: 25/12/2024 22:12:36
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Driver for DS1307 RTC module, providing I2C communication for setting and reading time and date.
 * Configurable time format (12-hour, 24-hour) and I2C speed. Supports initialization, time, 
 * and date management for easy integration into embedded systems.
 *
 */

#ifndef __DS1307_H__
#define __DS1307_H__

#include "atmega328p_i2c.h"

/******************************************************************************************
 *                                  BSP Specific Details                                  *
 ******************************************************************************************/
/*
 * Application configurable items
 * Define I2C interface and communication speed for DS1307.
 */
#define DS1307_I2C  			I2C
#define DS1307_I2C_SPEED 		I2C_SCL_SPEED_SM

/*
 * Register addresses
 * Memory map for accessing DS1307 internal registers.
 */
#define DS1307_ADDR_SEC 		0x00
#define DS1307_ADDR_MIN 		0x01
#define DS1307_ADDR_HRS			0x02
#define DS1307_ADDR_DAY			0x03
#define DS1307_ADDR_DATE		0x04
#define DS1307_ADDR_MONTH		0x05
#define DS1307_ADDR_YEAR		0x06

/*
 * Time format options
 * Defines constants for selecting time display format.
 */
#define TIME_FORMAT_12HRS_AM 	0
#define TIME_FORMAT_12HRS_PM 	1
#define TIME_FORMAT_24HRS 		2

/*
 * I2C device address
 * I2C slave address for the DS1307 RTC module.
 */
#define DS1307_I2C_ADDRESS      0x68

/*
 * Day constants
 * Enumerates days of the week for ease of use.
 */
#define SUNDAY  	1;
#define MONDAY  	2;
#define TUESDAY  	3;
#define WEDNESDAY   4;
#define THURSDAY  	5;
#define FRIDAY  	6;
#define SATURDAY  	7;

/*
 * RTC_date_t structure
 * Holds date information for the DS1307 RTC.
 */
typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_date_t;

/*
 * RTC_time_t structure
 * Holds time information for the DS1307 RTC.
 */
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t time_format;
}RTC_time_t;

/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/
/*
 * DS1307 initialization
 * Prepares the DS1307 RTC for operation.
 */
uint8_t ds1307_init(void);

/*
 * Time management functions
 * Set and retrieve the current time in the DS1307 RTC.
 */
void ds1307_set_current_time(RTC_time_t *);
void ds1307_get_current_time(RTC_time_t *);

/*
 * Date management functions
 * Set and retrieve the current date in the DS1307 RTC.
 */
void ds1307_set_current_date(RTC_date_t *);
void ds1307_get_current_date(RTC_date_t *);

#endif // __DS1307_H__

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
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

I2C_t g_ds1307I2cHandle;

/*********************************************************************
 * @fn            - ds1307_i2c_config
 *
 * @brief         - Configures the I2C peripheral for DS1307 communication.
 *
 * @param[in]     - None
 *
 * @return        - None
 *
 * @Note          - This function initializes the I2C handle with DS1307-specific settings.
 */
static void ds1307_i2c_config(void)
{
	g_ds1307I2cHandle.pReg = DS1307_I2C;
	g_ds1307I2cHandle.Config.Mode = I2C_MODE_MASTER;
	g_ds1307I2cHandle.Config.SCLSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2cHandle);
}

/*********************************************************************
 * @fn            - ds1307_write
 *
 * @brief         - Writes a value to a specific register of the DS1307.
 *
 * @param[in]     - value: The value to write.
 * @param[in]     - reg_addr: The address of the register to write to.
 *
 * @return        - None
 *
 * @Note          - Ensures proper communication with the DS1307 using I2C.
 */
static void ds1307_write(uint8_t value,uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;
	I2C_MasterSendData(&g_ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0);
}

/*********************************************************************
 * @fn            - ds1307_read
 *
 * @brief         - Reads a value from a specific register of the DS1307.
 *
 * @param[in]     - reg_addr: The address of the register to read from.
 *
 * @return        - The value read from the specified register.
 *
 * @Note          - Handles I2C communication to retrieve data.
 */
static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;
    I2C_MasterSendData(&g_ds1307I2cHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0);
    I2C_MasterReceiveData(&g_ds1307I2cHandle, &data, 1, DS1307_I2C_ADDRESS, 0);

    return data;
}

/*********************************************************************
 * @fn            - binary_to_bcd
 *
 * @brief         - Converts a binary value to BCD format.
 *
 * @param[in]     - value: The binary value to convert.
 *
 * @return        - The converted BCD value.
 *
 * @Note          - Used for DS1307 data formatting.
 */
static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m , n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value /10;
		n = value % 10;
		bcd = (m << 4) | n ;
	}

	return bcd;
}

/*********************************************************************
 * @fn            - bcd_to_binary
 *
 * @brief         - Converts a BCD value to binary format.
 *
 * @param[in]     - value: The BCD value to convert.
 *
 * @return        - The converted binary value.
 *
 * @Note          - Used for DS1307 data processing.
 */
static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t) ((value >> 4 ) * 10);
	n =  value & (uint8_t)0x0F;
	return (m+n);
}

/*********************************************************************
 * @fn            - ds1307_init
 *
 * @brief         - Initializes the DS1307 RTC module.
 *
 * @param[in]     - None
 *
 * @return        - 1 if initialization failed, 0 if successful.
 *
 * @Note          - Configures I2C and sets the clock halt bit to 0.
 */
uint8_t ds1307_init(void)
{

	//1. initialize the i2c peripheral
	ds1307_i2c_config();

	//2. Enable the I2C peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE);

	//3. Make clock halt = 0;
	ds1307_write(0x00,DS1307_ADDR_SEC);

	//4. Read back clock halt bit
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7 ) & 0x1);

}

/*********************************************************************
 * @fn            - ds1307_set_current_time
 *
 * @brief         - Sets the current time in the DS1307.
 *
 * @param[in]     - rtc_time: Pointer to a structure containing the time information.
 *
 * @return        - None
 *
 * @Note          - Supports 12-hour and 24-hour time formats.
 */
void ds1307_set_current_time(RTC_time_t *rtc_time)
{
	uint8_t seconds, hrs;
	seconds = binary_to_bcd(rtc_time->seconds);
	seconds &= ~( 1 << 7);
	ds1307_write(seconds, DS1307_ADDR_SEC);

	ds1307_write(binary_to_bcd(rtc_time->minutes), DS1307_ADDR_MIN);

	hrs = binary_to_bcd(rtc_time->hours);

	if(rtc_time->time_format == TIME_FORMAT_24HRS){
		hrs &= ~(1 << 6);
	}else{
		hrs |= (1 << 6);
		hrs = (rtc_time->time_format  == TIME_FORMAT_12HRS_PM) ? hrs | ( 1 << 5) :  hrs & ~( 1 << 5) ;
	}

	ds1307_write(hrs,DS1307_ADDR_HRS);

}

/*********************************************************************
 * @fn            - ds1307_set_current_date
 *
 * @brief         - Sets the current date in the DS1307.
 *
 * @param[in]     - rtc_date: Pointer to a structure containing the date information.
 *
 * @return        - None
 *
 * @Note          - Configures date, month, year, and day registers.
 */
void ds1307_set_current_date(RTC_date_t *rtc_date)
{
	ds1307_write(binary_to_bcd(rtc_date->date),DS1307_ADDR_DATE);

	ds1307_write(binary_to_bcd(rtc_date->month),DS1307_ADDR_MONTH);

	ds1307_write(binary_to_bcd(rtc_date->year),DS1307_ADDR_YEAR);

	ds1307_write(binary_to_bcd(rtc_date->day),DS1307_ADDR_DAY);

}

/*********************************************************************
 * @fn            - ds1307_get_current_time
 *
 * @brief         - Retrieves the current time from the DS1307.
 *
 * @param[in]     - rtc_time: Pointer to a structure to store the time information.
 *
 * @return        - None
 *
 * @Note          - Handles both 12-hour and 24-hour time formats.
 */
void ds1307_get_current_time(RTC_time_t *rtc_time)
{

	uint8_t seconds,hrs;

	seconds = ds1307_read(DS1307_ADDR_SEC);

	seconds &= ~( 1 << 7);

	rtc_time->seconds = bcd_to_binary(seconds);
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));

	hrs = ds1307_read(DS1307_ADDR_HRS);
	if(hrs & ( 1 << 6)){
		//12 hr format
		rtc_time->time_format =  !((hrs & ( 1 << 5)) == 0) ;
		hrs &= ~(0x3 << 5);//Clear 6 and 5
	}else{
		//24 hr format
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}

	rtc_time->hours = bcd_to_binary(hrs);
}

/*********************************************************************
 * @fn            - ds1307_get_current_date
 *
 * @brief         - Retrieves the current date from the DS1307.
 *
 * @param[in]     - rtc_date: Pointer to a structure to store the date information.
 *
 * @return        - None
 *
 * @Note          - Reads and decodes date, month, year, and day registers.
 */
void ds1307_get_current_date(RTC_date_t *rtc_date)
{
	rtc_date->day =  bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));

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
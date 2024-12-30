/*
 * 016serial_time_sync.c
 *
 * Created: 29/12/2024 20:34:31
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Demonstrates RTC synchronization via UART. The microcontroller requests
 * current date and time from a host script, updates the RTC, and displays it
 * on an LCD screen.
 *
 */

#include <stdio.h>
#include "lcd.h"
#include "ds1307.h"

#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1
extern FILE uart_stdin;
void UART_Init_Stdin(unsigned int ubrr);

/* Enable this macro if you want to test RTC on LCD */
#define PRINT_LCD

void Show_timendate(void);
void Sync_time_and_date(void);

void Delay_ms(uint32_t ms)
{
    // Assuming the ATmega328P has a 16 MHz clock 
    // Each iteration of the 'for' loop takes approximately 4 clock cycles
    // Therefore, a value is needed to adjust the delay duration
    const uint32_t cycles_per_ms = 471; // 16 MHz / 4 (cycles per instruction) / 1000 ms
    for (volatile uint32_t i = 0; i < (cycles_per_ms * ms); i++);
}

char* get_day_of_week(uint8_t i)
{
	char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	return days[i-1];
}

void number_to_string(uint8_t num , char* buf)
{

	if(num < 10){
		buf[0] = '0';
		buf[1] = num+48;
	}else if(num >= 10 && num < 99)
	{
		buf[0] = (num/10) + 48;
		buf[1]= (num % 10) + 48;
	}
}

//hh:mm:ss
char* time_to_string(RTC_time_t *rtc_time)
{
	static char buf[9];

	buf[2]= ':';
	buf[5]= ':';

	number_to_string(rtc_time->hours,buf);
	number_to_string(rtc_time->minutes,&buf[3]);
	number_to_string(rtc_time->seconds,&buf[6]);

	buf[8] = '\0';

	return buf;

}

//dd/mm/yy
char* date_to_string(RTC_date_t *rtc_date)
{
	static char buf[9];

	buf[2]= '/';
	buf[5]= '/';

	number_to_string(rtc_date->date,buf);
	number_to_string(rtc_date->month,&buf[3]);
	number_to_string(rtc_date->year,&buf[6]);

	buf[8]= '\0';

	return buf;

}

int main(void) {
    // Initialize necessary components
	RTC_time_t current_time;
	RTC_date_t current_date;

	UART_Init_Stdin(MY_UBRR);
	stdin = &uart_stdin;

	lcd_init();

	lcd_print_string("RTC Test...");

	Delay_ms(2000);

	lcd_display_clear();
	lcd_display_return_home();

	if(ds1307_init()){
		lcd_print_string("RTC init has failed");
		while(1);
	}

    // Synchronize time and date
    Sync_time_and_date();

	//ds1307_set_current_date(&current_date);
	//ds1307_set_current_time(&current_time);

	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
	}else{
		lcd_print_string(time_to_string(&current_time));
	}

	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));

    // Main loop
    while (1) 
	{
		Delay_ms(300);
		Show_timendate();
    }
    return 0;
}

void Sync_time_and_date(void) {
    RTC_time_t sync_time;
    RTC_date_t sync_date;

    char buffer[30];
    lcd_print_string("SYNC_REQUEST\n"); // Request synchronization
    
    // Wait for response (data will come via UART)
    if (fgets(buffer, sizeof(buffer), stdin)) {
        int year, month, date, day, hour, minute, second;

        lcd_display_clear();
        lcd_display_return_home();
        
        // Parse the received data (format: YYYY-MM-DD HH:MM:SS)
        if (sscanf(buffer, "%4d-%2d-%2d %d %2d:%2d:%2d", &year, &month, &date, &day, &hour, &minute, &second) == 7) {
            // Populate RTC structures
            sync_date.year = year - 2000; // Adjust to RTC's 2-digit year format
            sync_date.month = month;
            sync_date.date = date;
            sync_date.day = day;

            sync_time.hours = hour;
            sync_time.minutes = minute;
            sync_time.seconds = second;
            sync_time.time_format = TIME_FORMAT_24HRS;

            // Update RTC
            ds1307_set_current_date(&sync_date);
            ds1307_set_current_time(&sync_time);

            lcd_print_string("Sync Ok");
            Delay_ms(2000);
            lcd_display_clear();
            lcd_display_return_home();

        } else {
            // Handle incorrect format or failure
            lcd_print_string("Sync Error");
            Delay_ms(2000);
            lcd_display_clear();
            lcd_display_return_home();
        }
    }
}

void Show_timendate(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";

		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
	}else{

		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
	}

	ds1307_get_current_date(&current_date);

	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));
	lcd_print_char('<');
	lcd_print_string(get_day_of_week(current_date.day));
	lcd_print_char('>');
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
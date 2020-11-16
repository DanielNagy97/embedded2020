/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file clock_face.c
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Methods for showing the time and date on the screen
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "clock_face.h"

uint8_t clock_numbers[10][9]={
{6, 0x1e, 0x33, 0x37, 0x3b, 0x33, 0x33, 0x1e, 0x0}, /* 48 - 0x30 0 */
{6, 0xe,  0x1e, 0x6,  0x6,  0x6,  0x6,  0x6,  0x0}, /* 1 */
{6, 0x1e, 0x33, 0x3,  0x1f, 0x30, 0x33, 0x3f, 0x0}, /* 2 */
{6, 0x1e, 0x33, 0x3,  0xf,  0x3,  0x33, 0x1e, 0x0}, /* 3 */
{6, 0x6,  0xe,  0x16, 0x26, 0x3f, 0x6,  0x6,  0x0}, /* 4 */
{6, 0x3f, 0x30, 0x3e, 0x33, 0x3,  0x33, 0x1e, 0x0}, /* 5 */
{6, 0x1e, 0x33, 0x30, 0x3f, 0x33, 0x33, 0x1e, 0x0}, /* 6 */
{6, 0x3f, 0x23, 0x3,  0x6,  0x6,  0xc,  0xc,  0x0}, /* 7 */
{6, 0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x1e, 0x0}, /* 8 */
{6, 0x1e, 0x33, 0x33, 0x1f, 0x3,  0x33, 0x1e, 0x0}, /* 9 */
};

void show_clock_face(RTC_TimeTypeDef* currTime){
	/**
	  * @brief Drawing the time to the screen_buffer, then displaying it
	  * @param currTime The current time as RTC_TimeTypeDef*
	  * @return None
	  */
	char clock_buffer[7] = {0};

	sprintf(clock_buffer, "%02d%02d",
			currTime->Hours,
			currTime->Minutes);

	/* Drawing the digits */
	for(int i = 0; i<4; i++){
		uint8_t* digit = clock_numbers[clock_buffer[i]-0x30];
		uint8_t shift = 1;
		if(i == 1){
			shift = 2;
		}
		else if(i == 2){
			shift = 0;
		}
		put_character_to_screen_buffer(digit, i, shift);
	}

	/* Drawing the dots */
	for(int j = 1; j<=2; j++){
		put_byte_to_screen_buffer_with_OR(0x1, 1, j);
		put_byte_to_screen_buffer_with_OR(0x80, 2, j);
		put_byte_to_screen_buffer_with_OR(0x1, 1, j+4);
		put_byte_to_screen_buffer_with_OR(0x80, 2, j+4);
	}
	display_screen_buffer();
}

char* get_date_string(RTC_DateTypeDef* currDate){
	/**
	  * @brief Creating date string from RTC date struct
	  * @param currDate The current date as RTC_DateTypeDef*
	  * @return date_buffer The date string as char*
	  */
	static char date_buffer[11] = {0};

	char* week_days[7] = {"Hétfő", "Kedd", "Szerda",
			"Csütörtök", "Péntek", "Szombat", "Vasárnap"};

	sprintf(date_buffer, "%d.%02d.%02d. %s",
			currDate->Year+2000,
			currDate->Month,
			currDate->Date,
			week_days[currDate->WeekDay-1]);

	return date_buffer;
}

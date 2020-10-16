/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file max_display.c
 * @author Daniel Nagy
 * @date 16 Oct 2020
 * @brief Methods for operating the MAX7219 led screen.
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "max_display.h"

uint8_t numbers[10][8]={
{0x1e, 0x33, 0x37, 0x3b, 0x33, 0x33, 0x1e, 0x0},//0
{0xe,  0x1e, 0x6,  0x6,  0x6,  0x6,  0x6,  0x0},//1
{0x1e, 0x33, 0x3,  0x1f, 0x30, 0x33, 0x3f, 0x0},//2
{0x1e, 0x33, 0x3,  0xf,  0x3,  0x33, 0x1e, 0x0},//3
{0x6,  0xe,  0x16, 0x26, 0x3f, 0x6,  0x6,  0x0},//4
{0x3f, 0x30, 0x3e, 0x33, 0x3,  0x33, 0x1e, 0x0},//5
{0x1e, 0x33, 0x30, 0x3f, 0x33, 0x33, 0x1e, 0x0},//6
{0x3f, 0x23, 0x3,  0x6,  0x6,  0xc,  0xc,  0x0},//7
{0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x1e, 0x0},//8
{0x1e, 0x33, 0x33, 0x1f, 0x3,  0x33, 0x1e, 0x0},//9
};

uint8_t screen_buffer[NUMBER_OF_CELLS][8] = {0};

void write_byte (uint8_t byte){
	/**
	  * @brief Writing a byte to the MAX7219 led drivers
	  * @param byte The input byte as uint8_t
	  * @return None
	  */
	for (int i = 0; i<8; i++){
		HAL_GPIO_WritePin (MAXPORT, CLOCK_PIN, 0);
		HAL_GPIO_WritePin (MAXPORT, DATA_PIN, byte&0x80);
		byte = byte<<1;
		HAL_GPIO_WritePin (MAXPORT, CLOCK_PIN, 1);
	}
}

void write_max_cmd (uint8_t address, uint8_t cmd){
	/**
	  * @brief Sending commands to the MAX7219 led drivers
	  * @param address The register's address as uint8_t
	  * @param cmd The command to the register as uint8_t
	  * @return None
	  */
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	for (int i = 0; i<NUMBER_OF_CELLS; i++){
		write_byte (address);
		write_byte (cmd);
	}
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 1);
}

void max_clear(){
	/**
	  * @brief Initializing the MAX7219 led drivers
	  * @param None
	  * @return None
	  */
	for(int i = 0; i<8; i++){
		write_max_cmd(i+1, 0);
	}
}

void max_init (uint8_t brightness){
	/**
	  * @brief Initialising the MAX7219 led drivers
	  * @param brightness The intensity of the screen as uint8_t
	  * @return None
	  */
	max_clear();
	write_max_cmd(0x09, 0x00); /* decoding = 0 */
	write_max_cmd(0x0b, 0x07); /* scan limit = 8 */
	write_max_cmd(0x0c, 0x01); /* mode = 1 */
	write_max_cmd(0x0f, 0x00); /* test display = 0 */
	write_max_cmd(0x0a, brightness);
}

void set_byte_on_matrix(uint8_t byte, uint8_t row, uint8_t column){
	/**
	  * @brief Setting one byte on the 8 rows NUMBER_OF_CELLS columns matrix
	  * @param byte The input byte as uint8_t
	  * @param row Destination row
	  * @param column Destination column
	  * @return None
	  */
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);

	for (int i = 0; i<NUMBER_OF_CELLS; i++){
		if(i == column){
			write_byte (row);
			write_byte (byte);
		}
		else{
			write_byte (0);
			write_byte (0);
		}
	}
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 1);
}

void display_character(uint8_t* character, uint8_t cell){
	/**
	  * @brief Displaying a character in a specified cell
	  * @param character The character as a uint8_t * 8x8 matrix
	  * @param cell The index of the led matrix cell as uint8_t (starts from 0)
	  * @return None
	  */
	for(int j = 0; j<8; j++){
		set_byte_on_matrix(character[j], j+1, cell);
	}
}

void display_screen_buffer(){
	/**
	  * @brief Displaying the screen buffer on the led matrixes
	  * @param None
	  * @return None
	  */
	for(int i = 0; i<NUMBER_OF_CELLS; i++){
		for(int j = 0; j<8; j++){
			set_byte_on_matrix(screen_buffer[i][j], j+1, i);
		}
	}
}

void shift_screen_buffer_right(){
	/**
	  * @brief Shifting the whole screen buffer by 1 bit to the right
	  * @param None
	  * @return None
	  */
	for(int k = NUMBER_OF_CELLS; k>=0; k--){
		for(int j = 0; j<8; j++){
			screen_buffer[k][j] =
				screen_buffer[k][j] >> 1
					| (screen_buffer[k-1][j] & 0x1) << 7;
		}
	}
}

void shift_screen_buffer_left(){
	/**
	  * @brief Shifting the whole screen buffer by 1 bit to the left
	  * @param None
	  * @return None
	  */
	for(int k = 0; k<NUMBER_OF_CELLS; k++){
		for(int j = 0; j<8; j++){
			screen_buffer[k][j] =
				screen_buffer[k][j] << 1
					| (screen_buffer[k+1][j] & 0x80) >> 7;
		}
	}
}

void scroll_text_left(char* text, uint16_t speed, uint8_t blank_space){
	/**
	  * @brief Scrolling text on the screen
	  * @param text The input text as char*
	  * @param speed The delay rate of the shifting as uint16_t
	  * @param blank_space The 0 bits after the text as uint8_t
	  * @return None
	  */
	for(int k = 0; k<strlen(text); k++){
		for(int j = 7; j>=0; j--){
			shift_screen_buffer_left();
			for(int i = 0; i<8; i++){
				/* bit masking - The k.th bit of n: (n & ( 1 << k )) >> k */
				screen_buffer[3][i] =
				    screen_buffer[3][i]
					    | (numbers[text[k]-0x30][i] & ( 1 << j )) >> j;
			}
			display_screen_buffer();
			HAL_Delay(speed);
		}
	}
	/* Making blank space after the text */
	for(int i = 0; i<blank_space; i++){
		shift_screen_buffer_left();
		display_screen_buffer();
		HAL_Delay(speed);
	}
}

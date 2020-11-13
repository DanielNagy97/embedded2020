/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file max_driver.c
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Methods for operating the MAX7219 led driver.
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "max_driver.h"

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

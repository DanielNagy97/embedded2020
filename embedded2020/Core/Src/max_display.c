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

uint8_t character_table[103][9] = {
{2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, /* 32 space */
{1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x1, 0x0}, /* ! */
{4, 0x5, 0x5, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0}, /* " */
{5, 0x0, 0x5, 0x1f, 0xa, 0x1f, 0x14, 0x0, 0x0}, /* # */
{5, 0xf, 0x14, 0x14, 0xe, 0x5, 0x5, 0x1e, 0x0}, /* $ */
{5, 0x0, 0x19, 0x1a, 0x4, 0xb, 0x13, 0x0, 0x0}, /* % */
{5, 0xc, 0x12, 0x12, 0xd, 0x12, 0x12, 0xd, 0x0}, /* & */
{1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, /* ' */
{2, 0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x1, 0x0}, /* ( */
{2, 0x2, 0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x0}, /* ) */
{5, 0x0, 0x15, 0xe, 0x1f, 0xe, 0x15, 0x0, 0x0}, /* * */
{5, 0x0, 0x4, 0x4, 0x1f, 0x4, 0x4, 0x0, 0x0}, /* + */
{2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2}, /* , */
{3, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0}, /* - */
{1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0}, /* . */
{5, 0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x0, 0x0}, /* / */
{5, 0xe, 0x11, 0x13, 0x15, 0x19, 0x11, 0xe, 0x0}, /* 0 */
{3, 0x2, 0x6, 0x2, 0x2, 0x2, 0x2, 0x7, 0x0}, /* 1 */
{5, 0xe, 0x11, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x0}, /* 2 */
{5, 0xe, 0x11, 0x1, 0x6, 0x1, 0x11, 0xe, 0x0}, /* 3 */
{5, 0x2, 0x6, 0xa, 0x12, 0x1f, 0x2, 0x2, 0x0}, /* 4 */
{5, 0x1f, 0x10, 0x10, 0x1e, 0x1, 0x1, 0x1e, 0x0}, /* 5 */
{5, 0xe, 0x11, 0x10, 0x1e, 0x11, 0x11, 0xe, 0x0}, /* 6 */
{5, 0x1f, 0x11, 0x2, 0x4, 0x8, 0x8, 0x8, 0x0}, /* 7 */
{5, 0xe, 0x11, 0x11, 0xe, 0x11, 0x11, 0xe, 0x0}, /* 8 */
{5, 0xe, 0x11, 0x11, 0xf, 0x1, 0x11, 0xe, 0x0}, /* 9 */
{1, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x0}, /* : */
{2, 0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x2, 0x0}, /* ; */
{3, 0x0, 0x1, 0x2, 0x4, 0x2, 0x1, 0x0, 0x0}, /* < */
{5, 0x0, 0x0, 0x1f, 0x0, 0x1f, 0x0, 0x0, 0x0}, /* = */
{3, 0x0, 0x4, 0x2, 0x1, 0x2, 0x4, 0x0, 0x0}, /* > */
{5, 0xe, 0x11, 0x1, 0x2, 0x4, 0x0, 0x4, 0x0}, /* ? */
{5, 0xe, 0x11, 0x17, 0x15, 0x17, 0x8, 0x7, 0x0}, /* @ */
{5, 0xe, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0}, /* 65 A */
{5, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e, 0x0}, /* B */
{5, 0xf, 0x10, 0x10, 0x10, 0x10, 0x10, 0xf, 0x0}, /* C */
{5, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1e, 0x0}, /* D */
{5, 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f, 0x0}, /* E */
{5, 0x1f, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10, 0x0}, /* F */
{5, 0xe, 0x10, 0x10, 0x13, 0x11, 0x11, 0xe, 0x0}, /* G */
{5, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x0}, /* H */
{3, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x0}, /* I */
{5, 0x7, 0x1, 0x1, 0x1, 0x1, 0x11, 0xe, 0x0}, /* J */
{5, 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x0}, /* K */
{5, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x0}, /* L */
{5, 0x11, 0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x0}, /* M */
{5, 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x0}, /* N */
{5, 0xe, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0}, /* O */
{5, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10, 0x0}, /* P */
{5, 0xe, 0x11, 0x11, 0x11, 0x11, 0x12, 0xd, 0x0}, /* Q */
{5, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x11, 0x0}, /* R */
{5, 0xf, 0x10, 0x10, 0xe, 0x1, 0x1, 0x1e, 0x0}, /* S */
{5, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0}, /* T */
{5, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0}, /* U */
{5, 0x11, 0x11, 0x11, 0xa, 0xa, 0x4, 0x4, 0x0}, /* V */
{5, 0x11, 0x11, 0x15, 0x15, 0x15, 0x1b, 0x11, 0x0}, /* W */
{5, 0x11, 0x11, 0xa, 0x4, 0xa, 0x11, 0x11, 0x0}, /* X */
{5, 0x11, 0x11, 0xa, 0x4, 0x4, 0x4, 0x4, 0x0}, /* Y */
{5, 0x1f, 0x1, 0x2, 0x4, 0x8, 0x10, 0x1f, 0x0}, /* Z */
{5, 0x0, 0x0, 0xe, 0x1, 0xf, 0x11, 0xf, 0x0}, /*  97 a (index:59) */
{5, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x1e, 0x0}, /* b */
{5, 0x0, 0x0, 0xf, 0x10, 0x10, 0x10, 0xf, 0x0}, /* c */
{5, 0x1, 0x1, 0xf, 0x11, 0x11, 0x11, 0xf, 0x0}, /* d */
{5, 0x0, 0x0, 0xe, 0x11, 0x1f, 0x10, 0xf, 0x0}, /* e */
{3, 0x3, 0x4, 0x6, 0x4, 0x4, 0x4, 0x4, 0x0}, /* f */
{5, 0x0, 0x0, 0xf, 0x11, 0x11, 0xf, 0x1, 0xe}, /* g */
{5, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x11, 0x11, 0x0}, /* h */
{3, 0x2, 0x0, 0x6, 0x2, 0x2, 0x2, 0x7, 0x0}, /* i */
{3, 0x1, 0x0, 0x3, 0x1, 0x1, 0x1, 0x1, 0x6}, /* j */
{5, 0x10, 0x10, 0x11, 0x12, 0x1c, 0x12, 0x11, 0x0}, /* k */
{2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x1, 0x0}, /* l */
{5, 0x0, 0x0, 0x1a, 0x15, 0x15, 0x15, 0x15}, /* m */
{5, 0x0, 0x0, 0x16, 0x19, 0x11, 0x11, 0x11, 0x0}, /* n */
{5, 0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0}, /* o */
{5, 0x0, 0x0, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10}, /* p */
{5, 0x0, 0x0, 0xf, 0x11, 0x11, 0xf, 0x1, 0x1}, /* q */
{5, 0x0, 0x0, 0x16, 0x19, 0x10, 0x10, 0x10, 0x0}, /* r */
{5, 0x0, 0x0, 0xf, 0x10, 0xe, 0x1, 0x1e, 0x0}, /* s */
{4, 0x4, 0x4, 0xe, 0x4, 0x4, 0x4, 0x3, 0x0}, /* t */
{5, 0x0, 0x0, 0x11, 0x11, 0x11, 0x13, 0xd, 0x0}, /* u */
{5, 0x0, 0x0, 0x11, 0x11, 0xa, 0xa, 0x4, 0x0}, /* v */
{5, 0x0, 0x0, 0x15, 0x15, 0x15, 0x15, 0xa, 0x0}, /* w */
{5, 0x0, 0x0, 0x11, 0xa, 0x4, 0xa, 0x11, 0x0}, /* x */
{5, 0x0, 0x0, 0x11, 0x11, 0x11, 0xf, 0x1, 0xe}, /* y */
{5, 0x0, 0x0, 0x1f, 0x2, 0x4, 0x8, 0x1f, 0x0}, /* z */
{5, 0x2, 0x4, 0xe, 0x11, 0x1f, 0x11, 0x11, 0x0}, /* Á */
{5, 0x2, 0x4, 0x1f, 0x10, 0x1e, 0x10, 0x1f, 0x0}, /* É */
{5, 0x1, 0x2, 0xe, 0x4, 0x4, 0x4, 0xe, 0x0}, /* Í */
{5, 0x2, 0x4, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ó */
{5, 0xa, 0x0, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ö */
{5, 0x5, 0xa, 0xe, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ő */
{5, 0x2, 0x4, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ú */
{5, 0xa, 0x0, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ü */
{5, 0x5, 0xa, 0x11, 0x11, 0x11, 0x11, 0xe, 0x0}, /* Ű */
{5, 0x2, 0x4, 0xe, 0x1, 0xf, 0x11, 0xf, 0x0}, /* á */
{5, 0x2, 0x4, 0xe, 0x11, 0x1f, 0x10, 0xf, 0x0}, /* é */
{3, 0x1, 0x2, 0x6, 0x2, 0x2, 0x2, 0x7, 0x0}, /* í */
{5, 0x2, 0x4, 0x0, 0xe, 0x11, 0x11, 0xe, 0x0}, /* ó */
{5, 0x0, 0xa, 0x0, 0xe, 0x11, 0x11, 0xe, 0x0}, /* ö */
{5, 0x5, 0xa, 0x0, 0xe, 0x11, 0x11, 0xe, 0x0}, /* ő */
{5, 0x2, 0x4, 0x0, 0x11, 0x11, 0x13, 0xd, 0x0}, /* ú */
{5, 0x0, 0xa, 0x0, 0x11, 0x11, 0x13, 0xd, 0x0}, /* ü */
{5, 0x5, 0xa, 0x0, 0x11, 0x11, 0x13, 0xd, 0x0}, /* ű */
};

uint8_t screen_buffer[NUMBER_OF_CELLS][8] = {0};

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

void empty_screen_buffer(){
	/**
	  * @brief Setting all values to 0
	  * @param None
	  * @return None
	  */
	for(int i = 0; i<NUMBER_OF_CELLS; i++){
		for(int j = 0; j<8; j++){
			screen_buffer[i][j] = 0;
		}
	}
}

void shift_screen_buffer_right(){
	/**
	  * @brief Shifting the whole screen buffer by 1 bit to the right
	  * @param None
	  * @return None
	  */
	for(int i = NUMBER_OF_CELLS-1; i>=0; i--){
		for(int j = 0; j<8; j++){
			switch (i) {
				case 0:
					screen_buffer[i][j] =
						screen_buffer[i][j] >> 1;
					break;
				default:
					screen_buffer[i][j] =
						screen_buffer[i][j] >> 1
							| (screen_buffer[i-1][j] & 0x1) << 7;
					break;
			}
		}
	}
}

void shift_screen_buffer_left(){
	/**
	  * @brief Shifting the whole screen buffer by 1 bit to the left
	  * @param None
	  * @return None
	  */
	for(int i = 0; i<NUMBER_OF_CELLS; i++){
		for(int j = 0; j<8; j++){
			switch (i) {
				case NUMBER_OF_CELLS-1:
					screen_buffer[i][j] =
						screen_buffer[i][j] << 1;
					break;
				default:
					screen_buffer[i][j] =
						screen_buffer[i][j] << 1
							| ((screen_buffer[i+1][j] & 0x80) >> 7);
					break;
			}
		}
	}
}

uint8_t* get_character(int code){
	/**
	  * @brief Get a byte from a specified character
	  * @param code The ASCII code of the character
	  * @param i The i.th row of the character
	  * @return the pointer of the character
	  */
	uint8_t* character_byte;

	if (code >= 32 && code <= 90){
		character_byte = character_table[code-0x20];
	}
	else if (code >= 97 && code <= 122){
		character_byte = character_table[code-0x26];
	}
	else{
		switch (code) {
			case 129: /* 129 Á */
				character_byte = character_table[85];
				break;
			case 137: /* É */
				character_byte = character_table[86];
				break;
			case 141: /* Í */
				character_byte = character_table[87];
				break;
			case 147: /* Ó */
				character_byte = character_table[88];
				break;
			case 150: /* Ö */
				character_byte = character_table[89];
				break;
			case 144: /* Ő */
				character_byte = character_table[90];
				break;
			case 154: /* Ú */
				character_byte = character_table[91];
				break;
			case 156: /* Ü */
				character_byte = character_table[92];
				break;
			case 176: /* Ű */
				character_byte = character_table[93];
				break;
			case 161: /* á */
				character_byte = character_table[94];
				break;
			case 169: /* é */
				character_byte = character_table[95];
				break;
			case 173: /* í */
				character_byte = character_table[96];
				break;
			case 179: /* ó */
				character_byte = character_table[97];
				break;
			case 182: /* ö */
				character_byte = character_table[98];
				break;
			case 145: /* ő */
				character_byte = character_table[99];
				break;
			case 186: /* ú */
				character_byte = character_table[100];
				break;
			case 188: /* ü */
				character_byte = character_table[101];
				break;
			case 177: /* ű */
				character_byte = character_table[102];
				break;
			default: /* Unknown character will be displayed as starts */
				character_byte = character_table[10];
				break;
		}
	}
	return character_byte;
}

void put_column_to_screen_buffer(uint8_t* character, int char_column){
	/**
	  * @brief Putting a character column to the end of the screen_buffer
	  * @param character The character as uint8_t*
	  * @param char_column The column of the character to display as int
	  * @return None
	  */
	/* bit masking - The k.th bit of n: (n & ( 1 << k )) >> k */
    for(int i=0; i<8; i++){
        screen_buffer[NUMBER_OF_CELLS-1][i] =
            screen_buffer[NUMBER_OF_CELLS-1][i]
                | (character[i+1] & ( 1 << char_column )) >> char_column;
    }
}

void put_character_to_screen_buffer(uint8_t* character, uint8_t cell, uint8_t shift){
	/**
	  * @brief Putting character to a specified specified cell
	  * @param character The character as a uint8_t * 8x8 matrix
	  * @param cell The index of the led matrix cell as uint8_t (starts from 0)
	  * @return None
	  */
	for(int i = 0; i<8; i++){
		screen_buffer[cell][i] = character[i+1] << shift;
	}
}

void put_byte_to_screen_buffer_with_OR(uint8_t byte, uint8_t column, uint8_t row){
	/**
	  * @brief Putting a to the screen_buffer
	  * @param byte The byte as uint8_t*
	  * @param column The cell of the matrix
	  * @param row The row of the matrix
	  * @return None
	  */
	screen_buffer[column][row] = screen_buffer[column][row] | byte;
}

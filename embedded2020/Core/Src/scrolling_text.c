/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file st.c
 * @author Daniel Nagy
 * @date 16 Oct 2020
 * @brief Methods for scrolling text on the display
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "scrolling_text.h"

void scroll_text_left(char* text, uint16_t speed,
					  uint8_t front_blank_space, uint8_t back_blank_space){
	/**
	  * @brief Scrolling text on the screen (with delays)
	  * @param text The input text as char*
	  * @param speed The delay rate of the shifting as uint16_t
	  * @param front_blank_space The 0 bits before the text as uint8_t
	  * @param back_blank_space The 0 bits after the text as uint8_t
	  * @return None
	  */
	/* Making blank space before the text */
	for(int i = 0; i<front_blank_space; i++){
		shift_screen_buffer_left();
		display_screen_buffer();
		HAL_Delay(speed);
	}

	for(int i = 0; i<strlen(text); i++){
		/* Getting rid of the special character's prefix byte */
		if(text[i] == 195 || text[i] == 197){
			continue;
		}
		uint8_t* character = get_character(text[i]);
		/* character[0] is the width info of the character */
		for(int j = character[0]; j>=0; j--){
			shift_screen_buffer_left();
			put_column_to_screen_buffer(character, j);
			display_screen_buffer();
			HAL_Delay(speed);
		}
	}
	/* Making blank space after the text */
	for(int i = 0; i<back_blank_space; i++){
		shift_screen_buffer_left();
		display_screen_buffer();
		HAL_Delay(speed);
	}
}

void scroll_text_left_IT(Scrolling_text* st){
	/**
	  * @brief Scrolling text on the screen with interrupts
	  * (displaying one column at a time)
	  * @param st The Scrolling_text struct's pointer
	  * @return None
	  */
	if(st->char_index < strlen(st->text)){
	  	if(st->text[st->char_index] == 195 || st->text[st->char_index] == 197){
	  		(st->char_index)++;
	  	}
	  	uint8_t* character = get_character(st->text[st->char_index]);

	  	if(st->char_column < 0){
	  		st->char_column = character[0];
	  	}
	    if(st->char_column >= 0){
	        shift_screen_buffer_left();
	        put_column_to_screen_buffer(character, st->char_column);
	        display_screen_buffer();
	        (st->char_column)--;
	    }
	    if(st->char_column < 0){
	        (st->char_index)++;
	    }
	}
	else if (st->char_index < strlen(st->text)+(NUMBER_OF_CELLS*8)){
        /* shifting x times */
        shift_screen_buffer_left();
        display_screen_buffer();
        (st->char_index)++;
	}
	else{
		//text[0] = '\0'; // text nullázása
		st->char_index = 0;
		st->char_column = -1;
	}
}

void show_clock_face(char* time, int mp){

	  put_character_to_screen_buffer(time[0], 0, 1);
	  put_character_to_screen_buffer(time[1], 1, 2);
	  put_character_to_screen_buffer(time[2], 2, 0);
	  put_character_to_screen_buffer(time[3], 3, 1);

	  /*
	  if(mp){
		  for(int j = 1; j<=2; j++){
			  screen_buffer[1][j] = screen_buffer[1][j] | 0x1;
			  screen_buffer[2][j] = screen_buffer[2][j] | 0x80;
			  screen_buffer[1][j+4] = screen_buffer[1][j+4] | 0x1;
			  screen_buffer[2][j+4] = screen_buffer[2][j+4] | 0x80;
		  }
	  }
	  */

	  display_screen_buffer();
}

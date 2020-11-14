/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file scrolling_text.h
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Header file of the scrolling_text.c
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "main.h"
#include "max_display.h"

#ifndef INC_SCROLLING_TEXT_H_
#define INC_SCROLLING_TEXT_H_

typedef struct Scrolling_texts {
   char text[600];
   int times;
   int char_index;
   int char_column;
} Scrolling_text;

void scroll_text_left(char* text, uint16_t speed, uint8_t front_blank_space, uint8_t back_blank_space);

void scroll_text_left_IT(Scrolling_text *scrolling_text);

#endif /* INC_SCROLLING_TEXT_H_ */

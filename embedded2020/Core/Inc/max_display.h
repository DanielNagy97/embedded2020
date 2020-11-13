/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file max_display.h
 * @author Daniel Nagy
 * @date 16 Oct 2020
 * @brief Header file of the max_display.c
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "main.h"
#include "max_driver.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef INC_MAX_DISPLAY_H_
#define INC_MAX_DISPLAY_H_

volatile uint8_t update_screen;

void display_screen_buffer();

void empty_screen_buffer();

void shift_screen_buffer_right();

void shift_screen_buffer_left();

uint8_t* get_character(int code);

void put_column_to_screen_buffer(uint8_t* character, int char_column);

void put_character_to_screen_buffer(uint8_t* character, uint8_t cell, uint8_t shift);

void put_byte_to_screen_buffer_with_OR(uint8_t byte, uint8_t column, uint8_t row);

#endif /* INC_MAX_DISPLAY_H_ */

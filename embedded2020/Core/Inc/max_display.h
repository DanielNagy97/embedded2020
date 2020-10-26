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
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef INC_MAX_DISPLAY_H_
#define INC_MAX_DISPLAY_H_

#define MAXPORT GPIOA
#define DATA_PIN GPIO_PIN_7
#define CS_PIN GPIO_PIN_6
#define CLOCK_PIN GPIO_PIN_5
#define NUMBER_OF_CELLS 4

volatile uint32_t interrupt_counter;

void write_byte (uint8_t byte);

void write_max_cmd (uint8_t address, uint8_t cmd);

void max_clear();

void max_init (uint8_t brightness);

void set_byte_on_matrix(uint8_t byte, uint8_t row, uint8_t column);

void put_character_to_screen_buffer(char character, uint8_t cell, uint8_t shift);

void display_screen_buffer();

void shift_screen_buffer_right();

void shift_screen_buffer_left();

uint8_t* get_character(int code);

void scroll_text_left(char* text, uint16_t speed, uint8_t front_blank_space, uint8_t back_blank_space);

void show_clock_face(char* time, int mp);

#endif /* INC_MAX_DISPLAY_H_ */

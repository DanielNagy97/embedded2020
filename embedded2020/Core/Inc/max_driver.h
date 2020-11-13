/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file max_driver.h
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Header file of the max_driver.c
 * @see https://github.com/DanielNagy97/embedded2020
 */
#include "main.h"

#ifndef INC_MAX_DRIVER_H_
#define INC_MAX_DRIVER_H_

#define MAXPORT GPIOA
#define DATA_PIN GPIO_PIN_7
#define CS_PIN GPIO_PIN_6
#define CLOCK_PIN GPIO_PIN_5
#define NUMBER_OF_CELLS 4

void write_byte (uint8_t byte);

void write_max_cmd (uint8_t address, uint8_t cmd);

void max_clear();

void max_init (uint8_t brightness);

void set_byte_on_matrix(uint8_t byte, uint8_t row, uint8_t column);

#endif /* INC_MAX_DRIVER_H_ */

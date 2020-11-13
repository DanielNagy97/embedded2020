/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file clock_face.h
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Header file of the clock_face.c
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "main.h"
#include "max_display.h"

#ifndef INC_CLOCK_FACE_H_
#define INC_CLOCK_FACE_H_

volatile uint8_t update_clock;

void show_clock_face(RTC_TimeTypeDef* currTime);

char* get_date_string(RTC_DateTypeDef* currDate);

#endif /* INC_CLOCK_FACE_H_ */

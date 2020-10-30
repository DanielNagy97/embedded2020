/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file esp01.h
 * @author Daniel Nagy
 * @date 27 Oct 2020
 * @brief Header file of the esp01.c
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "main.h"
#include "max_display.h" /* for debugging */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef INC_ESP01_H_
#define INC_ESP01_H_

UART_HandleTypeDef huart1;

void send_uart(char* text, uint32_t timeout);

uint8_t* get_uart(uint32_t timeout);

uint8_t* uart_waitfor(char* text, uint32_t times, uint32_t timeout);

char* esp_init(char* ssid, char* pswd);

void server_handle();

#endif /* INC_ESP01_H_ */

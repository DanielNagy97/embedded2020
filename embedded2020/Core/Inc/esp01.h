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
#include "uart_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef INC_ESP01_H_
#define INC_ESP01_H_

UART_HandleTypeDef huart1;
volatile uint8_t uart_interrupt;
volatile int uart_interrupt_counter;

char* esp_init(char* ssid, char* pswd);

void server_handle(char* uart_receive, Scrolling_text *scrolling_text);

#endif /* INC_ESP01_H_ */

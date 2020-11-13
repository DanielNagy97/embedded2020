/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file uart_handler.h
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Header file of the uart_handler.c
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "main.h"
#include "scrolling_text.h"

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

UART_HandleTypeDef huart1;
volatile uint8_t uart_interrupt;

void send_uart(char* text, uint32_t timeout);

uint8_t* get_uart(uint32_t timeout);

uint8_t* uart_waitfor(char* text, uint32_t times, uint32_t timeout);

#endif /* INC_UART_HANDLER_H_ */

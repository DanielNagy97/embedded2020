/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file uart_handler.c
 * @author Daniel Nagy
 * @date 13 Nov 2020
 * @brief Methods for UART communication
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "uart_handler.h"

void send_uart(char* text, uint32_t timeout){
	/**
	  * @brief Sending data on UART
	  * @param text The input data as char*
	  * @param timeout The timeout of the UART communication as uint32_t
	  * @return None
	  */
	  HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), timeout);
}

uint8_t* get_uart(uint32_t timeout){
	/**
	  * @brief Getting data on UART
	  * @param uart_output The output data buffer's pointer as uint8_t*
	  * @param timeout The timeout of the UART communication as uint32_t
	  * @return None
	  */
	  static uint8_t uart_output[600] = {0};
	  HAL_UART_Receive(&huart1, uart_output, 600, timeout);
	  return uart_output;
}

uint8_t* uart_waitfor(char* text, uint32_t times, uint32_t timeout){
	/**
	  * @brief Waiting for a message on UART
	  * @param text The expected message as char*
	  * @param times The number of tries as uint32_t
	  * @return None
	  */
	  uint8_t* uart_receive;
	  int i = 0;
	  do{
		  if(i >= times){
			  /* for debugging/error showing */
			  scroll_text_left("ERROR", 30, 0, 0);
			  break;
		  }
		  uart_receive = get_uart(timeout);
		  i++;
	  }while(!strstr((char*)uart_receive, text));

	  return uart_receive;
}

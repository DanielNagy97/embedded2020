/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file esp01.c
 * @author Daniel Nagy
 * @date 27 Oct 2020
 * @brief Methods for operating the esp01 and for UART communication
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "esp01.h"

char* webpage = "HTTP/1.1 200 OK\n\
Content-Type: text/html\n\
Connection: close\n\n\
<!DOCTYPE html>\n\
<html>\n\
<head>\n\
<meta charset=\"UTF-8\">\n\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\">\n\
</head>\n\
<body>\n\
Send this:\n\
<input type=\"text\" id=\"TXT\" value=\"\">\n\
</br>\n\
<button onclick=\"sendTxt()\">Send to ESP</button>\n\
<script>\n\
function sendTxt() {\n\
var x = document.getElementById(\"TXT\").value;\n\
var url = document.URL;\n\
var msg = \"+MSG,\"+x;\n\
var xhr = new XMLHttpRequest();\n\
xhr.open(\"POST\", url, true);\n\
xhr.send(msg);\n\
}\n\
</script>\n\
</body>\n\
</html>\n\r\n";

/* TODO: Put UART methods to separate source files */

void send_uart(char* text, uint32_t timeout){
	/**
	  * @brief Sending data on UART
	  * @param text The input data as char*
	  * @param timeout The timeout of the UART communication as uint32_t
	  * @return None
	  */
	  HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), timeout);
}

void get_uart(uint8_t* uart_output, uint32_t timeout){
	/**
	  * @brief Getting data on UART
	  * @param uart_output The output data buffer's pointer as uint8_t*
	  * @param timeout The timeout of the UART communication as uint32_t
	  * @return None
	  */
	  HAL_UART_Receive(&huart1, uart_output, 600, timeout);
}

/* TODO: returns to received msg instead of referencing to outer pointer! */
void uart_waitfor(char* text, uint32_t times, uint8_t* uart_receive){
	/**
	  * @brief Waiting for a message on UART
	  * @param text The expected message as char*
	  * @param times The number of tries as uint32_t
	  * @return None
	  */
	uart_receive[0] = '\0';
	int i = 0;
	do{
		if(i >= times){
			scroll_text_left(text, 30, 0, 0);
			break;
		}
		get_uart(uart_receive, 3000);
		i++;
	}while(!strstr((char*)uart_receive, text));
}

char* esp_init(char* ssid, char* pswd){
	/**
	  * @brief Initalizing the esp-01 with AT commands
	  * @param ssid The SSID of the Wifi as char*
	  * @param pswd The password of the Wifi as char*
	  * @return ip_address The esp device's IP as char*
	  */
	  char data[80] = {0};
	  uint8_t uart_receive[200] = {0};

	  /* AT OK? */
	  send_uart("AT\r\n", 1000);
	  uart_waitfor("AT\r\r\n\r\nOK\r\n", 10, uart_receive);

	  /* CWMODE 1 */
	  send_uart("AT+CWMODE=1\r\n", 1000);
	  uart_waitfor("AT+CWMODE=1\r\r\n\r\nOK\r\n", 5, uart_receive);

	  /* Connect to Wifi */
	  sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pswd);
	  send_uart(data, 2000);
	  uart_waitfor("WIFI GOT", 10, uart_receive);

	  /* Get IP address */
	  send_uart("AT+CIFSR\r\n", 1000);
	  uart_waitfor("CIFSR:STAIP,\"", 10, uart_receive);
	  /* Getting to ip address out of the message*/
	  char* message_ip = strstr((char*)uart_receive, "CIFSR:STAIP,");
	  static char ip_address[20] = {0};
	  if(message_ip != NULL) {
		  message_ip = strtok(message_ip,"\"");
		  message_ip = strtok(NULL,"\"");
		  sprintf(ip_address, "%s", message_ip);
	  }

	  /* CIPMUX 1 */
	  send_uart("AT+CIPMUX=1\r\n", 1000);
	  uart_waitfor("AT+CIPMUX=1\r\r\n\r\nOK\r\n", 5, uart_receive);

	  /* Open port 80 */
	  send_uart("AT+CIPSERVER=1,80\r\n", 1000);
	  uart_waitfor("OK\r\n", 5, uart_receive);

	  return ip_address;
}

void server_start(){
	/**
	  * @brief Running the server
	  * @return None
	  */
	  //static char text_buffer[600] = {0};
	  uint8_t uart_receive[600] = {0};

	  get_uart(uart_receive, 10000); //request

	  if(strstr((char*)uart_receive, "GET")){
		  char *link_id = strstr((char*)uart_receive, "+IPD,");
		  if(link_id != NULL) {
			  link_id = strtok(link_id, ",");
			  link_id = strtok(NULL, ",");
		      //link_id = (uint8_t)pch;

		      int len = strlen(webpage);
		      char data[80];
		      sprintf (data, "AT+CIPSEND=%s,%d\r\n", link_id, len);

		      send_uart(data, 1000);
		      HAL_Delay(1000);
		      /* Wait for > (AT prompt)*/
		      send_uart(webpage, 5000);
		      HAL_Delay(500);

		      send_uart("AT+CIPCLOSE=0\r\n", 2000);
		  }
	  }
	  else if(strstr((char*)uart_receive, "POST")){
		  char *message = strstr((char*)uart_receive, "+MSG,");
		  if(message != NULL) {
			  message = strtok(message, ",");
			  message = strtok(NULL, ",");
			  scroll_text_left(message, 30, 6, 32);
		  }
		  char *link_id = strstr((char*)uart_receive, "+IPD,");
		  if(link_id != NULL) {
			  link_id = strtok(link_id, ",");
			  link_id = strtok(NULL, ",");
		      //link_id = (uint8_t)pch;


		  }
	  }

}
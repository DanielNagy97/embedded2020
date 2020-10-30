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
<link rel=\"shortcut icon\" href=\"\" />\
<meta charset=\"UTF-8\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\">\n\
<style>body{text-align:center;margin:100px 10% 200px 10%;}\n\
#cont1{padding:10px;box-shadow:9px 9px 17px 0px rgba(50, 50, 50, 0.75);\n\
background:lightgrey;font-size:30px;}\n\
input{width:90%;font-size:20px;margin-top:40px;margin-bottom:10px;}\n\
button{position:relative;background-color:grey;border:none;font-size:16px;\n\
color:white;padding:20px;width:150px;text-align:center;transition-duration:0.4s;\n\
text-decoration:none;overflow:hidden;cursor:pointer;}\n\
button:after{content:\"\";background:#f1f1f1;display:block;position:absolute;\n\
padding-top:300%;padding-left:350%;margin-left:-20px !important;margin-top:-120%;\n\
opacity:0;transition:all 0.8s}\n\
button:active:after{padding:0;margin:0;opacity:1;transition:0s}</style>\n\
<script>\n\
function sendTxt() {\n\
var x=document.getElementById(\"TXT\").value;\n\
var url=document.URL;\n\
var msg=\"+MSG~\"+x+\"~\";\n\
var xhr=new XMLHttpRequest();\n\
xhr.open(\"POST\",url,true);\n\
xhr.send(msg);\n\
}\n\
</script>\n</head>\n\
<body>\n\
<div id=\"cont1\">\n\
STM32 Clock\n\
<br>\n\
<input type=\"text\" id=\"TXT\" value=\"\">\n\
</br>\n\
<button onclick=\"sendTxt()\">Send text</button>\n\
</div>\n\
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

char* esp_init(char* ssid, char* pswd){
	/**
	  * @brief Initalizing the esp-01 with AT commands
	  * @param ssid The SSID of the Wifi as char*
	  * @param pswd The password of the Wifi as char*
	  * @return ip_address The esp device's IP as char*
	  */
	  char data[80] = {0};
	  uint8_t* uart_receive;

	  /* Resetting the esp-01 */
	  send_uart("AT+RST\r\n", 1000);
	  HAL_Delay(2000);

	  /* AT OK? */
	  send_uart("AT\r\n", 1000);
	  uart_waitfor("AT\r\r\n\r\nOK\r\n", 10, 1000);

	  /* CWMODE 1 / set Station mode */
	  send_uart("AT+CWMODE=1\r\n", 1000);
	  uart_waitfor("AT+CWMODE=1\r\r\n\r\nOK\r\n", 5, 1000);

	  /* Connect to Wifi */
	  sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pswd);
	  send_uart(data, 2000);
	  uart_waitfor("WIFI GOT", 10, 2000);
	  /* This delay here is mandatory */
	  HAL_Delay(1500);

	  /* Get IP address */
	  send_uart("AT+CIFSR\r\n", 1000);
	  uart_receive = uart_waitfor("CIFSR:STAIP,", 10, 3000);
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
	  uart_waitfor("AT+CIPMUX=1\r\r\n\r\nOK\r\n", 10, 1000);

	  /* Open port 80 */
	  send_uart("AT+CIPSERVER=1,80\r\n", 1000);
	  uart_waitfor("OK\r\n", 10, 1000);

	  return ip_address;
}

void server_handle(){
	/**
	  * @brief Running the server
	  * @return None
	  */
	  uint8_t* uart_receive;

	  /* TODO: Make this with interrupts! */
	  uart_receive = get_uart(1000); /* receive request */
	  if(!uart_receive){
		  return;
	  }
	  if(strstr((char*)uart_receive, "GET")){
		  char *link_id = strstr((char*)uart_receive, "+IPD,");
		  if(link_id != NULL) {
			  link_id = strtok(link_id, ",");
			  link_id = strtok(NULL, ",");

		      int len = strlen(webpage);
		      char data[30];
		      sprintf (data, "AT+CIPSEND=%s,%d\r\n", link_id, len);
		      send_uart(data, 1000);
		      uart_waitfor(">", 5, 1000); /* The AT prompt */
		      send_uart(webpage, 5000);
		      uart_waitfor("SEND OK", 5, 1000);
		      send_uart("AT+CIPCLOSE=0\r\n", 2000);
		      uart_waitfor("OK", 5, 1000);
		  }
	  }
	  else if(strstr((char*)uart_receive, "POST")){
		  char *message = strstr((char*)uart_receive, "+MSG~");
		  if(message != NULL) {
			  message = strtok(message, "~");
			  message = strtok(NULL, "~");
			  /* TODO: return this message to main*/
			  scroll_text_left(message, 40, 0, 32);
		  }
		  char *link_id = strstr((char*)uart_receive, "+IPD,");
		  if(link_id != NULL) {
			  link_id = strtok(link_id, ",");
			  link_id = strtok(NULL, ",");
		  }
	  }
}

/****************************************************************************
 * Copyright (C) 2020 by Daniel Nagy                                        *
 ****************************************************************************/

/**
 * @file esp01.c
 * @author Daniel Nagy
 * @date 27 Oct 2020
 * @brief Methods for operating the esp01
 * @see https://github.com/DanielNagy97/embedded2020
 */

#include "esp01.h"

extern RTC_HandleTypeDef hrtc;

char* webpage = "HTTP/1.1 200 OK\n\
Content-Type: text/html\n\
Connection: close\n\n\
<!DOCTYPE html>\n\
<html><head>\
<link rel=\"icon\" href=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAS0lEQVR42s2SMQ4AIAjE+P+ncSYdasgNXMJgcyIIlVKPIKdvioAXyWBeJmVpqRZKWtj9QWAKZyWll50b8IcL9JUeQF50n28ckyb0ADG8RLwp05YBAAAAAElFTkSuQmCC\" type=\"image/x-png\" />\n\
<meta charset=\"UTF-8\" />\n\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\" />\n\
<style>\n\
body{text-align:center;margin:100px 10% 0px 10%;background:GhostWhite;}\n\
#cont1{padding:10px;box-shadow:9px 9px 17px 0px rgba(50, 50, 50, 0.75);background:#DCDCDC;}\n\
input{box-sizing:border-box;background:white;width:100%;font-size:20px;height:40px;margin-bottom:5px;}\n\
input[type=number]{width:100px;height:61px;text-align:center;font-size:26px;}\n\
td{text-align:right;font-size:20px;}\n\
.ri{text-align:left;}\n\
#btn{float:right;background:#778899;border:none;font-size:16px;color:white;padding:20px;width:150px;text-align:center;text-decoration:none;}\n\
.btn:active{background:silver;}\n\
</style>\n\
<script>\n\
function sendTxt(){\n\
var d=new Date();\n\
var x=document.getElementById(\"TXT\").value;\n\
var t=document.getElementById(\"TIM\").value;\n\
var i=document.getElementById(\"INT\").value;\n\
var url=document.URL;\n\
var msg=\"+MSG~\"+t+\"~\"+i+\"~\"+d.getDay()+\"~\"+d.getMonth()+\"~\"+d.getDate()+\"~\"+d.getFullYear()+\"~\"+d.getHours()+\"~\"+d.getMinutes()+\"~\"+d.getSeconds()+\"~\"+x+\"~\";\n\
var xhr=new XMLHttpRequest();\n\
xhr.open(\"POST\",url,true);\n\
xhr.send(msg);}\n\
</script>\n\
</head>\n\
<body>\n\
<div id=\"cont1\">\n\
<h2>STM32 Clock</h2>\n\
<br />\n\
<input type=\"text\" id=\"TXT\" value=\"\" />\n\
<br />\n\
<table style=\"width:100%\">\n\
<tr>\n\
<td class=\"ri\">Scroll (1-10):</td>\n\
<td><input type=\"number\" id=\"TIM\" value=\"1\" min=\"1\" max=\"10\"/></td>\n\
</tr>\n\
<tr>\n\
<td class=\"ri\">Intensity (1-8):</td>\n\
<td><input type=\"number\" id=\"INT\" value=\"1\" min=\"1\" max=\"8\"/></td>\n\
</tr>\n\
</table>\n\
<button id=\"btn\" onclick=\"sendTxt()\">Send text</button>\n\
<br style=\"clear:both;\" />\n\
</div>\n\
</body>\n\
</html>\n\r\n";

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
	  scroll_text_left("Boot...", 30, 0, 2);
	  HAL_Delay(1000);

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

void server_handle(char* uart_receive, Scrolling_text *scrolling_text){
	/**
	  * @brief Running the server
	  * @see sudo tcpdump host <ip-of-esp> -v
	  * @return None
	  */
	  if(strstr(uart_receive, "GET")){
		  char *link_id = strstr(uart_receive, "+IPD,");
		  if(link_id != NULL) {
			  link_id = strtok(link_id, ",");
			  link_id = strtok(NULL, ",");

		      int len = strlen(webpage);
		      char data[30];
		      sprintf (data, "AT+CIPSEND=%s,%d\r\n", link_id, len);
		      send_uart(data, 1000);
		      HAL_Delay(100);
		      //uart_waitfor(">", 5, 1000); /* The AT prompt */
		      send_uart(webpage, 5000);
		      HAL_Delay(500);
		      //uart_waitfor("SEND OK", 5, 1000);
		      send_uart("AT+CIPCLOSE=0\r\n", 2000);
		      //uart_waitfor("OK", 5, 1000);
		  }
	  }
	  else if(strstr(uart_receive, "POST")){
		  char* message = strstr(uart_receive, "+MSG~");
		  if(message != NULL) {
			  message = strtok(message, "~");
			  int times = atoi(strtok(NULL, "~"));
			  int intensity = atoi(strtok(NULL, "~"));
			  intensity -= 1;

			  /* Getting the date out of the message */
			  int day = atoi(strtok(NULL, "~"));
			  int month = atoi(strtok(NULL, "~"));
			  int date = atoi(strtok(NULL, "~"));
			  int year = atoi(strtok(NULL, "~"));
			  int hours = atoi(strtok(NULL, "~"));
			  int minutes = atoi(strtok(NULL, "~"));
			  int seconds = atoi(strtok(NULL, "~"));

			  message = strtok(NULL, "~");

			  /* Setting time */
			  RTC_TimeTypeDef currTime = {hours, minutes, seconds};
			  RTC_DateTypeDef currDate = {day, month+1, date, year-2000};
			  HAL_RTC_SetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
			  HAL_RTC_SetTime(&hrtc, &currTime, RTC_FORMAT_BIN);

			  Scrolling_text scrolling_message = {};
			  scrolling_message.times = times; /* 1-10 */
			  scrolling_message.char_column = -1;
			  scrolling_message.char_index = 0;
			  strcpy(scrolling_message.text, message);

			  if(intensity <= 7 && intensity >= 0){
				  set_intensity((uint8_t)intensity);
			  }

			  *scrolling_text = scrolling_message;
		 }
	  }
}

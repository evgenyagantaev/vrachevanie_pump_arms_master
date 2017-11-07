/*
 * usart_obj.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#include "usart_obj.h"


void usart_send_message(char *message)
{
	HAL_UART_Transmit(&huart1, message, strlen(message), 500);
}

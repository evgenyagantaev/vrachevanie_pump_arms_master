/*
 * usart_obj.h
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#ifndef USART_OBJ_H_
#define USART_OBJ_H_

#include "usart.h"

extern UART_HandleTypeDef huart1;

void usart_send_message(char *mesage);

#endif /* USART_OBJ_H_ */

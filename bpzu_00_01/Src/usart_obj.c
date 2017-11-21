/*
 * usart_obj.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#include "string.h"
#include "usart_obj.h"

#include "inflator_obj.h"


void usart_send_message(char *message)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 500);
}

uint8_t *usart_receive_byte()
{
	return &usart_in_data;
}

UART_HandleTypeDef *get_usart_handle()
{
	return &huart1;
}

void usart_polling()
{
	if(new_char_received_flag)
	{
		input_message[input_message_index] = usart_in_data;

		reset_new_char_received_flag();

		if(input_message_index < (INPUT_MESSAGE_LENGTH -1))
			input_message_index++;
		if(usart_in_data == '\n') //end of message
		{
			input_message[input_message_index] = 0;
			set_new_message_received_flag();
		}

		//if(HAL_UART_Receive_IT(&huart1, (uint8_t *)usart_receive_byte(), 1) != HAL_OK)
		//{
			//Error_Handler();
		//}
	}
}

void command_interpreter()
{
	int strip_number, pressure;


	if (new_message_received_flag)
	{


		if((strstr((char *)input_message, "c7m") != NULL) && (strstr((char *)input_message, "p") != NULL) && (strlen((char *)input_message) == 10))
		{
			sscanf((char *)input_message, "c7m%dp%d\r\n", &strip_number, &pressure);

			set_lower_pressure(pressure*10);
			if(strip_number == 1)
			{
				inflate_right_arm();
			}
			else if(strip_number == 2)
			{
				inflate_left_arm();
			}
			else if(strip_number == 0)
			{
				inflate_both_arms();
			}
			inflator_rise_inflate_flag();
		}

		if((strstr((char *)input_message, "c5m") != NULL) && (strlen((char *)input_message) == 6))
		{
			sscanf((char *)input_message, "c5m%d\r\n", &strip_number);

			if(strip_number == 0)
			{
				drop_pressure();
				inflator_drop_inflate_flag();
				input_analizer_common_pressure_drop();
			}
		}


		reset_new_message_received_flag();
		input_message_index = 0;
		input_message[0] = 0;



	}
}

void set_new_char_received_flag()
{
	new_char_received_flag = 1;
}
void reset_new_char_received_flag()
{
	new_char_received_flag = 0;
}
int get_new_char_received_flag()
{
	return new_char_received_flag;
}

void set_new_message_received_flag()
{
	new_message_received_flag = 1;
}
void reset_new_message_received_flag()
{
	new_message_received_flag = 0;
}
int get_new_message_received_flag()
{
	return new_message_received_flag;
}

/*
 * usart_obj.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#include "string.h"
#include "usart_obj.h"

#include "inflator_obj.h"
#include "bluetooth_time_relay_object.h"

#define EEPROM_BASE_ADDRESS (uint32_t *)0x08080000

void eeprom_write_right_pressure(uint32_t right_pressure)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_BASE_ADDRESS);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, EEPROM_BASE_ADDRESS, right_pressure);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

void eeprom_write_left_pressure(uint32_t left_pressure)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_BASE_ADDRESS+4);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, EEPROM_BASE_ADDRESS+4, left_pressure);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

int32_t eeprom_read_right_pressure()
{
	return *(EEPROM_BASE_ADDRESS);
}

int32_t eeprom_read_left_pressure()
{
	return *(EEPROM_BASE_ADDRESS+4);
}


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
		// reset usart silence timout 
		bluetooth_time_relay_counter_reset();


		if(input_message_index < (INPUT_MESSAGE_LENGTH -1))
			input_message_index++;
		if(usart_in_data == '\n') //end of message
		{
			input_message[input_message_index] = 0;
			set_new_message_received_flag();
		}

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


			if(strip_number == 1)
			{
				set_right_lower_pressure(pressure);
				inflator_set_inflate_right_flag();
				inflate_right_arm();
			}
			else if(strip_number == 2)
			{
				set_left_lower_pressure(pressure);
				inflator_set_inflate_left_flag();
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

		if((strstr((char *)input_message, "c9m") != NULL) && (strstr((char *)input_message, "p") != NULL) && (strlen((char *)input_message) == 10))
		{
			sscanf((char *)input_message, "c9m%dp%d\r\n", &strip_number, &pressure);
			// set lower pressure
			if(strip_number == 1)
				eeprom_write_right_pressure(pressure);
			else if(strip_number == 2)
				eeprom_write_left_pressure(pressure);
			else
				eeprom_write_left_pressure(pressure);
		}

		reset_new_message_received_flag();
		input_message_index = 0;
		input_message[0] = 0;

		set_bluetooth_idle_mark();

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

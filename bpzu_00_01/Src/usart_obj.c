/*
 * usart_obj.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

// USART interrupts:				flag	enable bit
//Transmit data register empty	 	TXE 	TXEIE
//CTS interrupt					 	CTSIF 	CTSIE
//Transmission Complete			 	TC 		TCIE
//Receive data register not empty 	RXNE	RXNEIE
//Overrun error detected 			ORE
//Idle line detected				IDLE 	IDLEIE
//Parity error 						PE 		PEIE
//LIN break 						LBDF 	LBDIE
//Noise Flag, Overrun error and
//Framing Error in multibuffer
//communication. 					NF or
//									ORE or
//									FE 		EIE
//Character match 					CMF 	CMIE
//Receiver timeout error 			RTOF 	RTOIE
//End of Block 						EOBF 	EOBIE
//Wakeup from Stop mode 			WUF 	WUFIE

// usart cr1
// (28)M1 (27)EOBIE (26)RTOIE (25-21)DEAT[4:0] (20-16)DEDT[4:0] (15)OVER8 (14)CMIE
// (13)MME (12)M0 (11)WAKE (10)PCE (9)PS (8)PEIE (7)TXEIE (6)TCIE (5)RXNEIE (4)IDLEIE
// (3)TE (2)RE (1)UESM (0)UE

//RXNE: Read data register not empty
//This bit is set by hardware when the content of the RDR shift register has been transferred
//to the USARTx_RDR register. It is cleared by a read to the USARTx_RDR register. The
//RXNE flag can also be cleared by writing 1 to the RXFRQ in the USARTx_RQR register.
//An interrupt is generated if RXNEIE=1 in the USARTx_CR1 register.

// usart icr (interrupt flag clear register)
// (20)WUCF (17)CMCF (12)EOBCF (11)RTOCF (9)CTSCF (8)LBDCF (6)TCCF (4)IDLECF (3)ORECF
// (2)NCF (1)FECF (0)PECF
// drop all flags -> 0000 0000 | 0001 0010 | 0001 1011 | 0101 1111 -> 0x00121b5f



#include "string.h"
#include "usart_obj.h"

#include "inflator_obj.h"
#include "bluetooth_time_relay_object.h"

#define EEPROM_BASE_ADDRESS (uint32_t)0x08080000

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
	return *((uint32_t *)(EEPROM_BASE_ADDRESS));
}

int32_t eeprom_read_left_pressure()
{
	return *((uint32_t *)(EEPROM_BASE_ADDRESS+4));
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

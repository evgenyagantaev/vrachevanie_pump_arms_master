/*
 * usart_obj.h
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#ifndef USART_OBJ_H_
#define USART_OBJ_H_

#define INPUT_MESSAGE_LENGTH 256

#include "usart.h"

extern UART_HandleTypeDef huart1;

static uint8_t usart_in_data;
static uint8_t input_message[INPUT_MESSAGE_LENGTH];
static int input_message_index = 0;
static int new_message_received_flag = 0;

static int new_char_received_flag = 0;

void usart_send_message(char *mesage);
uint8_t usart_get_last_in_byte();
uint8_t *usart_receive_byte();
UART_HandleTypeDef *get_usart_handle();
void usart_polling();

void set_new_char_received_flag();
void reset_new_char_received_flag();
int get_new_char_received_flag();

void set_new_message_received_flag();
void reset_new_message_received_flag();
int get_new_message_received_flag();

void command_interpreter();

void eeprom_write_right_pressure(uint32_t right_pressure);
void eeprom_write_left_pressure(uint32_t left_pressure);
int32_t eeprom_read_right_pressure();
int32_t eeprom_read_left_pressure();

#endif /* USART_OBJ_H_ */

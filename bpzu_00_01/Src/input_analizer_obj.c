/*
 * input_analizer_obj.c
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#include "input_analizer_obj.h"
#include "usart_obj.h"
#include "gpio.h"

#include "pressure_sensor_obj.h"

void input_analizer_check_start_state()
{
	// turn on high level on input lines (pa11 high)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_SET);
	// pause
	HAL_Delay(30);
	// check input lines and fill flags array
	int i;
	for(i=0; i<8; i++)
	{
		if(HAL_GPIO_ReadPin(in_left1_GPIO_Port, input_pins[i]) != GPIO_PIN_RESET)
			start_state_line_flags[i] = 1;
	}
	// pause
	HAL_Delay(30);
	// turn off high level on input lines (pa11 low)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_RESET);
}


void input_analizer_check_lines()
{
	// turn on high level on input lines (pa11 high)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_SET);
	// pause
	HAL_Delay(30);
	// check input lines and fill flags array
	int i;
	for(i=0; i<8; i++)
	{
		if(HAL_GPIO_ReadPin(in_left1_GPIO_Port, input_pins[i]) != GPIO_PIN_RESET)
		{
			line_flags[i] = 1;
			if(!start_state_line_flags[i])
			{
				if(!inflator_get_inflate_flag())   // not yet inflate line signal (never before)
				{
					inflator_rise_inflate_flag();
				}
			}
		}
	}
	// pause
	HAL_Delay(30);
	// turn off high level on input lines (pa11 low)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_RESET);

	//debug_valve_led_imitation();

	// create message
	char aux_string[32];
	message[0] = 0;	// reset string
	for(i=0; i<8; i++)
	{
		sprintf(aux_string, "%c%d", delimeters[i], line_flags[i]);
		strncat(message, aux_string, 2);
	}
	strncat(message, "x\r\n", 3);

	uint32_t pressure = pressure_sensor_get_pressure();
	inflator_set_current_pressure(pressure);
	sprintf(aux_string, "%d\r\n", (int)pressure);
	strncat(message, aux_string, strlen(aux_string));

}

void input_analizer_send_status()
{
	//sprintf(message, "a0b0c1d0e0f0g0h0x\r\n");
	usart_send_message(message);
}

int *input_analizer_get_line_flags()
{
	return line_flags;
}

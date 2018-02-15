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

#include "inflator_obj.h"

void input_analizer_check_start_state()
{
	int i;

	// turn on high level on input lines (pa11 high)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_SET);
	// pause
	//HAL_Delay(30);
	for(i=0; i<2500; i++);
	// check input lines and fill flags array
	for(i=0; i<8; i++)
	{
		if(HAL_GPIO_ReadPin(in_left1_GPIO_Port, input_pins[i]) != GPIO_PIN_RESET)
			start_state_line_flags[i] = 1;
	}
	// pause
	//HAL_Delay(30);
	for(i=0; i<2500; i++);
	// turn off high level on input lines (pa11 low)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_RESET);
}

void input_analizer_common_pressure_drop()
{
	int i;

	for(i=0; i<8; i++)
	{
		start_state_line_flags[i] = line_flags[i];
	}
}


void input_analizer_check_lines()
{
	int i;

	// turn on high level on input lines (pa11 high)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_SET);
	// pause
	//HAL_Delay(30);
	for(i=0; i<2500; i++);
	// check input lines and fill flags array

	for(i=0; i<8; i++)
	{
		if(HAL_GPIO_ReadPin(in_left1_GPIO_Port, input_pins[i]) != GPIO_PIN_RESET)
		{
			line_flags[i] = 1;

		}
	}

	if((left_hand_condition() || right_hand_condition()) && (!inflator_get_inflate_flag()))
	{
		// not yet inflate line signal (never before)
		inflator_rise_inflate_flag();
	}

	if(left_hand_condition() && right_hand_condition())
	{
		inflate_both_arms();
		inflator_set_inflate_right_flag();
		inflator_set_inflate_left_flag();
	}
	else if(left_hand_condition())
	{
		inflate_left_arm();
		inflator_set_inflate_left_flag();
	}
	else if(right_hand_condition())
	{
		inflate_right_arm();
		inflator_set_inflate_right_flag();
	}


	// pause
	for(i=0; i<2500; i++);
	// turn off high level on input lines (pa11 low)
	HAL_GPIO_WritePin(out_lines_power_GPIO_Port, out_lines_power_Pin, GPIO_PIN_RESET);

}

int left_hand_condition()
{
	int l1, l2, l3, l4;

	l1 = line_flags[0] && (!start_state_line_flags[0]);
	l2 = line_flags[1] && (!start_state_line_flags[1]);
	l3 = line_flags[2] && (!start_state_line_flags[2]);
	l4 = line_flags[3] && (!start_state_line_flags[3]);

	return l1 || l2 || l3 || l4;
}

int right_hand_condition()
{
	int l5, l6, l7, l8;

	l5 = line_flags[4] && (!start_state_line_flags[4]);
	l6 = line_flags[5] && (!start_state_line_flags[5]);
	l7 = line_flags[6] && (!start_state_line_flags[6]);
	l8 = line_flags[7] && (!start_state_line_flags[7]);

	return l5 || l6 || l7 || l8;
}




void input_analizer_send_status()
{
	//sprintf(message, "a0b0c1d0e0f0g0h0x\r\n");
	//usart_send_message(message);
}

int input_analizer_get_line_flags(int index)
{
	return line_flags[index];
}

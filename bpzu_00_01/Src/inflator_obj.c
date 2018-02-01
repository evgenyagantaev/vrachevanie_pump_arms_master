/*
 * inflator_obj.c
 *
 *  Created on: May 19, 2017
 *      Author: root
 */

#include "inflator_obj.h"
#include "input_analizer_obj.h"
#include "gpio.h"

#include "pressure_sensor_obj.h"

#include "math.h"






void inflator_turn_motor_on()
{
	HAL_GPIO_WritePin(out_motor_control_GPIO_Port, out_motor_control_Pin, GPIO_PIN_SET);
	motor_on_flag = 1;
}
void inflator_turn_motor_off()
{
	HAL_GPIO_WritePin(out_motor_control_GPIO_Port, out_motor_control_Pin, GPIO_PIN_RESET);
	motor_on_flag = 0;
}
int inflator_get_motor_flag()
{   return motor_on_flag;   }

void inflator_turn_drop_valve_on()
{
	HAL_GPIO_WritePin(out_valve1_control_GPIO_Port, out_valve1_control_Pin, GPIO_PIN_SET);
}
void inflator_turn_drop_valve_off()
{
	HAL_GPIO_WritePin(out_valve1_control_GPIO_Port, out_valve1_control_Pin, GPIO_PIN_RESET);
}

void inflator_drop_inflate_flag()
{	inflate_line_flag = 0;	}
void inflator_rise_inflate_flag()
{	inflate_line_flag = 1;	}
int inflator_get_inflate_flag()
{	return inflate_line_flag;	}

void inflator_set_current_pressure(uint32_t pressure)
{
	current_pressure = pressure;
}

uint32_t inflator_get_current_pressure()
{
	return current_pressure;
}

void inflator_monitor()
{
	uint32_t real_pressure = current_pressure/10 - 25;

	if(inflate_line_flag)
	{
		// power up valves
		// pb10 = 1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

		uint32_t lower_pressure;
		uint32_t upper_pressure;
		if (inflate_right_flag && (!inflate_left_flag))
		{
			lower_pressure = right_lower_pressure;
			upper_pressure = right_upper_pressure;
		}
		else if((!inflate_right_flag) && inflate_left_flag)
		{
			lower_pressure = left_lower_pressure;
			upper_pressure = left_upper_pressure;
		}
		else if(inflate_right_flag && inflate_left_flag)
		{
			if(left_lower_pressure > right_lower_pressure)
				lower_pressure = left_lower_pressure;
			else
				lower_pressure = right_lower_pressure;
			upper_pressure = lower_pressure + PRESSURE_GAP;
		}

		if(real_pressure < lower_pressure)
		{
			pump_up_flag = 1;

		}
		else if(real_pressure > upper_pressure)
		{
			pump_up_flag = 0;
			inflator_turn_motor_off();
		}

		if(pump_up_flag)
			inflator_turn_motor_on();


	}
	else
	{
		pump_up_flag = 0;
		inflator_turn_motor_off();
	}
}

void set_right_lower_pressure(uint32_t pressure)
{
	right_lower_pressure = pressure;
	right_upper_pressure = right_lower_pressure + PRESSURE_GAP;
}
void set_left_lower_pressure(uint32_t pressure)
{
	left_lower_pressure = pressure;
	left_upper_pressure = left_lower_pressure + PRESSURE_GAP;
}

uint32_t inflator_get_right_lower_pressure()
{
	return right_lower_pressure;
}
uint32_t inflator_get_left_lower_pressure()
{
	return left_lower_pressure;
}

void drop_pressure()
{
	// pb4 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	// pb3 = pb5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_3, GPIO_PIN_RESET);
}

void inflate_right_arm()
{
	// pb5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	// pb3 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	// pb4 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

void inflate_left_arm()
{
	// pb5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	// pb3 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	// pb4 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

void inflate_both_arms()
{
	// pb5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	// pb3 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	// pb4 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

void inflator_set_inflate_right_flag()
{
	inflate_right_flag = 1;
}
void inflator_set_inflate_left_flag()
{
	inflate_left_flag = 1;
}





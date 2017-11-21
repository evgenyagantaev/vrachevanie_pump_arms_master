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




void inflate_line(int line_number)
{
	volatile long i;

	// pitanie na servoprivod on

	// pwm na otkrytie ***********
	// stop 1 Hz timer
	timer1hz_stop();
	// start 67 Hz timer
	timer100hz_start(1); // 67 HZ;  1 - flag otkrytiya

	//pauza
	HAL_Delay(2000);
	//for(i=0; i<50000000; i++);

	// pwm na zakrytie
	timer100hz_start(0); // 67 HZ;  0 - flag zakrytiya

	//pauza
	HAL_Delay(2000);
	//for(i=0; i<50000000; i++);

	// pitanie na servoprivod off
	// stop 67 Hz timer
	timer100hz_stop(); // 67 HZ
	// start 1 Hz timer
	timer1hz_start();
}


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
	if(inflate_line_flag)
	{

		if(current_pressure < lower_pressure)
		{
			pump_up_flag = 1;

		}
		else if(current_pressure > upper_pressure)
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

void set_lower_pressure(uint32_t pressure)
{
	lower_pressure = pressure;
	upper_pressure = lower_pressure + PRESSURE_GAP;
}

uint32_t inflator_get_lower_pressure()
{
	return lower_pressure;
}

void drop_pressure()
{
	// pb5 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	// pb3 = pb4 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_RESET);
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
	// pb5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	// pb3 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	// pb4 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

void inflate_both_arms()
{
	// pb5 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	// pb3 = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	// pb4 = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}







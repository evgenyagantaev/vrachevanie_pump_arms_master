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

void debug_valve_led_imitation()
{
	input_flags = input_analizer_get_line_flags();

	// check line flags and fill inflation status array
	int i;
	for(i=0; i<4; i++)
	{
		if(((input_flags[2*i] == 1) || (input_flags[2*i + 1] == 1)) && (line_inflated[i] == 0))
		{
			//inflate_line(i);
			//inflate_line_flag = 1;
			line_inflated[i] = 1;
		}
	}

	// turn on/off leds
	if(line_inflated[0] == 1)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);// turn on
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);// turn off
	if(line_inflated[1] == 1)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);// turn on
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);// turn off
	if(line_inflated[2] == 1)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);// turn on
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);// turn off
	if(line_inflated[3] == 1)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);// turn on
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);// turn off


}


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
}
void inflator_turn_motor_off()
{
	HAL_GPIO_WritePin(out_motor_control_GPIO_Port, out_motor_control_Pin, GPIO_PIN_RESET);
}
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

void inflator_monitor()
{
	if(inflate_line_flag)
	{
		inflator_turn_drop_valve_on();

		if(current_pressure < LOWER_PRESSURE)
		{
			pump_up_flag = 1;

		}
		else if(current_pressure > UPPER_PRESSURE)
		{
			pump_up_flag = 0;
			inflator_turn_motor_off();
		}

		if(pump_up_flag)
			inflator_turn_motor_on();

	}
}









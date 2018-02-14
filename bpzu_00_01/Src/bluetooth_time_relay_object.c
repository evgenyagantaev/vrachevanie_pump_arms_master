/*
 * bluetooth_time_relay_object.c
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */
#include "bluetooth_time_relay_object.h"
#include "one_hz_timer_obj.h"
#include "inflator_obj.h"
#include "input_analizer_obj.h"
#include "gpio.h"



void bluetooth_time_relay_counter_increment()
{
	if(old_one_hz_counter < get_one_hz_counter())
	{
		old_one_hz_counter = get_one_hz_counter();
		bluetooth_off_counter++;

		if(bluetooth_off_counter >= BLUETOOTH_OFF_TIMEOUT)
			set_bluetooth_off_flag();
	}
}


void bluetooth_time_relay_counter_reset()
{
	bluetooth_off_counter = 0;
}

int get_bluetooth_off_flag()
{
	return bluetooth_off_flag;
}

void set_bluetooth_off_flag()
{
	bluetooth_off_flag = 1;
}

int get_bluetooth_on_flag()
{
	return bluetooth_on_flag;
}

void reset_bluetooth_on_flag()
{
	bluetooth_on_flag = 0;
}


void bluetooth_off()
{
	// reset bluetooth control pin (turn bluetooth off)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	// turn analog circuit off
	HAL_GPIO_WritePin(out_analog_onoff_GPIO_Port, out_analog_onoff_Pin, GPIO_PIN_RESET);
	// drop inflate flag
	inflator_drop_inflate_flag();
	// check start state of lines
	input_analizer_check_start_state();

	//debug; turn motor on
	//inflator_turn_motor_on();

	set_right_lower_pressure(eeprom_read_right_pressure());
	set_left_lower_pressure(eeprom_read_left_pressure());
	inflator_reset_inflate_right_flag();
	inflator_reset_inflate_left_flag();

	reset_bluetooth_on_flag();
}




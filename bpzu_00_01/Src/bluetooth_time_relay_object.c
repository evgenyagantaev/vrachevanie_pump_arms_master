/*
 * bluetooth_time_relay_object.c
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */
#include "bluetooth_time_relay_object.h"
#include "one_hz_timer.h"
#include "gpio.h"



void bluetooth_time_relay_counter_increment()
{
	if(old_one_hz_counter < get_one_hz_counter())
	{
		old_one_hz_counter = get_one_hz_counter();
		bluetooth_off_counter++;
	}
}


void bluetooth_time_relay_counter_reset()
{
	bluetooth_time_relay_counter = 0;
}
void bluetooth_off()
{
	// reset bluetooth control pin (turn bluetooth off)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	
}

int get_bluetooth_off_flag()
{
	return bluetooth_off_flag;
}

int set_bluetooth_off_flag()
{
	bluetooth_off_flag = 1;
}

int get_bluetooth_on_flag()
{
	return bluetooth_on_flag;
}

int reset_bluetooth_on_flag();
{
	bluetooth_on_flag = 0;
}






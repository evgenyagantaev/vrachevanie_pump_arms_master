/*
 * bluetooth_time_relay_task.c
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */

#include "bluetooth_time_relay_object.h"
#include "gpio.h"

void bluetooth_time_relay_task()
{
	if(get_bluetooth_on_flag())
	{
		if(get_bluetooth_off_flag())
		{
			bluetooth_off();
		}
		else
		{
			bluetooth_time_relay_counter_increment();
		}

	}

}

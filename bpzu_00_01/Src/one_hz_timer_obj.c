/*
 * one_hz_timer_obj.c
 *
 *  Created on: Jul 25, 2017
 *      Author: root
 */


#include "one_hz_timer_obj.h"
#include "gpio.h"
#include "usart.h"

#include "usart_obj.h"
#include "inflator_obj.h"


void one_hz_timer_tick(void)
{
	systick_counter++;

	if(systick_counter >= 1000)
	{
		one_hz_counter++;
		systick_counter = 0;
		one_hz_timer_flag = 1;
	}
}

void one_hz_timer_poll(void)
{
	static int odd_even_flag = 0;

	if(one_hz_timer_flag)
	{
		one_hz_timer_flag = 0; // drop flag

		// led blink
		HAL_GPIO_TogglePin(GPIOA, out_led_Pin);
		//HAL_GPIO_TogglePin(GPIOB, out_led_Pin);
		uint32_t current_pressure = inflator_get_current_pressure()/10;
		if(current_pressure > 25)
			current_pressure = current_pressure - 25;
		else
			current_pressure = 0;
		inflator_set_current_pressure(current_pressure);



		input_analizer_check_lines();
		input_analizer_send_status();

		sprintf(message, "l%d_%d_%d_%d_%d_%d\r\n",
				inflator_get_motor_flag(), current_pressure, inflator_get_right_lower_pressure(),
				inflator_get_left_lower_pressure(),input_analizer_get_line_flags(2)||input_analizer_get_line_flags(3),
				input_analizer_get_line_flags(6)||input_analizer_get_line_flags(7));
		//HAL_UART_Transmit(&huart1, message, strlen((const char *)message), 500);
		usart_send_message(message);

		if(odd_even_flag)
		{
			odd_even_flag = 0;
		}
		else
		{
			odd_even_flag = 1;
		}

		one_hz_schedule_task();


	}
}


void one_hz_schedule_task(void)
{
	// check bluetooth idle period
	if((one_hz_counter - bluetooth_idle_mark) > BLUETOOTH_IDLE_PERIOD)
		// reset bluetooth control pin (turn bluetooth off)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}


void set_bluetooth_idle_mark()
{
	bluetooth_idle_mark = one_hz_counter;
}



int get_one_hz_counter()
{
	return one_hz_counter;
}

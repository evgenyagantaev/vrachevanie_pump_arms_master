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
		//sprintf(message, "Hello!\r\n");
		//HAL_UART_Transmit(&huart1, message, strlen((const char *)message), 500);
		//usart_send_message(message);

		input_analizer_check_lines();
		input_analizer_send_status();

		if(odd_even_flag)
		{
			//inflator_turn_drop_valve_on();
			odd_even_flag = 0;
		}
		else
		{
			//inflator_turn_drop_valve_off();
			odd_even_flag = 1;
		}


	}
}

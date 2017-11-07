/*
 * pressure_sensor_obj.c
 *
 *  Created on: Jul 28, 2017
 *      Author: root
 */
#include "pressure_sensor_obj.h"
#include "adc.h"

uint32_t pressure_sensor_get_pressure(void)
{

	ADC_ChannelConfTypeDef sConfig;

	// Select Channel 0 to be converted (VDC1)
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	sConfig.Channel = ADC_CHANNEL_1 | ADC_CHANNEL_2;
	sConfig.Rank = ADC_RANK_NONE;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	//***
	HAL_ADC_Start(&hadc);
	if(HAL_ADC_PollForConversion(&hadc, 1000) != HAL_OK)
	{
		//Error_Handler();
	}
	uint32_t adc_ch0 = HAL_ADC_GetValue(&hadc);

	return adc_ch0;
}

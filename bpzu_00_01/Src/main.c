/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

#include "one_hz_timer_obj.h"
#include "usart_obj.h"
#include "inflator_obj.h"

extern UART_HandleTypeDef huart1;


#define DEVICENAME "VRACHEVANIE_001"

// Private constants ---------------------------------------------------------

// Private variables ---------------------------------------------------------

// GLOBAL FLAGS

//*******************


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void ErrorHandler(void);
/* Private function prototypes -----------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//            MAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAIN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{

	volatile long i;

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_ADC_Init();


	//bluetooth startup configuration------------------------------------------------------------------
	usart_send_message("^#^$^%");
	for(i=0; i<2000000; i++); //wait some time with no data to enter command mode if connection is active
	usart_send_message("\r\n");
	for(i=0; i<2000000; i++); //wait some time with no data to enter command mode if connection is active
	usart_send_message("AT+AB HostEvent Disable\r\n"); //disable notification strings, only for this session
	for(i=0; i<25000; i++);
	usart_send_message("AT+AB Config PIN=0000\r\n"); //set pin
	for(i=0; i<25000; i++);
	usart_send_message("AT+AB DefaultLocalName "DEVICENAME"\r\n"); //set device name***************************************************************************
	for(i=0; i<25000; i++);
	usart_send_message("AT+AB Bypass\r\n"); //start connection if connection is still available
	for(i=0; i<250000; i++);

	// 1 blink
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<2500000; i++);

	uint8_t aux_byte;
	// read trash from usart
	*usart_receive_byte() = (uint8_t)(USART1->RDR);
	// clear flag
	USART1->ISR &= ~0x00000020;

	// Enable the UART Data Register not empty Interrupt
	USART1->CR1 |= 0x00000020;  // rxne
	// disable pe interrupt
	USART1->CR1 &= ~0x00000100;  // pe int disable
	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	//  NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));



	// 2 blinks
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<2500000; i++);


	// turn on analog circuit
	HAL_GPIO_WritePin(out_analog_onoff_GPIO_Port, out_analog_onoff_Pin, GPIO_PIN_SET);
	// drop inflate flag
	inflator_drop_inflate_flag();
	// check start state of lines
	input_analizer_check_start_state();

	//debug; turn motor on
	//inflator_turn_motor_on();

	set_right_lower_pressure(eeprom_read_right_pressure());
	set_left_lower_pressure(eeprom_read_left_pressure());

	// 3 blinks
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<2500000; i++);


    // main loop *******************************************************************************************
    // main loop *******************************************************************************************
    // main loop *******************************************************************************************
    // main loop *******************************************************************************************
    // main loop *******************************************************************************************
    while(1)
    {
    	one_hz_timer_poll();
    	inflator_monitor();
    	usart_polling();
    	command_interpreter();
    }
    // end main loop ***************************************************************************************
    // end main loop ***************************************************************************************
    // end main loop ***************************************************************************************
    // end main loop ***************************************************************************************
    // end main loop ***************************************************************************************
    
    


}// end main

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        END MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/(1000));  // systick 1000 uSec

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



static void ErrorHandler(void)
{
    /* Infinite loop */
    while(1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_Delay(300);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_Delay(300);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        HAL_Delay(300);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_Delay(300);
    }
}


void load()
{
	// proveryaem pravyi bort

}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}




#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

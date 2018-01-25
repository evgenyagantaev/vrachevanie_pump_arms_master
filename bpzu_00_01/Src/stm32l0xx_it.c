/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

#include "one_hz_timer_obj.h"
#include "usart_obj.h"
#include "gpio.h"

#include "stm32l0xx_hal.h"


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc;

/******************************************************************************/
/*            Cortex-M0+ Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  long i;

  while (1)
  {
	  // frequent blinks
	//888888888888888888888888888888888888888888888888888
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_SET);
	for(i=0; i<250000; i++);
	HAL_GPIO_WritePin(GPIOA, out_led_Pin, GPIO_PIN_RESET);
	for(i=0; i<2500000; i++);
	//888888888888888888888888888888888888888888888888888
  }

}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	/* USER CODE BEGIN SysTick_IRQn 1 */
	one_hz_timer_tick();
	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles ADC1, COMP1 and COMP2 interrupts (COMP interrupts through EXTI lines 21 and 22).
*/
void ADC1_COMP_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_COMP_IRQn 0 */

  /* USER CODE END ADC1_COMP_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc);
  /* USER CODE BEGIN ADC1_COMP_IRQn 1 */

  /* USER CODE END ADC1_COMP_IRQn 1 */
}

// This function handles USART1 global interrupt

// USART interrupts:				flag	enable bit
//Transmit data register empty	 	TXE 	TXEIE
//CTS interrupt					 	CTSIF 	CTSIE
//Transmission Complete			 	TC 		TCIE
//Receive data register not empty 	RXNE	RXNEIE
//Overrun error detected 			ORE
//Idle line detected				IDLE 	IDLEIE
//Parity error 						PE 		PEIE
//LIN break 						LBDF 	LBDIE
//Noise Flag, Overrun error and
//Framing Error in multibuffer
//communication. 					NF or
//									ORE or
//									FE 		EIE
//Character match 					CMF 	CMIE
//Receiver timeout error 			RTOF 	RTOIE
//End of Block 						EOBF 	EOBIE
//Wakeup from Stop mode 			WUF 	WUFIE

// usart cr1
// (28)M1 (27)EOBIE (26)RTOIE (25-21)DEAT[4:0] (20-16)DEDT[4:0] (15)OVER8 (14)CMIE
// (13)MME (12)M0 (11)WAKE (10)PCE (9)PS (8)PEIE (7)TXEIE (6)TCIE (5)RXNEIE (4)IDLEIE
// (3)TE (2)RE (1)UESM (0)UE

//RXNE: Read data register not empty
//This bit is set by hardware when the content of the RDR shift register has been transferred
//to the USARTx_RDR register. It is cleared by a read to the USARTx_RDR register. The
//RXNE flag can also be cleared by writing 1 to the RXFRQ in the USARTx_RQR register.
//An interrupt is generated if RXNEIE=1 in the USARTx_CR1 register.

// usart icr (interrupt flag clear register)
// (20)WUCF (17)CMCF (12)EOBCF (11)RTOCF (9)CTSCF (8)LBDCF (6)TCCF (4)IDLECF (3)ORECF
// (2)NCF (1)FECF (0)PECF
// drop all flags -> 0000 0000 | 0001 0010 | 0001 1011 | 0101 1111 -> 0x00121b5f

void USART1_IRQHandler(void)
{
	if((USART1->ISR & 0x00000020) != 0x00000000)
	{

		//read data
		*usart_receive_byte() = (uint8_t)(USART1->RDR);

		// rise flag
		set_new_char_received_flag();
	}

	// clear all interrupt flags
	USART1->ICR |= 0x00121b5f;
}



void SPI1_IRQHandler(void)
{
	int i;

	uint8_t spi1_in_data;
	int16_t aux16;
	uint8_t aux8;

	if((SPI1->SR & SPI_SR_RXNE) != 0)
	{
		// disable spi1 rxne interrupt
		SPI1->CR2 &= ~SPI_CR2_RXNEIE;   // disable rxne interrupt
		// stop systick timer
		SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;

		// read from spi data register
		spi1_in_data = SPI1->DR;

		//8888888888888888888888888888888888888888888888888


		//8888888888888888888888888888888888888888888888888

		// enable spi1 rxne interrupt
		SPI1->CR2 |= SPI_CR2_RXNEIE;   // enable rxne interrupt
		// start systick timer
		SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
	}
}
















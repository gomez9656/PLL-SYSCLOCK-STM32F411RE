/*
 * main.c
 *
 *  Created on: 23/01/2021
 *      Author: PC
 */

#include "stm32f4xx.h"
#include "main.h"
#include <string.h>
#include <stdint.h>

/*
 * Functions prototypes
 */

#define TRUE	1
#define FALSE	0

void UART2_Init();
void Error_handler();

UART_HandleTypeDef huart2; //Handle of UART 2


int main(){


	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	char msg[100];

	/* Basic initialization */
	HAL_Init();

	UART2_Init();

	/*
	 * Oscillator initialization. We are not actually using HSE because it does not exist
	 * But, we use the bypass mode and is configured in this way. This initialization
	 * starts the HSE, but that doesn't mean the system clock will use it
	 */
	memset(&osc_init, 0, sizeof(osc_init)); //Maybe there is garbage values, so memset to 0
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){

		Error_handler();
	}

	/*
	 * This part configures the clock to be used as the system clock and
	 * set different preescalers
	 */
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
	//This API changes from HSI to HSE
	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK){

		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Turn off HSI because we are using HSE

	/*
	 * With the HSI, systick timer was wroking with 16MHX, now we are using
	 * the HSE, so we need to reconfigure the ticks
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	UART2_Init();

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLOCK : %ldHz\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	/*
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "HLCK : %ldHz\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1 : %ldHz\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK2 : %ldHz\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	*/

	while(1);

	return 0;
}


/*
 * Function to initialize the UART2 peripheral
 */
void UART2_Init(){

	//1. Linking handle struct to base address
	huart2.Instance = USART2;

	//2. Init the handle variable
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	//3. Use the API to initialize
	if(HAL_UART_Init(&huart2) != HAL_OK){

		//There is a problem
		Error_handler();
	}
}


void Error_handler(void){
	while(1);
}
